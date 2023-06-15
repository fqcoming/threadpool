
### linux平台编译线程池动态库

#### 1.编译源文件 

```shell
$ g++ -fPIC -shared threadpool.cpp -o libtdpool.so -std=c++17
```

#### 2.将动态库移动到系统库目录下

linux 通常情况下是在如下目录下查找动态库(*.so)或静态库(*.a)
/usr/lib/
/usr/local/lib/

linux 通常情况下是在如下目录下查找头文件(*.h)
/usr/include/
/usr/local/include/

```shell
$ sudo rm /usr/local/lib/libtdpool.so  # 删除之前存在的库
$ sudo rm /usr/local/include/threadpool.h  # 删除之前存在的头文件

$ sudo mv libtdpool.so /usr/local/lib/
$ sudo cp threadpool.h /usr/local/include/
$ sudo ldconfig

$ ls /usr/local/lib/   # 可以库文件是否拷贝成功
$ ls /usr/local/include/
```


#### 3.使用动态库


GCC编译时与运行时查找动态库的路径方式不一致
在编译时找动态库是在目录 /usr/lib/ /usr/local/lib/
而在运行程序时找动态库是在文件 etc/ld.so.cache 中

如果只是仅仅把编译生成的动态库(如libtdpool.so)拷贝到如下目录(/usr/local/lib/)
执行如下命令编译会顺利完成

```shell
$ g++ example.cpp -o example -std=c++17 -ltdpool -lpthread
```

但是运行程序，执行如下命令时会发生错误
```shell
./example
```
> ./example: error while loading shared libraries: libtdpool.so: cannot open shared object file: No such file or directory


运行如下命令查看运行时查找的动态库路径
```shell
$ cat /etc/ld.so.cache
$ vim /etc/ld.so.cache
```

但是我们不需要修改 /etc/ld.so.cache 这个文件，而是修改 /etc/ld.so.conf 文件
```shell
$ vim /etc/ld.so.conf
```
文件内容如下即可
> include /etc/ld.so.conf.d/*.conf

在 /etc/ 目录下有3个重要的相关文件
ld.so.cache   
ld.so.conf    
ld.so.conf.d/ 

```shell
$ cd /etc/ld.so.conf.d/
$ ls
```
> fakeroot-x86_64-linux-gnu.conf  libc.conf  x86_64-linux-gnu.conf

在 /etc/ld.so.conf.d/ 目录下创建一个新的配置文件，里面配置库文件存放的目录
```shell
$ vim mylib.conf
```
文件中加入
> /usr/local/lib


如下命令是将 mylib.conf 配置文件中的路径全部刷新到 /etc/ld.so.cache 文件中
```shell
$ sudo ldconfig
```

现在在运行如下命令就不会出现找不到库文件的错误信息
```shell
./example
```


#### 4.GDB调试相关命令

```shell
./example  # 运行程序
```

```shell
$ ps -u  # 查看当前用户下的进程和查看example进程的PID，用于gdb调试
```

```shell
$ sudo gdb attach 54862
```

```shell
(gdb) info threads
(gdb) bt
(gdb) thread 5  # 切换到线程5
(gdb) bt        # 查看线程5的堆栈
```















