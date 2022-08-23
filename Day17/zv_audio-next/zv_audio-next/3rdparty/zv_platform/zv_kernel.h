
#ifndef _ZV_KERNEL_H_
#define _ZV_KERNEL_H_


#include <stdint.h>
#include "zv_platform.h"

#define ZV_HANDLE	void*


/*Time Define==============================================================================================*/
ZV_PLATFORM_API uint32_t zv_get_tick_count();
ZV_PLATFORM_API int64_t zv_get_time_msec();
ZV_PLATFORM_API void zv_msleep(uint32_t msec);
/*==========================================================================================================*/


/*Event Define==============================================================================================*/
/*Mutex Functions*/
ZV_PLATFORM_API ZV_HANDLE zv_mutex_create();
ZV_PLATFORM_API void zv_mutex_close(ZV_HANDLE hMutex);
ZV_PLATFORM_API void zv_mutex_lock(ZV_HANDLE hMutex);
ZV_PLATFORM_API bool zv_mutex_unlock(ZV_HANDLE hMutex);
ZV_PLATFORM_API bool zv_mutex_try_lock(ZV_HANDLE hMutex, uint32_t msec);

/*Event Functions*/
ZV_PLATFORM_API ZV_HANDLE zv_event_create(bool bManualReset, bool bInitState);
ZV_PLATFORM_API void zv_event_close(ZV_HANDLE hEvent);
ZV_PLATFORM_API bool zv_event_wait(ZV_HANDLE hEvent, uint32_t msec);
ZV_PLATFORM_API bool zv_event_set(ZV_HANDLE hEvent);
ZV_PLATFORM_API bool zv_event_reset(ZV_HANDLE hEvent);

/*Semaphore Functions*/
ZV_PLATFORM_API ZV_HANDLE zv_semaphore_create(uint32_t dwMaxCount);
ZV_PLATFORM_API void zv_semaphore_close(ZV_HANDLE hSema);
ZV_PLATFORM_API void zv_semaphore_wait(ZV_HANDLE hSema);
ZV_PLATFORM_API bool zv_semaphore_time_wait(ZV_HANDLE hSema, uint32_t msec);
ZV_PLATFORM_API bool zv_semaphore_post(ZV_HANDLE hSema, uint32_t count = 1);
/*==========================================================================================================*/


/*Thread Define=============================================================================================*/
typedef void(*ZV_THREAD_PROC)(void* lpParam);
typedef enum {
	ZV_THREAD_PRIORITY_INVALID = 0,
	ZV_THREAD_PRIORITY_TIME_CRITICAL,
	ZV_THREAD_PRIORITY_HIGHEST,
	ZV_THREAD_PRIORITY_ABOVE_NORMAL,
	ZV_THREAD_PRIORITY_NORMAL,
	ZV_THREAD_PRIORITY_BELOW_NORMAL,
	ZV_THREAD_PRIORITY_LOWEST,
	ZV_THREAD_PRIORITY_IDLE
} ZV_THREAD_PRIORITY;

ZV_PLATFORM_API ZV_HANDLE zv_thread_create(int32_t stackSize, ZV_THREAD_PRIORITY priority, ZV_THREAD_PROC lpThreadProc, void* lpParam);
ZV_PLATFORM_API bool zv_thread_join(ZV_HANDLE hThread);
ZV_PLATFORM_API bool zv_thread_close(ZV_HANDLE hThread);
ZV_PLATFORM_API bool zv_thread_set_priority(ZV_HANDLE hThread, ZV_THREAD_PRIORITY priority);
ZV_PLATFORM_API uint32_t zv_local_thread_id();
/*==========================================================================================================*/


#endif // _ZV_KERNEL_H_
