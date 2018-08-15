/*! \file AVAPIs2.h
This file describes all the APIs of the AVAPI2 module in IOTC platform.
AV module is a kind of data communication modules in IOTC platform to provide
fluent streaming Audio / Video data from AV servers to AV clients in
unidirectional way.

\copyright Copyright (c) 2010 by Throughtek Co., Ltd. All Rights Reserved.

Revision Table

Version     | Name             |Date           |Description
------------|------------------|---------------|-------------------
 0.1.0.1    |Jeff_Hsu          |2015-10-14     |Trial Version
 */


#ifndef _AVAPIs2_H_
#define _AVAPIs2_H_

/* ============================================================================
 * Platform Dependant Macro Definition
 * ============================================================================
 */
#include "P2PCam/AVFRAMEINFO.h"
#include "IOTCAPIs.h"

#ifdef _WIN32
/** @cond */
#ifdef IOTC_STATIC_LIB
#define AVAPI2_API
#elif defined AVAPI_EXPORTS
#define AVAPI2_API __declspec(dllexport)
#else
#define AVAPI2_API __declspec(dllimport)
#endif // #ifdef P2PAPI_EXPORTS
/** @endcond */
#endif // #ifdef _WIN32

#ifdef IOTC_ARC_HOPE312
#define AVAPI2_API
#define _stdcall
#endif // #ifdef IOTC_ARC_HOPE312

#if defined(__linux__) || defined (__APPLE__)
#define AVAPI2_API
#define _stdcall
#endif // #ifdef __linux__

#ifdef IOTC_UCOSII
#define AVAPI2_API
#define _stdcall
#endif // #ifdef IOTC_UCOSII

#ifdef IOTC_OV788
#define AVAPI2_API
#define _stdcall
#endif // #ifdef IOTC_UCOSII

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum
{
    AVAPI2_SERVER_STATUS_LOGIN_FAILED           = -1,   // Error when retry login to P2P Server
    AVAPI2_SERVER_STATUS_START_CANAL_FAILED     = -2,   // Error when start av server for new connected client
    AVAPI2_SERVER_STATUS_CREATE_SENDTASK_FAILED = -3,   // Error when create task for sending Video, Audio and IO control
    AVAPI2_SERVER_STATUS_CLEAN_BUFFER_TIMEOUT   = -4,   // Clean Video/Audio Buffer Timeout
    AVAPI2_SERVER_STATUS_START                  = 0,    // AVAPI2 Server start listening
    AVAPI2_SERVER_STATUS_LOGINED                = 1,    // AVAPI2 Server login to p2p server success
    AVAPI2_SERVER_STATUS_NEW_CANAL              = 2,    // A new client connection
    AVAPI2_SERVER_STATUS_WAIT_CLIENT            = 3,    // Wait client login
    AVAPI2_SERVER_STATUS_CLIENT_LOGINED         = 4,    // Client login success
    AVAPI2_SERVER_STATUS_CLEAN_BUFFER_SUCCESS   = 5,    // Clean Video/Audio Buffer Sucess
    AVAPI2_SERVER_STATUS_CLEAN_VIDEOBUF_REQUEST  = 6,   // Client request clean server video buffer
    AVAPI2_SERVER_STATUS_CLEAN_AUDIOBUF_REQUEST  = 7,   // Client request clean server audio buffer
    AVAPI2_SERVER_STATUS_DASA_LEVEL_CHANGE      = 8,    // Dynamic Adaptive Streaming over AVAPI change level
} AVAPI2_SERVER_STATUS;

typedef enum
{
    AVAPI2_CLIENT_STATUS_LOGIN_FAILED           = -1,   // Error when retry login to AVAPI2 Server
    AVAPI2_CLIENT_STATUS_LOGIN_TIMEOUT          = -2,   // Login to AVAPI2 Server Timeout
    AVAPI2_CLIENT_STATUS_CLEAN_BUFFER_TIMEOUT   = -3,   // Clean Video/Audio Buffer Timeout
    AVAPI2_CLIENT_STATUS_RECV_FRAME_BLOCK       = -4,   // Took to long to handle received video frame in callback function
    AVAPI2_CLIENT_STATUS_RECV_AUDIO_BLOCK       = -5,   // Took to long to handle received audio data in callback function
    AVAPI2_CLIENT_CONNECT_UID_ST_FAILED         = -6,   // A Client connects to a device failed.
    AVAPI2_CLIENT_CONNECT_UID_ST_START          = 0,    // A Client start connecting to a Device.
    AVAPI2_CLIENT_CONNECT_UID_ST_CONNECTING     = 1,    // A Client is connecting a device.
    AVAPI2_CLIENT_CONNECT_UID_ST_CONNECTED      = 2,    // The connection are established between a Client and a Device.
    AVAPI2_CLIENT_STATUS_LOGINED                = 3,    // Login to AVAPI2 Server Success
} AVAPI2_CLIENT_STATUS;

typedef enum
{
    AVAPI2_CONGESTION_CTRL_AUDIO_FIRST  = 0,    // IOCtrl > Audio > Video (Video will be drop)
    AVAPI2_CONGESTION_CTRL_VIDEO_FIRST  = 1,    // IOCtrl > Video > Audio (Video & Audio will be drop)
    AVAPI2_CONGESTION_CTRL_PLAYBACK     = 2,    // IOCtrl > Audio > Audio (Video & Audio will not be drop)
    AVAPI2_CONGESTION_CTRL_META         = 3,    // IOCtrl > Meta (without Video & Audio)
    AVAPI2_CONGESTION_CTRL_DASA         = 4,    // Enable Dynamic Adaptive Streaming over AVAPI, IOCtrl > Audio > Video (Video will be drop)
} AVAPI2_CONGESTION_CTRL;

/* ============================================================================
 * Structure Definition
 * ============================================================================
 */
#define MAX_STATISTICAL_DATA_SLOT_NUMBER 10
#define STATISTICAL_DATA_VERSION 0x00000003

/*! \details Statistical  data send from server to client (update every 10 secs)
 *           uTimeStamp : Statistical data timestamp in msec
 *           uVDataByte : video data bytes that device want to send
 *           uVSendByte : video data bytes that device already send
 *           uVDropByte : video data bytes that device dropped
 *           uVResendByte : video data bytes that device already resend
 *           usVFPS : video frame rate that device send
 *
 *           uADataByte : audio data bytes that device want to send
 *           uASendByte : audio data bytes that device already send
 *           uADropByte : audio data bytes that device dropped
 *           uAResendByte : audio data bytes that device already resend
 *           usAFPS : audio frame rate that device send
 */
typedef struct _StatisticalData
{
    unsigned int uTimeStamp;        //V1-Start
    unsigned int uLastTimeStamp;
    unsigned int uVDataByte;
    unsigned int uVSendByte;
    unsigned int uVDropByte;
    unsigned int uVResendByte;
    unsigned int uADataByte;
    unsigned int uASendByte;
    unsigned int uADropByte;
    unsigned int uAResendByte;
    unsigned int uVBufUsage;
    unsigned short usVFPS;
    unsigned short usAFPS;          //V1-End
    unsigned int uVResendReqCnt;    //V2-Start
    unsigned short usVCompleteFPS;
    unsigned short usACompleteFPS;  //V2-End
    unsigned short usResponeMin;    //V3-Start
    unsigned short usResponeMax;    //V3-End
}StatisticalData;

typedef struct _StatisticalDataSlot
{
    unsigned short usCount;
    unsigned short usIndex;
    unsigned int uVersion;
    unsigned int uDataSize;
    StatisticalData m_Data[MAX_STATISTICAL_DATA_SLOT_NUMBER];
}StatisticalDataSlot;

