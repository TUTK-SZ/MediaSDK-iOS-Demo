/**====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
*
* KalayVpnAPIs.h
*
* Copyright (c) by TUTK Co.LTD. All Rights Reserved.
*
* \brief       KalayVpnAPIs declaration
*
* \description KalayVpnAPIs declaration
*              
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
#ifndef __KALAYVPN_API_H__
#define __KALAYVPN_API_H__

#include <stdint.h>

#ifdef _WIN32
	#ifdef KALAY_STATIC_LIB
		#define KALAY_API
	#elif defined KALAY_API_EXPORTS
		#define KALAY_API __declspec(dllexport)
	#else
		#define KALAY_API __declspec(dllimport)
	#endif
#else
	#define KALAY_API
#endif /* _WIN32 */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* ============================================================================
 * Error Code Declaration
 * ============================================================================
 */
#define KVERR                                      int

/** The function is performed successfully. */
#define	KVPN_ER_NoERROR								 0

/** KalayVPN gets internal module errors. */
#define	KVPN_ER_INTERNAL_MODULE_FAIL				-1

/** KalayVPN module is not initialized yet. Please use KalayVPN_Initialize() for initialization. */
#define KVPN_ER_NOT_INITIALIZED                     -2

/** KalayVPN module is already initialized. It is not necessary to re-initialize. */
#define KVPN_ER_ALREADY_INITIALIZED                 -3

/** KalayVPN_Bind() isn't called. */
#define KVPN_ER_BIND_NOT_CALLED						-4

/** The specified UID is not licensed or expired. */
#define KVPN_ER_UID_UNLICENSE                       -5

/** The specified Kalay Session ID (KSID) is not valid or the KSID of the session is closed. */
#define KVPN_ER_INVALID_KSID                        -6

/** No connection is established from clients before timeout expires */
#define KVPN_ER_LISTEN_TIMEOUT		                -7

/**  Cannot find the specified KalayVPN server, probably caused by disconnection or server does not login yet. */
#define KVPN_ER_CANNOT_FIND_SERVER                  -8

/** The remote site already closes this session. */
#define KVPN_ER_SESSION_CLOSE_BY_REMOTE             -9

/** This session is disconnected due to remote site has no any response after a specified timeout expires. */
#define KVPN_ER_REMOTE_TIMEOUT_DISCONNECT           -10

/** The client fails to connect to a device due to device is not listening for connections. */
#define KVPN_ER_DEVICE_NOT_LISTENING                -11

/** Network is unreachable, please check the network settings */
#define	KVPN_ER_NETWORK_UNREACHABLE     			-12

/** A client fails to connect to a server via relay mode */
#define KVPN_ER_FAIL_SETUP_RELAY					-13

/** Not support relay connection */
#define KVPN_ER_NOT_SUPPORT_RELAY					-14

/** The arguments passed to a function is invalid. */
#define KVPN_ER_INVALID_ARG                         -15

/** The session was closed. */
#define KVPN_ER_SESSION_CLOSED						-16

/** The function is aborted by related function. */
#define KVPN_ER_ABORTED								-17

/** The buffer size exceed maximum packet size. */
#define KVPN_ER_EXCEED_MAX_PACKET_SIZE				-18

/** KalayVPN module has some resource allocating problem. */
#define KVPN_ER_RESOURCE_ERROR						-19

/** KalayVPN module gets system socket error. */
#define KVPN_ER_SYSTEM_SOCKET_ERROR                 -20

/** Portmapping ID doesn't exist. */
#define KVPN_ER_PORTMAPPING_ID_NOT_EXIST            -21

/** Portmapping service isn't supported. */
#define KVPN_ER_PORTMAPPING_SERVICE_NOT_SUPPORT     -22

/** Authentication fail. */
#define KVPN_ER_AUTH_FAIL						    -23

/** Authentication fail if account or password is NULL / empty string */
#define KVPN_ER_AUTH_INVALID_ACCOUNT_PWD		    -24

/** Authentication if account or password length > 128 */
#define KVPN_ER_AUTH_INVALID_ACCOUNT_PWD_LEN	    -25

/** KalayVPN Manager is invalid or has been recycled. */
#define KVPN_ER_INVALID_KALAYVPN_MANAGER		    -26

/** Reached maximum available connections  */
#define KVPN_ER_EXCEED_MAX_AVAILABLE_CONNECTIONS    -27

/** Servers have no response  */
#define KVPN_ER_SERVER_NOT_RESPONSE				    -28

/** No server information while client is connecting  */
#define KVPN_ER_NO_SERVER_LIST				    	-29

