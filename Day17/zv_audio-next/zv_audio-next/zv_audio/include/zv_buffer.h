
#ifndef _ZV_FRMAE_H_
#define _ZV_FRMAE_H_


#include "zva_common.h"


#define MAX_AUDIO_BUFFER_SIZE	(1024 * 8)	 // 最大支持8K
#define MAX_AUDIO_BUFFER_COUNT	(500)


// forward
class zv_buffer_manager;

class zv_buffer
{
	friend class zv_buffer_manager;

public:
	void ref();
	void unref();
	void reset();

	ZDP_HEAD* head() { return m_head; }
	ZDP_FRAME_HEAD* audio_head() { return m_frame_head; }

	char* buffer() { return m_buffer; }
	uint32_t size() { return m_buffer_size; }

public:
	uint32_t _len;
	uint32_t _offset;

private:
	zv_buffer(uint32_t size, zv_buffer_manager*);
	~zv_buffer();

	bool is_ready() { return m_buffer; }

private:
	zv_buffer_manager* m_manager;
	std::atomic<int> m_ref;
	std::mutex m_mutex;

	ZDP_HEAD* m_head;
	ZDP_FRAME_HEAD* m_frame_head;

	char* m_buffer;
	uint32_t m_buffer_size;
};


class zv_buffer_manager
{
friend class zv_buffer;

public:
	static zv_buffer_manager* get_instance();

	void ref();
	void unref();

	zv_buffer* get_buffer(uint32_t size);

private:
	zv_buffer_manager();
	~zv_buffer_manager();

	zv_buffer* malloc_buffer(uint32_t);
	void free_buffer(zv_buffer*);

	inline void cache_space_add(size_t);
	inline void cache_space_reduce(size_t);

private:
	static zv_buffer_manager* g_this;
	static std::mutex m_instanceMutex;
	std::atomic<int> m_ref;

	size_t m_cache_space_used;
	size_t m_cache_buffer_count;
	std::set<zv_buffer*> m_buffers;
	std::mutex m_bufferMutex;

	std::queue<zv_buffer*> m_cacheBuffers;
	std::mutex m_cacheBufferMutex;
};


#endif	// _ZV_FRMAE_H_
