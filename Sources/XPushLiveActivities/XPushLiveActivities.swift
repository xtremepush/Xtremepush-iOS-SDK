import Foundation
import XPush

#if canImport(ActivityKit)
import ActivityKit
#endif

@available(iOS 13.0, *)
public enum XPushLiveActivities {

    private static var pushToStartTasks: [ObjectIdentifier: Task<Void, Never>] = [:]
    private static var activityMonitorTasks: [ObjectIdentifier: Task<Void, Never>] = [:]
    private static var updateTokenTasks: [String: Task<Void, Never>] = [:] // key = activity.id
    private static var registeredActivityIDsByType: [ObjectIdentifier: Set<String>] = [:]
    private static var latestPushToStartTokenByType: [ObjectIdentifier: String] = [:]
    private static var nameByType: [ObjectIdentifier: String] = [:]

    private static let stateLock = NSLock()

    @available(iOS 16.2, *)
    @available(iOSApplicationExtension, unavailable)
    public static func registerPushToStartLiveActivity<A: XPushActivityAttributes>(
        forType attributesType: A.Type,
        name: String,
        activityID: String
    ) {
        #if canImport(ActivityKit)
        let typeKey = ObjectIdentifier(attributesType)

        var cachedToken: String?
        stateLock.lock()
        nameByType[typeKey] = name

        var ids = registeredActivityIDsByType[typeKey] ?? Set<String>()
        ids.insert(activityID)
        registeredActivityIDsByType[typeKey] = ids

        cachedToken = latestPushToStartTokenByType[typeKey]
        stateLock.unlock()

        if let token = cachedToken {
            XPush.handleLiveActivityToken(token, type: name, activityId: activityID)
        }

        if pushToStartTasks[typeKey] == nil {
            pushToStartTasks[typeKey] = _startPushToStartListener(forType: attributesType, typeKey: typeKey)
        }

        if activityMonitorTasks[typeKey] == nil {
            activityMonitorTasks[typeKey] = _monitorActivitiesAndAttachUpdateListeners(forType: attributesType)
        }
        #endif
    }

    public static func unsubscribeFromLiveActivity(activityID: String) {
        XPush.endLiveActivitySubscription(activityID)
    }

    @available(iOS 16.2, *)
    @available(iOSApplicationExtension, unavailable)
    private static func _startPushToStartListener<A: XPushActivityAttributes>(
        forType: A.Type,
        typeKey: ObjectIdentifier
    ) -> Task<Void, Never> {
        Task(priority: .background) {
            #if canImport(ActivityKit)
            guard #available(iOS 17.2, *) else { return }

            for await tokenData in Activity<A>.pushToStartTokenUpdates {
                let tokenString = _hexString(tokenData)

                var idsToNotify: [String] = []
                var name: String = ""

                stateLock.lock()
                latestPushToStartTokenByType[typeKey] = tokenString
                idsToNotify = Array(registeredActivityIDsByType[typeKey] ?? [])
                name = nameByType[typeKey] ?? ""
                stateLock.unlock()

                guard !name.isEmpty else { continue }

                for id in idsToNotify {
                    XPush.handleLiveActivityToken(tokenString, type: name, activityId: id)
                }
            }
            #endif
        }
    }


    @available(iOS 16.2, *)
    @available(iOSApplicationExtension, unavailable)
    private static func _monitorActivitiesAndAttachUpdateListeners<A: XPushActivityAttributes>(
        forType: A.Type
    ) -> Task<Void, Never> {
        Task(priority: .background) {
            #if canImport(ActivityKit)
            for activity in Activity<A>.activities {
                _attachUpdateTokenListenerIfNeeded(activity: activity)
            }

            for await activity in Activity<A>.activityUpdates {
                _attachUpdateTokenListenerIfNeeded(activity: activity)
            }
            #endif
        }
    }

    @available(iOS 16.2, *)
    private static func _attachUpdateTokenListenerIfNeeded<A: XPushActivityAttributes>(
        activity: Activity<A>
    ) {
        let key = activity.id

        stateLock.lock()
        let alreadyListening = (updateTokenTasks[key] != nil)
        stateLock.unlock()
        guard !alreadyListening else { return }

        let task = Task(priority: .background) {
            #if canImport(ActivityKit)

            let backendID = activity.attributes.activityID

            for await tokenData in activity.pushTokenUpdates {
                let tokenString = _hexString(tokenData)
                XPush.handleLiveActivityUpdateToken(tokenString, activityId: backendID)
            }

            stateLock.lock()
            updateTokenTasks[key] = nil
            stateLock.unlock()

            #endif
        }

        stateLock.lock()
        updateTokenTasks[key] = task
        stateLock.unlock()
    }


    private static func _hexString(_ data: Data) -> String {
        data.map { String(format: "%02x", $0) }.joined()
    }
}

@available(iOS 16.1, *)
public protocol XPushActivityAttributes: ActivityAttributes {
    var activityID: String { get }
}


