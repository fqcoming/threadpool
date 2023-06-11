
cmake_minimum_required(VERSION 3.0)
project(threadpool)

# 配置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")

# 配置最终的可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 配置头文件的搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 配置库文件的搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)


# 设置项目库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


# 加载子目录
add_subdirectory(src)
add_subdirectory(examples)















