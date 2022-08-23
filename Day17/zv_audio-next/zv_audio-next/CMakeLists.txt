
# 指定CMAKE版本
cmake_minimum_required(VERSION 3.10)

# 指定工程名称
project(zv_player)

# 添加子目录
add_subdirectory(zva_util)
add_subdirectory(zv_audio)
add_subdirectory(test)


# packeg debs
include("${CMAKE_SOURCE_DIR}/deb.cmake")
