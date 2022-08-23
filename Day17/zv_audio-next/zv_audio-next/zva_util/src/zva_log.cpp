
#include "zva_log.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>


#ifdef ZV_PLATFORM_WINDOWS
#define FOPEN(f, n, m)			f = fopen(n, m)
#define FREOPEN(f, n, m, s)		f = freopen(n, m, s)
#define LOCALTIME(lt, tt)		localtime_s(&lt, &tt)
static const char gs_log_path[] = "C:/Windows/Temp/";
#elif defined ZV_PLATFORM_LINUX
#define FOPEN(f, n, m)			f = fopen(n, m)
#define FREOPEN(f, n, m, s)		f = freopen(n, m, s)
#define LOCALTIME(lt, tt)		lt = *localtime(&tt)
static const char gs_log_path[] = "/var/log/";
#endif


static uint32_t gs_log_level = ZVA_LOG_FATAL | ZVA_LOG_ERROR | ZVA_LOG_WARN;

static FILE* gs_stream = NULL;

void zva_log_open(const char* filename)
{
	if (gs_stream) { return; }
	if (!filename || strlen(filename) == 0) { return; }

	char name[64] = { 0 };
	snprintf(name, 64, "%s%s", gs_log_path, filename);

	FILE* pfile = NULL;
	FOPEN(pfile, name, "a+");
	if (pfile)
	{
		gs_stream = pfile;
	}
}

void zva_log_close()
{
	if (!gs_stream) { return; }

	fclose(gs_stream);
	gs_stream = NULL;
}

void zva_log_enable(ZVA_LOG_TYPE type)
{
	gs_log_level |= type;
}

void zva_log_disable(ZVA_LOG_TYPE type)
{
	gs_log_level ^= type;
}

void zva_log_level_set(uint32_t level)
{
	gs_log_level = level;
}

uint32_t zva_log_level_get()
{
	return gs_log_level;
}


inline const char* _log_type(ZVA_LOG_TYPE level)
{
	switch (level)
	{
		case ZVA_LOG_FATAL:
			return "FATAL  ";
		case ZVA_LOG_ERROR:
			return "ERROR  ";
		case ZVA_LOG_WARN:
			return "WARN   ";
		case ZVA_LOG_INFO:
			return "INFO   ";
		case ZVA_LOG_DEBUG:
			return "DEBUG  ";
		case ZVA_LOG_DETAIL:
			return "DETAIL ";
		case ZVA_LOG_SESSION:
			return "SESSION";
		default:
			return "";
	}
}

void zva_save_log(const char* file, uint32_t line, ZVA_LOG_TYPE type, const char* format, ...)
{
	if (type & gs_log_level || ZVA_LOG_FATAL == type)
	{
		char str_dt[64] = {0};
		time_t tt = time(NULL);
		struct tm lt;
		LOCALTIME(lt, tt);
		strftime(str_dt, 64, "%Y-%m-%d %H:%M:%S", &lt);

		char str_format[1024] = {0};
		snprintf(str_format, 1024, "%s | %s | %s:%d # %s", str_dt,
				 _log_type(type), file, line, format);

		va_list valist;
		va_start(valist, format);
		if (gs_stream)
		{
			vfprintf(gs_stream, str_format, valist);
			fflush(gs_stream);
		}
		else
		{
			vfprintf(stdout, str_format, valist);
		}
		va_end(valist);
	}
}