/*! \details Statistical data send from client to server (update every 10 secs)
 *           uTimeStamp : Statistical data timestamp in msec
 *           uVRecvByte : video data bytes that client received (not include resend)
 *           uVUserByte : video data bytes that application got
 *           uVDropByte : video data bytes that received and dropped because some reasons (incomplete frame, clean video video buffer)
 *           uVResendByte : video data bytes that resend from server
 *           uVResendReqCnt : count of resend video request
 *           usVFPS : video data frame rate that client received
 *
 *           uARecvByte : audio data bytes that client received (not include resend)
 *           uAUserByte : audio data bytes that application got
 *           uADropCnt  : count of dropped audio data
 *           uAResendByte : audio data bytes that resend from server
 *           uAResendReqCnt : count of resend audio request
 *           usAFPS : audio data frame rate that client received
 */
typedef struct _StatisticalClientData
{
    unsigned int uTimeStamp;        //V1-Start
    unsigned int uLastTimeStamp;
    unsigned int uVRecvByte;
    unsigned int uVUserByte;
    unsigned int uVDropByte;
    unsigned int uVResendByte;
    unsigned int uVResendReqCnt;
    unsigned int uARecvByte;
    unsigned int uAUserByte;
    unsigned int uADropCnt;
    unsigned int uAResendByte;
    unsigned int uAResendReqCnt;
    unsigned int uVBufUsage;
    unsigned short usVFPS;
    unsigned short usAFPS;          //V1-End
    unsigned int uVRepeateByte;     //V2-Start-End
    unsigned short usResponeMin;    //V3-Start
    unsigned short usResponeMax;    //V3-End
}StatisticalClientData;

typedef struct _StatisticalClientDataSlot
{
    unsigned short usCount;
    unsigned short usIndex;
    unsigned int uVersion;
    unsigned int uDataSize;
    StatisticalClientData m_Data[MAX_STATISTICAL_DATA_SLOT_NUMBER];
}StatisticalClientDataSlot;

/* ============================================================================
 * Type Definition
 * ============================================================================
 */

/*! \details The prototype of authentication function used for an AVAPI2 server.
 *           The authentication function is set when an AVAPI2 server starts by AVAPI2_ServerStart().
 *           The AVAPI2 server will call back authentication function when an AVAPI2
 *           client wants to connect with szViewAccount and szViewAccount
 *  \param szViewAccount    [in] The view account provided by AVAPI2 clients for authentication
 *  \param szViewPassword   [in] The view password provided by AVAPI2 clients for authentication
 *
 *  \return true if authentication passes and false if fails.
 */
typedef int (_stdcall *authCB)(char *szViewAccount, char *szViewPassword);

/*! \details The prototype of ioCtrl receive callback function used for AVAPI2 server and AVAPI2 client.
 *           The ioCtrl receive callback function is set when AVAPI2 server starts by AVAPI2_ServerStart() or AVAPI2 client register by AVAPI2_RegRecvIoCtrlCB().
 *           An AVAPI2 server or an AVAPI2 client can register this callback function to receive ioCtrl.
 *
 *  \param nAVCanal     [in] Index of AVAPI2 canal
 *  \param nIoCtrlType  [in] The type of received IO control
 *  \param pIoCtrlBuf   [in] The buffer of received IO control data
 *  \param nIoCtrlBufLen[in] The length of received IO control data
 *  \param pUserData    [in] The user data register by AVAPI2_RegUserData
 *
 *  \return 0 if handle success and -1 if fail.
 */
typedef int (_stdcall *ioCtrlRecvCB)(int nAVCanal, unsigned int nIoCtrlType, unsigned char *pIoCtrlBuf, unsigned int nIoCtrlBufLen, void* pUserData);

/*! \details The prototype of video frame callback function used for an AVAPI2 client.
 *           The video frame callback function is set when an AVAPI2 client register by AVAPI2_StartRecvFrame().
 *           An AVAPI2 client can register this callback function to receive video frame.
 *
 * \param nAVCanal              [in] Index of AVAPI2 canal
 * \param nRet                  [in] Returned result
 *      if nRet < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response.
 *          - #AV_ER_INVALID_SID The IOTC session of this AV channel is not valid
 *          - #AV_ER_DATA_NOREADY The data is not ready for receiving yet.
 *          - #AV_ER_LOSED_THIS_FRAME The whole frame is lost during receiving
 *          - #AV_ER_BUFPARA_MAXSIZE_INSUFF The frame to be received exceeds the size of abFrameData, i.e. nFrameDataMaxSize
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *          - #AV_ER_INCOMPLETE_FRAME Some parts of a frame are lost during receiving
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *
 * \param pFrameData            [in] The buffer of video frame data
 * \param nActualFrameSize      [in] The actual size of frame data to be received, maybe less than expected size
 * \param nExpectedFrameSize    [in] The size of frame data expect to be received that sent from AV server
 * \param pFrameInfo            [in] The video frame information to be received, defined in AVFRAMEINFO.h struct FRAMEINFO_t
 * \param nFrameInfoSize        [in] The size of the video frame information
 * \param nFrmNo                [in] The sequence number of current receiving video frame
 * \param pUserData             [in] The user data register by AVAPI2_RegUserData
 *
 * \return 0 if handle success and -1 if fail.
 */
typedef int (_stdcall *videoRecvCB)(int nAVCanal, int nRet, char *pFrameData, int nActualFrameSize, int nExpectedFrameSize, char* pFrameInfo, int nFrameInfoSize, int nFrmNo, void* pUserData);

/*! \details The prototype of audio data callback function used for an AVAPI2 client.
 *           The audio data callback function is set when an AVAPI2 client register by AVAPI2_StartRecvAudio().
 *           An AVAPI2 client can register this callback function to receive audio data.
 *
 *  \param nAVCanal     [in] Index of AVAPI2 canal
 *  \param nRet         [in] Returned result
 *      if nRet < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response.
 *          - #AV_ER_INVALID_SID The IOTC session of this AV channel is not valid
 *          - #AV_ER_DATA_NOREADY The data is not ready for receiving yet.
 *          - #AV_ER_LOSED_THIS_FRAME The whole frame is lost during receiving
 *          - #AV_ER_BUFPARA_MAXSIZE_INSUFF The data and frame info to be received exceeds	the size of abAudioData and abFrameInfo, respectively.
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *
 *  \param pAudioData   [in] The buffer of audio data
 *  \param nAudioSize   [in] The size of audio data to be received
 *  \param pAudioInfo   [in] Audio data infomation structure , defined in AVFRAMEINFO.h struct FRAMEINFO_t
 *  \param pUserData    [in] The user data register by AVAPI2_RegUserData
 *
 *  \return 0 if handle success and -1 if fail.
 */
typedef int (_stdcall *audioRecvCB)(int nAVCanal, int nRet, char *pAudioData, int nAudioSize, char* pAudioInfo, int nFrmNo, void* pUserData);

/*! \details The prototype of meta data callback function used for an AVAPI2 client.
 *           The meta data callback function is set when an AVAPI2 client register by AVAPI2_RegRecvMetaCB().
 *           An AVAPI2 client can register this callback function to receive meta data.
 *
 * \param nAVCanal              [in] Index of AVAPI2 canal
 * \param nRet                  [in] Returned result
 *      if nRet < 0
 *          - #AV_ER_INVALID_ARG The AV channel ID is not valid or frame data is null
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response.
 *          - #AV_ER_INVALID_SID The IOTC session of this AV channel is not valid
 *          - #AV_ER_DATA_NOREADY The data is not ready for receiving yet.
 *          - #AV_ER_LOSED_THIS_FRAME The whole frame is lost during receiving
 *          - #AV_ER_BUFPARA_MAXSIZE_INSUFF The frame to be received exceeds the size of abFrameData, i.e. nFrameDataMaxSize
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *          - #AV_ER_INCOMPLETE_FRAME Some parts of a frame are lost during receiving
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *
 * \param pMetaData             [in] The buffer of meta data
 * \param nActualMetaSize       [in] The actual size of meta data to be received, maybe less than expected size
 * \param nExpectedMetaSize     [in] The size of meta data expect to be received that sent from av server
 * \param pMetaInfo             [in] The meta data information to be received
 * \param nMetaInfoSize         [in] The size of the meta data information
 * \param nMetaNo               [in] The sequence number of current receiving meta data
 * \param pUserData             [in] The user data register by AVAPI2_RegUserData
 *
 * \return 0 if handle success and -1 if fail.
 */
