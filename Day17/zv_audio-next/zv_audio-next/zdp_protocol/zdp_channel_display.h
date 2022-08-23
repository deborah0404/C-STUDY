
#ifndef _ZDP_CHANNEL_DISPLAY_H_
#define _ZDP_CHANNEL_DISPLAY_H_


#include <stdint.h>
#include "zdp_interface.h"
#include "zdp_channel_types.h"
#include "zdp_display_types.h"


#define DISPLAY_MAX_MONITOR_COUNT		8
#define DISPLAY_MAX_IPADDR_LEN			46


/***************************************************************************
 * ZDP protocol
 */

/* ZDP Display Cmds */
typedef enum {
	ZdpCmd_Display_Monitor_Get			= 0x01,		/* 获取云主机中显示器配置 */
	ZdpCmd_Display_Monitor_Set			= 0x02,		/* 设置云主机中显示器配置 */
	ZdpCmd_Display_Screen_Set			= 0x03,		/* 设置显示器分辨率配置 */
	ZdpCmd_Display_Ratio_Get			= 0x04,		/* 获取屏幕可用分辨率 */

	ZdpCmd_Display_Stream_Start			= 0x11,		/* start stream push */
	ZdpCmd_Display_Stream_Stop			= 0x12,		/* stop stream push */
	ZdpCmd_Display_Stream_Config_Get	= 0x13,		/* 获取视频编码配置 */
	ZdpCmd_Display_Stream_Config_Set	= 0x14,		/* 设置视频编码配置 */
	ZdpCmd_Display_Cursor_Config_Get	= 0x15,		/* 获取光标模式 */
	ZdpCmd_Display_Cursor_Config_Set	= 0x16,		/* 设置光标模式 */

	ZdpCmd_Display_Monitor_Change		= 0xA1,		/* 云主机显示器配置更改通知 */
} ZDP_CMD_DISPLAY;


/* ZdpCmd_Display_Monitor_Get & ack
 * ZdpCmd_Display_Monitor_Set & ack
 * ZdpCmd_Display_Monitor_Change
 * 设置云主机中显示器配置
 * 获取云主机中显示器配置
 * 云主机显示器配置更改通知
 */
typedef struct {
	uint8_t count;
	uint8_t main_monitor_index;	/* 主屏幕序号 */
	uint8_t reserved[22];
	uint32_t total_width;		/* 屏幕画布总宽度 */
	uint32_t total_height;		/* 屏幕画布总高度 */
	ZDP_MONITOR_INFO infos[DISPLAY_MAX_MONITOR_COUNT];
} DISPLAY_MONITOR_INFO;

typedef struct {
	uint8_t count;
	uint8_t reserved[3];
	ZDP_SCREEN_RATIO_INFO infos[DISPLAY_MAX_MONITOR_COUNT];
} DISPLAY_SCREEN_RATIO_INFO;


/***************************************************************************
 * interfaces for QEMU
 */

/* display */
#define ZDP_INTERFACE_DISPLAY				"display"
#define ZDP_INTERFACE_DISPLAY_MAJOR			1
#define ZDP_INTERFACE_DISPLAY_MINOR			0

typedef struct ZdpQXLWorker ZdpQXLWorker;
typedef struct ZdpDisplayInterface ZdpDisplayInterface;

struct ZdpQXLWorker {
	uint32_t minor_version;
	uint32_t major_version;

	/* These calls are deprecated. Please use the spice_qxl_* calls instead */
	void(*wakeup)(ZdpQXLWorker *worker);
	void(*oom)(ZdpQXLWorker *worker);
	void(*start)(ZdpQXLWorker *worker);
	void(*stop)(ZdpQXLWorker *worker);
	void(*update_area)(ZdpQXLWorker *worker, uint32_t surface_id, ZdpRect *area,
		ZdpRect *dirty_rects, uint32_t num_dirty_rects, uint32_t clear_dirty_region);
	void(*add_memslot)(ZdpQXLWorker *worker, ZdpDisplayMemSlotInfo *slot);
	void(*del_memslot)(ZdpQXLWorker *worker, uint32_t slot_group_id, uint32_t slot_id);
	void(*reset_memslots)(ZdpQXLWorker *worker);
	void(*destroy_surfaces)(ZdpQXLWorker *worker);
	void(*destroy_primary_surface)(ZdpQXLWorker *worker, uint32_t surface_id);
	void(*create_primary_surface)(ZdpQXLWorker *worker, uint32_t surface_id,
		ZdpDisplaySurfaceInfo *info);
	void(*reset_image_cache)(ZdpQXLWorker *worker);
	void(*reset_cursor)(ZdpQXLWorker *worker);
	void(*destroy_surface_wait)(ZdpQXLWorker *worker, uint32_t surface_id);
	void(*loadvm_commands)(ZdpQXLWorker *worker, ZdpQXLCommand *cmd, uint32_t count);
};

struct ZdpDisplayInterface {
	ZdpBaseInterface base;

	int32_t id;
	struct ZdpDispatcher *worker;

	void(*attach_worker)(ZdpDisplayInterface *itfs, ZdpQXLWorker *worker);
	void(*set_compression_level)(ZdpDisplayInterface *itfs, int32_t level);
	void(*set_mm_time)(ZdpDisplayInterface *itfs, uint32_t mm_time);

	void(*get_init_info)(ZdpDisplayInterface *itfs, ZdpDisplayInitInfo *info);
	int32_t(*get_command)(ZdpDisplayInterface *itfs, ZdpQXLCommand *cmd);
	int32_t(*req_cmd_notification)(ZdpDisplayInterface *itfs);
	void(*release_resource)(ZdpDisplayInterface *itfs, ZdpQXLReleaseInfo *info);
	int32_t(*get_cursor_command)(ZdpDisplayInterface *itfs, ZdpQXLCommand *cmd);
	int32_t(*req_cursor_notification)(ZdpDisplayInterface *itfs);
	void(*notify_update)(ZdpDisplayInterface *itfs, uint32_t update_id);
	int32_t(*flush_resources)(ZdpDisplayInterface *itfs);
	void(*async_complete)(ZdpDisplayInterface *itfs, uint64_t cookie);
	void(*update_area_complete)(ZdpDisplayInterface *itfs, uint32_t surface_id,
		ZdpRect *updated_rects, uint32_t num_updated_rects);
	void(*set_client_capabilities)(ZdpDisplayInterface *itfs, uint8_t client_present, uint8_t caps[58]);
	/* returns 1 if the interface is supported, 0 otherwise.
	 * if monitors_config is NULL nothing is done except reporting the
	 * return code. */
	int32_t(*client_monitors_config)(ZdpDisplayInterface *itfs, DISPLAY_MONITOR_INFO *info);
};


/* qxl */

#define ZDP_QXL_SET_IMAGE_ID(image, _group, _unique) {              \
    (image)->descriptor.id = (((uint64_t)_unique) << 32) | _group;	\
}


#endif /* _ZDP_CHANNEL_DISPLAY_H_ */
