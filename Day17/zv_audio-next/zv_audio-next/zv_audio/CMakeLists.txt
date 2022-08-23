
# 指定CMAKE版本
cmake_minimum_required(VERSION 3.10)

# 添加公共定义
include ("${CMAKE_SOURCE_DIR}/common.cmake")

# 指定工程名称和版本
project(zv_audio VERSION 1.0.1)

ADD_DEFINITIONS(-Dzv_audio_EXPORTS)

# 指定头文件目录
include_directories(
	${ZV_OS_INC_DIR}
	${FFMPEG_INC_DIR}
	${CMAKE_SOURCE_DIR}/zdp_protocol
	${CMAKE_SOURCE_DIR}/zva_util/include
	include
)

# 指定链接库目录
link_directories(
	${ZV_OS_LIB_DIR}
	${FFMPEG_LIB_DIR}
	${OUTPUT_PATH}
)

# 获取源文件列表
aux_source_directory(src SRC_LIST)
file(GLOB_RECURSE HEADER_LIST include/*.h)
file(GLOB_RECURSE ZDP_PROTOCOL ${CMAKE_SOURCE_DIR}/zdp_protocol/*.h)
IF (WIN32)
	set(SRC_LIB_DEF include/zv_audio.def)
ELSEIF (UNIX)
	set(SRC_LIB_DEF )
ELSEIF (APPLE)
	set(SRC_LIB_DEF )
ENDIF ()

# 指定生成动态库
add_library(
	${PROJECT_NAME}
	SHARED
	${SRC_LIST}
	${HEADER_LIST}
	${SRC_LIB_DEF}
	${ZDP_PROTOCOL}
)

# 添加链接库
target_link_libraries(
	${PROJECT_NAME}
	${ZV_OS_LIB}			# for OS
	${FFMPEG_LIB}			# for ffmpeg
	zva_util
)


IF (UNIX)
# for install
install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_PERFIX}/usr/lib64
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PERFIX}/usr/lib64
	RUNTIME DESTINATION ${CMAKE_INSTALL_PERFIX}/usr/bin
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PERFIX}/usr/include
	)
set(ZV_PLAYER_HEADERS
	include/zv_audio.h
	../zva_util/include/zva_event.h
	../zva_util/include/zva_type.h
	)
install(FILES ${ZV_PLAYER_HEADERS}
	DESTINATION ${CMAKE_INSTALL_PERFIX}/usr/include/zv_audio
	)
ENDIF ()
