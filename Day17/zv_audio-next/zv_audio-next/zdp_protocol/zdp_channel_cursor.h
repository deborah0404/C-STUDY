
#ifndef _ZDP_CHANNEL_CURSOR_H_
#define _ZDP_CHANNEL_CURSOR_H_


#include <stdint.h>
#include "zdp_interface.h"
#include "zdp_channel_types.h"


/***************************************************************************
 * ZDP protocol
 */

/* ZDP Cursor Cmds */
typedef enum {
	ZdpCmd_Cursor_Init		= 0x01,		/* 初始化 */
	ZdpCmd_Cursor_Reset		= 0x02,		/* 重置 */
	ZdpCmd_Cursor_Set		= 0x03,		/* 设置 */
	ZdpCmd_Cursor_Hide		= 0x04,		/* 隐藏 */
	ZdpCmd_Cursor_Trail		= 0x05,		/*  */
} ZDP_CMD_CURSOR;


/* zdp_cursor_head */
typedef struct tagCursorHead {
	uint32_t unique;			/* 对应光标形状的唯一标示，用于在缓存中存储或检索 */
	uint16_t type;				/* 光标类型 */
	uint8_t flags;				/* 消息字段标识 */
	uint8_t visible;			/* 可见性(1-可见/0-不可见) */
	uint16_t width;				/* 光标宽度 */
	uint16_t height;			/* 光标高度 */
	uint16_t hot_spot_x;		/* 光标热点X位置 */
	uint16_t hot_spot_y;		/* 光标热点Y位置 */
	uint16_t trail_length;		/* 光标轨迹长度 */
	uint16_t trail_frequency;	/* 轨迹更新频率 */
	uint32_t data_size;			/* 数据大小 */
} ZDP_CURSOR_HEAD;

/* ZDP Cursor Type*/
typedef enum {
	ZDP_Cursor_Type_Alpha 		= 0x00,
	ZDP_Cursor_Type_Mono 		= 0x01
} ZDP_CURSOR_TYPE;

/* ZDP Cursor Flags */
typedef enum {
	ZDP_CURSOR_FLAG_NONE		= (1 << 0),		/* 设置无效 */
	ZDP_CURSOR_FLAG_CACHE_ME	= (1 << 1),		/* 客户端应该将这个形状添加到缓存 */
	ZDP_CURSOR_FLAG_FROM_CACHE	= (1 << 2),		/* 客户端应该从光标缓存中获取光标的形状 */
												/* ZDP_CURSOR_INFO结构体中其他字段除了unique都是无效的 */
} ZDP_CURSOR_FLAG;

/* zdp_cursor_info */
typedef struct tagCursorInfo {
	ZDP_CURSOR_HEAD head;
	char* data;					/* 光标形状数据 */
} ZDP_CURSOR_INFO;


/***************************************************************************
 * messages for QEMU
 */

typedef struct ZdpCursorShape {
	ZdpBaseMessage base;

	ZDP_CURSOR_HEAD cursor_head;
	char* cursor_data;
} ZdpCursorShape;


#endif /* _ZDP_CHANNEL_CURSOR_H_ */