/** All master servers have no response  */
#define KVPN_ER_MASTER_NOT_RESPONSE			    	-30

/** Device is in sleep mode  */
#define KVPN_ER_DEVICE_IS_SLEEP				    	-31


/**
 *	Typedef KalayVPN Manager
 */
typedef void* KVManager;


/**
 * \details KVServiceType, used in KalayVPN_PortMapping_Service()
 *			to specify what kind of service for portmapping.
 */
typedef enum {
	KV_SERVICE_TCP 		= 0,
	KV_SERVICE_UDP		= 1,
	KV_SERVICE_FTP		= 2,
	KV_SERVICE_RTSP		= 3
} KVServiceType;


/**
 * \details KVConfigType, used in KalayVPN_SetConfigOpt()
 *			to specify what kind of configuration for setting.
 */
typedef enum {
	KV_CONFIG_TUNNEL_BUF_SIZE 		= 0,
	KV_CONFIG_MAX_STREAM_SNDBUF 	= 1
} KVConfigType;


/**
 * \details KVConnMode, used by KalayVPN_GetConnInfo()
 */
typedef enum {
	KV_CONN_LAN 		= 0,
	KV_CONN_P2P			= 1,
	KV_CONN_UDP_RLY		= 2,
	KV_CONN_TCP_RLY		= 3
} KVConnMode;


/**
 * \details KVConnInfo, used by KalayVPN_GetConnInfo()
 */
typedef struct st_KVConnInfo {
	KVConnMode	ConnMode;		// Current connection mode. It might be changed since TryP2PTimeout isn't zero.
	uint8_t		TryP2PTimeout;	// The timeout in unit of second which trying to establish P2P.
								// TryP2PTimeout > 0, it's still trying to establish P2P connection.
								// TryP2PTimeout = 0, it stops trying to establish P2P connection.
	uint8_t		LocalNatType;	// Local Nat Type.  0: Unknown type, 1: Type 1, 2: Type 2, 3: Type 3, 10: TCP only
	uint8_t		RemoteNatType;	// Remote Nat Type. 0: Unknown type, 1: Type 1, 2: Type 2, 3: Type 3, 10: TCP only
	uint32_t	RemoteVersion;	// Remote KalayVPN version. 
	char		RemoteIP[46];	// The IP address of remote site during this connection
	uint16_t	RemotePort;		// The port number of remote site during this connection
} KVConnInfo;


/**
 * \details The callback function of verify authentication data. 
 * 			Used by KalayVPN server to define their own rule of how to verify authentication data.
 *
 * \param data	  [out] the authentication account from KalayVPN client.
 * \param length  [out] the authentication password from KalayVPN client.
 *
 * \attention User define value.
 * 			  - if value == 0, it means KalayVPN server passes authentication from KalayVPN Client.
 *      	  - if value < 0, it means KalayVPN server rejects authentication from KalayVPN Client.
 */
typedef int (*KVAuthCB)(const char* account, const char* password);


/**
 * \brief Get KalayVPN version
 *
 * \param None
 * 
 * \return KalayVPN current version
 */
KALAY_API char* KalayVPN_GetVersion();


/**
 * \brief Initialize KalayVPN module
 *
 * \param maxConnections [in] The max connections can be established. <br>
 *                            The range is 0 <= maxConnections <= 128.  <br> 
 *                            maxConnections = 0 means using default value 128.
 *
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_ALREADY_INITIALIZED 				KalayVPN module is already initialized.
 *			- #KVPN_ER_EXCEED_MAX_AVAILABLE_CONNECTIONS Setting maxConnections exceeds 128.
 *			- #KVPN_ER_INTERNAL_MODULE_FAIL 			KalayVPN module gets internal module errors.
 *
 * \attention This function is the key entry of whole KalayVPN module. 		<br>
 * 			  That means, if you want to use other functions of KalayVPN, 	<br>
 * 			  you should use this function to initialize KalayVPN           <br>
 *			  module first before calling other functions.
 */
KALAY_API KVERR KalayVPN_Initialize(const unsigned int maxConnections);


/**
 * \brief DeInitialize KalayVPN service
 *
 * \param None.
 *
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 			KalayVPN module isn't initialized.
 * 
 * \attention KalayVPN_DeInitialize() will release all KalayVPN resources.		<br>
 * 			  It must be called in the end of module to avoid resource leakage.
 */
KALAY_API KVERR KalayVPN_DeInitialize();


