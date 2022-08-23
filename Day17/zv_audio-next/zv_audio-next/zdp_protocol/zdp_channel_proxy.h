
#ifndef _ZDP_CHANNEL_PROXY_H_
#define _ZDP_CHANNEL_PROXY_H_


#include <stdint.h>


/***************************************************************************
 * ZDP protocol
 */

/* ZDP Proxy Cmds */
typedef enum {
	ZdpCmd_Proxy_Unknow			= 0x00,

	/* client to server */
	ZdpCmd_Proxy_Connect		= 0xAA,		/* 请求连接代理 */
} ZDP_CMD_PROXY;


/* ZdpCmd_Proxy_Connect
 * 请求代理一个会话
 */
typedef struct {
	uint16_t type;
	uint16_t addr_len;			/* IP地址长度 */
	uint16_t license_len;		/* license长度 */
	uint16_t port;				/* 会话代理端口 */
	uint8_t addr[64];			/* 会话代理IP */
	uint8_t license[512];		/* 授权license */
} PROXY_CONNECT;


#endif /* _ZDP_CHANNEL_MAIN_H_ */
