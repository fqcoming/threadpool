

# 定义了一个SRC_LIST变量，包含了该目录下所有的源文件
aux_source_directory(. SRC_LIST)

# 编译成动态库
add_library(threadpool SHARED ${SRC_LIST})


