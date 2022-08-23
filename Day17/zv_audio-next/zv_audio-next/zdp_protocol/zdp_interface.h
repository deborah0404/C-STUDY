
#ifndef _ZDP_INTERFACE_H_
#define _ZDP_INTERFACE_H_


#include <stdint.h>


/*
 * ZDP Base Interface
 */
typedef struct {
	uint32_t channel_type;
	const char *interface_type;
	uint32_t major_version;
	uint32_t minor_version;
} ZdpBaseInterface;


/*
 * ZDP Base Message
 */
typedef struct {
	uint32_t channel_type;
	uint32_t message_type;
} ZdpBaseMessage;


/* ZDP Message Type */
typedef enum {
	emZdp_MSG_KEYBOARD_LEDS	= 0x1001,
	emZdp_MSG_CURSOR_SHAPE	= 0x2001,
	emZdp_MSG_VM_STATE		= 0x3001,

	/* for stream */
	emZdp_MSG_STREAM_STOP	= 0xA001,
	emZdp_MSG_STREAM_CTRL	= 0xA002,
	emZdp_MSG_STREAM_RESET	= 0xA003,

	/* for audio */
	emZdp_MSG_PLAYBACK_START= 0xB001,
	emZdp_MSG_PLAYBACK_STOP	= 0xB002,
	emZdp_MSG_PLAYBACK_SET	= 0xB003,
	emZdp_MSG_PLAYBACK_DATA	= 0xB004,

	emZdp_MSG_RECORD_START	= 0xB011,
	emZdp_MSG_RECORD_STOP	= 0xB012,
	emZdp_MSG_RECORD_SET	= 0xB013,
} ZDP_MESSAGE_TYPE;


/* emZdp_MSG_STREAM_RESET
 * ZDP Message Reset Player Session
 */

typedef struct ZdpResetSession {
	ZdpBaseMessage base;

	void* session;
} ZdpResetSession;


#endif /* _ZDP_INTERFACE_H_ */
