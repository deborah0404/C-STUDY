
#include "audio_session.h"
#include <assert.h>


std::queue<cmd_waiter*> audio_session::m_waiter_pool;

std::mutex audio_session::m_waiter_pool_mutex;

audio_session::audio_session(uint8_t local_module,
							 message_report_func func1,
							 frame_report_func func2,
							 zva_event_callback func3,
							 void* user)
	: m_bWorking(false)
	, m_bWorkThread(false)
	, m_socket(NULL, this)
	, m_msgId(0)
	, m_session_id(0)
	, m_local_module(local_module)
	, m_message_cb(func1)
	, m_frame_cb(func2)
	, m_event_cb(func3)
	, m_user(user)
{
	m_pBufferManager = zv_buffer_manager::get_instance();
	m_pBufferManager->ref();
}

audio_session::~audio_session()
{
	ZVA_INFO("audio_session::~audio_session()\n");

	destroy_waiters();

	m_pBufferManager->unref();
}

bool audio_session::connect_server(const char* addr, uint16_t port)
{
	if (m_bWorkThread) { return false; }

	ZVA_DEBUG("audio_session::connect_server()\n");
	if (0 == m_socket.connect_to_host(addr, port))
	{
		m_socket.set_timeout(100);

		m_bWorking = true;
		zv_thread_create(0, ZV_THREAD_PRIORITY_INVALID, _workThread, this);
		return true;
	}
	return false;
}

void audio_session::disconnect_server()
{
	ZVA_DEBUG("audio_session(%d) # disconnect_server\n", m_socket.fd());

	m_bWorking = false;
	m_socket.disconnect_from_host();

	while (m_bWorkThread)
	{
		zv_msleep(100);
	}
}

void audio_session::ping_server()
{
	MAIN_HEARTBEAT msg;
	msg.timestamp1 = (uint32_t)zv_get_time_msec();

	uint8_t channel = emZdp_Channel_Main;
	uint8_t cmd = ZdpCmd_Main_HeartBeat;
	uint16_t msg_id = m_msgId++;
	send_message(msg_id, emZdp_Module_Host, m_local_module, channel, cmd,
				 m_session_id, (char*)&msg, sizeof(msg));
}

bool audio_session::proxy_connect(const char* addr, uint16_t port, const char* license,
								   uint16_t& err_code, uint32_t msec)
{
	// send cmd
	PROXY_CONNECT request;
	memset((void*)&request, 0, sizeof(PROXY_CONNECT));
	request.type = 0;
	request.addr_len = strlen(addr);
	request.license_len = strlen(license);
	request.port = port;
	strncpy((char*)request.addr, addr, 64);
	strncpy((char*)request.license, license, 512);
	unpack_helper::hton_proxy_connect_info(request);

	uint8_t channel = emZdp_Channel_Proxy;
	uint8_t cmd = ZdpCmd_Proxy_Connect;
	uint16_t msg_id = m_msgId++;
	send_message(msg_id, emZdp_Modele_Proxy, m_local_module, channel, cmd,
		m_session_id, (char*)&request, sizeof(PROXY_CONNECT));

	if (msec == 0) { return true; }

	// wait response
	uint32_t response_len = 0;
	return wait_for_response(channel, cmd, msg_id, NULL, response_len, err_code, msec);
}

bool audio_session::append_session(uint8_t id, uint16_t& err_code, uint32_t msec)
{
	// send cmd
	MAIN_APPEND_SESSION request;
	memset((void*)&request, 0, sizeof(MAIN_APPEND_SESSION));
	request.id = id;
	request.type = m_local_module;

	uint8_t channel = emZdp_Channel_Main;
	uint8_t cmd = ZdpCmd_Main_AppendSession;
	uint16_t msg_id = m_msgId++;
	send_message(msg_id, emZdp_Module_Host, m_local_module, channel, cmd,
				 m_session_id, (char*)&request, sizeof(MAIN_REGISTER));

	if (msec == 0) { return true; }

	// wait response
	uint32_t response_len = 0;
	bool b_ret =
		wait_for_response(channel, cmd, msg_id, NULL, response_len, err_code, msec);
	if (b_ret)
	{
		m_session_id = id;
	}
	return b_ret;
}

