#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include <unordered_map>




// 任务抽象基类
// 用户可以自定义任意任务类型，从Task继承，重写run方法，实现自定义
class Task
{
public:
    virtual void run() = 0;

private:

};



// 线程支持的模式
// 不加class可以直接使用 MODE_FIXED, 但是当不同枚举类型里面定义了枚举名一样时会造成名字冲突
// 限定作用域的枚举类型
enum class PoolMode
{
	MODE_FIXED,   // 固定数量的线程
	MODE_CACHED,  // 线程数量可动态增长
};



class Thread
{
public:

	using ThreadFunc = std::function<void()>;
	Thread(ThreadFunc func);
	~Thread();
	void start();

private:
	ThreadFunc func_;

};



class ThreadPool
{
public:

	ThreadPool();
	~ThreadPool();

	void setMode(PoolMode mode);
    void setInitThreadSize(int size);
	void setTaskQueMaxThreshold(int threshhold);
    void submitTask(std::shared_ptr<Task> sp);
    void start(int initThreadSize = std::thread::hardware_concurrency());

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

private:
    // 1.线程回调函数定义为全局函数, 但不能访问ThreadPool对象的私有变量
    // 2.线程回调函数定义在Thread类中，但不能访问ThreadPool对象的私有变量
    // 3.定义在ThreadPool类中最好
	void threadFunc();

private:

    std::vector<Thread*> threads_;
	size_t initThreadSize_;  

	std::queue<std::shared_ptr<Task>> taskQue_;   
    // 这里要考虑两个问题：
    // 1.需要Task指针,实现多态机制 
    // 2.要考虑释放资源问题,如果传入指针,需要考虑生命周期,这里假设用户传入指向堆内存的指针

	std::atomic_int taskSize_;  // 任务数量
	int taskQueMaxThreshold_;  // 任务队列最大数量上限

	std::mutex taskQueMtx_;   // 保证任务队列的线程安全
	std::condition_variable notFull_;   // 表示任务队列不满,当消费一个任务时,消费者线程通知
	std::condition_variable notEmpty_;  // 表示任务队列不空,当提交一个任务时,通知消费者线程当前任务队列不空,消费者线程将停止阻塞

	PoolMode poolMode_; 
};


#endif