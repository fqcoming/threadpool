### threapool

#### 1.线程池项目编译
```shell
mkdir build
sh autobuild.sh
```

#### 2.线程池项目内存泄漏检查
```shell
valgrind --tool=memcheck --leak-check=full --show-reachable=yes --trace-children=yes ./bin/test
```



```shell
#!/bin/bash

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
```














