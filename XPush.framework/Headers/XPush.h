//
//  XPush.h
//  XtremePush
//  Copyright (c) 2017 Xtremepush. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <UserNotifications/UserNotifications.h>

#import "XPPublicConstants.h"
#import "XPMessageResponse.h"
#import "XPInboxItem.h"

@interface XPush : NSObject

/**
 * Call this method in your AppDelegate's 
 * [applicationDidFinishLaunchingWithOptions:]
 * Make sure to configure SDK options before calling it
 */
+ (void)applicationDidFinishLaunchingWithOptions:(NSDictionary *)launchOptions;


/** SDK CONFIGURATION **/

/**
 * Set application key retrieved from the platform
 */
+ (void)setAppKey:(NSString *)appKey;

/**
 * Should be turned on for development builds
 */
+ (void)setSandboxModeEnabled:(BOOL)sandboxModeEnabled;

/**
 * Turn on to show debug logs in console
 */
+ (void)setShouldShowDebugLogs:(BOOL)log;

/**
 * Turn on to run SDK in debug code
 */
+ (void)setDebugModeEnabled:(BOOL)debugModeEnabled;

/**
 * Turn on to enable IDFA collecting
 */
+ (void)setAttributionsEnabled:(BOOL)attributionsEnabled;

/**
 * Turn off to prevent device name collecting
 */
+ (void)setNameCollectingEnabled:(BOOL)nameCollectingEnabled;

/**
 * Turn on to enable in-app messages being shown on session start event
 */
+ (void)setInAppMessageEnabled:(BOOL)enabled;

/**
* Turn on to make XPush not register for push notification and request permissions from user
*/
+ (void)enableManualPushRegistration: (BOOL) enabled;

/** LOCATIONS CONFIGURATION **/

/**
 * Turn on to enable geofence monitoring in the app
 */
+ (void)setLocationEnabled:(BOOL)locationEnabled;

/**
 * Turn on to enable ibeacon monitoring in the app
 */
+ (void)setBeaconsEnabled:(BOOL)beaconsEnabled;

/**
 * Call this early if you want XP to avoid registering Push Categories
 * usefull if you prefer to manage those manually
 */
+ (void) disablePushCategories;

/**
 * Turn off to prevent SDK to request location persmissions automatically
 */
+ (void)setAsksForLocationPermissions:(BOOL)asksForLocationPermissions;

/**
 * Request locations permissions manually
 */
+ (void)askForLocationPermissions;

/**
 * Set user location radius in meters
 */
+ (void)setLocationUpdateRadius:(CGFloat) radius;

/**
* Enables delivery recipts for push notification
* should be called in you push extension file before any additional XPush calls
* prefereably in init method of Notification service
* Disabled by default
*/
+ (void) setDeliveryReceiptsEnabled:(BOOL) isEnabled;

/*
 * @param endpoint -- URL where Xtremepush is going to POST delivery receipts.
 * Payload will contain message_id and campaign_id
 */
+ (void) setDeliveryReceiptsEnabled:(BOOL) isEnabled
            customReportingEndpoint:(NSString*) endpoint;

/*
 * XPush will use encryption for push notifiction
 * you must use Push service extension and AppGroups to use encrypted pushes
 */
+ (void) enableEncryptedPush;

/*
 * @param appGroup -- name of the App Group you defined for your bundle identifier
 * Required for using delivery receipts or encrypted push notifications
 */
+ (void) enableAppGroups: (NSString*) appGroup;

/** DELEGATE BRIDGES **/

/**
 * Call this method in your AppDelegate's 
 * [application:didRegisterForRemoteNotificationsWithDeviceToken:]
 */
+ (void)applicationDidRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken;

/**
 * Call this method in your AppDelegate's 
 * [application:didFailToRegisterForRemoteNotificationsWithError:]
 */
+ (void)applicationDidFailToRegisterForRemoteNotificationsWithError:(NSError *)error;

+ (void)applicationDidRegisterForRemoteNotificationsWithDeviceTokenString:(NSString *)deviceTokenString;

/**
 * Call this method in your AppDelegate's 
 * [application:didReceiveRemoteNotification:fetchCompletionHandler:]
 */
+ (void)applicationDidReceiveRemoteNotification:(NSDictionary *)userInfo
                         fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler;