/**
 * \brief KalayVPN server binds specific UID
 *
 * \param UID [in] KalayVPN server's UID. UID Length is 20.
 *
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 			KalayVPN module isn't initialized.
 *			- #KVPN_ER_INVALID_ARG 				The arguments passed to a function is invalid.
 *			- #KVPN_ER_UID_UNLICENSE			The specified UID is not licensed or expired.
 *			- #KVPN_ER_RESOURCE_ERROR 			KalayVPN module has some resource allocating problem.
 * 
 * \attention (1) This function is a non-blocking function. <br>
 * 			  (2) This function only be used on server site. 
 */
KALAY_API KVERR KalayVPN_Bind(const char* UID);


/**
 * \brief Used by a KalayVPN server to listen connections from KalayVPN clients.
 *
 * \param msWaitTimeout [in] The timeout for this function in unit of millisecond, give 0 means block forever.
 * \param auth_callback [out] The authentication callback with data from KalalyVPN client.
 *
 * \return KalayVPN Session ID (KSID) if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 					The KalayVPN module is not initialized yet
 *			- #KVPN_ER_INTERNAL_MODULE_FAIL 			KalayVPN module gets internal module errors.
 *			- #KVPN_ER_BIND_NOT_CALLED					KalayVPN_Bind() isn't called.
 *			- #KVPN_ER_LISTEN_TIMEOUT 					No connection is established from clients before timeout expires.
 *   		- #KVPN_ER_AUTH_FAIL						Authentication is failed.
 *			- #KVPN_ER_EXCEED_MAX_AVAILABLE_CONNECTIONS Exceed max available listen connections.
 * 
 * \attention (1) This function is a blocking function. <br>
 * 			  (2) It waits for following two conditions happen before executing sequential instructions: <br>
 *			      (a) any connection from clients is established. <br>
 *			      (b) timeout expires.
 */
KALAY_API KVERR KalayVPN_Listen(const unsigned int msWaitTimeout, KVAuthCB auth_callback);


/**
 * \brief Used by a KalayVPN client to connect a KalayVPN server.
 *
 * \param UID      [in] The UID that client would like to connect.
 * \param account  [in] Account for authentication.
 * \param password [in] Password for authentication.
 *
 * \return KalayVPN Session ID (KSID) if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 					The KalayVPN module is not initialized yet.
 *			- #KVPN_ER_INTERNAL_MODULE_FAIL 			KalayVPN module gets internal module errors.
 *			- #KVPN_ER_UID_UNLICENSE 					The specified UID is not licensed or expired.
 *			- #KVPN_ER_DEVICE_NOT_LISTENING 			The client fails to connect to a device due to device is not listening for connections.
 *			- #KVPN_ER_CANNOT_FIND_SERVER 				Cannot find the specified server, probably caused by disconnection or server does not login yet.
 *			- #KVPN_ER_FAIL_SETUP_RELAY 				Fails to connect server via relay mode.
 *			- #KVPN_ER_NOT_SUPPORT_RELAY 				Not support relay connection.
 *			- #KVPN_ER_SERVER_NOT_RESPONSE				Servers have no response.
 *			- #KVPN_ER_NO_SERVER_LIST					No server information while client is connecting.
 *			- #KVPN_ER_MASTER_NOT_RESPONSE				All master servers have no response.
 *			- #KVPN_ER_DEVICE_IS_SLEEP					Device is in sleep mode.
 *   		- #KVPN_ER_AUTH_FAIL						Authentication is failed.
 *     		- #KVPN_ER_AUTH_INVALID_ACCOUNT_PWD 		Authentication fail if account or password is NULL / empty string
 *       	- #KVPN_ER_AUTH_INVALID_ACCOUNT_PWD_LEN 	Authentication if account or password length > 128
 *			- #KVPN_ER_EXCEED_MAX_AVAILABLE_CONNECTIONS Exceed max available connections.
 * 
 * \attention This function is a blocking function.
 */
KALAY_API KVERR KalayVPN_Connect(const char* UID, const char* account, const char* password);


/**
 * \brief Used by KalayVPN client to stop all "connecting connections" to KalayVPN server.
 *
 * \param None
 *
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 				The KalayVPN module is not initialized yet.
 *			- #KVPN_ER_INTERNAL_MODULE_FAIL 		KalayVPN module gets internal module errors.
 *			- #KVPN_ER_SESSION_CLOSE_BY_REMOTE		The remote site already closes this session.
 *			- #KVPN_ER_REMOTE_TIMEOUT_DISCONNECT 	This session is disconnected due to remote site has no any response after a specified timeout expires.
 * 
 * \attention This function is a non-blocking function.
 */
