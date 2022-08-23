
#include "audio_channel.h"
#include<SDL.h>

audio_channel::audio_channel()
	: m_bWorking(false)
	, m_bWorkThread(false)
	, m_status(emStatus_Stopped)
	// for proxy
	, m_proxy_enabled(false)
	, m_proxy_port(0)
	// for config
	, m_sPort(0)
	// for session
	, m_session(emZdp_Module_Audio, _message_cb, _frame_cb, _event_cb, this)
	// for report
	, m_event_cb(NULL)
	, m_user(NULL)
{

}

audio_channel::~audio_channel()
{
	ZVA_INFO("audio_channel::~audio_channel()\n");

	stop_channel();
}


void audio_channel::set_callback(zva_event_callback cb, void* user)
{
	m_event_cb = cb;
	m_user = user;
}

void audio_channel::set_proxy(const char* addr, uint16_t port, const char* license, const char* transport)
{
	m_proxy_ddr = addr;
	m_proxy_port = port;
	m_proxy_license = license;
	m_proxy_transport = transport;
	m_proxy_enabled = true;
}

void audio_channel::remove_proxy()
{
	m_proxy_enabled = false;
}


bool audio_channel::start_channel(const char* addr, uint16_t port,
								  const char* license, const char* transport, uint8_t id)
{
	if (m_bWorking || m_bWorkThread || emStatus_Stopped != m_status)
	{
		ZVA_INFO("[%s] # channel is running\n", m_sUrl.c_str());
		return true;
	}

	// 保存参数
	m_sAddr = addr;
	m_sPort = port;
	m_sLicense = license;
	m_sTransport = transport;
	m_session_id = id;

	char sTemp[128] = {0};
	snprintf(sTemp, 128, "audio@%s:%d", addr, port);
	m_sUrl = sTemp;

	// 启动任务线程
	m_bWorking = true;
	zv_thread_create(0, ZV_THREAD_PRIORITY_INVALID, _workThread, this);

	ZVA_INFO("[%s] # Start()\n", m_sUrl.c_str());
	return true;
}

void audio_channel::stop_channel()
{
	if (!m_bWorking && !m_bWorkThread)
	{
		return;
	}

	ZVA_INFO("[%s] # Stop()\n", m_sUrl.c_str());

	m_bWorking = false;
	while (m_bWorkThread)
	{
		zv_msleep(1);
	}

	ZVA_INFO("[%s] # Stoped()\n", m_sUrl.c_str());
}


// control
bool audio_channel::connect_server()
{
	const char* addr = nullptr;
	uint16_t port = 0;

	if (m_proxy_enabled)
	{
		addr = m_proxy_ddr.c_str();
		port = m_proxy_port;
	}
	else
	{
		addr = m_sAddr.c_str();
		port = m_sPort;
	}

	// 连接服务器
	if (!m_session.connect_server(addr, port))
	{
		ZVA_WARN("[%s] # connect_server(%s:%d) failed\n", m_sUrl.c_str(), addr, port);
		return false;
	}

	ZVA_INFO("[%s] # connect_server(%s:%d) success\n", m_sUrl.c_str(), addr, port);
	return true;
}

void audio_channel::disconnect_server()
{
	m_session.disconnect_server();

	ZVA_INFO("[%s] # disconnect_server()\n", m_sUrl.c_str());
}

bool audio_channel::register_proxy(uint32_t msec)
{
	if (!m_proxy_enabled) { return true; }

	uint16_t err_code = 0;
	if (!m_session.proxy_connect(m_sAddr.c_str(), m_sPort,
								 m_proxy_license.c_str(),
								 err_code, msec) ||
		err_code != Zdp_Success)
	{
		ZVA_WARN("[%s] # proxy_connect() failed, errno=%d\n",
				 m_sUrl.c_str(), err_code);
		return false;
	}

	ZVA_INFO("[%s] # register_proxy() success\n", m_sUrl.c_str());
	return true;
}

bool audio_channel::register_channel(uint32_t msec)
{
	uint16_t err_code = 0;
	if (!m_session.append_session(m_session_id, err_code, msec) ||
		err_code != Zdp_Success)
	{
		ZVA_WARN("[%s] # append_session(%d) failed, errno=%d\n",
				 m_sUrl.c_str(), m_session_id, err_code);
		return false;
	}

	if (!m_session.register_audio(err_code, msec) ||
		err_code != Zdp_Success)
	{
		ZVA_WARN("[%s] # register_audio() failed, errno=%d\n",
				 m_sUrl.c_str(), err_code);
		return false;
	}

	ZVA_INFO("[%s] # register_stream() success\n", m_sUrl.c_str());
	return true;
}


bool audio_channel::start_playback()
{
	return false;
}

void audio_channel::stop_playback()
{

}

bool audio_channel::start_record()
{
	return false;
}

void audio_channel::stop_record()
{

}


// callback
void audio_channel::_message_cb(ZDP_PACKET* packet, void* user)
{
	audio_channel* pThis = reinterpret_cast<audio_channel*>(user);
	pThis->_recv_message(packet);
}

void audio_channel::_frame_cb(AUDIO_FRAME_PACKET* packet, void* user)
{
	audio_channel* pThis = reinterpret_cast<audio_channel*>(user);
	pThis->_recv_frame(packet);
}

