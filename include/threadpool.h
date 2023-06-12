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



// 怎么构建一个Any类型
class Any 
{
public:
    Any() = default;
    ~Any() = default;

    Any(const Any&) = delete;
    Any& operator=(const Any&) = delete;

    Any(Any&&) = default;
    Any& operator=(Any&&) = default;

    // 这个构造函数可以让Any类型接收任意其他类型的数据
    template <typename T>
    Any(T data) : base_(std::make_unique<Derive<T>>(data)) {}

    // 将data数据取出来
    template <typename T>
    T cast_() {
        Derive<T> *pd = dynamic_cast<Derive<T>*>(base_.get());
        if (pd == nullptr) {
            throw "type is unmatch!";
        }
        return pd->data_;
    }


private:
    class Base 
    {
    public:
        virtual ~Base() = default;
    };

    template <typename T>
    class Derive : public Base
    {
    public:
        Derive(T data) : data_(data) {}
        T data_;
    };

private:
    std::unique_ptr<Base> base_;

};





class Semaphore
{
public:
	Semaphore(int limit = 0) 
		: resLimit_(limit)
	{}
	~Semaphore() = default;

	void wait()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		cond_.wait(lock, [&]()->bool {return resLimit_ > 0; });
		resLimit_--;
	}


	void post()
	{
		std::unique_lock<std::mutex> lock(mtx_);
		resLimit_++;
		cond_.notify_all();  
	}

private:
	int resLimit_;
	std::mutex mtx_;
	std::condition_variable cond_;
};



class Task;

class Result
{
public:
	Result(std::shared_ptr<Task> task, bool isValid = true);
	~Result() = default;

	void setVal(Any any);

	Any get();
private:
	Any any_;    // 存储任务的返回值
	Semaphore sem_; 
	std::shared_ptr<Task> task_; 
	std::atomic_bool isValid_;
};







// 任务抽象基类
// 用户可以自定义任意任务类型，从Task继承，重写run方法，实现自定义
class Task
{
public:
	Task();
	~Task() = default;
	void exec();
	void setResult(Result* res);

	virtual Any run() = 0;

private:
	Result* result_; 
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
    Result submitTask(std::shared_ptr<Task> sp);
    void start(int initThreadSize = std::thread::hardware_concurrency());

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

private:
    // 1.线程回调函数定义为全局函数, 但不能访问ThreadPool对象的私有变量
    // 2.线程回调函数定义在Thread类中，但不能访问ThreadPool对象的私有变量
    // 3.定义在ThreadPool类中最好
	void threadFunc();

private:

    std::vector<std::unique_ptr<Thread>> threads_;
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