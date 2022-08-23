
#ifndef _ZV_SOCKET_H_
#define _ZV_SOCKET_H_


#include <stdint.h>
#include "zv_platform.h"


// host -> network
ZV_PLATFORM_API uint64_t zv_htonll(uint64_t host64);
ZV_PLATFORM_API uint32_t zv_htonl(uint32_t host32);
ZV_PLATFORM_API uint16_t zv_htons(uint16_t host16);

// network -> host
ZV_PLATFORM_API uint64_t zv_ntohll(uint64_t net64);
ZV_PLATFORM_API uint32_t zv_ntohl(uint32_t net32);
ZV_PLATFORM_API uint16_t zv_ntohs(uint16_t net16);


// for socket
typedef uint64_t ZV_SOCKET;

// init socket
ZV_PLATFORM_API void zv_socket_init();

// disable Nagle algorithm
ZV_PLATFORM_API void zv_socket_nodelay(ZV_SOCKET);

// set non block
ZV_PLATFORM_API void zv_socket_nonblock(ZV_SOCKET);

// set read and write timeout
ZV_PLATFORM_API void zv_socket_timeout(ZV_SOCKET, uint32_t msec);


typedef enum {
	ZV_SOCK_STATE_Unconnected = 0,
	ZV_SOCK_STATE_Conected,
	ZV_SOCK_STATE_Closing,
	ZV_SOCK_STATE_Disconected
} ZV_SOCKET_STATE;
typedef void (*socket_state_cb)(ZV_SOCKET_STATE state, void* user);

typedef enum {
	ZV_SOCK_EVENT_Read		= 1 << 1,
	ZV_SOCK_EVENT_Write		= 1 << 2,
	ZV_SOCK_EVENT_Exception = 1 << 3,
} ZV_SOCKET_EVENT;
typedef void (*socket_event_cb)(int event, void* user);


// socket
class ZV_PLATFORM_API zv_socket
{
public:
	zv_socket() : m_fd(0) {}
	zv_socket(ZV_SOCKET fd) : m_fd(fd) {}
	virtual ~zv_socket() {}

	ZV_SOCKET fd() { return m_fd; }

	void set_nodelay();
	void set_nonblock();
	void set_timeout(uint32_t msec);

protected:
	volatile ZV_SOCKET m_fd;
};


// tcp socket
class ZV_PLATFORM_API zv_tcp_socket : public zv_socket
{
public:
	zv_tcp_socket(socket_state_cb cb, void* user);
	~zv_tcp_socket();

	int connect_to_host(const char* addr, uint16_t port);
	int disconnect_from_host();

	bool is_connected() { return (m_fd > 0); }
	ZV_SOCKET_STATE status() { return m_status; }
	int64_t get_last_err();

	int read_buffer(char* buf, uint32_t len);
	int write_buffer(const char* buf, uint32_t len);

private:
	void update_state(ZV_SOCKET_STATE state);
	void socket_err(int64_t error);

private:
	volatile ZV_SOCKET_STATE m_status;
	socket_state_cb m_state_cb;
	void* m_user;
};


// for select
#define ZV_SELECT_MAX_SIZE	32

typedef struct {
	ZV_SOCKET fds[ZV_SELECT_MAX_SIZE];
	uint32_t count;
} ZV_FD_SET;

ZV_PLATFORM_API bool zv_socket_select(ZV_FD_SET* read,
									  ZV_FD_SET* wirte,
									  ZV_FD_SET* exception,
									  uint32_t msec);


#endif // _ZV_SOCKET_H_
