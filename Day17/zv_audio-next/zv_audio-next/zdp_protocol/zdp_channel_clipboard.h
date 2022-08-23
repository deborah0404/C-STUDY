
#ifndef _ZDP_CHANNEL_CLIPBOARD_H_
#define _ZDP_CHANNEL_CLIPBOARD_H_


#include <stdint.h>
#include "zdp_interface.h"
#include "zdp_channel_types.h"


/***************************************************************************
 * ZDP protocol
 */

/* ZDP Clipboard Cmds */
typedef enum {
	ZdpCmd_Clipboard_Update				= 0xA1,		/* 剪切板内容更新 */
	ZdpCmd_Clipboard_CopyFile			= 0xA2,
} ZDP_CMD_CLIPBOARD;


#define CLIPBOARD_DATA_LEN			1024


/* ZdpCmd_Clipboard_Update
 * 剪切板内容更新通知
 */
typedef struct {
	uint8_t type;					/* 剪切板数据类型 */
	uint8_t flags;
	uint8_t reserved[32];
	uint16_t data_len;				/* 数据长度 */
	uint8_t data[CLIPBOARD_DATA_LEN];
} CLIPBOARD_UPDATE;

typedef enum {
	emZdp_Clipboard_Text,
	emZdp_Clipboard_File
} CLIPBOARD_DATAT;


#endif /* _ZDP_CHANNEL_CLIPBOARD_H_ */
