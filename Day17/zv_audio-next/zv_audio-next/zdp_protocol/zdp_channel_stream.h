
#ifndef _ZDP_CHANNEL_STREAM_H_
#define _ZDP_CHANNEL_STREAM_H_


#include <stdint.h>
#include "zdp_interface.h"
#include "zdp_channel_types.h"


/***************************************************************************
 * ZDP protocol
 */

/* ZDP Stream Cmds */
typedef enum {
	/* client to server/guest */
	ZdpCmd_Stream_Register		= 0x01,		/* register stream index */
	ZdpCmd_Stream_Start			= 0x02,		/* start stream push */
	ZdpCmd_Stream_Stop			= 0x03,		/* stop stream push */
	ZdpCmd_Stream_KeyFrame		= 0x04,		/* request a key frame */
	ZdpCmd_Stream_Frame			= 0x05,		/* request a frame */

	ZdpCmd_Stream_Config_Get	= 0x10,		/* 获取视频编码配置 */
	ZdpCmd_Stream_Config_Set	= 0x11,		/* 设置视频编码配置 */

	/* server to client */
	ZdpCmd_Stream_ResetPlayer	= 0x21,		/* 重置播放器 */
} ZDP_CMD_STREAM;


/* ZdpCmd_Stream_Register
 * 注册视频推送
 */
typedef struct {
	uint8_t index;
	uint8_t reserved[15];
} STREAM_REGISTER_INFO;


typedef enum {
	emControl_Fps			= (1 << 0),
	emControl_Bps			= (1 << 1),
	emControl_Cfr			= (1 << 2),
	emControl_Idr			= (1 << 3),
	emControl_Profile		= (1 << 4),
	emControl_Endcoder		= (1 << 5),
	emControl_Quality		= (1 << 6),
	emControl_Cursor		= (1 << 7),
} STREAM_CONFIG_FLAG;

/* ZdpCmd_Stream_Config_Get ack
 * ZdpCmd_Stream_Config_Set
 * ZdpCmd_Stream_Config_Set ack
 * 获取视频编码配置回复
 * 设置视频编码配置
 * 设置视频编码配置成功回复
 */
typedef struct {
	uint32_t ctrl_flag;		/* STREAM_CONFIG_FLAG */
	uint32_t fps;			/* 视频帧率 */
	uint32_t bps;			/* 视频码率 */
	uint32_t cfr;			/* 动态帧率 */
	uint32_t idr;			/* 视频帧数(关键帧间隔) */
	uint32_t profile;		/* 编码质量 */
	uint32_t encoder;		/* 编码格式 */
	uint32_t quality;		/* */
	uint32_t cursor;		/* 光标模式 */
	uint8_t reserved[28];
} STREAM_CONFIG_INFO;


/* zdp_send_frame */
typedef struct {
	uint8_t index;
	uint8_t encode_type;
	uint8_t format_type;
	uint8_t profile;
	uint16_t width;
	uint16_t height;
	uint8_t reserved[56];
} STREAM_FRAME_INFO;


/***************************************************************************
 * Frame Header
 *
 * 0__________________8_________________16_________________24_________________31
 * |    index(8bit)   | encode type(8bit)| frame type(8bit) |  profile(8bit)   |
 * |__________________|__________________|__________________|__________________|
 * |            width(16bit)             |            height(16bit)            |
 * |_____________________________________|_____________________________________|
 * |                              frame len(32bit)                             |
 * |___________________________________________________________________________|
 * |                            timestamp usec(32bit)                          |
 * |___________________________________________________________________________|
 *
 */
typedef struct {
	uint8_t index;
	uint8_t encode_type;
	uint8_t format_type;
	uint8_t profile;
	union {
		uint16_t width;
		uint8_t width_c[2];
	};
	union {
		uint16_t height;
		uint8_t height_c[2];
	};
	union {
		uint32_t frame_len;
		uint8_t frame_len_c[4];
	};
	union {
		uint32_t timestamp;
		uint8_t timestamp_c[4];
	};
} STREAM_FRAME_HEAD, *STREAM_FRAME_HEAD_P;


#endif /* _ZDP_CHANNEL_STREAM_H_ */