KALAY_API KVERR KalayVPN_Connect_Stop();


/**
 * \brief Used by a KalayVPN client to disconnect an exist connection of KalayVPN server.
 *
 * \param KSID [in] The Kalay Session ID (KSID) that client would like to disconnect. 		<br>
 * 					The KSID is returned from KalayVPN_Connect().
 *
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 				The KalayVPN module is not initialized yet.
 *			- #KVPN_ER_INVALID_KSID					The specified Kalay Session ID (KSID) is invalid.
 * 
 * \attention This function is a non-blocking function.
 */
KALAY_API KVERR KalayVPN_Disconnect(int KSID);


/**
 * \brief Used by KalayVPN server / KalayVPN client to get current connection information.
 *
 * \param KSID 		[in]  The KSID is returned from KalayVPN_Listen() / KalayVPN_Connect().
 * \param connInfo 	[out] The connection info of specified KSID.
 * 
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 				The KalayVPN module is not initialized yet.
 *			- #KVPN_ER_INVALID_ARG 					The arguments passed to a function is invalid.
 *			- #KVPN_ER_SESSION_CLOSE_BY_REMOTE		The remote site already closes this session.
 *			- #KVPN_ER_REMOTE_TIMEOUT_DISCONNECT 	This session is disconnected due to remote site has no any response after a specified timeout expires.
 *
 * \attention This function is a non-blocking function.
 */
KALAY_API KVERR KalayVPN_GetConnInfo(int KSID, KVConnInfo* connInfo);


/**
 * \brief Get KalayVPN Manager
 *
 * \param KSID 	  [in] 	pass a valid KalayVPN Session ID (KSID).
 * \param manager [out]	KVManager handle.
 * 
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 			The KalayVPN module is not initialized yet.
 *			- #KVPN_ER_INVALID_ARG 				The arguments passed to a function is invalid.
 *			- #KVPN_ER_INVALID_KSID				The specified Kalay Session ID (KSID) is not valid or the KSID of the session is closed.
 *			- #KVPN_ER_RESOURCE_ERROR 			KalayVPN module has some resource allocating problem.
 * 
 * \attention (1) This function is a non-blocking function. <br>
 * 			  (2) KVManager is the key role in KalayVPN. <br>
 * 			  	  Either KalayVPN server or KalayVPN client should get at least one KVManager for using. <br>
 * 			      KVManager responses for: <br>
 * 			        - Smart link on LAN / P2P / RLY
 * 			        - NAT Traversal
 * 			        - Message control
 * 			        - Stream data transfer
 * 			        - Routing tasks management
 *
 * \attention This function is a non-blocking function.
 */
KALAY_API KVERR KalayVPN_GetManager(int KSID, KVManager* manager);


/**
 * \brief Start port mapping specific service.
 *
 * \description This function is used by a KalayVPN client to indicate service type for mapping specific service. 
 * 				The KalayVPN client specifies the serveic type / local port in localhost and the remote port
 * 				which provides the service. The KalayVPN client will handle all the handshake detail automatically.
 *
 * \param service     [in] The service type for portmapping.
 * \param manager     [in] KVManager handle.
 * \param local_port  [in] local port which waits for application to connect.
 * \param remote_port [in] remote port which provides specific service.
 *
 * \return KalayVPN PortMapping ID if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 					The KalayVPN module is not initialized yet.
 *			- #KVPN_ER_SYSTEM_SOCKET_ERROR 				KalayVPN module gets system socket error.
 *			- #KVPN_ER_INVALID_ARG 						The arguments passed to a function is invalid.
 *			- #KVPN_ER_INTERNAL_MODULE_FAIL 			KalayVPN module gets internal module errors.
 *			- #KVPN_ER_PORTMAPPING_SERVICE_NOT_SUPPORT	The input service type isn't supported.
 *			- #KVPN_ER_INVALID_KALAYVPN_MANAGER			KalayVPN Manager is invalid or has been recycled.
 *														Using KalayVPN_GetManager() to get a new manager handle.
 *
 * \attention This function is a non-blocking function.
 */
KALAY_API KVERR KalayVPN_PortMapping_Service(KVServiceType service,
											 KVManager manager, 
											 const int local_port, 
											 const int remote_port);


