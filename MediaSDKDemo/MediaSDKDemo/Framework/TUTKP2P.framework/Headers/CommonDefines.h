//
//  CommonDefines.h
//  TUTKP2P
//
//  Created by steven_yang on 2018/1/17.
//  Copyright © 2018年 tutksz_ios. All rights reserved.
//

#import "AVIOCTRLDEFs.h"

#ifndef CommonDefines_h
#define CommonDefines_h

#define SNYC_VIDEO   // 开启视频同步

#define MAX_SESSION 128
#define SERVTYPE_STREAM_SERVER              0
#define MAX_CLIENT_NUMBER                   128

#define MAX_SIZE_IOCTRL_BUF                 1024
#define RECV_VIDEO_BUFFER_SIZE              512*1024
#define RECV_AUDIO_BUFFER_SIZE              1280

#define CONNECTION_MODE_NONE            -1
#define CONNECTION_MODE_P2P             0
#define CONNECTION_MODE_RELAY           1
#define CONNECTION_MODE_LAN             2

#define STATUS_CHECK_NORMAL             0
#define STATUS_CHECK_DELAY              1
#define STATUS_CHECK_BLOCK              2

/* only for client */
#define CLIENT_STATE_NONE               0
#define CLIENT_STATE_CONNECTING         1
#define CLIENT_STATE_CONNECTED          2
#define CLIENT_STATE_DISCONNECTED       3
#define CLIENT_STATE_UNKNOWN_DEVICE     4
#define CLIENT_STATE_WRONG_PASSWORD     5
#define CLIENT_STATE_TIMEOUT            6
#define CLIENT_STATE_UNSUPPORTED        7
#define CLIENT_STATE_CONNECT_FAILED     8
#define CLIENT_STATE_NEW_CONNECTED      9
#define CLIENT_STATE_CHANNEL_USED       10
#define CLIENT_STATE_DEVICE_OFFLINE     11

#define CLIENT_STATE_SLEEP              -64

/* only for device */
#define DEV_STATE_LOGINING              20
#define DEV_STATE_LOGINED               21
#define DEV_STATE_EXIT_LISTEN           22
#define DEV_STATE_EXCEED_MAX_SESSION    23
#define DEV_STATE_NEW_CLIENT            24
#define DEV_STATE_CONNECTING            25
#define DEV_STATE_CONNECTED             26
#define DEV_STATE_DISCONNECTED          27
#define DEV_STATE_CONNECT_FAILED        28

#endif /* CommonDefines_h */
