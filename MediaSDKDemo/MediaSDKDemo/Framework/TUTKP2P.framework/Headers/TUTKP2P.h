//
//  TUTKP2P.h
//  TUTKP2P
//
//  Created by steven_yang on 2018/1/16.
//  Copyright © 2018年 tutksz_ios. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "IOTCAPIs.h"
#import "AVAPIs.h"
#import "IOTCWakeUp.h"
#import "TUTKP2P-Prefix.pch"
#import "AVIOCTRLDEFs.h"
#import "AVFRAMEINFO.h"
#import "CommonDefines.h"
#import "AVSession.h"

//! Project version number for TUTKP2P.
FOUNDATION_EXPORT double TUTKP2PVersionNumber;

//! Project version string for TUTKP2P.
FOUNDATION_EXPORT const unsigned char TUTKP2PVersionString[];

@protocol TK_p2p_Delegate<NSObject>

@optional

/**
 device端视频数据输出
 
 @param data 视频数据
 @param isKeyFrame 是否是关键帧
 @param timeStamp 视频时间戳
 @param session 连线相关信息
 */
- (void)TK_device_outputVideoData:(NSData *) data
                       isKeyFrame:(BOOL)isKeyFrame
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session;

/**
 client端视频数据输出
 
 @param data 视频数据
 @param isKeyFrame 是否是关键帧
 @param timeStamp 视频时间戳
 @param session 连线相关信息
 */
- (void)TK_client_outputVideoData:(NSData *) data
                       isKeyFrame:(BOOL)isKeyFrame
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session;

/**
 device端音频数据输出
 
 @param data 音频数据
 @param timeStamp 音频时间戳
 @param session 连线相关信息
 */
- (void)TK_device_outputAudioData:(NSData *)data
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session;

/**
 client端音频数据输出
 
 @param data 音频数据
 @param timeStamp 音频时间戳
 @param session 连线相关信息
 */
- (void)TK_client_outputAudioData:(NSData *)data
                        timeStamp:(unsigned long long)timeStamp
                          session:(AVSession *)session;

/**
 device端IO Command输出
 
 @param data 命令数据
 @param type 命令类型
 @param session 连线相关信息
 */
- (void)TK_device_outputIOCtrlData:(NSData *)data
                              type:(NSInteger)type
                           session:(AVSession *)session;

/**
 client端IO Command输出
 
 @param data 命令数据
 @param type 命令类型
 @param session 连线相关信息
 */
- (void)TK_client_outputIOCtrlData:(NSData *)data
                              type:(NSInteger)type
                           session:(AVSession *)session;


/**
 device端连线状态
 
 @param state 状态
 @param session 连线相关信息
 */
- (void)TK_device_sessionStatus:(int)state
                        session:(AVSession *)session;

/**
 client端连线状态
 
 @param state 状态
 @param session 连线相关信息
 */
- (void)TK_client_sessionStatus:(int)state
                        session:(AVSession *)session;


/**
 device端网络时延等级
 
 @param rttLevel 时延等级  STATUS_CHECK_NORMAL/BLOCK/DELAY
 @param session 连线相关信息
 */
- (void)TK_device_sessionRTT:(int)rttLevel
                     session:(AVSession *)session;

/**
 client端网络时延等级
 
 @param rttLevel 时延等级  STATUS_CHECK_NORMAL/BLOCK/DELAY
 @param session 连线相关信息
 */
- (void)TK_client_sessionRTT:(int)rttLevel
                     session:(AVSession *)session;

/**
 获取Debug资讯的接口
 
 @param videoFps 视频fps
 @param audioFps 音频fps
 @param frameCount 视频总帧数
 @param dropCount 丢弃的视频帧数
 @param videoBps 视频的bps
 @param audioBps 音频的bps
 @param audioDelayTime 音频延迟时间
 @param videoDelayTime 视频延迟时间
 @param rtt 网络时延
 @param sessionMode 连线模式  0: P2P mode, 1: Relay mode, 2: LAN mode
 @param session 连线相关信息
 */
- (void)TK_debug_p2pInfoWithVideoFps:(int)videoFps
                            audioFps:(int)audioFps
                          frameCount:(unsigned int)frameCount
                           dropCount:(int)dropCount
                            videoBps:(int)videoBps
                            audioBps:(int)audioBps
                      audioDelayTime:(int)audioDelayTime
                      videoDelayTime:(int)videoDelayTime
                                 rtt:(int)rtt
                         sessionMode:(int)sessionMode
                             session:(AVSession *)session;

@end

@interface TUTKP2P : NSObject

+(instancetype)shareInstance;
/**
 IOTC SDK初始化
 */
- (void) TK_init;
/**
 IOTC SDK反初始化
 */
- (void) TK_unInit;

/**
 获取P2P模块版本号
 */
+ (NSString *)TK_getP2PVersion;

/**
 获取IOTC版本号
 */
+ (NSString *) TK_getIOTCAPIsVersion;

/**
 获取AVAPI版本号
 */
+ (NSString *) TK_getAVAPIsVersion;

/**
 开启日志记录
 */
+ (void)TK_enableLogFile;

/**
 设置代理
 */
- (void)setP2pDelegate:(id<TK_p2p_Delegate>)p2pDelegate;


/**
 device端注册
 
 @param uid 20位的P2P UID
 @param password 密码
 */
