
#ifndef _ZDP_DISPLAY_TYPES_H_
#define _ZDP_DISPLAY_TYPES_H_


#include <stdint.h>


/*
 * common types
 */

typedef struct {
	int32_t x;
	int32_t y;
} ZdpPoint;

typedef struct {
	int16_t x;
	int16_t y;
} ZdpPoint16;


typedef struct {
	int32_t top;
	int32_t left;
	int32_t bottom;
	int32_t right;
} ZdpRect;

typedef struct {
	uint32_t top;
	uint32_t left;
	uint32_t bottom;
	uint32_t right;
} ZdpURect;


/*
 * display enums
 */

typedef enum {
	ZDP_IMAGE_TYPE_BITMAP,
	ZDP_IMAGE_TYPE_QUIC,
	ZDP_IMAGE_TYPE_RESERVED,
	ZDP_IMAGE_TYPE_LZ_PLT = 100,
	ZDP_IMAGE_TYPE_LZ_RGB,
	ZDP_IMAGE_TYPE_GLZ_RGB,
	ZDP_IMAGE_TYPE_FROM_CACHE,
	ZDP_IMAGE_TYPE_SURFACE,
	ZDP_IMAGE_TYPE_JPEG,
	ZDP_IMAGE_TYPE_FROM_CACHE_LOSSLESS,
	ZDP_IMAGE_TYPE_ZLIB_GLZ_RGB,
	ZDP_IMAGE_TYPE_JPEG_ALPHA,
	ZDP_IMAGE_TYPE_LZ4,

	ZDP_IMAGE_TYPE_ENUM_END
} ZdpImageType;

typedef enum {
	ZDP_IMAGE_FLAGS_CACHE_ME = (1 << 0),
	ZDP_IMAGE_FLAGS_HIGH_BITS_SET = (1 << 1),
	ZDP_IMAGE_FLAGS_CACHE_REPLACE_ME = (1 << 2),

	ZDP_IMAGE_FLAGS_MASK = 0x7
} ZdpImageFlags;

typedef enum {
	ZDP_BITMAP_FMT_INVALID,
	ZDP_BITMAP_FMT_1BIT_LE,
	ZDP_BITMAP_FMT_1BIT_BE,
	ZDP_BITMAP_FMT_4BIT_LE,
	ZDP_BITMAP_FMT_4BIT_BE,
	ZDP_BITMAP_FMT_8BIT,
	ZDP_BITMAP_FMT_16BIT,
	ZDP_BITMAP_FMT_24BIT,
	ZDP_BITMAP_FMT_32BIT,
	ZDP_BITMAP_FMT_RGBA,
	ZDP_BITMAP_FMT_8BIT_A,

	ZDP_BITMAP_FMT_ENUM_END
} ZdpBitmapFmt;

typedef enum {
	ZDP_CLIP_TYPE_NONE,
	ZDP_CLIP_TYPE_RECTS,

	ZDP_CLIP_TYPE_ENUM_END
} ZdpClipType;

typedef enum {
	ZDP_ROPD_INVERS_SRC = (1 << 0),
	ZDP_ROPD_INVERS_BRUSH = (1 << 1),
	ZDP_ROPD_INVERS_DEST = (1 << 2),
	ZDP_ROPD_OP_PUT = (1 << 3),
	ZDP_ROPD_OP_OR = (1 << 4),
	ZDP_ROPD_OP_AND = (1 << 5),
	ZDP_ROPD_OP_XOR = (1 << 6),
	ZDP_ROPD_OP_BLACKNESS = (1 << 7),
	ZDP_ROPD_OP_WHITENESS = (1 << 8),
	ZDP_ROPD_OP_INVERS = (1 << 9),
	ZDP_ROPD_INVERS_RES = (1 << 10),

	ZDP_ROPD_MASK = 0x7ff
} ZdpRopd;

typedef enum ZdpDisplaySurfaceFmt {
	ZDP_SURFACE_FMT_INVALID,
	ZDP_SURFACE_FMT_1_A,
	ZDP_SURFACE_FMT_8_A = 8,
	ZDP_SURFACE_FMT_16_555 = 16,
	ZDP_SURFACE_FMT_32_xRGB = 32,
	ZDP_SURFACE_FMT_16_565 = 80,
	ZDP_SURFACE_FMT_32_ARGB = 96,

	ZDP_SURFACE_FMT_ENUM_END
} ZdpDisplaySurfaceFmt;


/*
 * QXL enums
 */

typedef enum {
	ZDP_QXL_CMD_NOP,
	ZDP_QXL_CMD_DRAW,
	ZDP_QXL_CMD_UPDATE,
	ZDP_QXL_CMD_CURSOR,
	ZDP_QXL_CMD_MESSAGE,
	ZDP_QXL_CMD_SURFACE,
} ZdpQXLCmdType;

