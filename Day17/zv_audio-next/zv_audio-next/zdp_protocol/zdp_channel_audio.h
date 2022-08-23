
#ifndef _ZDP_CHANNEL_AUDIO_H_
#define _ZDP_CHANNEL_AUDIO_H_


#include <stdint.h>
#include "zdp_interface.h"
#include "zdp_channel_types.h"


/***************************************************************************
 * ZDP protocol
 */

/* ZDP Sound Cmds */
typedef enum {
	/* client to server/guest */
	ZdpCmd_Audio_Register		= 0x01,		/* register sound */

	/* server to client */
	ZdpCmd_Audio_PlaybackStart	= 0xA1,		/* start playback */
	ZdpCmd_Audio_PlaybackStop	= 0xA2,		/* stop playback */
	ZdpCmd_Audio_PlaybackSet	= 0xA3,		/* set playback */

	ZdpCmd_Audio_RecordStart	= 0xB1,		/* start record */
	ZdpCmd_Audio_RecordStop		= 0xB2,		/* stop record */
	ZdpCmd_Audio_RecordSet		= 0xB3,		/* stop record */

	/* for audio data */
	ZdpCmd_Audio_PlaybackData	= 0xD1,		/* playback data */
	ZdpCmd_Audio_RecordData		= 0xD2,		/* record data */
} ZDP_CMD_AUDIO;


/* ZdpCmd_Audio_PlaybackStart
 * ZdpCmd_Audio_RecordStart
 * 启动playback/record
 */
typedef struct {
	uint16_t freq;				/* 采样频率 */
	uint8_t channels;			/* 通道数量 */
	uint8_t format;				/* 音频格式 */
	uint8_t reserved[60];
} AUDIO_CONFIG_INFO;


/* ZdpCmd_Audio_PlaybackSet
 * ZdpCmd_Audio_RecordSet
 * 设置playback/record
 */
typedef struct {
	uint8_t mute;
	uint8_t channels;
	uint16_t volume_l;			/* 左声道音量 */
	uint16_t volume_r;			/* 右声道音量 */
	uint8_t reserved[58];
} AUDIO_SETTING_INFO;


/***************************************************************************
 * Audio Frame Header
 *
 * 0__________________8_________________16_________________24_________________31
 * |          packet id(16bit)           |   encoder(8bit)  |    flags(8bit)   |
 * |_____________________________________|__________________|__________________|
 * |             freq(16bit)             |  channels(8bit)  |   format(8bit)   |
 * |_____________________________________|__________________|__________________|
 * |                              frame len(32bit)                             |
 * |___________________________________________________________________________|
 * |                            timestamp usec(32bit)                          |
 * |___________________________________________________________________________|
 *
 */

/* audio frame head */
typedef struct {
	union {
		uint16_t packet_id;
		uint8_t packet_id_c[2];
	};
	uint8_t encoder;
	uint8_t flags;
	union {
		uint16_t freq;
		uint8_t freq_c[2];
	};
	uint8_t channels;
	uint8_t format;
	union {
		uint32_t frame_len;
		uint8_t frame_len_c[4];
	};
	union {
		uint32_t timestamp;
		uint8_t timestamp_c[4];
	};
} AUDIO_FRAME_HEAD, *AUDIO_FRAME_HEAD_P;


/* Audio Frame Packet Info */
typedef struct {
	uint16_t packet_id;
	uint8_t encoder;
	uint8_t flags;
	uint16_t freq;
	uint8_t channels;
	uint8_t format;
	uint32_t frame_len;
	uint32_t time_stamp;
	AUDIO_FRAME_HEAD* head;		/* 报文头 */
	const char* data;			/* 报文数据 */
	const char* buffer;			/* 原始buffer */
} AUDIO_FRAME_PACKET, *AUDIO_FRAME_PACKET_P;


/***************************************************************************
 * interfaces for QEMU
 */

/* playback */
#define ZDP_INTERFACE_PLAYBACK				"playback"
#define ZDP_INTERFACE_PLAYBACK_MAJOR		1
#define ZDP_INTERFACE_PLAYBACK_MINOR		0

#define ZDP_INTERFACE_PLAYBACK_FREQ			44100
#define ZDP_INTERFACE_PLAYBACK_CHAN			2

typedef struct ZdpPlaybackInterface {
	ZdpBaseInterface base;

	ZDP_AUDIO_CONFIG config;
} ZdpPlaybackInterface;


/* record */
#define ZDP_INTERFACE_RECORD				"record"
#define ZDP_INTERFACE_RECORD_MAJOR			1
#define ZDP_INTERFACE_RECORD_MINOR			0

#define ZDP_INTERFACE_RECORD_FREQ			44100
#define ZDP_INTERFACE_RECORD_CHAN			2

typedef struct ZdpRecordInterface {
	ZdpBaseInterface base;

	ZDP_AUDIO_CONFIG config;
} ZdpRecordInterface;


/***************************************************************************
 * messages for QEMU
 */

typedef struct ZdpAudioStart {
	ZdpBaseMessage base;

	ZDP_AUDIO_CONFIG config;
} ZdpAudioStart;


typedef struct ZdpAudioSet {
	ZdpBaseMessage base;

	ZDP_AUDIO_SETTING setting;
} ZdpAudioSet;


typedef struct ZdpAudioData {
	ZdpBaseMessage base;

	uint32_t* samples;
	uint32_t size;
} ZdpAudioData;


#endif /* _ZDP_CHANNEL_AUDIO_H_ */
