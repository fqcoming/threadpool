
#include "threadpool.h"
#include <functional>
#include <iostream>


/////////////////////// 线程池类相关定义

const int TASK_MAX_THRESHHOLD = 4;

ThreadPool::ThreadPool()
	: initThreadSize_(4)
	, taskSize_(0)
	, taskQueMaxThreshold_(TASK_MAX_THRESHHOLD)
	, poolMode_(PoolMode::MODE_FIXED)
{
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::setMode(PoolMode mode)
{
	poolMode_ = mode;
}

void ThreadPool::setTaskQueMaxThreshold(int threshold)
{
	taskQueMaxThreshold_ = threshold;
}

void ThreadPool::setInitThreadSize(int size)
{
    initThreadSize_ = size;
}

Result ThreadPool::submitTask(std::shared_ptr<Task> sp) 
{
	// 获取锁
	std::unique_lock<std::mutex> lock(taskQueMtx_);

	// 线程的通信 等待线程队列有空余
#if 0
	while (taskQue_.size() == taskQueMaxThreshold_) {
		notFull_.wait(lock);
	}
#elif 0
	notFull_.wait(lock, [&]()->bool { return taskQue_.size() < taskQueMaxThreshold_; });
#else  
	// 用户提交任务，最长不能阻塞超过1s，否则判断提交任务失败，返回
	if (!notFull_.wait_for(lock, std::chrono::seconds(1), 
		[&]()->bool { return taskQue_.size() < taskQueMaxThreshold_; })) {

			// 表示notFull_等待1s,条件依然没有满足
			std::cerr << "task queue is full, submit task fail!" << std::endl;

			return Result(sp, false);
	}
#endif

	// 如果有空余，把任务放入任务队列中
	taskQue_.emplace(sp);
	taskSize_++;

	// 任务队列不空，notEmpty_进行通知消费者线程
	notEmpty_.notify_all();  // 加入一个任务通知所有消费者线程会不会有点浪费系统资源,为什么不直接通知一个线程???

	return Result(sp);
}

void ThreadPool::start(int initThreadSize)
{
	initThreadSize_ = initThreadSize;

	for (int i = 0; i < initThreadSize_; i++)
	{
		auto ptr = std::make_unique<Thread>(std::bind(&ThreadPool::threadFunc, this));
		threads_.emplace_back(std::move(ptr));
	}

	for (int i = 0; i < initThreadSize_; i++)
	{
		threads_[i]->start(); 
	}
}

void ThreadPool::threadFunc() 
{

	for (;;) {

		std::shared_ptr<Task> task;

		{		
			// 先获取锁
			std::unique_lock<std::mutex> lock(taskQueMtx_);

			std::cout << "tid: " << std::this_thread::get_id() << "尝试获取任务..." << std::endl;

			// 等待notEmpty_条件
			notEmpty_.wait(lock, [&]()->bool { return taskQue_.size() > 0; });

			std::cout << "tid: " << std::this_thread::get_id() << "获取任务成功..." << std::endl;

			// 从任务队列中取一个任务出来
			task = taskQue_.front();
			taskQue_.pop();
			taskSize_--;

			if (taskQue_.size() > 0) {
				notEmpty_.notify_all();
			}

			notFull_.notify_all();
		}

		// 当前线程负责执行这个任务
		if (task != nullptr) {
			// task->run();
			task->exec();
		} else {
			std::cout << "task is nullptr" << std::endl;
		}
		
	}

}



///////////////////////// 线程类相关定义


Thread::Thread(ThreadFunc func) : func_(func) 
{
}

Thread::~Thread() 
{
}

void Thread::start() 
{
    std::thread t(func_);
    t.detach();
}



/////////////////////// Result类相关实现


Result::Result(std::shared_ptr<Task> task, bool isValid)
	: isValid_(isValid)
	, task_(task)
{
	task_->setResult(this);
}

Any Result::get() 
{
	if (!isValid_)
	{
		return "";
	}
	sem_.wait();
	return std::move(any_);
}

void Result::setVal(Any any) 
{
	this->any_ = std::move(any);
	sem_.post();
}



///////////////////////// Task相关类的实现

Task::Task()
	: result_(nullptr)
{
}

void Task::exec()
{
	if (result_ != nullptr)
	{
		result_->setVal(run()); 
	} else {
		std::cout << "result is nullptr" << std::endl;
	}
}

void Task::setResult(Result* res)
{
	result_ = res;
}




