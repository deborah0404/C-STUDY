
#ifndef _VIDEO_SESSION_H_
#define _VIDEO_SESSION_H_


#include "zva_common.h"
#include "zv_buffer.h"
#include "unpack_helper.h"


enum {
	emSessionErr_NOERR = 0,
	emSessionErr_DISCONNECT = 1,
	emSessionErr_ALLOCBUFFER = 2,
	emSessionErr_PACKETERR = 3,
};


// forward
class cmd_waiter;


class audio_session
{
public:
	typedef void (*message_report_func)(ZDP_PACKET*, void*);
	typedef void (*frame_report_func)(AUDIO_FRAME_PACKET*, void*);

	audio_session(uint8_t local_module, message_report_func,
				   frame_report_func, zva_event_callback, void*);
	~audio_session();

	bool connect_server(const char*, uint16_t);
	void disconnect_server();
	void ping_server();

	bool proxy_connect(const char* addr, uint16_t port, const char* license,
					   uint16_t& err_code, uint32_t);
	bool append_session(uint8_t id, uint16_t& err_code, uint32_t);
	bool register_audio(uint16_t& err_code, uint32_t);

private:
	bool wait_for_response(uint8_t channel, uint8_t cmd, uint16_t id,
						   char* data, uint32_t& len, uint16_t& err_code,
						   uint32_t msec);
	bool wait_for_response(cmd_waiter* waiter, uint32_t msec);

	int send_message(uint16_t msg_id, uint8_t dest, uint8_t src, uint8_t channel,
					 uint8_t cmd, uint8_t index, const char* data, uint16_t len,
					 uint8_t error = 0);
	void handle_message(ZDP_PACKET&);
	void handle_frame(ZDP_FRAME_PACKET&);

	void report_message(ZDP_PACKET*);
	void report_frame(AUDIO_FRAME_PACKET*);
	void report_event(const zva_event&);

private:
	int read_buffer(char* buf, uint32_t len);
	int write_buffer(const char* buf, uint32_t len);

	static void _workThread(void*);
	int recv_message();
	int recv_data(char* buffer, uint32_t data_size);

	cmd_waiter* get_waiter();
	void free_waiter(cmd_waiter*);
	void destroy_waiters();

	void insert_waiter(cmd_waiter*);
	void remove_waiter(cmd_waiter*);

private:
	volatile bool m_bWorking;
	volatile bool m_bWorkThread;
	zv_tcp_socket m_socket;
	std::atomic<uint16_t> m_msgId;
	uint8_t m_session_id;

	uint8_t m_local_module;
	message_report_func m_message_cb;
	frame_report_func m_frame_cb;
	zva_event_callback m_event_cb;
	void* m_user;

private:
	zv_buffer_manager* m_pBufferManager;

	static std::queue<cmd_waiter*> m_waiter_pool;
	static std::mutex m_waiter_pool_mutex;

	std::vector<cmd_waiter*> m_waiter_list;
	std::mutex m_waiter_list_mutex;
};


class cmd_waiter
{
	friend class audio_session;

public:
	cmd_waiter();
	~cmd_waiter();

	void init(uint8_t channel, uint8_t cmd, uint16_t id);
	bool wait(uint32_t msec);
	char* buffer()
	{
		return _buffer;
	}
	uint32_t len()
	{
		return _len;
	}

private:
	ZV_HANDLE _sem;

	uint8_t _channel;
	uint8_t _cmd;
	uint16_t _msg_id;

	char* _buffer;
	uint32_t _len;
	uint16_t _err_code;
};


#endif	// _VIDEO_SESSION_H_
