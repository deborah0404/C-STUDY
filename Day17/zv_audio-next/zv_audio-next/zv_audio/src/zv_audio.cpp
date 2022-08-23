
#include "zv_audio.h"
#include "audio_channel.h"


static audio_channel* gs_audio_p = NULL;
static std::mutex gs_audio_mutex;


static bool zva_create()
{
	std::lock_guard<std::mutex> lock(gs_audio_mutex);

	if (gs_audio_p) { return true; }

	ZVA_DEBUG("zva_create\n");

	gs_audio_p = new audio_channel();
	assert(gs_audio_p);

	return true;
}

static void zva_destroy()
{
	std::lock_guard<std::mutex> lock(gs_audio_mutex);

	if (!gs_audio_p) { return; }

	ZVA_DEBUG("zva_destroy\n");

	gs_audio_p->stop_channel();
	delete gs_audio_p;
	gs_audio_p = NULL;
}


bool zva_init(const char* level, const char* log_file)
{
	if (!zva_create()) { return false; }

	int log_level = 0;
	if (level)
	{
		if (strcmp(level, "all") == 0)
		{
			log_level = 0xFFFFFFFF;
		}
		else if (strcmp(level, "debug") == 0)
		{
			log_level = ZVA_LOG_FATAL | ZVA_LOG_ERROR | ZVA_LOG_WARN | ZVA_LOG_INFO | ZVA_LOG_DEBUG;
		}
		else if (strcmp(level, "low") == 0)
		{
			log_level = ZVA_LOG_FATAL | ZVA_LOG_ERROR | ZVA_LOG_WARN | ZVA_LOG_INFO;
		}
		else if (strcmp(level, "normal") == 0)
		{
			log_level = ZVA_LOG_FATAL | ZVA_LOG_ERROR | ZVA_LOG_WARN;
		}
		else if (strcmp(level, "high") == 0)
		{
			log_level = ZVA_LOG_FATAL | ZVA_LOG_ERROR;
		}
		else if (strcmp(level, "none") == 0)
		{
			log_level = ZVA_LOG_FATAL;
		}
	}

	if (log_file && strlen(log_file) > 0)
	{
		zva_log_open(log_file);
	}
	zva_log_level_set(log_level);

	return true;
}


void zva_uninit()
{
	zva_destroy();

	zva_log_close();
}


void zva_set_callback(zva_event_callback cb, void* user)
{
	std::lock_guard<std::mutex> lock(gs_audio_mutex);

	assert(gs_audio_p);

	gs_audio_p->set_callback(cb, user);
}

void zva_set_proxy(const char* addr, uint16_t port, const char* license, const char* transport)
{
	std::lock_guard<std::mutex> lock(gs_audio_mutex);

	assert(gs_audio_p);

	gs_audio_p->set_proxy(addr, port, license, transport);
}

void zva_remove_proxy()
{
	std::lock_guard<std::mutex> lock(gs_audio_mutex);

	assert(gs_audio_p);

	gs_audio_p->remove_proxy();
}

bool zva_start(const char* addr, uint16_t port, const char* license, const char* transport, uint8_t id)
{
	std::lock_guard<std::mutex> lock(gs_audio_mutex);

	assert(gs_audio_p);

	return gs_audio_p->start_channel(addr, port, license, transport, id);
}

void zva_stop()
{
	std::lock_guard<std::mutex> lock(gs_audio_mutex);

	assert(gs_audio_p);

	gs_audio_p->stop_channel();
}
