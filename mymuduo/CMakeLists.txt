cmake_minimum_required(VERSION 3.0)
project(mymuduo)

#mymuduo最终编译成so动态库，设置动态库的路劲
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#设置调试信息 启动c++11语言标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

#定义参与编译的源文件
aux_source_directory(. SRC_LIST)
#编译动态库
add_library(mymuduo SHARED ${SRC_LIST})