/**
 * You should never call this method.
 * Use applicationDidReceiveRemoteNotification:fetchCompletionHandler:
 */
+ (void)applicationColdLaunchRemoteNotification:(NSDictionary *)userInfo;

/**
 * Call this method in your AppDelegate's 
 * [application:didReceiveLocalNotification:]
 */
+ (void)applicationDidReceiveLocalNotification:(UILocalNotification *)notification;

/**
 * Call this method in your AppDelegate's 
 * [application:handleActionWithIdentifier:forRemoteNotification:completionHandler:]
 */
+ (void)         application:(UIApplication *)application
  handleActionWithIdentifier:(NSString *)identifier
       forRemoteNotification:(NSDictionary *)userInfo
           completionHandler:(XPSimpleCompletionBlock)completionHandler;

/**
 * Call this method in your AppDelegate's
 * [application:handleActionWithIdentifier:forLocalNotification:completionHandler:]
 */
+ (void)         application:(UIApplication *)application
  handleActionWithIdentifier:(NSString *)identifier
        forLocalNotification:(UILocalNotification *)localNotification
           completionHandler:(XPSimpleCompletionBlock)completionHandler;

/**
 * Call this method in your custom UNNotificationCenterDelegate's
 * [userNotificationCenter:willPresentNotification:withCompletionHandler:]
 */
+ (void)userNotificationCenter:(UNUserNotificationCenter *)center
       willPresentNotification:(UNNotification *)notification
         withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler API_AVAILABLE(ios(10.0));

/**
 * Call this method in your custom UNNotificationCenterDelegate's
 * [userNotificationCenter:didReceiveNotificationResponse:withCompletionHandler:]
 */
+ (void)userNotificationCenter:(UNUserNotificationCenter *)center
didReceiveNotificationResponse:(UNNotificationResponse *)response
         withCompletionHandler:(XPSimpleCompletionBlock)completionHandler API_AVAILABLE(ios(10.0));

/**
 * Call this method in your custom UNNotificationCenterDelegate's
 * [userNotificationCenter:openSettingsForNotification:]
 */
+ (void)userNotificationCenter:(UNUserNotificationCenter *)center
   openSettingsForNotification:(UNNotification *)notification API_AVAILABLE(ios(12.0));

/** PUSH MESSAGING */

/**
 * Register current application and this lib to receive notifications.
 * Starting from iOS 12, XPush automatically provides you with new Apple's
 * provisional authorization experience.
 * If you used to prompt user with notification permission dialog at Application start,
 * you don't need to do anything to take advantage of provisional authorization.
 * If you have tailored user experience with notification promp showing at particular moment,
 * you might want to use method with `provisionalAuthorization` param to turn off provisional authorization.
 */
+ (void)registerForRemoteNotificationTypes:(XPNotificationType)types;

/**
 * Register current application and this lib to receive notifications
 * @param provisional - pass NO if you'd like to keep prompting user for notifications
 */
+ (void)registerForRemoteNotificationTypes:(XPNotificationType)types
                  provisionalAuthorization:(BOOL)provisional;

/**
 * Unregister current application and this lib to receive notifications
 */
+ (void)unregisterForRemoteNotifications;

/**
 * Customise foreground notification behaviours
 */
+ (void)registerForegroundNotificationOptions:(XPForegroundNotificationOptions)options;

/**
 * Should or not application reset a badge icon
 */
+ (void)setShouldWipeBadgeNumber:(BOOL)shouldWipeBadgeNumber;

/**
 * Should or not application be processing notifications from the launch options
 */
+ (void)setShouldProcessNotificationsFromLaunchOptions:(BOOL)shouldProcess;



/** MESSAGE HANDLING */

/**
 * Register custom message response handler
 */
+ (void)registerMessageResponseHandler:(XPMessageInteractionCallback)callback;

/**
 * Register custom message response handler
 * Use this version if you want to support background handling (such as notification buttons in background mode)
 */
+ (void)registerMessageResponseHandlerWithCompletion:(XPContinousMessageInteractionCallback)callback;

/**
 * Register standard deeplinks handler
 */
+ (void)registerDeeplinkHandler: (XPDeeplinkCallback)callback;

