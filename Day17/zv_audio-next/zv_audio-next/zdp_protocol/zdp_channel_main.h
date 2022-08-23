
#ifndef _ZDP_CHANNEL_MAIN_H_
#define _ZDP_CHANNEL_MAIN_H_


#include <stdint.h>
#include "zdp_interface.h"
#include "zdp_channel_types.h"


/***************************************************************************
 * ZDP protocol
 */

/* ZDP Main Cmds */
typedef enum {
	ZdpCmd_Main_Unknow			= 0x00,

	/* client to server */
	ZdpCmd_Main_Register		= 0x01,		/* 注册会话 */
	ZdpCmd_Main_AppendSession	= 0x02,		/* 附加会话 */
	ZdpCmd_Main_RequestCtrl		= 0x03,		/* 请求控制权 */

	ZdpCmd_Main_Encrypt_Use		= 0x11,		/* 协商是否使用加密传输 */
	ZdpCmd_Main_Encrypt_Algo 	= 0x12,		/* 协商使用的加解密算法 */
	ZdpCmd_Main_Encrypt_Key		= 0x13,		/* 协商加解密密钥 */

	/* guest to client */
	ZdpCmd_Main_Exit_Client		= 0x61,		/* 退出客户端 */
	ZdpCmd_Main_Entry_Full		= 0x62,		/* 进入全屏 */
	ZdpCmd_Main_Exit_Full		= 0x63,		/* 退出全屏 */
	ZdpCmd_Main_Guest_Cap		= 0x6A,		/* Guest能力通知 */

	/* server <=> guest */
	ZdpCmd_Main_SessionUpdate	= 0xA1,		/* 会话更新通知 */
	ZdpCmd_Main_SessionList		= 0xA2,		/* 会话列表查询 */
	ZdpCmd_Main_StopGuest		= 0xA3,		/* 所有客户端断开后，停止guest */

	/* server/guest => client */
	ZdpCmd_Main_VmStateNotify	= 0xB1,		/* 云主机状态更新通知 */
	ZdpCmd_Main_GuestOnline		= 0xB2,		/* Guest服务上线 */
	ZdpCmd_Main_GuestOffline	= 0xB3,		/* Guest服务下线 */

	/* client => guest */
	ZdpCmd_Main_ResetGuest		= 0xC1,		/* Guest状态重置(恢复) */
	ZdpCmd_Main_CheckGuest		= 0xC2,		/* 检查Guest的状态 */

	ZdpCmd_Main_HeartBeat		= 0xFF,		/* 心跳探测 */
} ZDP_CMD_MAIN;


/* ZdpCmd_Main_Register
 * 请求注册一个会话
 */
typedef struct {
	uint8_t id;					/* 会话ID(服务器端唯一编号) */
								/* 注册时由服务器端分配 */
	uint8_t type;				/* 会话类型(ZDP_MODULE_TYPE) */
	uint8_t name[60];			/* 会话的名称(客户端自行填写) */

	/* MainChannel注册时需要 */
	uint16_t len;				/* license长度 */
	uint8_t license[256];		/* 授权license */
} MAIN_REGISTER;

/* ZdpCmd_Main_Register ack
 * 注册成功返回会话信息
 */
typedef struct {
	uint8_t id;					/* 会话ID(服务器端唯一编号) */
	uint8_t type;				/* 会话的类型(ZDP_MODULE_TYPE) */
	uint8_t reserved[2];
	uint8_t authority[64];		/* 会话授权信息 */
} MAIN_REGISTER_ACK;


/* ZdpCmd_Main_AppendSession
 * ZdpCmd_Main_AppendSession ack
 * 请求附加一个会话
 */
typedef struct {
	uint8_t id;					/* 需要附加到的会话ID */
	uint8_t type;				/* 会话类型(ZDP_MODULE_TYPE) */
	uint8_t reserved[14];
} MAIN_APPEND_SESSION, MAIN_APPEND_SESSION_ACK;


/* ZdpCmd_Main_Encrypt_Use ack
 * 回复是否使用加密传输
 */
typedef struct {
	uint8_t use;				/* 0-不使用/1使用 */
	uint8_t reserved[3];
} MAIN_ENCRYPT_USE_ACK;


/* ZdpCmd_Main_Encrypt_Algo
 * 发送支持的加解密算法列表
 */
typedef struct {
	uint8_t count;				/* infos结构体的数量 */
	uint8_t reserved[15];
	ZDP_ALGO_INFO infos[];
} MAIN_ENCRYPT_ALGO;

/* ZdpCmd_Main_Encrypt_Algo ack
 * 选择一个加解密算法
 */
typedef struct {
	ZDP_ALGO_INFO info;
} MAIN_ENCRYPT_ALGO_ACK;


/* ZdpCmd_Main_Encrypt_Key ack
 * 回复加密密钥
 */
