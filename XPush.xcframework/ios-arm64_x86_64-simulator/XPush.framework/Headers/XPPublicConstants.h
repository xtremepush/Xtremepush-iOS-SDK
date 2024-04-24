//
//  XPPublicConstants.h
//  XPush
//
//  Created by Vlad Soroka on 7/18/17.
//  Copyright © 2017 XPush. All rights reserved.
//

#ifndef XPPublicConstants_h
#define XPPublicConstants_h

@class XPMessageResponse;
@class XPMessage;
@class XPInboxItem;
@class UNNotificationCategory;

/**
 *	Notification name, which will be sent when device registered in the server.
 */
extern NSString * _Nonnull const XPushDeviceRegistrationNotification;

/**
 *	Notification name, which will be sent when inbox badge has changed
 */
extern NSString * _Nonnull const XPushInboxBadgeChangeNotification;

typedef void (^XPMessageCompletionBlock)(void);
typedef void (^XPSimpleCompletionBlock)(void);

typedef void(^XPMessageCallback)(XPMessageResponse* _Nonnull x);

/**
 * Callback you'll receive upon any interaction with PushNotification. If you want to leverage background processing with push notification consider using |XPContinousMessageInteractionCallback|
 */
typedef XPMessageCallback XPMessageInteractionCallback;

/**
 * Callback you'll receive upon user clicking "Manage settings" in notification center.
 * Assosiated message will provide context for which user decided to manage notification settings
 *  @param message - push notification, from which user decided to open notification settings. Is nil if user opens from iOS Settings
 */
typedef void(^XPNotificationSettingCallback)(XPMessage* _Nullable message);

/**
 * Callback you'll receive upon any interaction with PushNotification that requires background processing.
 */
typedef void(^XPContinousMessageInteractionCallback)(XPMessageResponse* _Nonnull x,
                                                     XPMessageCompletionBlock _Nonnull callback);

/**
 *  Callback you'll receive upon deeplink interaction happens
 */
typedef void(^XPDeeplinkCallback)(NSString* _Nonnull x);

/**
 *  Callback you'll receive upon fetching inbox list
 */
typedef void(^XPInboxListCallback)(NSArray<XPInboxItem *> * _Nullable list, NSError* _Nullable error);

/**
 *  Callback you'll receive upon change in inbox badge
 */
typedef void(^XPInboxBadgeCallback)(NSInteger badge);

/**
 *  Callback you'll receive upon fetching inbox list
 */
typedef void(^XPChannelPreferencesCallback)(NSDictionary* _Nullable jsonResponse, NSError* _Nullable error);

/**
 *  Callback you'll receive upon native message
 */
typedef void(^XPNativeMessageCallback)(XPMessage* _Nonnull message);

/**
 *  Callback you'll receive if you want to register push categories manually
 */
typedef void(^XPRegisterPushCategoriesCallback)(NSSet< UNNotificationCategory* >* _Nonnull categories);

/**
 *
 */
typedef NS_ENUM(NSInteger, XPActionType) {
 
    /**
     * User interacted with notification by clicking on it
     */
    XPActionType_Click,
    
    /**
     * Notification was delivered to your application and some UI could have been shown. User has not interacted with notification in any way yet
     */
    XPActionType_Present,
    
    /**
     * Notification was delivered to your application, but the user dismissed it
     */
    XPActionType_Dismiss,
    
};

/**
 * Available message types
 */
typedef NS_ENUM(NSInteger, XPMessageType) {
    
    /**
     * Push message
     */
    XPMessageType_Push,
    
    /**
     * Inbox message
     */
    XPMessageType_Inbox,
    
    /**
     * In-app message
     */
    XPMessageType_Inapp
    
};

/**
 * Available notification options
 */
typedef NS_OPTIONS(NSInteger, XPNotificationType) {
  
    XPNotificationType_None  = 0,
    
    XPNotificationType_Alert = 1 << 0,
    
    XPNotificationType_Sound = 1 << 1,
    
    XPNotificationType_Badge = 1 << 2
    
};

/**
 * You can provide different behaviours for showing foreground push notification
 */
typedef XPNotificationType(^XPForegroundNotificationOptions)(XPMessage* _Nonnull x);

#endif /* XPPublicConstants_h */