typedef int (_stdcall *metaRecvCB)(int nAVCanal, int nRet, char *pMetaData, int nActualMetaSize, int nExpectedMetaSize, char* pMetaInfo, int nMetaInfoSize, int nMetaNo, void* pUserData);

/*! \details The prototype of canal status callback function used for AVAPI2 client and AVAPI2 server.
 *           The canal status callback function is set when an AVAPI2 client start by AVAPI2_ClientConnect() or AVAPI2 server start by AVAPI2_ServerStart().
 *           An AVAPI2 client or AVAPI2 server can register this callback function to handle canal error status.
 *
 *  \param nAVCanal     [in] Index of AVAPI2 canal
 *  \param nError       [in] Error code
 *      if nError < 0
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *          - #AV_ER_CLIENT_NO_AVLOGIN An AV client does not pass authentication yet
 *          - #AV_ER_INVALID_SID The IOTC session ID is not valid
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response.
 *          - #AV_ER_SERVER_EXIT Users stop this function with avServExit() in another thread
 *          - #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
 *          - #AV_ER_IOTC_SESSION_CLOSED The local IOTC session has been closed
 *          - #IOTC_ER_CH_NOT_ON  The IOTC channel of specified channel ID is not turned on
 *
 *  \param nChannelID   [in] ChannelID used by this AVAPI2 canal
 *  \param pStSInfo     [in] Session infomation structure , defined in IOTCAPIs.h struct st_SInfo
 *  \param pUserData    [in] The user data register by AVAPI2_RegUserData
 *
 *  \return 0 if handle success and -1 if fail.
 */
typedef int (_stdcall *canalStatusCB)(int nAVCanal, int nError, unsigned char nChannelID, struct st_SInfo* pStSInfo, void* pUserData);

/*! \details The prototype of AVAPI2 server status callback function used for an AVAPI2 server.
 *           The AVAPI2 server status callback function is set when an AVAPI2 server starts by AVAPI2_ServerStart().
 *
 *  \param nStatus      [in] The status of AVAPI2 server
 *                           -#define in enum AVAPI2_SERVER_STATUS
 *  \param nError       [in] The error code
 *      if nStatus == AVAPI2_SERVER_STATUS_LOGIN_FAILED
 *          nError will be one of these
 *          - #IOTC_ER_UNLICENSE The UID is not licensed or expired
 *          - #IOTC_ER_NOT_INITIALIZED The IOTC module is not initialized yet
 *          - #IOTC_ER_LOGIN_ALREADY_CALLED The device is already under login process currently
 *          - #IOTC_ER_FAIL_CREATE_SOCKET Fails to create sockets
 *          - #IOTC_ER_FAIL_SOCKET_OPT Fails to set up socket options
 *          - #IOTC_ER_FAIL_SOCKET_BIND Fails to bind sockets
 *          - #IOTC_ER_FAIL_CREATE_THREAD Fails to create threads
 *          - #IOTC_ER_FAIL_RESOLVE_HOSTNAME Cannot resolve masters' host name
 *          - #IOTC_ER_NO_PERMISSION The device does not support TCP relay
 *          - #IOTC_ER_FAIL_GET_LOCAL_IP Fails to get the device's local IP address
 *          - #IOTC_ER_NETWORK_UNREACHABLE Network is unreachable
 *
 *      if nStatus == AVAPI2_SERVER_STATUS_START_CANAL_FAILED
 *          nError will be one of these
 *          - #IOTC_ER_INVALID_ARG Invalid input argument.
 *          - #IOTC_ER_NOT_INITIALIZED The module has not bee initialized.
 *          - #IOTC_ER_EXCEED_MAX_SESSION It reaches the max session number.
 *          - #IOTC_ER_DEVICE_IS_BANNED The device is banned.
 *          - #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID and IOTC channel ID.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *          - #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response.
 *          - #AV_ER_INVALID_SID The IOTC session ID is not valid
 *          - #AV_ER_SERVER_EXIT Users stop this function with avServExit() in another thread
 *          - #AV_ER_TIMEOUT The timeout specified by nTimeout expires before AVAPI2 server start is performed completely
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel, please check if the IOTC channel is used correctly
 *
 *  \param nAVCanal     [in] Index of AVAPI2 canal
 *  \param nChannelID   [in] ChannelID used by this AVAPI2 canal
 *  \param pStSInfo     [in] Session infomation structure , defined in IOTCAPIs.h struct st_SInfo
 *  \param pUserData    [in] The user data register by AVAPI2_RegUserData
 *
 *  \return 0 if handle success and -1 if fail.
 */
typedef int (_stdcall *serverStatusCB)(int nStatus, int nError, int nAVCanal, unsigned char nChannelID, struct st_SInfo* pStSInfo, void* pUserData);

/*! \details The prototype of AVAPI2 client status callback function used for an AVAPI2 client.
 *           The AVAPI2 client status callback function is set when an AVAPI2 client register by AVAPI2_RegClientStatusCB().
 *
 *  \param nStatus      [in] AVAPI2 client status
 *                           - #define in enum AVAPI2_CLIENT_STATUS
 *  \param nError       [in] The error code
 *      if nStatus == AVAPI2_CLIENT_STATUS_LOGIN_FAILED
 *          nError will be
 *              - #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due to incorrect view account or password
 *
 *      if nStatus == AVAPI2_CLIENT_STATUS_RECV_FRAME_BLOCK
 *          nError will return blocking time in millisecond
 *
 *      if nStatus == AVAPI2_CLIENT_STATUS_RECV_AUDIO_BLOCK
 *          nError will return blocking time in millisecond
 *
 *      if nStatus == AVAPI2_CLIENT_CONNECT_UID_ST_FAILED
 *          nError will be one of these
 *              - #IOTC_ER_FAIL_SETUP_RELAY Fails to connect the device via relay mode
 *              - #IOTC_ER_NO_PERMISSION The specified device does not support TCP relay
 *              - #IOTC_ER_TIMEOUT No connection is established from clients before timeout expires
 *
 *  \param nAVCanal     [in] Index of AVAPI2 canal
 *  \param nChannelID   [in] ChannelID used by this AVAPI2 canal
 *  \param pStSInfo     [in] Session infomation structure , defined in IOTCAPIs.h struct st_SInfo
 *  \param pUserData    [in] The user data register by AVAPI2_RegUserData
 *
 *  \return 0 if handle success and -1 if fail.
 */
typedef int (_stdcall *clientStatusCB)(int nStatus, int nError, int nAVCanal, unsigned char nChannelID, struct st_SInfo* pStSInfo, void* pUserData);

/*! \details The prototype of send IOCtrl status call back.
 *           The status callback function is set when send IOCtrl by AVAPI2_SendIOCtrlWithCallBack().
 *
 *  \param nError       [in] Error code
 *  \param nAVCanal     [in] AVAPI index
 *  \param nIOCtrlType  [in] The type of IO control
 *  \param pUserData    [in] The user data register by AVAPI2_RegUserData
 *
 *  \return 0 if handle success and -1 if fail.
 */
typedef int (_stdcall *ioCtrlSendStautsCB)(int nError, int nAVCanal, unsigned int nIOCtrlType, void* pUserData);

/* ============================================================================
 * Function Declaration
 * ============================================================================
 */

/*! \brief Get IOTC and AVAPI2 version string
 *
 *  \param szVersion    [out] IOTC and AVAPI2 Version String
 *  \param nLen         [in] buffer length of szVersion
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG szVersion is null or nLen less than 1.
 */
AVAPI2_API int AVAPI2_GetVersion(char* szVersion, int nLen);

