#!/bin/bash

# 启动脚本的调试模式，会将脚本执行的详细信息打印到标准输出
set -x

# 设置脚本在任何命令失败时立即退出
set -e


# 如果没有build目录，创建该目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi


rm -rf `pwd`/build/*
cd `pwd`/build &&
        cmake .. &&
        make

# 回到项目根目录
cd ..


# 把头文件拷贝到 /usr/include/mymuduo  so库拷贝到 /usr/lib    PATH
if [ ! -d /usr/include/mymuduo ]; then 
    mkdir /usr/include/mymuduo
fi

for header in `ls *.h`
do
    cp $header /usr/include/mymuduo
done

cp `pwd`/lib/libmymuduo.so /usr/lib

ldconfig

