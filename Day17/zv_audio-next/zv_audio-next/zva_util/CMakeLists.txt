
# 指定CMAKE版本
cmake_minimum_required(VERSION 3.10)

# 添加公共定义
include("${CMAKE_SOURCE_DIR}/common.cmake")

# 指定工程名称和版本
project(zva_util VERSION 1.0.0)

# 添加头文件目录
include_directories(
	include
)

# 获取源文件列表
aux_source_directory(src SRC_LIST)
file(GLOB_RECURSE HEADER_LIST include/*.h)

# 指定生成静态库
add_library(
	${PROJECT_NAME}
	STATIC
	${HEADER_LIST}
	${SRC_LIST}
	${SRC_PLATFORM}
)
