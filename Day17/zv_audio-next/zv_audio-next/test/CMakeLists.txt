
# 指定CMAKE版本
cmake_minimum_required(VERSION 3.10)

# 添加公共定义
include ("../common.cmake")

# 指定工程名称和版本
project(audio_test VERSION 1.0.0)

# 添加头文件目录
include_directories(
	${ZV_OS_INC_DIR}
	../zdp_protocol
	../zva_util/include
	../zv_audio/include
	include
)

# 指定链接库目录
link_directories(
	${ZV_OS_INC_DIR}
	${OUTPUT_PATH}
)

# 获取源文件列表
aux_source_directory(src SRC_LIST)
file(GLOB_RECURSE HEADER_LIST include/*.h)

# 指定生可执行程序
add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADER_LIST})

# 添加链接库
target_link_libraries(
	${PROJECT_NAME}
	${ZV_OS_LIB}
	zva_util
	zv_audio
)
