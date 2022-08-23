
#if defined(ZV_PLATFORM_WINDOWS)

#ifdef zv_audio_EXPORTS
#define ZV_AUDIO_API __declspec(dllexport)
#else
#define ZV_AUDIO_API __declspec(dllimport)
#endif

#elif defined(ZV_PLATFORM_LINUX)

#ifdef zv_audio_EXPORTS
#define ZV_AUDIO_API  __attribute__((visibility("default")))
#else
#define ZV_AUDIO_API
#endif

#else
#define ZV_AUDIO_API
#endif


#include "zva_event.h"


ZV_AUDIO_API bool zva_init(const char* level, const char* log_file);

ZV_AUDIO_API void zva_uninit();

ZV_AUDIO_API void zva_set_callback(zva_event_callback cb, void* user);

ZV_AUDIO_API void zva_set_proxy(const char* addr,
								uint16_t port,
								const char* license,
								const char* transport);

ZV_AUDIO_API void zva_remove_proxy();

ZV_AUDIO_API bool zva_start(const char* addr,
							uint16_t port,
							const char* license,
							const char* transport,
							uint8_t id);

ZV_AUDIO_API void zva_stop();
