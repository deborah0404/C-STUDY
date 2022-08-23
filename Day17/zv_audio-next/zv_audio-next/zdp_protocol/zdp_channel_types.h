
#ifndef _ZDP_CHANNEL_TYPE_H_
#define _ZDP_CHANNEL_TYPE_H_


#include <stdint.h>


/* for ZDP channel */
typedef struct {
	uint8_t id;					/* 会话ID(服务器端唯一编号) */
	uint8_t type;				/* Session的类型(ZDP_MODULE_TYPE) */
	int16_t index;				/* 会话序号(同类型会话序号) */
	uint16_t port;				/* 会话远端端口 */
	char addr[50];				/* 会话远端IP */
	void* session;				/* 会话句柄 */
} ZDP_SESSION, *ZDP_SESSION_P;


/* channel context */
typedef struct {
	uint8_t mask[2];
	uint8_t type;				/* channel type(ZDP_CHANNEL_TYPE) */
	uint8_t reserved;
	uint32_t datalen;			/* len of context data */
	uint8_t data[];				/* context of channel */
} ZDP_CHANNEL_CONTEXT;


/***************************************************************************
 * for Main channel
 */

#define ZDP_ALGO_NAME_LEN		32
#define ZDP_KEY_MAX_LEN			1024


/* vm state */
typedef enum {
	emZdp_VmState_Shutdown		= 0x00,
	emZdp_VmState_Running		= 0x01,
	emZdp_VmState_Paused		= 0x02,
	emZdp_VmState_Restarting	= 0x03,
} ZDP_VM_STATE;


/* encrypt algo info */
typedef struct {
	uint8_t type;
	uint8_t reserved[15];
	char name[ZDP_ALGO_NAME_LEN];
} ZDP_ALGO_INFO;


/* encrypt key info */
typedef struct {
	uint32_t len;
	char key[ZDP_KEY_MAX_LEN];
} ZDP_KEY_INFO;


/* session info */
typedef struct {
	ZDP_SESSION session;		/* Session信息 */
	uint8_t details[128];		/* Session的描述信息 */
} ZDP_SESSION_INFO;


/***************************************************************************
 * for Display
 */

/* monitor info */
typedef struct {
	uint8_t index;				/* 屏幕序号 */
	uint8_t id;					/* 屏幕ID */
	uint8_t enabled;			/* 屏幕是否启用 */
	uint8_t reserved[17];
	uint16_t refresh_rate;		/* 屏幕刷新率 */
	uint16_t color_depth;		/* 显示位深 */
	int32_t posx;				/* 屏幕坐标X */
	int32_t posy;				/* 屏幕坐标Y */
	uint32_t width;				/* 屏幕宽度 */
	uint32_t height;			/* 屏幕高度 */
} ZDP_MONITOR_INFO;


#define ZDP_SCREEN_RATIO_MAX		30

/* ratio info */
typedef struct {
	uint16_t width;				/* 屏幕宽度 */
	uint16_t height;			/* 屏幕高度 */
} ZDP_RATIO_INFO;

typedef struct {
	uint8_t id;				/* 屏幕序号 */
	uint8_t count;			/* 分辨率个数 */
	uint8_t reserved[2];
	ZDP_RATIO_INFO infos[ZDP_SCREEN_RATIO_MAX];
} ZDP_SCREEN_RATIO_INFO;


/***************************************************************************
 * for UsbRedir
 */

#define ZDP_USBDEV_NAME_LEN		64


/* usb type */
typedef enum {
	emZdp_USB_Type_Unknow	= 0x00,
	emZdp_USB_Type_1_0		= 0x10,
	emZdp_USB_Type_2_0		= 0x20,
	emZdp_USB_Type_3_0		= 0x30,
	emZdp_USB_Type_3_1		= 0x31,
	emZdp_USB_Type_3_2		= 0x32,
} ZDP_USB_TYPE;


