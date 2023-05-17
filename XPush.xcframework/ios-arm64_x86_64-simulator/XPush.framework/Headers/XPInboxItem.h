//
//  XPInboxItem.h
//  XPush
//
//  Created by Vlad Soroka on 10/25/18.
//  Copyright © 2018 XPush. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XPInboxItemStyle.h"
#import "XPMessageResponse.h"

NS_ASSUME_NONNULL_BEGIN

@interface XPInboxItem : NSObject

@property (nonatomic, readonly) NSInteger identifier;

@property (nonatomic, readonly) BOOL isOpened;
@property (nonatomic, readonly) BOOL isClicked;
@property (nonatomic, readonly) BOOL isDelivered;

@property (nonatomic, readonly) NSNumber* createTimestamp;
@property (nonatomic, readonly, nullable) NSNumber* expirationTimestamp;

@property (nonatomic, readonly) XPInboxItemStyle* style;

@property (nonatomic, readonly) BOOL isCard;

@property (nonatomic) XPMessageResponse* response;

@end

NS_ASSUME_NONNULL_END