/*! \brief Set AVAPI2 module connected canal limit
 *
 *  \details This function is used by AVAPI2 servers or AVAPI2 clients to set AVAPI2 module connected canal limit
 *          and shall be called before any AVAPI2 module related function is invoked.
 *
 *  \param nSessionLimit [in] The max number of IOTC Session.
 *                            If it is specified less than 1, AVAPI2 will set max number of IOTC session as 128.
 *  \param nChannelLimit [in] The max number of channel in IOTC Session.
 *                            If it is specified less than 1, AVAPI2 will set max number of channel as 32.
 *
 *  \return The actual maximum number of AVAPI2 connected channels to be set.
 *  \return Error code if return value < 0
 *			- #IOTC_ER_FAIL_CREATE_THREAD Fails to create threads
 *			- #IOTC_ER_NOT_ENOUGH_MEMORY Not enough memory
 *			- #IOTC_ER_RESOURCE_ERROR IOTC module has some resource allocating problem
 *          - #AV_ER_ALREADY_INITIALIZED AVAPI2 module is already initialized
 *			- #AV_ER_NO_PERMISSION The AVAPI2 module does not support Lite UID
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *
 *  \attention This function will initialize AV module and IOTC module both.
 */
AVAPI2_API int AVAPI2_SetCanalLimit(int nSessionLimit, int nChannelLimit);

/*! \brief Initial an AVAPI2 Server
 *
 *  \param nSessionLimit [in] The max number of IOTC Session.
 *                            If it is specified less than 1, AVAPI2 will set max number of IOTC session as 128.
 *  \param nChannelLimit [in] The max number of channel in IOTC Session.
 *                            If it is specified less than 1, AVAPI2 will set max number of channel as 32.
 *  \param nUDPPort      [in] Specify a UDP port. Random UDP port is used if it is specified as 0.
 *
 *  \return The actual maximum number of AVAPI2 connected channels to be set.
 *  \return Error code if return value < 0
 *          - #IOTC_ER_FAIL_CREATE_THREAD Fails to create threads
 *          - #IOTC_ER_NOT_ENOUGH_MEMORY Not enough memory
 *          - #IOTC_ER_RESOURCE_ERROR IOTC module has some resource allocating problem
 *          - #AV_ER_ALREADY_INITIALIZED AVAPI2 module is already initialized
 *          - #AV_ER_NO_PERMISSION The AVAPI2 module does not support Lite UID
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *
 *  \attention This function will initialize AV module and IOTC module both.
 */
AVAPI2_API int AVAPI2_ServerInitial(int nSessionLimit, int nChannelLimit, unsigned short nUDPPort);

/*! \brief Start an AVAPI2 Server
 *
 *  \details Start an AVAPI2 Server and listen client connect
 *           AVAPI2 module will use pServerStatusCB to return server status and error code.
 *           AVAPI2 module will use pCanalStatusCB to return canal status and error code.
 *
 *  \param szUID                [in] The UID of device that running AVAPI2 server
 *  \param nServerInitChannel   [in] The channel ID of the channel to create AVAPI2 server
 *  \param nServerType          [in] The user-defined service type.
 *                                   An AV client will get this value when it invokes avClientStart() successfully.
 *  \param pfnAuthCB            [in] Call back function for handling client authentication
 *  \param pfnServerStatusCB    [in] Call back function for return server status
 *  \param pfnCanalStatusCB     [in] Call back function for return canal status
 *  \param pfnIoCtrlRecvCB      [in] Call back function for handling IO control received
 *
 *  \return IOTC_ER_NoERROR if server start successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AVAPI2 module does not support Lite UID
 *          - #AV_ER_INVALID_ARG szUID is NULL or pAuthCB is NULL or pServerStatusCB is NULL
 *          - #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 */
AVAPI2_API int AVAPI2_ServerStart(char* szUID, int nServerInitChannel, int nServerType, authCB pfnAuthCB, serverStatusCB pfnServerStatusCB, canalStatusCB pfnCanalStatusCB, ioCtrlRecvCB pfnIoCtrlRecvCB);

/*! \brief Stop an AV Canal
 *
 *  \param nAVCanal   [in] The nAVCanal to be stop
 *
 *  \return IOTC_ER_NoERROR if canal stop successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NO_PERMISSION The AVAPI2 module does not support Lite UID
 */
AVAPI2_API int AVAPI2_ServerStopCanal(int nAVCanal);

/*! \brief Forced an canal to exit before client connect.
 *
 *  \details Application won't get nAVCanal at this step, so use nSessionID and nChannelID to find canal.
 *
 *  \param nSID         [in] Session ID
 *  \param nChannelID   [in] The IOTC channel ID for the channel connecting to the server
 */
AVAPI2_API void AVAPI2_ServerExitCanal(int nSID, unsigned char nChannelID);

/*! \brief Stop AVAPI2 server and release all exist canal
 *
 *  \attention This function will deinitialize AV module and IOTC module.
 */
AVAPI2_API void AVAPI2_ServerStop();

/*! \brief Set AVAPI2 Server Video Pre-Buffer Size
 *
 *  \param nAVCanal         [in] The nAVCanal to be set
 *  \param nVideoPreBufSize [in] The maximum size of video pre-buffer, in unit of kilo-byte
 *
 *  \return IOTC_ER_NoERROR if set pre-buffer successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AVAPI2 module does not support Lite UID or resend mode is disabled.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *
 *  \attention Video Pre-Buffer is used for doing video audio time sync and congestion control.
 */
AVAPI2_API int AVAPI2_ServerSetVideoPreBufSize(int nAVCanal, unsigned int nVideoPreBufSize);

/*! \brief Set AVAPI2 Server Audio Pre-Buffer Size
 *
 *  \param nAVCanal         [in] The nAVCanal to be set
 *  \param nAudioPreBufSize [in] The maximum size of audio pre-buffer, in unit of kilo-byte
 *
 *  \return IOTC_ER_NoERROR if set pre-buffer successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AVAPI2 module does not support Lite UID or resend mode is disabled.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *
 *  \attention Audio Pre-Buffer is used for doing video audio time sync and congestion control.
 */
AVAPI2_API int AVAPI2_ServerSetAudioPreBufSize(int nAVCanal, unsigned int nAudioPreBufSize);

/*! \brief Set AVAPI2 Server IOCtrl Buffer Size
 *
 *  \param nAVCanal         [in] The nAVCanal to be set
 *  \param nIOCtrlBufSize   [in] The maximum size of IOCtrl buffer, in unit of kilo-byte
 *
 *  \return IOTC_ER_NoERROR if set IOCtrl buffer successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AVAPI2 module does not support Lite UID
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 */
AVAPI2_API int AVAPI2_ServerSetIoCtrlBufSize(int nAVCanal, unsigned int nIOCtrlBufSize);

/*! \brief Set AVAPI2 Server IOCtrl Buffer Size
 *
 *  \param nAVCanal [in] The nAVCanal to be set
 *  \param nMode    [in] The congestion control mode to be set
 *
 *  \return IOTC_ER_NoERROR if set IOCtrl buffer successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AVAPI2 module does not support Lite UID or resend mode is disabled.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal or nMode is not valid
 */
AVAPI2_API int AVAPI2_ServerSetCongestionCtrlMode(int nAVCanal, AVAPI2_CONGESTION_CTRL nMode);

/*! \brief Set Dynamic Adaptive Streaming over AVAPI
 *
 *  \param nAVCanal [in] The nAVCanal to be set
 *  \param nMode    [in] The congestion control mode to be set
 *  \param nCleanBufferCondition [in] Set clean buffer condition in secs, must bigger than 3 secs and smaller than 9 secs
 *  \param nCleanBufferRatio [in] Set clean buffer ratio, must bigger than 50 percent and smaller than 100 percent
 *  \param nAdjustmentKeepTime [in] Set adjustment AV_DASA_LEVEL keep time, must bigger than 5 secs
 *  \param nIncreaseQualityCond [in] Set accumulation network stable time for increasing AV_DASA_LEVEL, must bigger than 3 secs
 *  \param nDecreaseRatio [in] Set decreasing AV_DASA_LEVEL ratio, set nDecreaseRatio smaller will trigger decreasing more offen.
 *  \param nInitLevel [in] Set initial quality level
 *
 *  \return IOTC_ER_NoERROR if set IOCtrl buffer successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AVAPI2 module does not support Lite UID or resend mode is disabled.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal or nMode is not valid
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function
 */