typedef enum {
	ZDP_QXL_EFFECT_BLEND = 0,
	ZDP_QXL_EFFECT_OPAQUE = 1,
	ZDP_QXL_EFFECT_REVERT_ON_DUP = 2,
	ZDP_QXL_EFFECT_BLACKNESS_ON_DUP = 3,
	ZDP_QXL_EFFECT_WHITENESS_ON_DUP = 4,
	ZDP_QXL_EFFECT_NOP_ON_DUP = 5,
	ZDP_QXL_EFFECT_NOP = 6,
	ZDP_QXL_EFFECT_OPAQUE_BRUSH = 7
} ZdpQXLEffectType;

typedef enum {
	ZDP_QXL_DRAW_NOP,
	ZDP_QXL_DRAW_FILL,
	ZDP_QXL_DRAW_OPAQUE,
	ZDP_QXL_DRAW_COPY,
	ZDP_QXL_DRAW_COPY_BITS,
	ZDP_QXL_DRAW_BLEND,
	ZDP_QXL_DRAW_BLACKNESS,
	ZDP_QXL_DRAW_WHITENESS,
	ZDP_QXL_DRAW_INVERS,
	ZDP_QXL_DRAW_ROP3,
	ZDP_QXL_DRAW_STROKE,
	ZDP_QXL_DRAW_TEXT,
	ZDP_QXL_DRAW_TRANSPARENT,
	ZDP_QXL_DRAW_ALPHA_BLEND,
	ZDP_QXL_DRAW_COMPOSITE
} ZdpQXLDrawType;

typedef enum {
	ZDP_QXL_IMAGE_GROUP_DRIVER,
	ZDP_QXL_IMAGE_GROUP_DEVICE,
	ZDP_QXL_IMAGE_GROUP_RED,
	ZDP_QXL_IMAGE_GROUP_DRIVER_DONT_CACHE,
} ZdpQXLImageGroupType;

typedef enum {
	ZDP_QXL_BITMAP_DIRECT = (1 << 0),
	ZDP_QXL_BITMAP_UNSTABLE = (1 << 1),
	ZDP_QXL_BITMAP_TOP_DOWN = (1 << 2), /* == ZDP_BITMAP_FLAGS_TOP_DOWN */
} ZdpQXLBitmapFlags;


/*
 * display types
 */

typedef struct ZdpDisplayInitInfo {
	uint32_t num_memslots_groups;
	uint32_t num_memslots;
	uint8_t memslot_gen_bits;
	uint8_t memslot_id_bits;
	uint32_t qxl_ram_size;
	uint8_t internal_groupslot_id;
	uint32_t n_surfaces;
} ZdpDisplayInitInfo;

typedef struct ZdpDisplayMemSlotInfo {
	uint32_t slot_group_id;
	uint32_t slot_id;
	uint32_t generation;
	uint64_t virt_start;
	uint64_t virt_end;
	uint64_t addr_delta;
	uint32_t qxl_ram_size;
} ZdpDisplayMemSlotInfo;

typedef struct ZdpDisplaySurfaceInfo {
	uint32_t width;
	uint32_t height;
	int32_t stride;
	uint32_t format;
	uint32_t position;
	uint32_t mouse_mode;
	uint32_t flags;
	uint32_t type;
	uint64_t mem;
	uint32_t group_id;
} ZdpDisplaySurfaceInfo;


/*
 * QXL types
 */

typedef struct ZdpQXLCopyBits {
	ZdpPoint src_pos;
} ZdpQXLCopyBits;

typedef struct ZdpQXLClip {
	uint32_t type;
	uint64_t data;
} ZdpQXLClip;

typedef struct ZdpQXLPattern {
	uint64_t pat;
	ZdpPoint pos;
} ZdpQXLPattern;

typedef struct ZdpQXLBrush {
	uint32_t type;
	union {
		uint32_t color;
		ZdpQXLPattern pattern;
	} u;
} ZdpQXLBrush;

typedef struct ZdpQXLQMask {
	uint8_t flags;
	ZdpPoint pos;
	uint64_t bitmap;
} ZdpQXLQMask;

typedef struct ZdpQXLFill {
	ZdpQXLBrush brush;
	uint16_t rop_descriptor;
	ZdpQXLQMask mask;
} ZdpQXLFill;

typedef struct ZdpQXLOpaque {
	uint64_t src_bitmap;
	ZdpRect src_area;
	ZdpQXLBrush brush;
	uint16_t rop_descriptor;
	uint8_t scale_mode;
	ZdpQXLQMask mask;
} ZdpQXLOpaque;

typedef struct ZdpQXLCopy {
	uint64_t src_bitmap;
	ZdpRect src_area;
	uint16_t rop_descriptor;
	uint8_t scale_mode;
	ZdpQXLQMask mask;
} ZdpQXLCopy, ZdpQXLBlend;

