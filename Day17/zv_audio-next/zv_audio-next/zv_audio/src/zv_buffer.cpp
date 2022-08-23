
#include "zv_buffer.h"


// for zv_buffer

zv_buffer::zv_buffer(uint32_t size, zv_buffer_manager* parent)
	: _len(0)
	, _offset(0)
	, m_manager(parent)
	, m_ref(0)
{
	m_buffer = new char[size];
	if (m_buffer)
	{
		m_buffer_size = size;
		m_head = (ZDP_HEAD*)m_buffer;
		m_frame_head = (ZDP_FRAME_HEAD*)m_buffer;
	}
}

zv_buffer::~zv_buffer()
{
	if (m_buffer)
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}
}

void zv_buffer::reset()
{
	if (m_buffer)
	{
		memset(m_buffer, 0, m_buffer_size);
	}
}

void zv_buffer::ref()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_ref++;
}

void zv_buffer::unref()
{
	// class could not destroy with mutex locked.
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (0 == m_ref.load()) { return; }
		m_ref--;
	}

	if (0 == m_ref.load())
	{
		m_manager->free_buffer(this);
	}
}


// for zv_buffer_manager

zv_buffer_manager* zv_buffer_manager::g_this = NULL;
std::mutex zv_buffer_manager::m_instanceMutex;

zv_buffer_manager::zv_buffer_manager()
	: m_ref(0)
	, m_cache_space_used(0)
	, m_cache_buffer_count(0)
{
}

zv_buffer_manager::~zv_buffer_manager()
{
	ZVA_INFO("zv_buffer_manager::~zv_buffer_manager()\n");

	m_bufferMutex.lock();
	for (auto iter = m_buffers.begin(); iter != m_buffers.end(); iter++)
	{
		delete *iter;
	}
	m_buffers.clear();
	m_bufferMutex.unlock();
}

zv_buffer_manager* zv_buffer_manager::get_instance()
{
	if (!g_this)
	{
		std::lock_guard<std::mutex> lock(m_instanceMutex);
		if (!g_this)
		{
			g_this = new zv_buffer_manager();
		}
	}
	return g_this;
}

void zv_buffer_manager::ref()
{
	std::lock_guard<std::mutex> lock(m_instanceMutex);
	m_ref++;
}

void zv_buffer_manager::unref()
{
	std::lock_guard<std::mutex> lock(m_instanceMutex);

	if (0 == m_ref) { return; }
	if (0 == --m_ref)
	{
		delete g_this;
		g_this = NULL;
	}
}

zv_buffer* zv_buffer_manager::get_buffer(uint32_t len)
{
	zv_buffer* buffer = NULL;

	if (len <= MAX_AUDIO_BUFFER_SIZE)
	{
		len = MAX_AUDIO_BUFFER_SIZE;

		m_cacheBufferMutex.lock();
		if (m_cacheBuffers.size() > 0)
		{
			buffer = m_cacheBuffers.front();
			m_cacheBuffers.pop();
		}
		m_cacheBufferMutex.unlock();
	}

	if (!buffer)
	{
		buffer = malloc_buffer(len);
	}

	if (buffer)
	{
		buffer->reset();
		buffer->ref();
	}
	return buffer;
}

zv_buffer* zv_buffer_manager::malloc_buffer(uint32_t size)
{
	zv_buffer* buffer = new zv_buffer(size, this);
	if (!buffer)
	{
		ZVA_WARN("not enough memery!\n");
		return NULL;
	}

	if (!buffer->is_ready())
	{
		delete buffer;
		buffer = NULL;

		ZVA_WARN("not enough memery!\n");
		return NULL;
	}

	if (buffer)
	{
		m_bufferMutex.lock();
		m_buffers.insert(buffer);
		cache_space_add(size);
		m_bufferMutex.unlock();
	}
	return buffer;
}

void zv_buffer_manager::free_buffer(zv_buffer* buffer)
{
	if (buffer->size() <= MAX_AUDIO_BUFFER_SIZE &&
		m_cache_buffer_count <= MAX_AUDIO_BUFFER_COUNT)
	{
		m_cacheBufferMutex.lock();
		m_cacheBuffers.push(buffer);
		m_cacheBufferMutex.unlock();
	}
	else
	{
		m_bufferMutex.lock();
		m_buffers.erase(buffer);
		cache_space_reduce(buffer->size());
		m_bufferMutex.unlock();

		delete buffer;
		buffer = NULL;
	}
}

void zv_buffer_manager::cache_space_add(size_t size)
{
	m_cache_space_used += size;
	if (size <= MAX_AUDIO_BUFFER_SIZE)
	{
		m_cache_buffer_count++;
	}

	ZVA_DEBUG("zv_buffer add(%d), count(%d), cache size=%d\n",
		size, m_buffers.size(), m_cache_space_used);
}

void zv_buffer_manager::cache_space_reduce(size_t size)
{
	m_cache_space_used -= size;
	if (size <= MAX_AUDIO_BUFFER_SIZE)
	{
		m_cache_buffer_count--;
	}

	ZVA_DEBUG("zv_buffer reduce(%d), count(%d), cache size=%d\n",
		size, m_buffers.size(), m_cache_space_used);
}
