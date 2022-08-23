
#ifndef _ZDP_ERROR_CODE_H_
#define _ZDP_ERROR_CODE_H_


#define Zdp_Success					0x0000
#define Zdp_Error					0xFFFF

/* session error */
#define Zdp_Err_License				0x0001
#define Zdp_Err_NoSessionType		0x0002
#define Zdp_Err_NoSessionId			0x0003
#define Zdp_Err_OverMaxClient		0x0004
#define Zdp_Err_NoAuthority			0x0005
#define Zdp_Err_NotCtrlSession		0x0006

/* channel error */
#define Zdp_Err_NoChannel			0x1001
#define Zdp_Err_NoMotionMode		0x1002
#define Zdp_Err_NoData				0x1003
#define Zdp_Err_UnpackData			0x1004
#define Zdp_Err_ChannelUsed			0x1005
#define Zdp_Err_ChannelReopen		0x1006
#define Zdp_Err_CmdNotSupport		0x1007
#define Zdp_Err_ChannelMatch		0x1008

/* guest error */


/* proxy error */
#define Zdp_Err_CreateProxy         0x3001
#define Zdp_Err_ProxyRule			0x3002


#endif /* _ZDP_ERROR_CODE_H_ */