void audio_channel::_event_cb(const zva_event* event, void* user)
{
	audio_channel* pThis = reinterpret_cast<audio_channel*>(user);
	pThis->_recv_event(event);
}


// handle message
void audio_channel::_recv_message(ZDP_PACKET* packet)
{
	if (packet->channel != emZdp_Channel_Audio) { return; }
	switch (packet->cmd)
	{
	case ZdpCmd_Audio_PlaybackStart:
		{
			AUDIO_CONFIG_INFO msg;
			if (0 == unpack_helper::unpack_audio_start_message(msg, packet->data, packet->data_len))
			{
				ZDP_AUDIO_CONFIG config;
				config.freq = msg.freq;
				config.channels = msg.channels;
				config.format = msg.format;
				m_playback.start(config, (SDL_AudioSpec *) &m_playback.sdlAudioSpec);
			}
		}
		break;
	case ZdpCmd_Audio_PlaybackStop:
		m_playback.stop();
		break;
	case ZdpCmd_Audio_PlaybackSet:
		{
			AUDIO_SETTING_INFO msg;
			if (0 == unpack_helper::unpack_audio_setting_message(msg, packet->data, packet->data_len))
			{
				ZDP_AUDIO_SETTING setting;
				setting.mute = msg.mute;
				setting.channels = msg.channels;
				setting.volume_l = msg.volume_l;
				setting.volume_r = msg.volume_r;
				m_playback.set(setting);
			}
		}
		break;
	case ZdpCmd_Audio_RecordStart:
		break;
	case ZdpCmd_Audio_RecordStop:
		break;
	default:
		break;
	}
}

void audio_channel::_recv_frame(AUDIO_FRAME_PACKET* frame)
{
	m_playback.play_frame(frame,m_playback.sdlAudioSpec);
}

void audio_channel::_recv_event(const zva_event* event)
{
	m_eventListMutex.lock();
	m_eventList.push(new zva_event(event->_type));
	m_eventListMutex.unlock();
}


// handle events
bool audio_channel::_handle_events()
{
	zva_event* pEvent = NULL;
	while (m_bWorking)
	{
		m_eventListMutex.lock();
		if (m_eventList.size() > 0)
		{
			pEvent = m_eventList.front();
			m_eventList.pop();
		}
		m_eventListMutex.unlock();

		if (NULL == pEvent) { return true; }

		if (m_bWorking)
		{
			switch (pEvent->_type)
			{

			default:
				break;
			}
		}

		delete pEvent;
		pEvent = NULL;
	}
	return true;
}

void audio_channel::_report_event(const zva_event& evt)
{
	if (m_event_cb)
	{
		m_event_cb(&evt, m_user);
	}
}

void audio_channel::_update_status(ZV_AUDIO_STATUS status)
{
	m_status = status;
	switch (status)
	{
	case emStatus_Stopped:
		_report_event(zva_event(emZvaEvent_Stopped));
		break;
	case emStatus_Starting:
		_report_event(zva_event(emZvaEvent_Starting));
		break;
	case emStatus_Started:
		_report_event(zva_event(emZvaEvent_Started));
		break;
	case emStatus_Connected:
		_report_event(zva_event(emZvaEvent_Connected));
		break;
	case emStatus_Proxy:
		_report_event(zva_event(emZvaEvent_Connected));
		break;
	case emStatus_Register:
		_report_event(zva_event(emZvaEvent_Registered));
		break;
	case emStatus_Ready:
		_report_event(zva_event(emZvaEvent_Running));
		break;
	case emStatus_Disconnected:
		_report_event(zva_event(emZvaEvent_Disconnected));
		break;
	case emStatus_Restarting:
		_report_event(zva_event(emZvaEvent_Restarting));
		break;
	default:
		break;
	}
}


void audio_channel::_workThread(void* pUserData)
{
	audio_channel* pThis = reinterpret_cast<audio_channel*>(pUserData);
	pThis->m_bWorkThread = true;
	ZVA_INFO("[%s] # _workThread Start\n", pThis->m_sUrl.c_str());

	pThis->_update_status(emStatus_Started);
	while (pThis->m_bWorking)
	{
		pThis->_handle_events();

		switch (pThis->m_status)
		{
		case emStatus_Started:
			if (pThis->connect_server())
			{
				pThis->_update_status(emStatus_Connected);
				continue;
			}
			break;
		case emStatus_Connected:
			if (pThis->register_proxy(5000))
			{
				pThis->_update_status(emStatus_Proxy);
				continue;
			}
			break;
		case emStatus_Proxy:
			if (pThis->register_channel(2000))
			{
				pThis->_update_status(emStatus_Ready);
				continue;
			}
			break;
		case emStatus_Register:
			pThis->_update_status(emStatus_Ready);
			break;
		case emStatus_Ready:

			break;
		case emStatus_Disconnected:
			pThis->_update_status(emStatus_Restarting);
			break;
		case emStatus_Restarting:
			pThis->_update_status(emStatus_Started);
		default:
			break;
		}
		zv_msleep(500);
	}

	pThis->_update_status(emStatus_Stopped);
	pThis->stop_record();
	pThis->disconnect_server();

	pThis->m_bWorkThread = false;
	ZVA_INFO("[%s] # _workThread Exit\n", pThis->m_sUrl.c_str());
}