/**
 * Register standard deeplinks handler
 * Called when user hits "Manage notification" in notification center.
 * You are given option to show user your custom Notification settings UI
 */
+ (void)registerNotificationSettingsHandler: (XPNotificationSettingCallback)callback;

/**
 * Register handler for native messages
 */
+ (void)registerNativeMessageHandler: (XPNativeMessageCallback)callback;

/**
 * Progamatically process message click event after showing custom dialog
 */
+ (void)clickMessage:(XPMessage *)message;

/**
 * Progamatically process message click with certain action identifier event after showing custom dialog
 */
+ (void)clickMessage:(XPMessage *)message actionIdentifier:(NSString*)actionIdentifier;

/**
 * Report message being clicked after showing custom dialog
 */
+ (void)reportMessageClicked:(XPMessage *)message;

/**
 * Report message being delivered after showing custom dialog
 */
+ (void)reportMessageDelivered:(XPMessage *)message;

/**
 * Report message being clicked with certain action identifier after showing custom dialog
 */
+ (void)reportMessageClicked:(XPMessage *)message actionIdentifier:(NSString*)actionIdentifier;

/**
 * Report message being closed after showing custom dialog
 */
+ (void)reportMessageDismissed:(XPMessage *)message;



/** INBOX **/

/**
 * Turn on to enable Inbox
 */
+ (void)setInboxEnabled:(BOOL)inboxEnabled;

/**
 * Open inbox
 */
+ (void)openInbox;

/**
 * Force presents inbox regardless of currently presented ViewController
 * We discourage you from using this method. It could lead to unexpected presentation issues with your already presented viewController
 * We recommend using openInbox and make sure that there isn't any viewControllers presented at the moment
 */
+ (void)forceOpenInbox;

/**
 * Get current inbox badge
 */
+ (NSInteger)getInboxBadge;

+ (void) registerInboxChangeCallback:(void(^)(NSInteger)) callback;

/** INAPP BEHAVIOR */

/**
 * Record in-app event hit
 */
+ (void)hitEvent:(NSString *)event;

/**
 * Calls "eventHit" with value.
 */
+ (void)hitEvent:(NSString *)event withValue:(NSString *)value;

/**
 *	Calls "eventHit" with multiple values.
 */
+ (void)hitEvent:(NSString *)event withValues:(NSDictionary *)values;

/**
 * Record tag hit
 */
+ (void)hitTag:(NSString *)tag;

/**
 * Record tag with value or attribute
 */
+ (void)hitTag:(NSString *)tag withValue:(NSString *)value;

/**
 * Record impression hit
 */
+ (void)hitImpression:(NSString *)impression;

/**
 * Force presents inapp message regardless of currently presented ViewController
 * We discourage you from using this method. It could lead to unexpected presentation issues with your already presented viewController
 * We recommend making sure that there isn't any viewControllers presented at the moment
 */
+ (void) forcePresentInappMessage: (BOOL) shouldForcePresent;

/** DEVICE INFORMATION **/

/**
 * Set external id of a device which can be used then on platform to target devices
 */
+ (void)setExternalId:(NSString *)externalId;

/**
 * Switch device's push subscription on/off
 */
+ (void)setSubscription:(BOOL)subscription;

/**
 * Returns current subscription status
 */
+ (BOOL)getSubscription;

/**
 *	Returns version of the lib.
 */
+ (NSString *)version;

/**
 *	Returns dictionary with device token and device id.
 *  XPushDeviceID - key for XtremePush device id.
 *  deviceToken - key for token.
 *  deviceID - key for device identifier (IDFV).
 *  externalID - key for external id.
 */
+ (NSDictionary *)deviceInfo;


/** INAPP BEHAVIOR RELEASING CONFIGURATION **/

/**
 * Turn off to send tag hits to the server in realtime
 */
+ (void)setTagsBatchingEnabled:(BOOL)tagsBatchingEnabled;

/**
 * Send stored tag hits to the server
 */
+ (void)sendTags;

/**
 * Turn off to send impression hits to the server in realtime
 */
+ (void)setImpressionsBatchingEnabled:(BOOL)impressionsBatchingEnabled;

/**
 * Send stored impression hits to the server
 */
+ (void)sendImpressions;

/**
 *  Set a limit for a maximum number of stored tags, impressions and sessions
 */