typedef struct {
	ZDP_KEY_INFO info;
} MAIN_ENCRYPT_KEY_ACK;


/* ZdpCmd_Main_GuestCap
 * Guest能力通知
 * ZdpCmd_Main_CheckGuest ack
 * 检查Guest状态回复
 */
typedef struct {
	uint8_t display;			/* 显示器支持 */
	uint8_t encode;				/* 编码支持 */
	uint8_t cursor;				/* 光标支持 */
	uint8_t clipboard;			/* 剪切板支持 */
	uint8_t file_transfer;		/* 文件传输支持 */
	uint8_t usb_redir;			/* USB重定向支持 */
	uint8_t disk_redir;			/* 磁盘重定向支持 */
	uint8_t audio_redir;		/* 音频重定向支持 */
	uint8_t reserved[512];
} MAIN_GUEST_CAP, MAIN_CHECKGUEST_ACK;


/* ZdpCmd_Main_SessionUpdate
 * 通知会话状态更新
 */
typedef struct {
	uint8_t status;				/* 1-连接，0-断连 */
	uint8_t reserved[15];
	ZDP_SESSION_INFO info;		/* 会话信息 */
} MAIN_SESSION_UPDATE;


/* ZdpCmd_Main_VmStateNotify
 * 云主机状态更新通知
 */
typedef struct {
	uint8_t state;				/* 云主机状态(ZDP_VM_STATE) */
	char reserved[15];
} MAIN_VMSTATE_NOTIFY;


/* ZdpCmd_Main_HeartBeat
 * ZdpCmd_Main_HeartBeat ack
 * 心跳探测
 */
typedef struct {
	uint32_t timestamp1;		/* 心跳发送时间(ms) */
	uint32_t timestamp2;		/* 心跳接收时间(ms) */
	uint32_t timestamp3;		/* 心跳回复时间(ms) */
	uint32_t timestamp4;		/* 心跳接收时间(ms) */
	uint8_t reserved[16];
} MAIN_HEARTBEAT;


/***************************************************************************
 * interfaces for QEMU
 */

/* core */
#define ZDP_INTERFACE_CORE					"core"
#define ZDP_INTERFACE_CORE_MAJOR			2
#define ZDP_INTERFACE_CORE_MINOR			0

typedef void* ZDP_TIMER_P;
typedef void (*ZdpTimerFunc)(void *opaque);

typedef struct ZdpCoreInterface {
	ZdpBaseInterface base;

	ZDP_TIMER_P (*timer_add)(ZdpTimerFunc func, void* opaque);
	void (*timer_start)(ZDP_TIMER_P timer, uint32_t ms);
	void (*timer_cancel)(ZDP_TIMER_P timer);
	void (*timer_remove)(ZDP_TIMER_P timer);

	void (*io_thread_lock)(void);
	void (*io_thread_unlock)(void);
} ZdpCoreInterface;


/* main */
#define ZDP_INTERFACE_MAIN					"main"
#define ZDP_INTERFACE_MAIN_MAJOR			1
#define ZDP_INTERFACE_MAIN_MINOR			0

typedef struct ZdpMainInterface {
	ZdpBaseInterface base;

	void (*channel_event)(const ZDP_SESSION_INFO*, int32_t event);
} ZdpMainInterface;


/* char device */
#define ZDP_INTERFACE_CHAR_DEVICE			"char-device"
#define ZDP_INTERFACE_CHAR_DEVICE_MAJOR		1
#define ZDP_INTERFACE_CHAR_DEVICE_MINOR		0

#define SUBTYPE_VDAGENT			"vdagent"
#define SUBTYPE_USBREDIR		"usbredir"

typedef void* ZDP_CHARDEV_P;
typedef void* ZDP_CHARCHAN_P;

typedef struct ZdpCharDeviceContext {
	const char *subtype;			/* 串口类型 */
	const ZdpBaseInterface *itfs;	/* 串口读写接口 */
	ZDP_CHARDEV_P chardev;			/* 串口设备(QEMU使用) */
	ZDP_CHARCHAN_P charchan;		/* 串口通道(ZDP使用) */
	uint32_t index;					/* 串口编号 */
} ZdpCharDeviceContext;

typedef struct ZdpCharDeviceInterface {
	ZdpBaseInterface base;

	void (*state)(ZdpCharDeviceContext *chrdev, int32_t connected);
	int32_t (*write)(ZdpCharDeviceContext *chrdev, const char *buf, int32_t len);
	int32_t (*read)(ZdpCharDeviceContext *chrdev, char *buf, int32_t len);
	void (*event)(ZdpCharDeviceContext *chrdev, uint8_t event);
} ZdpCharDeviceInterface;


/***************************************************************************
 * messages for QEMU
 */

typedef struct ZdpVmState {
	ZdpBaseMessage base;

	ZDP_VM_STATE state;
} ZdpVmState;


#endif /* _ZDP_CHANNEL_MAIN_H_ */