AVAPI2_API int AVAPI2_ServerSetDASAEnable(int nAVCanal, AVAPI2_CONGESTION_CTRL nMode, int nCleanBufferCondition, int nCleanBufferRatio, int nAdjustmentKeepTime, int nIncreaseQualityCond, int nDecreaseRatio, int nInitLevel);

/**
* \brief Dynamic Adaptive Streaming over AVAPI Status Reset
*
* \param nAVCanal [in] The nAVCanal to be reset
* \param nLevel [in] Set reset quality level
*
* \return Error code if return value < 0
*          - #AV_ER_INVALID_ARG The AV channel ID is not valid or not support resend
*          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function
*          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
*
* \return #AV_ER_NoERROR if setting successfully
*/
AVAPI2_API int AVAPI2_ServerSetDASAReset(int nAVCanal, int nLevel);

/*! \brief Clean the video and audio buffer of AVAPI2 client and AVAPI2 server.
 *
 *  \details Using by AVAPI2 server to clean the video and audio buffer of AVAPI2 client and AVAPI2 server.
 *
 *  \param nAVCanal [in] The nAVCanal to be clean
 *  \param nTimeout [in] The timeout to clean buffer. default is 10 secs
 *
 *  \return IOTC_ER_NoERROR if clean buffer in processing
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION Resend mode is disabled.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_CLIENT_NO_AVLOGIN An AV client does not pass authentication yet
 *          - #AV_ER_CLEANBUF_ALREADY_CALLED This AV canal is already in processing clean buffer.
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function to clean buffer
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *
 *  \attention AVAPI2 server will return server status AVAPI2_SERVER_STATUS_CLEAN_BUFFER_TIMEOUT or AVAPI2_SERVER_STATUS_CLEAN_BUFFER_SUCCESS by callback function serverStatusCB registered at AVAPI2_ServerStart.
 *             Application should reconnect when clean buffer timeout occurred.
 */
AVAPI2_API int AVAPI2_ServerCleanBuf(int nAVCanal, unsigned int nTimeout);

/*! \brief Clean the video buffer of AVAPI2 client and AVAPI2 server
 *
 *  \details Using by AVAPI2 server to clean the video buffer of AVAPI2 client and AVAPI2 server.
 *
 *  \param nAVCanal [in] The nAVCanal to be clean
 *  \param nTimeout [in] The timeout to clean buffer. default is 10 secs
 *
 *  \return IOTC_ER_NoERROR if clean buffer in processing
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION Resend mode is disabled.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_CLIENT_NO_AVLOGIN An AV client does not pass authentication yet
 *          - #AV_ER_CLEANBUF_ALREADY_CALLED This AV canal is already in processing clean buffer.
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function to clean buffer
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *
 *  \attention AVAPI2 server will return server status AVAPI2_SERVER_STATUS_CLEAN_BUFFER_TIMEOUT or AVAPI2_SERVER_STATUS_CLEAN_BUFFER_SUCCESS by callback function serverStatusCB registered at AVAPI2_ServerStart.
 *             Application should reconnect when clean buffer timeout occurred.
 */
AVAPI2_API int AVAPI2_ServerCleanVideoBuf(int nAVCanal, unsigned int nTimeout);

/*! \brief Clean the audio buffer of AVAPI2 client and AVAPI2 server
 *
 *  \details Using By AVAPI2 Server to clean the Audio Buffer of AVAPI2 Client and AVAPI2 Server.
 *
 *  \param nAVCanal [in] The nAVCanal to be clean
 *  \param nTimeout [in] The timeout to clean buffer. default is 10 secs
 *
 *  \return IOTC_ER_NoERROR if clean buffer in processing
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION Resend mode is disabled.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_CLIENT_NO_AVLOGIN An AV client does not pass authentication yet
 *          - #AV_ER_CLEANBUF_ALREADY_CALLED This AV canal is already in processing clean buffer.
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function to clean buffer
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *
 *  \attention AVAPI2 server will return server status AVAPI2_SERVER_STATUS_CLEAN_BUFFER_TIMEOUT or AVAPI2_SERVER_STATUS_CLEAN_BUFFER_SUCCESS by callback function serverStatusCB registered at AVAPI2_ServerStart.
 *             Application should reconnect when clean buffer timeout occurred.
 */
AVAPI2_API int AVAPI2_ServerCleanAudioBuf(int nAVCanal, unsigned int nTimeout);

/*! \brief Send video frame from AVAPI2 server to AVAPI2 client.
 *
 *  \details An AVAPI2 server uses this function to send video frame to AVAPI2 client.
 *
 *  \param nAVCanal     [in] The nAVCanal to send video frame
 *  \param usCodecID    [in] The video frame codecID, defined in AVFRAMEINFO.h ENUM_CODECID
 *  \param bFrameFlags  [in] The video frame flags, defined in AVFRAMEINFO.h ENUM_FRAMEFLAG
 *  \param pFrameBuf    [in] The data buffer of video frame to be sent
 *  \param nFrameSize   [in] The size of the video frame
 *
 *  \return - #AV_ER_NoERROR if sending successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid or pAudioData is null or nAudioSize is zero
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function to send frame data
 *          - #AV_ER_CLIENT_NO_AVLOGIN An AV client does not pass authentication yet
 *          - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response
 *          - #AV_ER_INVALID_SID The IOTC session ID is not valid
 *          - #AV_ER_EXCEED_MAX_SIZE The audio data to be sent exceeds currently remaining audio data buffer
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 */
AVAPI2_API int AVAPI2_SendFrameData(int nAVCanal, unsigned short usCodecID, unsigned char bFrameFlags, char* pFrameBuf, int nFrameSize);

/*! \brief Send audio data from AVAPI2 server to AVAPI2 client.
 *
 *  \details An AVAPI2 server uses this function to send audio data to AVAPI2 client.
 *
 *  \param nAVCanal     [in] The nAVCanal to send audio data
 *  \param usCodecID    [in] The audio data codecID, defined in AVFRAMEINFO.h ENUM_CODECID
 *  \param bFrameFlags  [in] The Audio data flags =  (samplerate << 2) | (databits << 1) | (channel)
 *                           defined in AVFRAMEINFO.h ENUM_AUDIO_SAMPLERATE , ENUM_AUDIO_DATABITS , ENUM_AUDIO_CHANNEL
 *  \param pAudioData   [in] The data buffer of audio data to be sent
 *  \param nAudioSize   [in] The size of the audio data
 *
 *  \return - #AV_ER_NoERROR if sending successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid or pAudioData is null or nAudioSize is zero
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function to send frame data
 *          - #AV_ER_CLIENT_NO_AVLOGIN An AV client does not pass authentication yet
 *          - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response
 *          - #AV_ER_INVALID_SID The IOTC session ID is not valid
 *          - #AV_ER_EXCEED_MAX_SIZE The audio data to be sent exceeds currently remaining audio data buffer
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *          - #AV_ER_BUFPARA_MAXSIZE_INSUFF Audio slot is full
 */
AVAPI2_API int AVAPI2_SendAudioData(int nAVCanal, unsigned short usCodecID, unsigned char bFrameFlags, char* pAudioData, int nAudioSize);

/*! \brief Sends meta data from AVAPI2 server to AVAPI2 client.
 *
 *  \details An AV server uses this function to send meta data to AV client.
 *
 *  \param nAVCanal     [in] The nAVCanal to send meta data
 *  \param pMetaData    [in] The data buffer of meta data to be sent
 *  \param nMetaSize    [in] The size of the meta data
 *  \param cabMetaInfo  [in] The meta data information sturcture
 *  \param nMetaInfoSize[in] The size of the meta data information sturcture
 *
 *  \return - #AV_ER_NoERROR if sending successfully
 *  \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid or pAudioData is null or nAudioSize is zero
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_CLIENT_NOT_SUPPORT An AV client uses this function to send frame data
 *          - #AV_ER_CLIENT_NO_AVLOGIN An AV client does not pass authentication yet
 *          - #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response
 *          - #AV_ER_INVALID_SID The IOTC session ID is not valid
 *          - #AV_ER_EXCEED_MAX_SIZE The meta data to be sent exceeds currently remaining meta data buffer
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 */
AVAPI2_API int AVAPI2_SendMetaData(int nAVCanal, char* pMetaData, int nMetaSize, const void *cabMetaInfo, int nMetaInfoSize);