bool audio_session::register_audio(uint16_t& err_code, uint32_t msec)
{
	uint8_t channel = emZdp_Channel_Audio;
	uint8_t cmd = ZdpCmd_Audio_Register;
	uint16_t msg_id = m_msgId++;
	send_message(msg_id, emZdp_Module_Host, m_local_module, channel, cmd,
				 m_session_id, NULL, 0);

	if (msec == 0) { return true; }

	// wait response
	uint32_t response_len = 0;
	return wait_for_response(channel, cmd, msg_id, NULL, response_len, err_code, msec);
}


// wait for response
bool audio_session::wait_for_response(uint8_t channel, uint8_t cmd, uint16_t msg_id,
									   char* data, uint32_t& len, uint16_t& err_code,
									   uint32_t msec)
{
	if (msec <= 0) { return false; }

	cmd_waiter* waiter = get_waiter();
	if (!waiter) { return false; }

	bool b_recved = false;
	waiter->init(channel, cmd, msg_id);
	if (wait_for_response(waiter, msec))
	{
		ZDP_PACKET packet;
		unpack_helper::unpack_cmd_buffer(packet, waiter->_buffer, waiter->_len);
		if (packet.error_code == Zdp_Success)
		{
			if (data && len > 0)
			{
				len = (len >= packet.data_len) ? packet.data_len : len;
				memcpy(data, packet.data, len);
			}
		}

		b_recved = true;
		err_code = packet.error_code;
	}

	free_waiter(waiter);
	return b_recved;
}

bool audio_session::wait_for_response(cmd_waiter* waiter, uint32_t msec)
{
	bool b_recved = false;
	insert_waiter(waiter);

	if (waiter->wait(msec))
	{
		b_recved = true;
	}

	remove_waiter(waiter);
	return b_recved;
}


int audio_session::send_message(uint16_t msg_id, uint8_t dest, uint8_t src,
								 uint8_t channel, uint8_t cmd, uint8_t index,
								 const char* data, uint16_t len, uint8_t error)
{
	zv_buffer* buffer = m_pBufferManager->get_buffer(MAX_AUDIO_BUFFER_SIZE);
	assert(buffer && (sizeof(ZDP_HEAD) + len) <= MAX_AUDIO_BUFFER_SIZE);

	int buf_len = unpack_helper::pack_cmd_buffer(
		*buffer, msg_id, dest, src, channel, cmd, index, data, len, error);
	int wsize = write_buffer(buffer->buffer(), buf_len);

	buffer->unref();
	return (wsize == buf_len);
}

void audio_session::handle_message(ZDP_PACKET& packet)
{
	ZVA_SESSION("dest=%02X, src=%02X, channel=%02X, cmd=%02X, msg_id=%d data_len=%d, code=%d\n",
		packet.dest, packet.src, packet.channel, packet.cmd,
		packet.msg_id, packet.data_len, packet.error_code);

	// 唤醒等待线程
	m_waiter_list_mutex.lock();
	for (auto iter = m_waiter_list.begin(); iter != m_waiter_list.end(); iter++)
	{
		cmd_waiter* waiter = *iter;
		if (waiter->_channel == packet.channel &&
			waiter->_cmd == packet.cmd/* &&
			waiter->_msg_id == packet.msg_id*/)
		{
			waiter->_len = packet.head_len + packet.data_len;
			if (waiter->_len <= MAX_AUDIO_BUFFER_SIZE)
			{
				memcpy(waiter->_buffer, packet.buffer, waiter->_len);
			}
			else
			{
				memcpy(waiter->_buffer, packet.buffer, MAX_AUDIO_BUFFER_SIZE);
			}

			m_waiter_list.erase(iter);
			zv_semaphore_post(waiter->_sem, 1);
			break;
		}
	}
	m_waiter_list_mutex.unlock();

	report_message(&packet);
}

void audio_session::handle_frame(ZDP_FRAME_PACKET& packet)
{
	AUDIO_FRAME_PACKET frame;
	if (0 != unpack_helper::unpack_audio_frame(frame, packet.data, packet.data_len))
	{
		ZVA_DEBUG("unpack_audio_data_message() Err\n");
		return;
	}
	//pyx
	if (frame.frame_len != packet.data_len)
	{
		ZVA_DEBUG("handle_frame() Err [%d/%d]\n", frame.frame_len, packet.data_len);
		return;
	}

	report_frame(&frame);

	}

void audio_session::report_message(ZDP_PACKET* packet)
{
	if (m_message_cb)
	{
		m_message_cb(packet, m_user);
	}
}

void audio_session::report_frame(AUDIO_FRAME_PACKET* frame)
{
	if (m_frame_cb)
	{
		m_frame_cb(frame, m_user);
	}
}