+ (void)setTagsStoreLimit:(NSUInteger )limit;
+ (void)setImpressionsStoreLimit:(NSUInteger )limit;
+ (void)setSessionsStoreLimit:(NSUInteger)limit;

/**
 *    Used to get a list of inbox messages for current device
 */
+ (void)inboxListWithOffset:(NSUInteger)offset limit:(NSUInteger)limit callback:(XPInboxListCallback)callback;

/**
 *    Used to remove a message from inbox by it's id
 */
+ (void)removeInboxMessage:(XPInboxItem*)message callback:(XPInboxBadgeCallback)callback;

/** DEPRECATED PUSH MESSAGES ACCESS METHODS */

/**
 *	Used to get a list of push notifications for current device
 */
+ (void)getPushNotificationsOffset:(NSUInteger)offset limit:(NSUInteger)limit completion:(void(^)(NSArray *pushList, NSError *error))completion;

/**
 *	Used to get a list of push notifications for current device
 */
+ (void)getPushNotificationsRead:(BOOL)read offset:(NSUInteger)offset limit:(NSUInteger)limit completion:(void(^)(NSArray *pushList, NSError *error))completion;

/**
 *	Used to manually mark a push as read.
 */
+ (void)markPushAsRead:(NSString *)actionId;

+ (void)setCordovaLaunchMode:(BOOL) enable;

/**
 *  User setting
 */
+ (void) setUser:(NSString*) userId;
+ (void) setTempUser:(NSString*) userId;


+ (void)setServerURL:(NSString *)url;
+ (void)setServerExpectedCertificate:(NSString *)certDataString;
+ (void)setServerExpectedCertificates:(NSArray *)certDataStringArray;
+ (void)setServerExpectedCertificateFromFile:(NSString *)filePath;
+ (void)setServerExpectedCertificateFromFiles:(NSArray *)filePathArray;

+ (void)setServerExpectedCertificate:(NSString *)certDataString preferedIndex:(NSNumber*)index;
+ (void)setServerExpectedCertificates:(NSArray *)certDataStringArray preferedIndex:(NSNumber*)index;
+ (void)setServerExpectedCertificateFromFile:(NSString *)filePath preferedIndex:(NSNumber*)index;
+ (void)setServerExpectedCertificateFromFiles:(NSArray *)filePathArray preferedIndex:(NSNumber*)index;

/**
 *  Notification service extension
 */

NS_ASSUME_NONNULL_BEGIN;

+ (NSString* _Nullable) didReceiveNotificationRequest:(UNNotificationRequest * )request
                                   withContentHandler:(void (^)(UNNotificationContent *))contentHandler;
+ (void) serviceExtensionTimeWillExpireWith:(NSString* _Nullable ) token;

/**
 *  Notification preferences
 */

+ (void)updateUserWith:(NSDictionary*) preferences completionHandler:(XPChannelPreferencesCallback) callback;

+ (void)updatePushSubscription :(BOOL) subscriptionStatus completionHandler:(XPChannelPreferencesCallback) callback;
+ (void)updateEmailSubscription:(BOOL) subscriptionStatus completionHandler:(XPChannelPreferencesCallback) callback;
+ (void)updateSmsSubscription  :(BOOL) subscriptionStatus completionHandler:(XPChannelPreferencesCallback) callback;

+ (void)updatePushSubscriptionPreferences :(NSDictionary*) preferences completionHandler:(XPChannelPreferencesCallback) callback;
+ (void)updateEmailSubscriptionPreferences:(NSDictionary*) preferences completionHandler:(XPChannelPreferencesCallback) callback;
+ (void)updateSmsSubscriptionPreferences  :(NSDictionary*) preferences completionHandler:(XPChannelPreferencesCallback) callback;

+ (void)importUser:(NSDictionary*) preferences
 completionHandler:(XPChannelPreferencesCallback) callback;

NS_ASSUME_NONNULL_END;

@end

/** INBOX BUTTON **/

@interface XPInboxButton : UIButton
- (UILabel *)badge;
- (void)setBadgeSize:(NSInteger)badgeSize;
- (void)setBadgePosition:(CGPoint)badgeSize;
- (void)setBadgeColor:(UIColor *)color;
- (void)setBadgeTextColor:(UIColor *)color;
@end


