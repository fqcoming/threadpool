
#include "threadpool.h"
#include <functional>
#include <iostream>



/////////////////////// 线程池类相关定义



const int TASK_MAX_THRESHHOLD = 1024;


ThreadPool::ThreadPool()
	: initThreadSize_(4)
	, taskSize_(0)
	// , idleThreadSize_(0)
	// , curThreadSize_(0)
	, taskQueMaxThreshold_(TASK_MAX_THRESHHOLD)
	// , threadSizeThreshHold_(THREAD_MAX_THRESHHOLD)
	, poolMode_(PoolMode::MODE_FIXED)
	// , isPoolRunning_(false)
{}


ThreadPool::~ThreadPool()
{
	// isPoolRunning_ = false;
	// std::unique_lock<std::mutex> lock(taskQueMtx_);
	// notEmpty_.notify_all();
	// exitCond_.wait(lock, [&]()->bool {return threads_.size() == 0; });
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


void ThreadPool::submitTask(std::shared_ptr<Task> sp) {




}



void ThreadPool::start(int initThreadSize)
{

	initThreadSize_ = initThreadSize;

	for (int i = 0; i < initThreadSize_; i++)
	{
		threads_.emplace_back(new Thread(std::bind(&ThreadPool::threadFunc, this)));
	}

	for (int i = 0; i < initThreadSize_; i++)
	{
		threads_[i]->start(); 
	}
}



void ThreadPool::threadFunc() {

    std::cout << "begin threadFunc tid: " << std::this_thread::get_id() << std::endl;




    std::cout << "end threadFunc tid: " << std::this_thread::get_id() << std::endl;
}



///////////////////////// 线程类相关定义


Thread::Thread(ThreadFunc func) : func_(func) {

}




Thread::~Thread() {

}


void Thread::start() {
    std::thread t(func_);
    t.detach();
}









