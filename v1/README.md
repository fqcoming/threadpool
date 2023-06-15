


### 使用了C++17新特性的线程池最终版

```shell
g++ -o test test.cpp -lpthread -std=c++17
```



```shell
g++ -fPIC -shared threadpool.cpp -o libtdpool.so -std=c++17 编译成动态库
// .a 静态库  .so 动态库
su root
mv libtdpool.so /usr/local/lib/
mv threadpool.h /usr/local/include/
cd /etc/ld.so.conf.d/
vim mylib.conf  加上 /usr/local/lib
ldconfig
g++ -o main main.cpp -std=c++17 -ltdpool -lpthread
./main

ps -u
gdb attach 16908
info threads
```




