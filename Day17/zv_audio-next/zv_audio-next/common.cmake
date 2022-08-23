
# 指定CMAKE版本
cmake_minimum_required(VERSION 3.10)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译参数
IF (UNIX)
	set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -pthread")
	set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall -g2 -ggdb -pthread")
	set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -Werror -Bsymbolic -fvisibility=hidden")
ENDIF ()

# 设置输出目录
set(OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${OUTPUT_PATH})
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_PATH})

# 判断平台类型
IF (WIN32)
	ADD_DEFINITIONS(-DZV_PLATFORM_WINDOWS)
	# for OS
	set(ZV_OS_INC_DIR ${CMAKE_SOURCE_DIR}/3rdparty/zv_platform)
	set(ZV_OS_LIB_DIR ${CMAKE_SOURCE_DIR}/3rdparty/zv_platform)
	set(ZV_OS_LIB ws2_32 zv_platform)
ELSEIF (UNIX)
	ADD_DEFINITIONS(-DZV_PLATFORM_LINUX)
	# for OS
	set(ZV_OS_INC_DIR /usr/include/zv_platform)
	set(ZV_OS_LIB_DIR /usr/lib64)
	set(ZV_OS_LIB zv_platform)
ELSEIF (APPLE)
	ADD_DEFINITIONS(-DZV_PLATFORM_IOS)
	set(ZV_OS_INC_DIR )
	set(ZV_OS_LIB_DIR )
	set(ZV_OS_LIB )
ELSE ()
	message(FATAL_ERROR "Unsupported Platform.")
ENDIF ()
