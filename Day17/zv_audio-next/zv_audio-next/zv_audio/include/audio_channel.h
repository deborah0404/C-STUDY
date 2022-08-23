
#ifndef _AUDIO_CHANNEL_H_
#define _AUDIO_CHANNEL_H_


#include "zva_common.h"
#include "audio_session.h"
#include "audio_playback.h"
#include "audio_record.h"


typedef enum {
	emStatus_Stopped,		// 已停止
	emStatus_Starting,		// 启动中
	emStatus_Started,		// 已启动
	emStatus_Connected,		// 连接
	emStatus_Proxy,			// 代理
	emStatus_Register,		// 注册
	emStatus_Ready,			// 已就绪
	emStatus_Disconnected,	// 已断连
	emStatus_Restarting,	// 重连中
} ZV_AUDIO_STATUS;


class audio_channel
{
public:
	audio_channel();
	~audio_channel();

	void set_callback(zva_event_callback cb, void* user);
	void set_proxy(const char* addr, uint16_t port, const char* license, const char* transport);
	void remove_proxy();

	bool start_channel(const char* addr, uint16_t port,
					   const char* license, const char* transport, uint8_t id);
	void stop_channel();

private:
	// control
	bool connect_server();
	void disconnect_server();
	bool register_proxy(uint32_t);
	bool register_channel(uint32_t);

	bool start_playback();
	void stop_playback();
	bool start_record();
	void stop_record();

private:
	// callback
	static void _message_cb(ZDP_PACKET*, void*);
	static void _frame_cb(AUDIO_FRAME_PACKET*, void*);
	static void _event_cb(const zva_event*, void*);

	// handle message
	void _recv_message(ZDP_PACKET*);
	void _recv_frame(AUDIO_FRAME_PACKET*);
	void _recv_event(const zva_event*);

	// handle events
	bool _handle_events();
	void _report_event(const zva_event&);
	void _update_status(ZV_AUDIO_STATUS);

	static void _workThread(void*);

private:
	volatile bool m_bWorking;
	volatile bool m_bWorkThread;
	ZV_AUDIO_STATUS m_status;

	// for proxy
	bool m_proxy_enabled;
	std::string m_proxy_ddr;
	uint16_t m_proxy_port;
	std::string m_proxy_license;
	std::string m_proxy_transport;

	// for config
	std::string m_sUrl;
	std::string m_sAddr;
	uint16_t m_sPort;
	std::string m_sLicense;
	std::string m_sTransport;
	uint8_t m_session_id;

	// for session
	audio_session m_session;

	// for audio
	audio_playback m_playback;
	audio_record m_record;

	// for events
	std::queue<zva_event*> m_eventList;
	std::mutex m_eventListMutex;

	// for report
	zva_event_callback m_event_cb;
	void* m_user;
};


#endif	// _AUDIO_CHANNEL_H_