/*! \brief Send AVAPI2 IO control
 *
 *  \details This function is used by AVAPI2 server or AVAPI2 client to send a AVAPI2 IO control.
 *
 *  \param nAVCanal         [in] The nAVCanal to send IO control
 *  \param nIOCtrlType      [in] The type of IO control
 *  \param cabIOCtrlData    [in] The buffer of IO control data
 *  \param nIOCtrlDataSize  [in] The length of IO control data
 *
 *  \return #AV_ER_NoERROR if prepare to send successfully
 *  \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV canal is not valid
 *			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds currently remaining IO control buffer.
 *			- #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *			- #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *			- #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *
 *  \attention The IO control data will keep in IO control data buffer and send by task inside of AVAPI2 module.
 */
AVAPI2_API int AVAPI2_SendIOCtrl(int nAVCanal, unsigned int nIOCtrlType, const char *cabIOCtrlData, int nIOCtrlDataSize);

/*! \brief Send AVAPI2 IO control
 *
 *  \details This function is used by AV servers or AV clients to send a AVAPI2 IO control.
 *           Non-Blocking function.
 *
 *  \param nAVCanal         [in] The AV Index of the AV channel to send IO control
 *  \param nIOCtrlType      [in] The type of IO control
 *  \param cabIOCtrlData    [in] The buffer of IO control data
 *  \param nIOCtrlDataSize  [in] The length of IO control data
 *  \param ioCtrlSendStautsCB [in] Call back function of send IOCtrl status
 *
 *  \return #AV_ER_NoERROR if sending successfully
 *  \return Error code if return value < 0
 *			- #AV_ER_INVALID_ARG The AV canal is not valid
 *			- #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *			- #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *			- #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds currently remaining IO control buffer.
 *			- #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *			- #AV_ER_IOTC_SESSION_CLOSED the local IOTC session has been closed
 *			- #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 */
AVAPI2_API int AVAPI2_SendIOCtrlWithCallBack(int nAVCanal, unsigned int nIOCtrlType, const char *cabIOCtrlData, int nIOCtrlDataSize, ioCtrlSendStautsCB pIOCtrlSendStatusCB);

/*! \brief Start an AVAPI2 client (Blocking Function)
 *
 *  \details Start an AVAPI2 client and connect to AVAPI2 server
 *           AVAPI2 module will use pCanalStatusCB to return canal status and error code.
 *
 *  \param nSID             [in] Session ID
 *  \param szUser           [in] The username for authentication
 *  \param szPass           [in] The password for authentication
 *  \param nTimeout         [in] The timeout for this function in unit of second
 *						         Specify it as 0 will make this AV client try connection once and this process will exit immediately if not connection is unsuccessful.
 *  \param pnServType       [out] The user-defined service type set when an AVAPI2 server starts. Can be NULL.
 *  \param nChannelID       [in] The channel ID of the channel to connect AVAPI2 server
 *  \param pnResend         [out] The re-send is enabled or not.
 *  \param pfnCanalStatusCB [in] Call back function of canal connection status
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID and IOTC channel ID.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *          - #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *          - #AV_ER_SERV_NO_RESPONSE The AV server has no response
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response.
 *          - #AV_ER_INVALID_SID The IOTC session ID is not valid
 *          - #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
 *          - #AV_ER_TIMEOUT The timeout specified by nTimeout expires before AV start is performed completely
 *          - #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due to incorrect view account or password
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel, please check if the IOTC channel is used correctly
 *
 *  \attention Application should use IOTC_Connect_ByUID_Parallel() to get nSID before invoke AVAPI2_ClientConnect().
 */
AVAPI2_API int AVAPI2_ClientConnect(int nSID, char *szUser, char *szPass, unsigned int nTimeout, unsigned int *pnServType, int nChannelID, int *pnResend, canalStatusCB pfnCanalStatusCB);

/*! \brief Start an AVAPI2 client (Non-Blocking Function)
 *
 *  \details Start an AVAPI2 client and connect to AVAPI2 server
 *           AVAPI2 module will use pCanalStatusCB to return canal status and error code.
 *
 *  \param szUID            [in] The UID of AVAPI2 server that being to connect
 *  \param szUser           [in] The username for authentication
 *  \param szPass           [in] The password for authentication
 *  \param nTimeout         [in] The timeout for connect to AVAPI2 server in unit of second
 *						         Specify it as 0 will make this AV client try connection once and this process will exit immediately if not connection is unsuccessful.
 *  \param nChannelID       [in] The channel ID of the session to connect AVAPI2 server
 *  \param pfnCanalStatusCB [in] Call back function of canal connection status
 *  \param pfnClientStatusCB[in] Call back function of AVAPI2 client status
 *  \param pUserData        [in] The user data for call back function
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_INVALID_ARG szUID is null or szUser is null or szPass is null or nChannelID < 0
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 *          - #IOTC_ER_NOT_SUPPORT The IOTC library client functions is disabled
 *          - #IOTC_ER_NOT_INITIALIZED The IOTC module is not initialized yet
 *          - #IOTC_ER_UNLICENSE The specified UID of that device is not licensed or expired
 *          - #IOTC_ER_EXCEED_MAX_SESSION The number of IOTC sessions has reached maximum in client side
 *          - #IOTC_ER_DEVICE_NOT_LISTENING The device is not listening for connection now
 *          - #IOTC_ER_FAIL_CONNECT_SEARCH The client stop connecting to the device
 *          - #IOTC_ER_FAIL_RESOLVE_HOSTNAME Cannot resolve masters' host name
 *          - #IOTC_ER_FAIL_CREATE_THREAD Fails to create threads
 *          - #IOTC_ER_CAN_NOT_FIND_DEVICE IOTC servers cannot locate the specified device
 *          - #IOTC_ER_NO_PERMISSION The specified device does not support TCP relay
 *          - #IOTC_ER_FAIL_GET_LOCAL_IP Fails to get the client's local IP address
 *          - #IOTC_ER_FAIL_SETUP_RELAY Fails to connect the device via relay mode
 *          - #IOTC_ER_FAIL_CREATE_SOCKET Fails to create sockets
 *          - #IOTC_ER_FAIL_SOCKET_OPT Fails to set up socket options
 *          - #IOTC_ER_FAIL_SOCKET_BIND Fails to bind sockets
 *          - #IOTC_ER_NOT_SUPPORT_RELAY Not support relay connection by IOTC servers
 *          - #IOTC_ER_INVALID_SID The specified IOTC session ID is not valid
 */
AVAPI2_API int AVAPI2_ClientConnectByUID(char* szUID, char *szUser, char *szPass, unsigned int nTimeout, int nChannelID, canalStatusCB pfnCanalStatusCB, clientStatusCB pfnClientStatusCB, void* pUserData);

/*! \brief Disconnect to AVAPI2 server (Blocking Function)
 *
 *  \details Disconnect to AVAPI2 server by this function, if this canal is no longer required.
 *
 *  \param nAVCanal   [in] The nAVCanal to be stop
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG  The AV canal is not valid
 *
 *  \attention This function only stop canal.
 */
AVAPI2_API int AVAPI2_ClientDisconnect(int nAVCanal);

/*! \brief Disconnect to AVAPI2 server and close IOTC connection (Blocking Function)
 *
 *  \details Disconnect to AVAPI2 server by this function, if this canal is no longer required.
 *           And this function will also close IOTC connection.
 *
 *  \param nAVCanal   [in] The nAVCanal to be stop
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG  The AV canal is not valid
 */
AVAPI2_API int AVAPI2_ClientDisconnectAndCloseIOTC(int nAVCanal);

/*! \brief Forced an AVAPI2 client to exit before connection establish.
 *
 *  \details Application won't get nAVCanal at this step, so use nSessionID and nChannelID to find canal.
 *
 *  \param nSID         [in] Session ID
 *  \param nChannelID   [in] The IOTC channel ID for the channel connecting to the server
 */
