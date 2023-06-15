 

### 项目名称：

基于可变参模板实现的线程池

 

### 应用场景：

​    高并发网络服务器

​    master-slave线程模型

​    耗时任务处理

 

 

https://github.com/fqcoming/threadpool.git

 

平台工具：

VS Code开发，ubuntu22，

 

### 项目描述：（有哪些技术点）

1、 基于可变参模板编程和引用折叠原理，实现线程池submitTask接口，支持任意任务函数和任意参数的传递；

2、 使用future类型定制submitTask提交任务的返回值；

3、 使用map和queue容器管理线程对象和任务；

4、 基于条件变量condition_variable和互斥锁mutex实现任务提交线程和任务执行线程间的通信机制；

5、 支持fixed和cached模式的线程池定制；

 

### 项目问题：

1、 在ThreadPool的资源回收，等待线程池所有线程退出时，发生死锁问题，导致进程无法退出。

2、 在windows平台下运行良好的线程池，在linux平台下运行发生死锁问题，平台运行结果有差异化。

 

### 分析定位问题：

​    主要通过gdb attach到正在运行的进程，通过info threads，thread tid，bt等命令查看各个线程的调用堆栈信息，结合项目代码，定位到发生死锁的代码片段，分析死锁问题发生的原因，





