- (void) TK_device_loginWithUID:(NSString *)uid
                       Password:(NSString *)password;

/**
 device端反注册
 */
- (void) TK_device_logout;

/**
 device端从后台进前台
 */
- (void) TK_device_willEnterForeground;


/**
 获取device端是否在线
 
 @param UID    要检查的设备UID
 @param timeout 检查的超时时长
 @param handler 设备状态结果回调
 *            result:
 *            - #IOTC_ER_NoERROR 设备在线
 *            - #IOTC_ER_NETWORK_UNREACHABLE              网络已断开
 *            - #IOTC_ER_MASTER_NOT_RESPONSE              所有masters服务器都没有响应
 *            - #IOTC_ER_TCP_CONNECT_TO_SERVER_FAILED     通过TCP无法连接到IOTC服务器
 *            - #IOTC_ER_CAN_NOT_FIND_DEVICE IOTC         IOTC服务器没有找到设备
 *            - #IOTC_ER_SERVER_NOT_RESPONSE              所有服务器都没有响应
 *            - #IOTC_ER_TCP_TRAVEL_FAILED                通过UDP或者TCP都不能连接到masters服务器
 *            - #IOTC_ER_DEVICE_OFFLINE                   设备已离线
 */
- (void)TK_device_checkOnLine:(NSString *)UID
                      timeout:(int)timeout
                      handler:(void (^)(int result))handler;


/**
 device端断开与client端的连线
 
 @param session 连线session相关信息
 */
- (void)TK_device_disconnect:(AVSession *)session;

/**
 device端断开与所有client端的连线
 
 */
- (void)TK_device_disconnectAll;

/**
 获取device当前连线的所有session相关信息
 */
- (NSArray *)TK_device_getAVSessions;

/**
 device端发送命令
 
 @param type 命令类型
 @param data 命令数据
 @param session 连线session相关信息
 */
- (void)TK_device_sendIOCtrl:(int)type
                        data:(NSData *)data
                     session:(AVSession *)session;

/**
 device端向client端发送音频数据
 
 @param data           音频数据
 @param timeStamp      音频时间戳
 @param session 连线session相关信息
 */
- (void) TK_device_onSendAudioData:(NSData *)data
                         timeStamp:(unsigned long long)timeStamp
                           session:(AVSession *)session;


/**
 device端向client端发送视频数据

 @param data 视频数据
 @param isIFrame 是否是I帧
 @param timeStamp 视频时间戳
 @param session 连线session相关信息
 */
- (void) TK_device_onSendVideoData:(NSData *)data
                          isIFrame:(BOOL)isIFrame
                         timeStamp:(unsigned long long)timeStamp
                           session:(AVSession *)session;


/**
 client端连线

 @param uid 20位的P2P UID
 @param password 密码
 @param channel AV通道
 */
- (void) TK_client_connectWithUID:(NSString *)uid
                         password:(NSString *)password
                          channel:(int) channel;

/**
 client端断开连线

 @param uid 20位的P2P UID
 */
- (void) TK_client_disconnectWithUID:(NSString *)uid;


/**
 client端断开所有连线
 */
- (void) TK_client_disconnectAll;


/**
 client端获取所有连线相关信息

 @return 返回连线信息
 */
- (NSArray *)TK_client_getAVSessions;


/**
 client端发送命令

 @param type 命令类型
 @param data 命令数据
 @param session 连续session相关信息
 */
- (void)TK_client_sendIOCtrl:(int)type
                        data:(NSData *)data
                     session:(AVSession *)session;


/**
 client端开启发送视频

 @param session 连续session相关信息
 */
- (void) TK_client_startSendVideo:(AVSession *)session;


/**
 client端停止发送视频

 @param session 连续session相关信息
 */
- (void) TK_client_stopSendVideo:(AVSession *)session;


/**
 client端开启接收视频

 @param session 连续session相关信息
 */
- (void) TK_client_startRecvVideo:(AVSession *)session;


/**
 client端停止接收视频

 @param session 连续session相关信息
 */
- (void) TK_client_stopRecvVideo:(AVSession *)session;


/**
 client端开启监听

 @param session 连续session相关信息
 */
- (void) TK_client_startListener:(AVSession *)session;


/**
 client端停止接听

 @param session 连续session相关信息
 */
- (void) TK_client_stopListener:(AVSession *)session;


/**
 client端开启对讲

 @param session 连续session相关信息
 */
- (void) TK_client_startSpeaking:(AVSession *)session;


/**
 client端停止对讲

 @param session 连续session相关信息
 */
- (void) TK_client_stopSpeaking:(AVSession *)session;



/**
 client端向device端发送音频数据

 @param data 音频数据
 @param timeStamp 音频时间戳
 @param session 连续session相关信息
 */
- (void) TK_client_onSendAudioData:(NSData *)data
                         timeStamp:(unsigned long long)timeStamp
                           session:(AVSession *)session;


/**
 client端向device端发送视频数据

 @param data 视频数据
 @param isIFrame 是否是I帧
 @param timeStamp 视频时间戳
 @param session 连续session相关信息
 */
- (void) TK_client_onSendVideoData:(NSData *)data
                          isIFrame:(BOOL)isIFrame
                         timeStamp:(unsigned long long)timeStamp
                           session:(AVSession *)session;

@end
