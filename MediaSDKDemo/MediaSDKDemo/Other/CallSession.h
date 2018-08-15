//
//  CallSession.h
//  Demo
//
//  Created by Joe_Liu on 2018/7/27.
//  Copyright © 2018年 Joe_Liu. All rights reserved.
//

#import <Foundation/Foundation.h>
@class AVSession;

@interface CallSession : NSObject

@property (nonatomic, copy)NSString    *idString;
@property (nonatomic, strong)AVSession *session;

@end
