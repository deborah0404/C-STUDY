
#if defined(ZV_PLATFORM_WINDOWS)

#ifdef zv_platform_EXPORTS
#define ZV_PLATFORM_API __declspec(dllexport)
#else
#define ZV_PLATFORM_API __declspec(dllimport)
#endif

#elif defined(ZV_PLATFORM_LINUX)

#ifdef zv_platform_EXPORTS
#define ZV_PLATFORM_API  __attribute__((visibility("default")))
#else
#define ZV_PLATFORM_API
#endif

#endif
