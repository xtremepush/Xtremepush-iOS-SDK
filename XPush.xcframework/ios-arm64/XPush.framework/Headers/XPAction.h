//
//  XPAction.h
//  XPush
//
//  Created by Vlad Soroka on 8/23/17.
//  Copyright © 2017 XPush. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "XPPublicConstants.h"

@interface XPAction : NSObject

///click, dismiss, display
@property (nonatomic, readonly) XPActionType type;

@property (nonatomic, readonly, nullable) NSString *identifier;
@property (nonatomic, readonly, nullable) NSURL *url;
@property (nonatomic, readonly, nullable) NSString *deeplink;
@property (nonatomic, readwrite, nullable) NSString* inapp;
@property (nonatomic, readonly, nullable) NSString* action;
@property (nonatomic, readonly, nullable) NSString *requestPushPermission;

- (NSDictionary  * _Nonnull ) dictionaryRepresentation;

@end