/* usb device type */
typedef enum {
	emZdp_USBDEV_Type_Unknow	= 0x00,
	emZdp_USBDEV_Type_Mouse		= 0x01,		/* 鼠标设备 */
	emZdp_USBDEV_Type_Keyboard	= 0x02,		/* 键盘设备 */
	emZdp_USBDEV_Type_UDisk		= 0x03,		/* 存储设备 */
	emZdp_USBDEV_Type_Audio		= 0x04,		/* 音频设备 */
	emZdp_USBDEV_TYpe_Video		= 0x05,		/* 视频设备 */
	emZdp_USBDEV_Type_Printer	= 0x06,		/* 打印设备 */
	emZdp_USBDEV_Type_Physical	= 0x07,		/* 物理设备 */
	emZdp_USBDEV_Type_Image		= 0x08,		/* 图像设备 */
	emZdp_USBDEV_Type_Bluetooth	= 0x09,		/* 蓝牙设备 */
	emZdp_USBDEV_Type_Hub		= 0x10,		/* 集线器设备 */
	emZdp_USBDEV_TYpe_HID		= 0x11,		/* 人机接口设备 */
	emZdp_USBDEV_TYpe_Media		= 0x12,		/* 多媒体设备 */
	emZdp_USBDEV_Type_CDC		= 0x13,		/* CDC通信设备 */
	emZdp_USBDEV_Type_Phone		= 0x14,		/* 手机设备 */
	emZdp_USBDEV_Type_SmartCard	= 0x15,		/* 智能卡设备 */
	emZdp_USBDEV_Type_Security	= 0x16,		/* 安全设备 */
	emZdp_USBDEV_Type_Health	= 0x17,		/* 个人健康设备 */
	emZdp_USBDEV_Type_Wireless	= 0x18,		/* 无线控制器设备 */
	emZdp_USBDEV_Type_Other		= 0x19,		/* 其他设备 */
} ZDP_USBDEV_TYPE;


/* usb redir channel info */
typedef struct {
	uint16_t id;				/* 通道ID(Server端唯一ID) */
	uint8_t type;				/* 通道类型(enum ZDP_USB_TYPE) */
	uint8_t used;				/* 通道状态 */
								/* 0-未占用，1-已占用 */
	uint8_t reserved[16];
} ZDP_USBCHANNEL_INFO;


/* usb redir rule info */
typedef struct {
	int32_t device_class;		/* 设备类型 */
	int32_t vendor_id;			/* 厂商ID(-1=所有厂商) */
	int32_t product_id;			/* 设备ID(-1=所有设备) */
	int32_t version_bcd;		/* (-1=所有版本)*/
	int32_t rule;				/* 重定向规则 */
								/* -1=不允许重定向，0=允许重定向，1=自动重定向 */
} ZDP_USBRULE_INFO;


/* usb device info */
typedef struct {
	uint64_t uid;				/* 设备ID(全局唯一) */
	uint16_t vid;				/* 厂商ID */
	uint16_t pid;				/* 设备ID */
	uint16_t bcd_usb;			/* USB协议版本 */
	uint16_t bcd_dev;
	uint8_t dev_class;			/* 用于判断设备类型 */
	uint8_t dev_subclass;		/* 用于判断设备类型 */
	uint8_t dev_protocol;		/* 用于判断设备类型 */
	uint8_t dev_type;			/* 设备类型(enum ZDP_USBDEV_TYPE) */
	uint8_t dev_speed;
	uint8_t reserved[11];
	char name[ZDP_USBDEV_NAME_LEN];	/* 设备名称 */
} ZDP_USBDEV_INFO;


/***************************************************************************
 * for Audio
 */

/* audio config */
typedef struct {
	uint16_t freq;				/* 采样频率 */
	uint8_t channels;			/* 通道数量 */
	uint8_t format;				/* 音频格式 */
} ZDP_AUDIO_CONFIG;


/* audio setting */
typedef struct {
	uint8_t mute;
	uint8_t channels;
	uint16_t volume_l;			/* 左声道音量 */
	uint16_t volume_r;			/* 右声道音量 */
	uint8_t reserved[18];
} ZDP_AUDIO_SETTING;


/* audio format */
typedef enum {
    emZdp_Audio_Format_U8,
    emZdp_Audio_Format_S8,
    emZdp_Audio_Format_U16,
    emZdp_Audio_Format_S16,
    emZdp_Audio_Format_U32,
    emZdp_Audio_Format_S32
} ZDP_AUDIO_FORMAT;


/* audio encoder */
typedef enum {
	emZdp_Audio_Encoder_none	= 0x00,
} ZDP_AUDIO_ENCODER;


/***************************************************************************
 * ZDP Enums
 */

/* ZDP Log Level */
typedef enum {
	emZdp_LOG_LVL_ALL 		= 0,
	emZdp_LOG_LVL_DEBUG 	= 1,
	emZdp_LOG_LVL_LOW 		= 2,
	emZdp_LOG_LVL_NOMAL 	= 3,
	emZdp_LOG_LVL_HIGH 		= 4
} ZDP_LOG_LEVEL;


/* define of invalid values */
#define ZDP_INVALID_MODULE		0x00
#define ZDP_INVALID_CHANNEL		0x00
#define ZDP_INVALID_SID			0x00
#define ZDP_INVALID_SINDEX		0xFF


#endif /* _ZDP_CHANNEL_TYPE_H_ */
