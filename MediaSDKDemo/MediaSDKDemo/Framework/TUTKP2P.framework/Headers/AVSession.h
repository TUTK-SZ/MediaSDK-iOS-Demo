//
//  AVSession.h
//  TUTKP2P
//
//  Created by Joe_Liu on 2018/3/8.
//  Copyright © 2018年 tutksz_ios. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AVSession : NSObject

@property (nonatomic, copy)NSString          *uid;

@property (nonatomic, assign)int              sessionID;
@property (nonatomic, assign)int              sessionStatus;
@property (nonatomic, assign)int              mode;
@property (nonatomic, assign)int              errorCode;

//- (instancetype)initWithUID:(NSString *)uid
//                        SID:(int)SID
//              sessionStatus:(int)sessionStatus
//                    errCode:(int)errCode
//                    natMode:(int)natMode;

@end
