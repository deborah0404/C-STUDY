
#ifndef _ZDP_CHANNEL_INPUTS_H_
#define _ZDP_CHANNEL_INPUTS_H_


#include <stdint.h>
#include "zdp_interface.h"
#include "zdp_channel_types.h"


#define ZDP_INPUTS_SCANCODE_ScrollLock	0x46	/* ScrollLock扫描码 */
#define ZDP_INPUTS_SCANCODE_NumLock		0x45	/* NumLock扫描码 */
#define ZDP_INPUTS_SCANCODE_CapsLock	0x3A	/* CapsLock扫描码 */


/*
 * ZDP protocol
 */

/* ZDP Inputs Cmds */
typedef enum {
	/* client to host */
	ZdpCmd_Inputs_Curosr_Motion		= 0x01,		/* 光标移动事件 */
	ZdpCmd_Inputs_Mouse_Button		= 0x02,		/* 鼠标按键事件 */
	ZdpCmd_Inputs_Key_Code			= 0x03,		/* 键盘按键事件 */
	ZdpCmd_Inputs_Led_Synch			= 0x04,		/* LED灯状态同步事件 */

	/* host to client */
	ZdpCmd_Inputs_Led_Init			= 0xA0,		/* 键盘LED灯初始化 */
	ZdpCmd_Inputs_Led_Modify		= 0xA1,		/* 键盘LED灯更新通知 */

	ZdpCmd_Inputs_Err				= 0xE0,
} ZDP_CMD_INPUTS;


/* ZdpCmd_Inputs_Curosr_Motion
 * Client上报光标移动事件
 */
typedef struct tagCursorMotionInfo {
	uint8_t display_id;			/* 屏幕ID */
	uint8_t motion_mode;		/* 鼠标模式 */
	uint8_t reserved[2];
	int32_t dx;					/* 鼠标的相对位移x */
	int32_t dy;					/* 鼠标的相对位移y */
	int32_t px;					/* 鼠标的绝对位置x */
	int32_t py;					/* 鼠标的绝对位置y */
	uint32_t width;				/* 屏幕的绝对宽度 */
	uint32_t height;			/* 屏幕的绝对高度 */
	uint8_t reserved1[20];
} INPUTS_CURSOR_MOTION_INFO, *INPUTS_CURSOR_MOTION_INFO_P;

typedef enum {
	emZdp_Motion_Mode_QemuRel = 0,	/* QEMU相对模式 */
	emZdp_Motion_Mode_QemuAbs,		/* QEMU绝对模式 */
	emZdp_Motion_Mode_GuestRel,		/* Guest相对模式 */
	emZdp_Motion_Mode_GuestAbs,		/* Guest绝对模式 */
} INTPUS_MOTION_MODE;


/* ZdpCmd_Inputs_Mouse_Button
 * Client上报鼠标按键事件
 */
typedef struct tagMouseButtonInfo {
	uint8_t display_id;			/* 屏幕ID */
	uint8_t button;				/* 鼠标按键 */
	uint8_t button_state;		/* 鼠标按键状态 */
	uint8_t motion_mode;		/* 鼠标模式 */
	int32_t dx;					/* 鼠标的相对位移x */
	int32_t dy;					/* 鼠标的相对位移y */
	int32_t px;					/* 鼠标的绝对位置x */
	int32_t py;					/* 鼠标的绝对位置y */
	uint32_t width;				/* 屏幕的绝对宽度 */
	uint32_t height;			/* 屏幕的绝对高度 */
	uint8_t reserved1[20];
} INPUTS_MOUSE_BUTTON_INFO, *INPUTS_MOUSE_BUTTON_INFO_P;

/* mouse button */
typedef enum {
	emZdp_Mouse_LButton 		= 0x01,		/* 左键 */
	emZdp_Mouse_MButton 		= 0x02,		/* 中间 */
	emZdp_Mouse_RButton 		= 0x04,		/* 右键 */
} INPUTS_MOUSE_BUTTON;

/* key status */
typedef enum {
	emZdp_Button_Release		= 0x00,		/* 弹起 */
	emZdp_Button_Pressed		= 0x01,		/* 按下 */
	emZdp_Button_Roll_Up		= 0x02,		/* 滚轮上滚 */
	emZdp_Button_Roll_Down		= 0x03,     /* 滚轮下滚 */
} INPUTS_BUTTON_STATE;


/* ZdpCmd_Inputs_Key_Code
 * Client上报键盘按键事件
 */
typedef struct tagKeyCodeInfo {
	uint8_t display_id;			/* 屏幕ID */
	uint8_t key_code_len;		/* 扫描码长度 */
	uint8_t key_code[8];		/* 按键扫描码 */
	uint8_t button_state;		/* 按键状态 */
	uint8_t flags;				/* not used */
	uint8_t reserved[20];
} INPUTS_KEY_CODE_INFO, *INPUTS_KEY_CODE_INFO_P;


/* ZdpCmd_Inputs_Led_Init
 * ZdpCmd_Inputs_Led_Modify
 * Host通知键盘LED灯改变
 */
typedef struct tagLedsInfo {
	int32_t leds;
} INPUTS_LEDS_INFO, *INPUTS_LEDS_INFO_P;

/* 键盘LED指示灯标识 */
typedef enum {
	emZdp_Led_Flag_ScrollLock 	= (1 << 0),
	emZdp_Led_Flag_NumLock 		= (1 << 1),
	emZdp_Led_Flag_CapsLock 	= (1 << 2)
} INPUTS_LED_FLAG;


/*
 * interfaces for QEMU
 */

/* keyboard */
#define ZDP_INTERFACE_KEYBOARD				"keyboard"
#define ZDP_INTERFACE_KEYBOARD_MAJOR		1
#define ZDP_INTERFACE_KEYBOARD_MINOR		0

typedef struct ZdpKbdInterface {
	ZdpBaseInterface base;

	void(*push_key_down)(uint8_t key);
	void(*push_key_up)(uint8_t key);
	void(*push_key_pause)(void);
	int32_t(*get_leds)(void);
} ZdpKbdInterface;


/* mouse */
#define ZDP_INTERFACE_MOUSE					"mouse"
#define ZDP_INTERFACE_MOUSE_MAJOR			1
#define ZDP_INTERFACE_MOUSE_MINOR			0

typedef struct ZdpMouseInterface {
	ZdpBaseInterface base;

	void(*motion)(int32_t dx, int32_t dy, int32_t dz, uint32_t buttons);
	void(*buttons)(uint32_t buttons);
} ZdpMouseInterface;


/* tablet */
#define ZDP_INTERFACE_TABLET				"tablet"
#define ZDP_INTERFACE_TABLET_MAJOR			1
#define ZDP_INTERFACE_TABLET_MINOR			0

typedef struct ZdpTabletInterface {
	ZdpBaseInterface base;

	void(*position)(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t buttons);
	void(*wheel)(int32_t wheel, uint32_t buttons);
	void(*buttons)(uint32_t buttons);
} ZdpTabletInterface;


/*
 * messages for QEMU
 */

typedef struct ZdpKeyboardLeds {
	ZdpBaseMessage base;

	int32_t leds;
} ZdpKeyboardLeds;


#endif /* _ZDP_CHANNEL_INPUTS_H_ */