/**
 * \brief Remove port mapping service.
 *
 * \description This function is used by a KalayVPN client to stop port mapping service. 
 *
 * \param manager [in] KVManager handle.
 * \param pm_id   [in] PortMapping ID.
 *
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 			The KalayVPN module is not initialized yet.
 *			- #KVPN_ER_INVALID_ARG 				The arguments passed to a function is invalid.
 *   		- #KVPN_ER_PORTMAPPING_ID_NOT_EXIST Portmapping ID doesn't exist
 *			- #KVPN_ER_INVALID_KALAYVPN_MANAGER	KalayVPN Manager is invalid or has been recycled.
 *												Using KalayVPN_GetManager() to get a new manager handle.
 * 
 * \attention (1) This function is a non-blocking function. <br>
 * 			  (2) If not call KalayVPN_RemovePortMapping local port can't be used until OS release it.
 */
KALAY_API KVERR KalayVPN_RemovePortMapping(KVManager manager, int pm_id);


/**
 * \brief Start KalayVPN Service
 *
 * \param manager  [in] KVManager handle.
 * \param isDetach [in]	If isDetach is true, it's a "non-blocking" function.    <br>
 *     					If isDetach is false, it's a "blocking" function.
 *
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 * 		   - #KVPN_ER_INTERNAL_MODULE_FAIL 		 KalayVPN module gets internal module errors.
 *		   - #KVPN_ER_INVALID_KALAYVPN_MANAGER	 KalayVPN Manager is invalid or has been recycled.
 *												 Using KalayVPN_GetManager() to get a new manager handle.
 *
 * \attention (1) If isDetach is true, it is a non-blocking function. <br>
 *                It always be used in server side for accepting more than one connections. <br>
 * 		  	  (2) If isDetach is false, it is a blocking function.  <br>
 * 		  	      It always be used in client side. <br>
 * 		  	      When KalayVPN_StartService finish tasks or get errors, it will break from KalayVPN_StartService().			
 */
KALAY_API KVERR KalayVPN_StartService(KVManager manager, int isDetach);


/**
 * \brief Stop KalayVPN Service
 *
 * \param manager  [in] KVManager handle.
 * 
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 * 		   - #KVPN_ER_INVALID_ARG 				The arguments passed to a function is invalid.
 * 		   - #KVPN_ER_INTERNAL_MODULE_FAIL 		KalayVPN module gets internal module errors.
 *		   - #KVPN_ER_INVALID_KALAYVPN_MANAGER	KalayVPN Manager is invalid or has been recycled.
 *												Using KalayVPN_GetManager() to get a new manager handle.
 *
 * \attention This function is a non-blocking function.
 */
KALAY_API KVERR KalayVPN_StopService(KVManager manager);


/**
 * \brief Set the absolute path of log file
 *
 * \param path    [in] log path.
 * \param MaxSize [in] The maximum size of log file in Bytes, 0 = unlimited.
 * 
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 * 		   - #KVPN_ER_INVALID_ARG 	The arguments passed to a function is invalid.
 *
 * \attention (1) This function is a non-blocking function. <br>
 *            (2) KalayVPN_Set_Log_Path() "MUST" be set before KalayVPN_Initialize().
 */
KALAY_API KVERR KalayVPN_Set_Log_Path(const char *path, int maxSize);


/**
 * \brief Setting specific configuration of KalayVPN
 *
 * \description This function is used in both KalayVPN server / client to set specific configuration.
 *
 * \param config_type  [in] The configuration type provided by KalayVPN for setting.
 * \param manager      [in] KVManager handle.
 * \param config_value [in] The configuration value for the requested configuration type.
 *
 * \return #KVPN_ER_NoERROR if no error occurs.
 * \return Error code if return value < 0
 *			- #KVPN_ER_NOT_INITIALIZED 					The KalayVPN module is not initialized yet.
 *			- #KVPN_ER_INVALID_ARG 						The arguments passed to a function is invalid.
 *			- #KVPN_ER_INVALID_KALAYVPN_MANAGER			KalayVPN Manager is invalid or has been recycled.
 *														Using KalayVPN_GetManager() to get a new manager handle.
 *
 * \attention (1) This function is a non-blocking function.	<br>
 * 			  (2) Valid value range of configuration :		<br>
 * 			      - KV_CONFIG_TUNNEL_BUF_SIZE, 32 * 1024 (32KB) < config_value < 512 * 1024 (512KB), Default value : 128KB
 * 			      - KV_CONFIG_MAX_STREAM_SNDBUF, 1 * 1024 * 1024 (1MB) < config_value < 8 * 1024 * 1024 (8MB), Default value : 1MB
 */
KALAY_API KVERR KalayVPN_SetConfigOpt(KVConfigType config_type, KVManager manager, const unsigned int config_value);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__KALAYVPN_API_H__