void audio_session::report_event(const zva_event& event)
{
	if (m_event_cb)
	{
		m_event_cb(&event, m_user);
	}
}


int audio_session::read_buffer(char* buf, uint32_t len)
{
	if (!m_socket.is_connected())
	{
		return -1;
	}
	return m_socket.read_buffer(buf, len);
}

int audio_session::write_buffer(const char* buf, uint32_t len)
{
	if (!m_socket.is_connected())
	{
		return -1;
	}
	return m_socket.write_buffer(buf, len);
}


// 数据接收线程
void audio_session::_workThread(void* user)
{
	audio_session* pThis = reinterpret_cast<audio_session*>(user);
	pThis->m_bWorkThread = true;
	ZVA_DEBUG("audio_session(%d) # _workThread IN\n", pThis->m_socket.fd());

	ZV_SOCKET sock_fd;
	ZV_FD_SET fd_read;
	ZV_FD_SET fd_write;
	ZV_FD_SET fd_exception;
	while (true)
	{
		if (!pThis->m_socket.is_connected())
		{
			goto ret_code;
		}

		sock_fd = pThis->m_socket.fd();
		fd_read.fds[0] = sock_fd;
		fd_write.fds[0] = sock_fd;
		fd_exception.fds[0] = sock_fd;

		fd_read.count = 1;
		fd_write.count = 0;
		fd_exception.count = 1;

		if (zv_socket_select(&fd_read, &fd_write, &fd_exception, 1000))
		{
			if (fd_exception.count)
			{
				goto ret_code;
			}

			if (fd_read.count)
			{
				int error_code = pThis->recv_message();
				if (emSessionErr_DISCONNECT == error_code)
				{
					ZVA_INFO("audio_session(%d) # _workThread() Closed\n", pThis->m_socket.fd());
					goto ret_code;
				}
			}
		}
	}

ret_code:
	if (pThis->m_bWorking)
	{
		pThis->m_bWorking = false;
		pThis->m_socket.disconnect_from_host();
		pThis->report_event(zva_event(emZvaEvent_Disconnected));
	}

	pThis->m_bWorkThread = false;
	ZVA_DEBUG("audio_session(%d) # _workThread OUT\n", pThis->m_socket.fd());
}

int audio_session::recv_message()
{
	zv_buffer* pbuffer = m_pBufferManager->get_buffer(MAX_AUDIO_BUFFER_SIZE);
	if (!pbuffer)
	{
		return emSessionErr_ALLOCBUFFER;
	}

	int cmd_head_size = sizeof(ZDP_HEAD);
	int frame_head_size = sizeof(ZDP_FRAME_HEAD);
	uint32_t head_size = (cmd_head_size > frame_head_size) ? cmd_head_size : frame_head_size;
	int error_code = emSessionErr_NOERR;
	{
		// 读报文头
		error_code = recv_data(pbuffer->buffer(), head_size);
		if (error_code != emSessionErr_NOERR)
		{
			goto gt_ret;
		}

		// cmd data
		uint8_t* temp_buffer = (uint8_t*)pbuffer->buffer();
		if (temp_buffer[0] == 0x55 && temp_buffer[1] == 0x55)
		{
			ZDP_PACKET cmd_packet;

			// 解析报文头
			if (0 != unpack_helper::unpack_cmd_buffer(cmd_packet, pbuffer->buffer(), head_size))
			{
				error_code = emSessionErr_PACKETERR;
				goto gt_ret;
			}

			// 是否有扩展报文头
			if (cmd_packet.head_len > head_size)
			{
				error_code = recv_data(pbuffer->buffer() + head_size, cmd_packet.head_len - head_size);
				if (error_code != emSessionErr_NOERR)
				{
					goto gt_ret;
				}
			}

			// 报文是否超长
			assert(cmd_packet.head_len + cmd_packet.data_len <= MAX_AUDIO_BUFFER_SIZE);

			// 读报文内容
			error_code = recv_data((char*)cmd_packet.data, cmd_packet.data_len);
			if (error_code != emSessionErr_NOERR)
			{
				goto gt_ret;
			}

			handle_message(cmd_packet);
			goto gt_ret;
		}
		// frame data
		else if (temp_buffer[0] == 0xAA && temp_buffer[1] == 0xAA)
		{
			ZDP_FRAME_PACKET frame_packet;

			// 解析报文头
			if (0 != unpack_helper::unpack_frame_buffer(frame_packet, pbuffer->buffer(), head_size))
			{
				error_code = emSessionErr_PACKETERR;
				goto gt_ret;
			}

			// 是否有扩展报文头
			if (frame_packet.head_len > head_size)
			{
				error_code = recv_data(pbuffer->buffer() + head_size, frame_packet.head_len - head_size);
				if (error_code != emSessionErr_NOERR)
				{
					goto gt_ret;
				}
			}

			// 报文是否超长
			assert(frame_packet.head_len + frame_packet.data_len <= MAX_AUDIO_BUFFER_SIZE);

			// 读报文内容
			error_code = recv_data((char*)frame_packet.data, frame_packet.data_len);
			if (error_code != emSessionErr_NOERR)
			{
				goto gt_ret;
			}

			handle_frame(frame_packet);
			goto gt_ret;
		}
		else
		{
			ZVA_DEBUG("recv_message Head Error!\n");
			goto gt_ret;
		}
	}

gt_ret:
	if (pbuffer)
	{
		pbuffer->unref();
		pbuffer = NULL;
	}

	if (emSessionErr_DISCONNECT == error_code)
	{
		ZVA_DEBUG("recv_message Socket Closed!\n");
	}
	else if (emSessionErr_PACKETERR == error_code)
	{
		ZVA_DEBUG("recv_message packet Error\n");
	}

	return error_code;
}

