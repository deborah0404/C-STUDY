
#ifndef _ZDP_PROTOCOL_H_
#define _ZDP_PROTOCOL_H_


#include <stdint.h>
#include "zdp_channel_main.h"
#include "zdp_channel_display.h"
#include "zdp_channel_inputs.h"
#include "zdp_channel_cursor.h"
#include "zdp_channel_usbredir.h"
#include "zdp_channel_clipboard.h"
#include "zdp_channel_audio.h"
#include "zdp_channel_stream.h"
#include "zdp_channel_proxy.h"
#include "zdp_error_code.h"


#define ZDP_PROTOCOL_TYPE_MAJOR			0x02
#define ZDP_PROTOCOL_TYPE_MINOR			0x01


/***************************************************************************
 * ZDP Protocol Cmd Header
 *
 * 0__________________8_________________16_________________24_________________31
 * |         marks(16bit)(0x5555)        |            version(16bit)           |
 * |_____________________________________|_____________________________________|
 * |ACK|          |QOS|  head len(8bit)  |             msg id(16bit)           |
 * |___|__________|___|__________________|_____________________________________|
 * |     dest(8bit)   |     src(8bit)    |   channel(8bit)  |    cmd(8bit)     |
 * |__________________|__________________|__________________|__________________|
 * |                              data len(32bit)                              |
 * |___________________________________________________________________________|
 * |    index(8bit)   |                  |           error code(16bit)         |
 * |__________________|__________________|_____________________________________|
 *
 */
typedef struct {
	uint8_t marks[2];			/* 协议标识 */
	uint8_t version[2];			/* 协议版本 */
	uint8_t flags;				/* 标识位 */
	uint8_t head_len;			/* 报文头长度 */
	uint8_t msg_id[2];			/* 消息ID */
	uint8_t dest;				/* 目标模块 */
	uint8_t src;				/* 源模块 */
	uint8_t channel;			/* 消息通道 */
	uint8_t cmd;				/* 消息类型 */
	uint8_t data_len[4];		/* 数据长度 */
	uint8_t index;				/* 通信标识(用于区分多客户端) */
	uint8_t reserved;
	uint8_t error_code[2];		/* 错误码 */
} ZDP_HEAD, *ZDP_HEAD_P;


/* ZDP Module Type */
typedef enum {
	emZdp_Module_Unknow		= 0x00,
	/* host */
	emZdp_Modele_Proxy		= 0xA0,
	emZdp_Module_Host		= 0xA1,
	/* client */
	emZdp_Module_Client		= 0xC1,
	emZdp_Module_Player		= 0xC2,
	emZdp_Module_Usbredir	= 0xC3,
	emZdp_Module_Audio		= 0xC4,
	/* guest */
	emZdp_Module_Guest		= 0xF1
} ZDP_MODULE_TYPE;


/* ZDP Channel Type */
typedef enum {
	emZdp_Channel_Start		= 0x00,
	emZdp_Channel_Main		= 0x01,	/* 实现握手|协议协商|密钥协商|模式协商|探活 */
	emZdp_Channel_Display	= 0x02,	/* 实现显示模式协商 */
	emZdp_Channel_Inputs	= 0x03,	/* 实现鼠标键盘输入 */
	emZdp_Channel_Cursor	= 0x04, /* 实现光标形状设置 */
	emZdp_Channel_UsbRedir	= 0x05,	/* 实现USB重定向 */
	emZdp_Channel_ClipBoard	= 0x06,	/* 实现剪切板重定向 */
	emZdp_Channel_Audio		= 0x07,	/* 实现音频流输入输出 */
	emZdp_Channel_Stream	= 0x0A,	/* 实现视频流输出 */
	emZdp_Channel_Proxy		= 0x0E,	/* 实现网关代理协商 */
	emZdp_Channel_End		= 0x0F
} ZDP_CHANNEL_TYPE;


/* ZDP QOS Level */
typedef enum {
	emZdp_QOS_Low 			= 0,
	emZdp_QOS_Normal 		= 1,
	emZdp_QOS_High 			= 2
} ZDP_QOS_LEVEL;


/* ZDP Packet Info */
typedef struct {
	uint8_t version[2];			/* 协议版本 */
	uint8_t dest;
	uint8_t src;
	uint8_t channel;
	uint8_t cmd;
	uint8_t index;
	uint8_t ack;
	uint8_t qos;
	uint8_t head_len;
	uint32_t data_len;
	uint16_t msg_id;
	uint16_t error_code;
	ZDP_HEAD* head;				/* 报文头 */
	const char* data;			/* 报文数据 */
	const char* buffer;			/* 原始buffer */
} ZDP_PACKET, *ZDP_PACKET_P;


/***************************************************************************
 * ZDP Protocol Frame Header
 *
 * 0__________________8_________________16_________________24_________________31
 * |         marks(16bit)(0xAAAA)        |           packet id(16bit)          |
 * |_____________________________________|_____________________________________|
 * |    flags(8bit)   |  head len(8bit)  |            msg id(16bit)            |
 * |__________________|__________________|_____________________________________|
 * |                             data len(32bit)                               |
 * |___________________________________________________________________________|
 * |                           data offset(32bit)                              |
 * |___________________________________________________________________________|
 * |                            timestamp(32bit)                               |
 * |___________________________________________________________________________|
 *
 */

typedef struct {
	uint8_t marks[2];		/* 0xAAAA-Frame Data */
	uint8_t packet_id[2];	/* only used by udp */
	uint8_t flags;
	uint8_t head_len;
	uint8_t msg_id[2];
	uint8_t data_len[4];
	uint8_t data_offset[4];
	uint8_t time_stamp[4];
} ZDP_FRAME_HEAD, *ZDP_FRAME_HEAD_P;


/* ZDP Frame Packet Info */
typedef struct {
	uint16_t packet_id;
	uint16_t msg_id;
	uint8_t flags;
	uint8_t head_len;
	uint32_t data_len;
	uint32_t data_offset;
	uint32_t time_stamp;
	ZDP_FRAME_HEAD* head;		/* 报文头 */
	const char* data;			/* 报文数据 */
	const char* buffer;			/* 原始buffer */
} ZDP_FRAME_PACKET, *ZDP_FRAME_PACKET_P;


#endif /* _ZDP_PROTOCOL_H_ */
