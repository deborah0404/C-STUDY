
#ifndef _ZDP_CHANNEL_USBREDIR_H_
#define _ZDP_CHANNEL_USBREDIR_H_


#include <stdint.h>
#include "zdp_interface.h"
#include "zdp_channel_types.h"


/***************************************************************************
 * ZDP protocol
 */

/* ZDP Usbredir Cmds */
typedef enum {
	/* client to host */
	ZdpCmd_UsbRedir_GetChannels		= 0x01,		/* 获取USB挂载通道信息 */
	ZdpCmd_UsbRedir_GetRules		= 0x02,		/* 获取USB重定向策略信息 */

	ZdpCmd_UsbRedir_RequestStart	= 0x11,		/* 请求启动USB设备重定向 */
	ZdpCmd_UsbRedir_RequestStop		= 0x12,		/* 请求关闭USB设备重定向 */

	/* client to guest */
	/* host to guest */
	ZdpCmd_UsbRedir_DeviceAdd		= 0x21,		/* USB设备接入通知 */
	ZdpCmd_UsbRedir_DeviceRemove	= 0x22,		/* USB设备拔出通知 */
	ZdpCmd_UsbRedir_DeviceUpdate	= 0x23,		/* USB设备状态更新通知 */

	/* guest to client */
	ZdpCmd_UsbRedir_ControlStart	= 0x31,		/* 启动设备重定向 */
	ZdpCmd_UsbRedir_ControlStop		= 0x32,		/* 关闭设备重定向 */
	ZdpCmd_UsbRedir_ListDevice		= 0x33,		/* 获取USB设备列表 */

	/* host to client */
	ZdpCmd_UsbRedir_NotifyChannels	= 0xA1,		/* USB重定向通道信息更新通知 */
	ZdpCmd_UsbRedir_NotifyRules		= 0xA2,		/* USB重定向策略信息更新通知 */

	/* USB Data */
	ZdpCmd_UsbRedir_UsbData			= 0xD1,		/* USB重定向数据(直接透传) */
} ZDP_CMD_USBREDIR;


#define ZDP_MAX_USBREDIR_CHANNELS	30
#define ZDP_MAX_USBREDIR_RULES		30


/* ZdpCmd_UsbRedir_GetChannels ack
 * ZdpCmd_UsbRedir_NotifyChannels
 * USB挂载通道信息回复/通知
 */
typedef struct {
	uint8_t count;					/* infos结构体的数量 */
	uint8_t reserved[15];
	ZDP_USBCHANNEL_INFO infos[ZDP_MAX_USBREDIR_CHANNELS];
} USBREDIR_CHANNELS;


/* ZdpCmd_UsbRedir_GetRules ack
 * ZdpCmd_UsbRedir_NotifyRules
 * USB重定向规则信息回复/通知
 */
typedef struct {
	uint8_t count;					/* infos结构体的数量 */
	uint8_t reserved[15];
	ZDP_USBRULE_INFO infos[ZDP_MAX_USBREDIR_RULES];
} USBREDIR_RULES;


/* ZdpCmd_UsbRedir_RequestStart
 * ZdpCmd_UsbRedir_RequestStop
 * 请求启动/关闭USB设备重定向
 *
 * ZdpCmd_UsbRedir_DeviceAdd
 * ZdpCmd_UsbRedir_DeviceRemove
 * ZdpCmd_UsbRedir_DeviceUpdate
 * 客户端接入的USB设备状态更新
 */
typedef struct {
	uint8_t client_id;				/* 设备所属的客户端ID */
	uint8_t status;					/* 设备状态(0-未重定向,1-已重定向) */
	uint8_t channel;				/* 重定向挂载的channel */
									/* client请求重定向时使用 */
	uint8_t reserved[13];
	ZDP_USBDEV_INFO info;			/* USB设备信息 */
} USBREDIR_DEVICE;


/* ZdpCmd_UsbRedir_ControlStart
 * ZdpCmd_UsbRedir_ControlStop
 * Tools控制客户端USB重定向
 *
 * ZDP_USBDEV_INFO
 */


#endif /* _ZDP_CHANNEL_USBREDIR_H_ */
