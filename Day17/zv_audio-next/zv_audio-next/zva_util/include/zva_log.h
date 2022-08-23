
#ifndef _ZVA_LOG_H_
#define _ZVA_LOG_H_


#include <stdio.h>
#include <stdint.h>


typedef enum {
	// normal
	ZVA_LOG_SESSION	= 0x01000000,	/* communication message for debug*/
	ZVA_LOG_DETAIL	= 0x02000000,	/* detailed message for debug*/
	ZVA_LOG_DEBUG	= 0x04000000,	/* general message for debug */
	ZVA_LOG_INFO	= 0x08000000,	/* important message for debug */

	// important
	ZVA_LOG_WARN	= 0x10000000,	/* warnning */
	ZVA_LOG_ERROR	= 0x20000000,	/* general Error */
	ZVA_LOG_FATAL	= 0x80000000,	/* critcal Error*/
} ZVA_LOG_TYPE;


void zva_log_open(const char* filename);

void zva_log_close();

void zva_log_enable(ZVA_LOG_TYPE type);

void zva_log_disable(ZVA_LOG_TYPE type);

void zva_log_level_set(uint32_t level);

uint32_t zva_log_level_get();

void zva_save_log(const char* file, uint32_t line, ZVA_LOG_TYPE type, const char* format, ...);


#define ZVA_SESSION(...) zva_save_log(__FILE__, __LINE__, ZVA_LOG_SESSION, __VA_ARGS__)
#define ZVA_DETAIL(...)  zva_save_log(__FILE__, __LINE__, ZVA_LOG_DETAIL, __VA_ARGS__)
#define ZVA_DEBUG(...)   zva_save_log(__FILE__, __LINE__, ZVA_LOG_DEBUG, __VA_ARGS__)
#define ZVA_INFO(...)    zva_save_log(__FILE__, __LINE__, ZVA_LOG_INFO, __VA_ARGS__)

#define ZVA_WARN(...)    zva_save_log(__FILE__, __LINE__, ZVA_LOG_WARN, __VA_ARGS__)
#define ZVA_ERROR(...)   zva_save_log(__FILE__, __LINE__, ZVA_LOG_ERROR, __VA_ARGS__)
#define ZVA_FATAL(...)   zva_save_log(__FILE__, __LINE__, ZVA_LOG_FATAL, __VA_ARGS__)


#endif	// _ZVA_LOG_H_