int audio_session::recv_data(char* buffer, uint32_t data_size)
{
	int error_code = emSessionErr_NOERR;
	uint32_t read_size = 0;
	while (read_size < data_size)
	{
		int size = read_buffer(buffer + read_size, data_size - read_size);
		if (size < 0)
		{
			ZVA_WARN("audio_session(%d) # recv_data size(%d) errcode(%d)\n",
				m_socket.fd(), size, m_socket.get_last_err());

			error_code = emSessionErr_DISCONNECT;
			return error_code;
		}
		read_size += size;
	}
	return error_code;
}


// for cmd waiter
cmd_waiter* audio_session::get_waiter()
{
	cmd_waiter* waiter = NULL;

	m_waiter_pool_mutex.lock();
	if (m_waiter_pool.size() > 0)
	{
		waiter = m_waiter_pool.front();
		m_waiter_pool.pop();
	}
	m_waiter_pool_mutex.unlock();

	if (!waiter)
	{
		waiter = new cmd_waiter;
	}

	return waiter;
}

void audio_session::free_waiter(cmd_waiter* waiter)
{
	m_waiter_pool_mutex.lock();
	m_waiter_pool.push(waiter);
	m_waiter_pool_mutex.unlock();
}

void audio_session::destroy_waiters()
{
	cmd_waiter* waiter = NULL;

	m_waiter_pool_mutex.lock();
	while (m_waiter_pool.size() > 0)
	{
		waiter = m_waiter_pool.front();
		m_waiter_pool.pop();
		delete waiter;
	}
	m_waiter_pool_mutex.unlock();
}

void audio_session::insert_waiter(cmd_waiter* waiter)
{
	m_waiter_list_mutex.lock();
	m_waiter_list.push_back(waiter);
	m_waiter_list_mutex.unlock();
}

void audio_session::remove_waiter(cmd_waiter* waiter)
{
	m_waiter_list_mutex.lock();
	for (auto iter = m_waiter_list.begin(); iter != m_waiter_list.end(); iter++)
	{
		if (*iter == waiter)
		{
			m_waiter_list.erase(iter);
			break;
		}
	}
	m_waiter_list_mutex.unlock();
}


// for cmd_waiter

cmd_waiter::cmd_waiter() : _channel(0), _cmd(0), _msg_id(0), _len(0)
{
	_buffer = new char[MAX_AUDIO_BUFFER_SIZE];
	_sem = zv_semaphore_create(1);
}

cmd_waiter::~cmd_waiter()
{
	delete[] _buffer;
	zv_semaphore_close(_sem);
}

void cmd_waiter::init(uint8_t channel, uint8_t cmd, uint16_t id)
{
	_channel = channel;
	_cmd = cmd;
	_msg_id = id;
	_len = 0;
}

bool cmd_waiter::wait(uint32_t msec)
{
	int64_t msec_start = zv_get_time_msec();
	int64_t msec_wait = msec;
	bool b_recved = false;
	while (msec_wait > 0)
	{
		if (zv_semaphore_time_wait(_sem, msec_wait))
		{
			b_recved = true;
		}
		if (b_recved)
		{
			break;
		}
		msec_wait = msec + msec_start - zv_get_time_msec();
	}
	return b_recved;
}
