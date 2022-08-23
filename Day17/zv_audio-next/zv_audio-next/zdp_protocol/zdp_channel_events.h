
#ifndef _ZDP_CHANNEL_EVENT_H_
#define _ZDP_CHANNEL_EVENT_H_


#include <stdint.h>


typedef enum {
	/* for channel status */
	emZdpEvent_ChannelStopped		= 0x0000,
	emZdpEvent_ChannelStarting		= 0x0001,	/* 正在启动 */
	emZdpEvent_ChannelStarted		= 0x0002,	/* 已经启动 */
	emZdpEvent_ChannelConnected		= 0x0003,	/* 连接服务器成功 */
	emZdpEvent_ChannelRegistered	= 0x0004,	/* 注册端口成功 */
	emZdpEvent_ChannelRunning		= 0x0005,	/* 正在工作 */
	emZdpEvent_ChannelRestarting	= 0x0006,	/* 正在重启 */
	emZdpEvent_ChannelStopping		= 0x0007,	/* 正在停止 */
	emZdpEvent_ChannelDisconnected	= 0x0008,	/* 网络连接断开 */
	emZdpEvent_ChannelBlocked		= 0x0009,	/* 网络阻塞 */

	/* for main channel */
	emZdpEvent_Main					= 0x0100,

	/* for display channel */
	emZdpEvent_Display				= 0x0200,

	/* for inputs channel */
	emZdpEvent_Inputs				= 0x0300,

	/* for inputs channel */
	emZdpEvent_Cursor				= 0x0400,

	/* for usbredir channel */
	emZdpEvent_Usbredir				= 0x0500,

	/* for clipboard channel */
	emZdpEvent_ClipBoard			= 0x0600,

	/* for audio channel */
	emZdpEvent_Audio				= 0x0700,

	/* for video channel */
	emZdpEvent_Video				= 0x0A00,
} ZDP_EVENT_TYPE;


typedef void(*zdp_event_callback)(uint16_t type, void* event, void* user);


#endif /* _ZDP_CHANNEL_TYPE_H_ */
