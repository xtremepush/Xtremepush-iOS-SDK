import Foundation
import XPush

#if canImport(ActivityKit)
import ActivityKit
#endif

@available(iOS 13.0, *)
public enum XPushLiveActivities {

    fileprivate static func _hexString(_ data: Data) -> String {
        data.map { String(format: "%02x", $0) }.joined()
    }
}

#if canImport(ActivityKit)
import ActivityKit

@available(iOS 16.2, *)
public protocol XPushActivityAttributes: ActivityAttributes {
    var activityID: String { get }
}

// Put ALL ActivityKit-dependent API into an availability-gated extension
@available(iOS 16.2, *)
@available(iOSApplicationExtension, unavailable)
public extension XPushLiveActivities {

    // Actor and all state live inside this availability island
    actor State {
        var pushToStartTasks: [ObjectIdentifier: Task<Void, Never>] = [:]
        var activityMonitorTasks: [ObjectIdentifier: Task<Void, Never>] = [:]
        var updateTokenTasks: [String: Task<Void, Never>] = [:]

        var registeredActivityIDsByType: [ObjectIdentifier: Set<String>] = [:]
        var latestPushToStartTokenByType: [ObjectIdentifier: String] = [:]
        var nameByType: [ObjectIdentifier: String] = [:]

        func register(typeKey: ObjectIdentifier, name: String, activityID: String) -> String? {
            nameByType[typeKey] = name
            var ids = registeredActivityIDsByType[typeKey] ?? []
            ids.insert(activityID)
            registeredActivityIDsByType[typeKey] = ids
            return latestPushToStartTokenByType[typeKey]
        }

        func setLatestPushToStartToken(_ token: String, for typeKey: ObjectIdentifier) {
            latestPushToStartTokenByType[typeKey] = token
        }

        func idsToNotify(for typeKey: ObjectIdentifier) -> [String] {
            Array(registeredActivityIDsByType[typeKey] ?? [])
        }

        func name(for typeKey: ObjectIdentifier) -> String {
            nameByType[typeKey] ?? ""
        }

        func isUpdateListenerAttached(activityKey: String) -> Bool {
            updateTokenTasks[activityKey] != nil
        }

        func setUpdateListenerTask(_ task: Task<Void, Never>?, for activityKey: String) {
            updateTokenTasks[activityKey] = task
        }

        func ensurePushToStartTaskExists(typeKey: ObjectIdentifier, start: () -> Task<Void, Never>) {
            if pushToStartTasks[typeKey] == nil { pushToStartTasks[typeKey] = start() }
        }

        func ensureActivityMonitorTaskExists(typeKey: ObjectIdentifier, start: () -> Task<Void, Never>) {
            if activityMonitorTasks[typeKey] == nil { activityMonitorTasks[typeKey] = start() }
        }
        
        func unregister(activityID: String) {
            for typeKey in Array(registeredActivityIDsByType.keys) {
                guard var ids = registeredActivityIDsByType[typeKey] else {
                    continue
                }
                ids.remove(activityID)
                if ids.isEmpty {
                    registeredActivityIDsByType.removeValue(forKey: typeKey)
                } else {
                    registeredActivityIDsByType[typeKey] = ids
                }
            }
        }
    }

    static let state = State()
    
    static func unsubscribeFromLiveActivity(activityID: String) {
        Task(priority: .background) {
            await state.unregister(activityID: activityID)
            XPush.endLiveActivitySubscription(activityID)
        }
    }

    @available(iOS 16.2, *)
    static func registerPushToStartLiveActivity<A: XPushActivityAttributes>(
        forType attributesType: A.Type,
        name: String,
        activityID: String
    ) {
        let typeKey = ObjectIdentifier(attributesType)

        Task(priority: .background) {
            let cachedToken = await state.register(typeKey: typeKey, name: name, activityID: activityID)

            if let token = cachedToken {
                XPush.handleLiveActivityToken(token, type: name, activityId: activityID)
            }

            await state.ensurePushToStartTaskExists(typeKey: typeKey) {
                _startPushToStartListener(forType: attributesType, typeKey: typeKey)
            }

            await state.ensureActivityMonitorTaskExists(typeKey: typeKey) {
                _monitorActivitiesAndAttachUpdateListeners(forType: attributesType)
            }
        }
    }

    @available(iOS 16.2, *)
    private static func _startPushToStartListener<A: XPushActivityAttributes>(
        forType: A.Type,
        typeKey: ObjectIdentifier
    ) -> Task<Void, Never> {
        Task(priority: .background) {
            guard #available(iOS 17.2, *) else { return }

            for await tokenData in Activity<A>.pushToStartTokenUpdates {
                let tokenString = XPushLiveActivities._hexString(tokenData)
                await state.setLatestPushToStartToken(
                    tokenString,
                    for: typeKey
                )
                let idsToNotify = await state.idsToNotify(for: typeKey)
                let name = await state.name(for: typeKey)
                for id in idsToNotify {
                    XPush.handleLiveActivityToken(
                        tokenString,
                        type: name,
                        activityId: id
                    )
                }
            }
        }
    }

    @available(iOS 16.2, *)
    private static func _monitorActivitiesAndAttachUpdateListeners<A: XPushActivityAttributes>(
        forType: A.Type
    ) -> Task<Void, Never> {
        Task(priority: .background) {
            for activity in Activity<A>.activities {
                _attachUpdateTokenListenerIfNeeded(activity: activity)
            }
            for await activity in Activity<A>.activityUpdates {
                _attachUpdateTokenListenerIfNeeded(activity: activity)
            }
        }
    }

    @available(iOS 16.2, *)
    private static func _attachUpdateTokenListenerIfNeeded<A: XPushActivityAttributes>(
        activity: Activity<A>
    ) {
        let activityKey = activity.id

        Task(priority: .background) {
            let alreadyListening = await state.isUpdateListenerAttached(activityKey: activityKey)
            guard !alreadyListening else { return }

            let task = Task(priority: .background) {
                let backendID = activity.attributes.activityID
                for await tokenData in activity.pushTokenUpdates {
                    let tokenString = XPushLiveActivities._hexString(tokenData)
                    XPush.handleLiveActivityUpdateToken(tokenString, activityId: backendID)
                }

                await state.setUpdateListenerTask(nil, for: activityKey)
            }
            
            await state.setUpdateListenerTask(task, for: activityKey)
        }
    }
}
#endif

