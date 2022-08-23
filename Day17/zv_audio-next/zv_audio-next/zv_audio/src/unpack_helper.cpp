
#include "unpack_helper.h"


int unpack_helper::unpack_cmd_buffer(ZDP_PACKET& packet, const char* buffer, uint32_t len)
{
	if (len < sizeof(ZDP_HEAD)) { return -1; }

	ZDP_HEAD* pHead = (ZDP_HEAD*)buffer;
	if (pHead->marks[0] != 0x55 && pHead->marks[1] != 0x55) { return -1; }
	if (pHead->version[0] != ZDP_PROTOCOL_TYPE_MAJOR) { return -1; }

	packet.head = pHead;
	packet.version[0] = pHead->version[0];
	packet.version[1] = pHead->version[1];
	packet.dest = pHead->dest;
	packet.src = pHead->src;
	packet.channel = pHead->channel;
	packet.cmd = pHead->cmd;
	packet.index = pHead->index;
	packet.ack = (pHead->flags & 0x80) >> 7;
	packet.qos = pHead->flags & 0x03;
	packet.head_len = pHead->head_len;
	packet.data_len = ((uint32_t)pHead->data_len[0] << 24) |
					  ((uint32_t)pHead->data_len[1] << 16) |
					  ((uint32_t)pHead->data_len[2] << 8) |
					  ((uint32_t)pHead->data_len[3]);
	packet.msg_id = ((uint16_t)pHead->msg_id[0] << 8) | pHead->msg_id[1];
	packet.error_code = ((uint16_t)pHead->error_code[0] << 8) | pHead->error_code[1];
	packet.data = (char*)buffer + pHead->head_len;
	packet.buffer = buffer;
	return 0;
}

int unpack_helper::unpack_frame_buffer(ZDP_FRAME_PACKET& packet, const char* buffer, uint32_t len)
{
	if (len < sizeof(ZDP_FRAME_HEAD)) { return -1; }

	ZDP_FRAME_HEAD* pHead = (ZDP_FRAME_HEAD*)buffer;
	if (pHead->marks[0] != 0xAA && pHead->marks[1] != 0xAA) { return -1; }

	packet.head = pHead;
	packet.flags = pHead->flags;
	packet.head_len = pHead->head_len;
	packet.msg_id = ((uint16_t)pHead->msg_id[0] << 8) | pHead->msg_id[1];
	packet.packet_id = ((uint16_t)pHead->packet_id[0] << 8) | pHead->packet_id[1];
	packet.data_len = (uint32_t)pHead->data_len[0] << 24 |
					  (uint32_t)pHead->data_len[1] << 16 |
					  (uint32_t)pHead->data_len[2] << 8 |
					  (uint32_t)pHead->data_len[3];
	packet.data_offset = (uint32_t)pHead->data_offset[0] << 24 |
						 (uint32_t)pHead->data_offset[1] << 16 |
						 (uint32_t)pHead->data_offset[2] << 8 |
						 (uint32_t)pHead->data_offset[3];
	packet.time_stamp = (uint32_t)pHead->time_stamp[0] << 24 |
					   (uint32_t)pHead->time_stamp[1] << 16 |
					   (uint32_t)pHead->time_stamp[2] << 8 |
					   (uint32_t)pHead->time_stamp[3];
	packet.data = (char*)buffer + packet.head_len;
	packet.buffer = buffer;
	return 0;
}