AVAPI2_API void AVAPI2_ClientExit(int nSID, unsigned char nChannelID);

/*! \brief Stop AVAPI2 Client and deinitialize AV module and IOTC module (Blocking Function)
 *
 *  \details This function will block and wait all client threads stop, and then deinitialize all AV canals and IOTC sessions.
 */
AVAPI2_API void AVAPI2_ClientStop();

/*! \brief Register AVAPI2 client status call back function
 *
 *  \details AVAPI2 client will return status by this call back function.
 *
 *  \param nAVCanal             [in] The existing nAVCanal to start receive ioCtrl
 *  \param pfnClientStatusCB    [in] Call back function to return client status
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_RegClientStatusCB(int nAVCanal, clientStatusCB pfnClientStatusCB);

/*! \brief Register receive meta data callback function
 *
 *  \details AVAPI2 module will use pRecvMetaCB to handle meta data received.
 *
 *  \param nAVCanal         [in] The existing nAVCanal to receive meta data
 *  \param pfnRecvMetaCB    [in] Call back function of meta data received
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_RegRecvMetaCB(int nAVCanal, metaRecvCB pfnRecvMetaCB);

/*! \brief Start receive IOCtrl by callback function
 *
 *  \details AVAPI2 module will use pIoCtrlRecvCB to handle IOCtrl received.
 *
 *  \param nAVCanal         [in] The existing nAVCanal to start receive ioCtrl
 *  \param pfnIoCtrlRecvCB  [in] Call back function of ioCtrl received
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_RegRecvIoCtrlCB(int nAVCanal, ioCtrlRecvCB pfnIoCtrlRecvCB);

/*! \brief Register user data for callback function
 *
 *  \details AVAPI2 module will return user data at each callback function.
 *
 *  \param nAVCanal         [in] The existing nAVCanal to receive meta data
 *  \param pUserData        [in] The user data for call back function
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_RegUserData(int nAVCanal, void* pUserData);

/*! \brief Start receive video frame by callback function
 *
 *  \details Send IOCtrl IOTYPE_USER_IPCAM_START to AVAPI2 server and start to receive video frame and invoke pRecvFrameCB when video frame received.
 *
 *  \param nAVCanal         [in] The existing nAVCanal to start receive video frame
 *  \param nChannelID       [in] The IOTC channel ID for receive video frame
 *  \param pfnRecvVideoCB   [in] Call back function of video frame received
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_IOTC_SESSION_CLOSED The local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds currently remaining IO control buffer.
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 */
AVAPI2_API int AVAPI2_StartRecvFrame(int nAVCanal, unsigned int nChannelID, videoRecvCB pfnRecvVideoCB);

/*! \brief Stop receive video frame
 *
 *  \details Send IOCtrl IOTYPE_USER_IPCAM_STOP to AVAPI2 server and stop to receive video frame.
 *
 *  \param nAVCanal     [in] The existing nAVCanal to stop receive video frame
 *  \param nChannelID   [in] The IOTC channel ID to stop receive video frame
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_IOTC_SESSION_CLOSED The local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds currently remaining IO control buffer.
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 */
AVAPI2_API int AVAPI2_StopRecvFrame(int nAVCanal, unsigned int nChannelID);

/*! \brief Start receive audio data by callback function
 *
 *  \details Send IOCtrl IOTYPE_USER_IPCAM_AUDIOSTART to AVAPI2 server and start to receive audio data and invoke pfnRecvAudioCB when audio data received.
 *
 *  \param nAVCanal         [in] The existing nAVCanal to start receive audio data
 *  \param nChannelID       [in] The IOTC channel ID for receive audio data
 *  \param pfnRecvAudioCB   [in] Call back function of audio data received
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_IOTC_SESSION_CLOSED The local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds currently remaining IO control buffer.
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 */
AVAPI2_API int AVAPI2_StartRecvAudio(int nAVCanal, unsigned int nChannelID, audioRecvCB pfnRecvAudioCB);

/*! \brief Stop receive audio data
 *
 *  \details Send IOCtrl IOTYPE_USER_IPCAM_AUDIOSTOP to AVAPI2 server and stop to receive audio data.
 *
 *  \param nAVCanal     [in] The existing nAVCanal to stop receive audio data
 *  \param nChannelID   [in] The IOTC channel ID for receive audio data
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_IOTC_SESSION_CLOSED The local IOTC session has been closed
 *          - #AV_ER_IOTC_DEINITIALIZED IOTC has been deinitialized
 *          - #AV_ER_EXCEED_MAX_SIZE The IO control data and type to be sent exceeds currently remaining IO control buffer.
 *          - #AV_ER_MEM_INSUFF Insufficient memory for allocation
 */
AVAPI2_API int AVAPI2_StopRecvAudio(int nAVCanal, unsigned int nChannelID);

/*! \brief Set enable or disable video audio time synchronized
 *
 *  \param nAVCanal [in] The existing nAVCanal to stop receive audio data
 *  \param nEnable  [in] Set 1 to enable audio/video time Synchronized and 0 to disable (default is disable)
 *
 *  \return Error code if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_CLIENT_NOT_SUPPORT This AVAPI2 client not support this function
 *
 *  \attention Enable time synchronized will let video and audio stream recevived not smoothly.
 */
AVAPI2_API int AVAPI2_ClientSetAVTimeSync(int nAVCanal, int nEnable);

/*! \brief Clean video and audio buffer of AVAPI2 client and AVAPI2 server
 *
 *  \details Using by AVAPI2 client to clean video audio buffer of AVAPI2 client only or AVAPI2 client and AVAPI2 server both.
 *
 *  \param nAVCanal         [in] The nAVCanal to be clean
 *  \param nTimeout         [in] The timeout to clean buffer. default is 10 secs
 *  \param nCleanServerBuf  [in] Input 1 to clean video and audio buffer of AVAPI2 server.
 *
 *  \return IOTC_ER_NoERROR if clean buffer in processing
 *  \return Error code if return value < 0
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_CLEANBUF_ALREADY_CALLED This AV canal is already in processing clean buffer.
 *
 *  \attention AVAPI2 client will return client status AVAPI2_CLIENT_STATUS_CLEAN_BUFFER_TIMEOUT by callback function clientStatusCB registered at AVAPI2_RegClientStatusCB().
 *             Application should reconnect when clean buffer timeout occurred.
 */
AVAPI2_API int AVAPI2_ClientCleanBuf(int nAVCanal, unsigned int nTimeout, int nCleanServerBuf);

/*! \brief Clean video buffer of AVAPI2 client and AVAPI2 server
 *
 *  \details Using by AVAPI2 client to clean video buffer of AVAPI2 client only or AVAPI2 client and AVAPI2 server both.
 *
 *  \param nAVCanal         [in] The nAVCanal to be clean
 *  \param nTimeout         [in] The timeout to clean buffer. default is 10 secs
 *  \param nCleanServerBuf  [in] Input 1 to clean Video Buffer of AVAPI Server.
 *
 *  \return IOTC_ER_NoERROR if clean buffer in processing
 *  \return Error code if return value < 0
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_CLEANBUF_ALREADY_CALLED This AV canal is already in processing clean buffer.
 *
 *  \attention AVAPI2 client will return client status AVAPI2_CLIENT_STATUS_CLEAN_BUFFER_TIMEOUT by callback function clientStatusCB registered at AVAPI2_RegClientStatusCB().
 *             Application should reconnect when clean buffer timeout occurred.
 */
AVAPI2_API int AVAPI2_ClientCleanVideoBuf(int nAVCanal, unsigned int nTimeout, int nCleanServerBuf);

/*! \brief Clean audio buffer of AVAPI2 client
 *
 *  \details Using by AVAPI2 client to clean audio buffer of AVAPI2 client only.
 *
 *  \param nAVCanal [in] The nAVCanal to be clean
 *
 *  \return IOTC_ER_NoERROR if clean buffer in processing
 *  \return Error code if return value < 0
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_CLEANBUF_ALREADY_CALLED This AV canal is already in processing clean buffer.
 */
