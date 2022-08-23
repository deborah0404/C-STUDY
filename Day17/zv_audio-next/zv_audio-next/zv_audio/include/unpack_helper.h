
#ifndef _UNPACK_HELPER_H_
#define _UNPACK_HELPER_H_


#include "zva_common.h"
#include "zv_buffer.h"


class unpack_helper
{
public:
	static int unpack_cmd_buffer(ZDP_PACKET& packet, const char* buffer, uint32_t len);

	static int unpack_frame_buffer(ZDP_FRAME_PACKET& packet, const char* buffer, uint32_t len);

	static int pack_cmd_buffer(zv_buffer& buffer, uint16_t id,
							   uint8_t dest, uint8_t src, uint8_t channel,
							   uint8_t cmd, uint8_t index, const char* data,
							   uint32_t len, uint16_t errcode);

	static int unpack_audio_start_message(AUDIO_CONFIG_INFO& msg, const char* buffer, uint32_t len);

	static int unpack_audio_setting_message(AUDIO_SETTING_INFO& msg, const char* buffer, uint32_t len);

	static int unpack_audio_frame(AUDIO_FRAME_PACKET& msg, const char* buffer, uint32_t len);

public:
	static void hton_proxy_connect_info(PROXY_CONNECT&);

	static void ntoh_audio_start_info(AUDIO_CONFIG_INFO&);

	static void ntoh_audio_setting_info(AUDIO_SETTING_INFO&);
};


#endif	// _UNPACK_HELPER_H_
