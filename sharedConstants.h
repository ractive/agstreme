// constants for socks proxy servers
#define SOCKS_NUM_AUTH_METHS 2 
#define SOCKS_METH_REPLY_LEN 2 
#define SOCKS_AUTH_REPLY_LEN 2

#define SOCKS5_CONN_REPLY_LEN 10
#define SOCKS4_CONN_REPLY_LEN 8

//responses to a request (such as connect)
#define SOCKS5_REPLY_SUCCESS 0
#define SOCKS5_REPLY_SERVER_FAILURE 1
#define SOCKS5_REPLY_ACCESS_DENIED 2
#define SOCKS5_REPLY_NET_UNREACHABLE 3
#define SOCKS5_REPLY_HOST_UNREACHABLE 4
#define SOCKS5_REPLY_CONN_REFUSED 5
#define SOCKS5_REPLY_TTL_EXPIRED 6
#define SOCKS5_REPLY_COMMAND_NOT_SUPPORTED 7
#define SOCKS5_REPLY_ADDRESS_TYPE_NOT_SUPPORTED 8

#define SOCKS4_REPLY_SUCCESS 90
#define SOCKS4_REPLY_FAILURE 91
#define SOCKS4_REPLY_AUTH_FAILURE 92
#define SOCKS4_REPLY_AUTH_ID_MISMATCH 93


#define SOCKS_VERSION_5 5
#define SOCKS_VERSION_4 4
#define SOCKS_UNAME_AUTH_VER 1

#define SOCKS_NO_AUTH_METH 0
#define SOCKS_UNAME_METH 2
#define SOCKS_NO_ACCEPTABLE_METH 0xff

#define SOCKS_CONNECT_REQ 1
#define SOCKS_ATYP_IPV4 1
#define SOCKS_ATYP_DOMAINNAME 3

// proxy type meanings (type is a field in the proxyInfo struct)
#define NO_PROXY 0
#define SOCKS_V4 1
#define SOCKS_V5_NO_AUTH 2
#define SOCKS_V5_USERNAME 3


// states a connection can be in (connectState var in struct)
#define noConnection				0x00
#define createNewListenSocket		0x01
#define bindPort					0x02
#define listenPort					0x03
#define waitforConnection			0x04

#define	createNewConnectSocket		0x10
#define	connectMachine				0x20
#define connectMachineWait			0x30
#define connectFailed				0x40

#define connectionEstablished		0x11
#define	handshakeA					0x12
#define	handshakeBC					0x13

#define	handshakeD					0x15
#define receivingFile				0x16
#define sendingFile					0x17
#define connectionClosed			0x18
#define flushReceieveBuffer			0x19
#define fileSent					0x20
#define	notUsed						0x21

//constants used when handshaking with the proxy server
#define proxyWaitingForMethReply	0x22
#define proxyWaitingForAuthReply	0x23
#define proxyWaitingForConnectReply 0x24

//server specific states
#define	sendInit					0x100
#define serverWait					0x101
#define sendPing					0x102
#define waitingForAccountStatus		0x103

// mesgs from the UI
#define NEW_ACCOUNT_INFO_FROM_UI 		100
#define TEST_ACCOUNT_FROM_UI 			101
#define LOGIN 							102
#define RESET_SERVER_CONNECTION 		103
#define INIT_RESCAN 					104
#define CHANGE_BANDWIDTH_SETTING 		105
#define KEEP_LOG_FILE 					106
#define PROXY_SETTINGS 					107
#define EX_NEW_ACCOUNT_INFO_FROM_UI 	108

// mesgs to the UI
#define FILE_TRANS_STATUS 1
#define FILE_SCAN_STARTED 2
#define FILE_SCAN_COMPLETE 3
#define FILE_TRANS_COMPLETE 4
#define END_OF_TRANS_STATUS 5
#define VERSION_INFO 6
#define REJECTED_BY_SERVER 7
#define CREATE_ACCOUNT_RESPONSE_TO_UI 150                                                        
#define TEST_ACCOUNT_RESPONSE_TO_UI 151
#define PLAY_SONG_COMMAND_TO_UI 152                
#define PLAY_SONG_LIST_COMMAND_TO_UI 153                
#define MASTER_SERVER_CONN_STATE 154
#define SERVER_CONN_STATE 155

//results of trying to connect to the master server
#define MS_UNKNOWN 0
#define MS_SUCCESS 1
#define MS_INVALID_PORT 2
#define MS_READ_FAILED 3
#define MS_CONNECT_FAILED 4

//results of trying to connect to the master server when there
//is a proxy involved
#define MS_PROXY_S5_NO_AUTH_METHS 5
#define MS_PROXY_S5_METH_READ_FAILED 6
#define MS_PROXY_S5_AUTH_FAILED 7
#define MS_PROXY_S5_AUTH_READ_FAILED 8
#define MS_PROXY_S5_SEND_CONNECT_FAILED 9
#define MS_PROXY_S5_CONNECT_FAILED 10
#define MS_PROXY_S5_CONNECT_READ_FAILED 11
#define MS_PROXY_S4_HOSTNAME_LOOKUP_FAILED 12
#define MS_PROXY_S4_SEND_CONNECT_FAILED 13
#define MS_PROXY_S4_CONNECT_FAILED 14
#define MS_PROXY_S4_CONNECT_READ_FAILED 15
#define MS_PROXY_CONNECT_FAILED 16

#define MS_CONNECT_DELAYED 17

#ifdef UNIX
#define version				"0.520L"
#endif
#ifdef WIN32
#define version				"0.605W"
#endif

