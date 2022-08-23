
#ifndef _ZVA_EVENT_H_
#define _ZVA_EVENT_H_


#include <stdint.h>


enum {
	/* for  status */
	emZvaEvent_Stopped		= 0x0000,
	emZvaEvent_Starting		= 0x0001,	/* 正在启动 */
	emZvaEvent_Started		= 0x0002,	/* 已经启动 */
	emZvaEvent_Connected	= 0x0003,	/* 连接服务器成功 */
	emZvaEvent_Registered	= 0x0004,	/* 注册端口成功 */
	emZvaEvent_Running		= 0x0005,	/* 正在工作 */
	emZvaEvent_Restarting	= 0x0006,	/* 正在重启 */
	emZvaEvent_Stopping		= 0x0007,	/* 正在停止 */
	emZvaEvent_Disconnected	= 0x0008,	/* 网络连接断开 */
	emZvaEvent_Blocked		= 0x0009,	/* 网络阻塞 */
};


class zva_event
{
public:
	zva_event(uint16_t type) : _type(type) { }
	virtual ~zva_event() { }
	uint16_t _type;
};


typedef void(*zva_event_callback)(const zva_event* event, void* user);


#endif	// _ZVA_EVENT_H_