int unpack_helper::pack_cmd_buffer(zv_buffer& buffer, uint16_t id,
								   uint8_t dest, uint8_t src, uint8_t channel,
								   uint8_t cmd, uint8_t index, const char* data,
								   uint32_t len, uint16_t errcode)
{
	ZDP_HEAD* phead = (ZDP_HEAD*)buffer.buffer();
	phead->marks[0] = 0x55;
	phead->marks[1] = 0x55;
	phead->version[0] = ZDP_PROTOCOL_TYPE_MAJOR;
	phead->version[1] = ZDP_PROTOCOL_TYPE_MINOR;
	phead->flags = 0;
	phead->head_len = sizeof(ZDP_HEAD);
	phead->msg_id[0] = (id & 0x0000FF00) >> 8;
	phead->msg_id[1] = (id & 0x000000FF);
	phead->dest = dest;
	phead->src = src;
	phead->channel = channel;
	phead->cmd = cmd;
	phead->index = index;
	phead->data_len[0] = (len & 0xFF000000) >> 24;
	phead->data_len[1] = (len & 0x00FF0000) >> 16;
	phead->data_len[2] = (len & 0x0000FF00) >> 8;
	phead->data_len[3] = (len & 0x000000FF);
	phead->error_code[0] = (errcode & 0x0000FF00) >> 8;
	phead->error_code[1] = (errcode & 0x000000FF);
	int offset = sizeof(ZDP_HEAD);

	if (data && len)
	{
		memcpy(buffer.buffer() + offset, data, len);
		offset += len;
	}
	return offset;
}


int unpack_helper::unpack_audio_start_message(AUDIO_CONFIG_INFO& msg, const char* buffer, uint32_t len)
{
	if (len < sizeof(AUDIO_CONFIG_INFO)) { return -1; }

	memcpy((void*)&msg, buffer, sizeof(AUDIO_CONFIG_INFO));
	ntoh_audio_start_info(msg);
	return 0;
}

int unpack_helper::unpack_audio_setting_message(AUDIO_SETTING_INFO& msg, const char* buffer, uint32_t len)
{
	if (len < sizeof(AUDIO_SETTING_INFO)) { return -1; }

	memcpy((void*)&msg, buffer, sizeof(AUDIO_SETTING_INFO));
	ntoh_audio_setting_info(msg);
	return 0;
}

int unpack_helper::unpack_audio_frame(AUDIO_FRAME_PACKET& packet, const char* buffer, uint32_t len)
{
	if (len < sizeof(AUDIO_FRAME_HEAD)) { return -1; }

	AUDIO_FRAME_HEAD* pHead = (AUDIO_FRAME_HEAD*)buffer;
	packet.packet_id = (uint16_t)pHead->packet_id_c[0] << 8 |
					   (uint16_t)pHead->packet_id_c[1];
	packet.encoder = pHead->encoder;
	packet.flags = pHead->flags;
	packet.freq = (uint16_t)pHead->freq_c[0] << 8 |
				  (uint16_t)pHead->freq_c[1];
	packet.format = pHead->format;
	packet.channels = pHead->channels;
	packet.frame_len = (uint32_t)pHead->frame_len_c[0] << 24 |
					   (uint32_t)pHead->frame_len_c[1] << 16 |
					   (uint32_t)pHead->frame_len_c[2] << 8 |
					   (uint32_t)pHead->frame_len_c[3];
	packet.time_stamp = (uint32_t)pHead->timestamp_c[0] << 24 |
						(uint32_t)pHead->timestamp_c[1] << 16 |
						(uint32_t)pHead->timestamp_c[2] << 8 |
						(uint32_t)pHead->timestamp_c[3];
	packet.head = pHead;
	packet.data = buffer + sizeof(AUDIO_FRAME_HEAD);
	packet.buffer = buffer;
	return 0;
}

void unpack_helper::hton_proxy_connect_info(PROXY_CONNECT& info)
{
	info.type = zv_htons(info.type);
	info.addr_len = zv_htons(info.addr_len);
	info.license_len = zv_htons(info.license_len);
	info.port = zv_htons(info.port);
}

void unpack_helper::ntoh_audio_start_info(AUDIO_CONFIG_INFO& info)
{
	info.freq = zv_ntohs(info.freq);
}

void unpack_helper::ntoh_audio_setting_info(AUDIO_SETTING_INFO& info)
{
	info.volume_l = zv_ntohs(info.volume_l);
	info.volume_r = zv_ntohs(info.volume_r);
}