typedef struct ZdpQXLTransparent {
	uint64_t src_bitmap;
	ZdpRect src_area;
	uint32_t src_color;
	uint32_t true_color;
} ZdpQXLTransparent;

typedef struct ZdpQXLAlphaBlend {
	uint16_t alpha_flags;
	uint8_t alpha;
	uint64_t src_bitmap;
	ZdpRect src_area;
} ZdpQXLAlphaBlend;

typedef struct ZdpQXLRop3 {
	uint64_t src_bitmap;
	ZdpRect src_area;
	ZdpQXLBrush brush;
	uint8_t rop3;
	uint8_t scale_mode;
	ZdpQXLQMask mask;
} ZdpQXLRop3;

typedef struct ZdpQXLLineAttr {
	uint8_t flags;
	uint8_t join_style;
	uint8_t end_style;
	uint8_t style_nseg;
	int32_t width;
	int32_t miter_limit;
	uint64_t style;
} ZdpQXLLineAttr;

typedef struct ZdpQXLStroke {
	uint64_t path;
	ZdpQXLLineAttr attr;
	ZdpQXLBrush brush;
	uint16_t fore_mode;
	uint16_t back_mode;
} ZdpQXLStroke;

typedef struct ZdpQXLText {
	uint64_t str;
	ZdpRect back_area;
	ZdpQXLBrush fore_brush;
	ZdpQXLBrush back_brush;
	uint16_t fore_mode;
	uint16_t back_mode;
} ZdpQXLText;

typedef struct ZdpQXLBlackness {
	ZdpQXLQMask mask;
} ZdpQXLBlackness, ZdpQXLInvers, ZdpQXLWhiteness;

typedef struct ZdpQXLComposite {
	uint32_t flags;
	uint64_t src;
	uint64_t src_transform;		/* May be NULL */
	uint64_t mask;				/* May be NULL */
	uint64_t mask_transform;	/* May be NULL */
	ZdpPoint16 src_origin;
	ZdpPoint16 mask_origin;
} ZdpQXLComposite;


typedef struct ZdpQXLCommand {
	uint64_t data;
	uint32_t type;
	uint32_t padding;
	uint32_t group_id;
	uint32_t flags;
} ZdpQXLCommand;

typedef struct ZdpQXLReleaseInfo {
	uint64_t id;      /* in */
	uint64_t next;    /* out */
	uint32_t group_id;
} ZdpQXLReleaseInfo;

typedef struct ZdpQXLDrawable {
	ZdpQXLReleaseInfo release_info;
	uint32_t surface_id;
	uint8_t effect;
	uint8_t type;
	uint8_t self_bitmap;
	ZdpRect self_bitmap_area;
	ZdpRect bbox;
	ZdpQXLClip clip;
	uint32_t mm_time;
	int32_t surfaces_dest[3];
	ZdpRect surfaces_rects[3];
	union {
		ZdpQXLFill fill;
		ZdpQXLOpaque opaque;
		ZdpQXLCopy copy;
		ZdpQXLTransparent transparent;
		ZdpQXLAlphaBlend alpha_blend;
		ZdpQXLCopyBits copy_bits;
		ZdpQXLBlend blend;
		ZdpQXLRop3 rop3;
		ZdpQXLStroke stroke;
		ZdpQXLText text;
		ZdpQXLBlackness blackness;
		ZdpQXLInvers invers;
		ZdpQXLWhiteness whiteness;
		ZdpQXLComposite composite;
	} u;
} ZdpQXLDrawable;


typedef struct ZdpQXLImageDescriptor {
	uint64_t id;
	uint8_t type;
	uint8_t flags;
	uint32_t width;
	uint32_t height;
} ZdpQXLImageDescriptor;

typedef struct ZdpQXLBitmap {
    uint8_t format;
    uint8_t flags;
    uint32_t x;
    uint32_t y;
    uint32_t stride;
    uint64_t palette;
    uint64_t data;
} ZdpQXLBitmap;

typedef struct ZdpQXLQUICData {
    uint32_t data_size;
    uint8_t data[0];
} ZdpQXLQUICData, ZdpQXLLZRGBData, ZdpQXLJPEGData;

typedef struct ZdpQXLSurfaceId {
	uint32_t surface_id;
} ZdpQXLSurfaceId;

typedef struct ZdpQXLImage {
	ZdpQXLImageDescriptor descriptor;
	union { /* variable length */
		ZdpQXLBitmap bitmap;
		ZdpQXLQUICData quic;
		ZdpQXLSurfaceId surface_image;
	};
} ZdpQXLImage;


#endif /* _ZDP_DISPLAY_TYPES_H_ */