AVAPI2_API int AVAPI2_ClientCleanAudioBuf(int nAVCanal);

/*! \brief Create a new canal connection for sending data to same device with existing nSID
 *
 *  \details This function will block to wait AVAPI2 client login when nResend is set to 0.
 *           For Speaker function, You need to get free channel by AVAPI2_GetFreeChannel() before calling this function.
 *
 *  \param nSID             [in] The existing IOTC Session ID to be create new channel
 *  \param nTimeout         [in] The timeout to create new channel connection
 *  \param nServType        [in] The user-defined service type. An AV client will get this value when it invokes avClientStart() successfully.
 *  \param nChannel         [in] The channel number to be created
 *  \param nResend          [in] Set the resend is enabled or not.
 *  \param pfnServerStatusCB[in] Call back function for return av server status
 *  \param pfnCanalStatusCB [in] Call back function for return av canal status
 *
 *  \return if return value >= 0, create new channel success and return nAVCanal
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID and IOTC channel ID.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *			- #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response.
 *          - #AV_ER_INVALID_SID The IOTC session ID is not valid
 *          - #AV_ER_SERVER_EXIT Users stop this function with avServExit() in another thread
 *          - #AV_ER_TIMEOUT The timeout specified by nTimeout expires before AV start is performed completely
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel, please check if the IOTC channel is used correctly
 *          - #IOTC_ER_CH_NOT_ON The number of IOTC channels has reached maximum
 *          - #IOTC_ER_RESOURCE_ERROR IOTC module has some resource allocating problem
 */
AVAPI2_API int AVAPI2_CreateChannelForSend(int nSID, unsigned int nTimeout, unsigned int nServType, unsigned char nChannel, int nResend, serverStatusCB pfnServerStatusCB, canalStatusCB pfnCanalStatusCB);

/*! \brief Release a canal connection for sending data
 *
 *  \param nAVCanal [in] The nAVCanal to be released
 *
 *  \return IOTC_ER_NoERROR if release successfully
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_ReleaseChannelForSend(int nAVCanal);

/*! \brief Create a new canal connection for receiving data to same device with existing nSID
 *
 *  \param nSID             [in] The existing IOTC Session ID to be create new channel
 *  \param nChannel         [in] The channel number to be created
 *  \param nResend          [in] Set the resend is enabled or not.
 *  \param nTimeout         [in] The timeout to login to AV Server. default is 10 secs
 *  \param pfnClientStatusCB[in] Call back function for return av client status
 *  \param pfnCanalStatusCB [in] Call back function for return av canal status
 *  \param pfnIoCtrlRecvCB  [in] Call back function for handling IO control received
 *  \param pfnRecvFrameCB   [in] Call back function of video frame received
 *  \param pfnRecvAudioCB   [in] Call back function of audio data received
 *
 *  \return if return value >= 0, create new channel success and return nAVCanal
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG The IOTC session ID is incorrect or reuse of IOTC session ID and IOTC channel ID.
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *          - #AV_ER_EXCEED_MAX_CHANNEL The number of AV channels has reached maximum
 *          - #AV_ER_FAIL_CREATE_THREAD Fails to create threads
 *          - #AV_ER_SERV_NO_RESPONSE The AV server has no response
 *          - #AV_ER_SESSION_CLOSE_BY_REMOTE The remote site already closes this IOTC session
 *          - #AV_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response.
 *          - #AV_ER_INVALID_SID The IOTC session ID is not valid
 *          - #AV_ER_CLIENT_EXIT Users stop this function with avClientExit() in another thread
 *          - #AV_ER_TIMEOUT The timeout specified by nTimeout expires before AV start is performed completely
 *          - #AV_ER_WRONG_VIEWACCorPWD The client fails in authentication due to incorrect view account or password
 *          - #AV_ER_NO_PERMISSION the AV module does not support Lite UID
 *          - #AV_ER_IOTC_CHANNEL_IN_USED the IOTC channel has been used by another av channel, please check if the IOTC channel is used correctly
 *          - #IOTC_ER_CH_NOT_ON The number of IOTC channels has reached maximum
 *          - #IOTC_ER_RESOURCE_ERROR IOTC module has some resource allocating problem
 */
AVAPI2_API int AVAPI2_CreateChannelForReceive(int nSID, int nChannelID, int nResend, unsigned int nTimeout, clientStatusCB pfnClientStatusCB, canalStatusCB pfnCanalStatusCB, ioCtrlRecvCB pfnIoCtrlRecvCB, videoRecvCB pfnRecvFrameCB, audioRecvCB pfnRecvAudioCB);

/*! \brief Release a channel connection for receiving data
 *
 *  \param nAVCanal [in] The nAVCanal to be released
 *
 *  \return IOTC_ER_NoERROR if release successfully
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_ReleaseChannelForReceive(int nAVCanal);

/*! \brief Find AV canal by IOTC Session ID & IOTC channel ID
 *
 *  \param nSID     [in] The IOTC Session ID of that device login to AV servers
 *  \param nChannel [in] The channel ID of the IOTC channel
 *
 *  \return Get AV canal if return value >= 0
 *          if return value < 0
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_GetAVCanalBySIDChannel(int nSID, unsigned char nChannel);

/*! \brief Find AV canal by UID & IOTC channel
 *
 *  \param szUID    [in] The UID of that device login to AV servers
 *  \param nChannel [in] The channel ID of the IOTC channel
 *
 *  \return Get AV canal if return value >= 0
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG szUID is null or nChannel is negtive number
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_GetAVCanalByUIDChannel(char* szUID, int nChannel);

/*! \brief Get IOTC Session ID used by AV canal
 *
 *  \param nAVCanal [in] The AV Canal.
 *
 *  \return Get IOTC Session ID if return value >= 0
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_GetSessionIDByAVCanal(int nAVCanal);

/*! \brief Get IOTC channel used by AV canal
 *
 *  \param nAVCanal [in] The AV Canal.
 *
 *  \return Get IOTC channel ID if return value >= 0
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_GetChannelByAVCanal(int nAVCanal);

/*! \brief Get UID used by AV canal
 *
 *  \param nAVCanal [in] The AV Canal.
 *  \param szUID    [out] Get UID result
 *
 *  \return IOTC_ER_NoERROR if get UID successfully
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NO_PERMISSION The AV module does not support Lite UID
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 *			- #IOTC_ER_NOT_INITIALIZED The IOTC module is not initialized yet
 *			- #IOTC_ER_INVALID_SID The specified IOTC session ID is not valid
 *			- #IOTC_ER_SESSION_CLOSE_BY_REMOTE The IOTC session of specified session ID has been closed by remote site
 *			- #IOTC_ER_REMOTE_TIMEOUT_DISCONNECT The timeout expires because remote site has no response
 *			- #IOTC_ER_INVALID_ARG The arguments passed in to this function is invalid.
 */
AVAPI2_API int AVAPI2_GetUIDByAVCanal(int nAVCanal, char* szUID);

/*! \brief Get statistical data send from server to client
 *
 *  \param nAVCanal             [in] The AV Canal.
 *  \param pAnalyticsDataSlot   [out] Get StatisticalDataSlot result
 *
 *  \return IOTC_ER_NoERROR if get UID successfully
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_GetStatisticalData(int nAVCanal, StatisticalDataSlot *pAnalyticsDataSlot);

/*! \brief Get statistical data send from client to server
 *
 *  \param nAVCanal                 [in] The AV Canal.
 *  \param pAnalyticsClientDataSlot [out] Get StatisticalClientDataSlot result
 *
 *  \return IOTC_ER_NoERROR if get UID successfully
 *          if return value < 0
 *          - #AV_ER_INVALID_ARG The AV canal is not valid
 *          - #AV_ER_NOT_INITIALIZED AV module is not initialized yet
 */
AVAPI2_API int AVAPI2_GetStatisticalClientData(int nAVCanal, StatisticalClientDataSlot *pAnalyticsClientDataSlot);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _AVAPIs2_H_ */

