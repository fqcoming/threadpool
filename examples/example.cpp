#include <iostream>
#include <chrono>
#include "threadpool.h"


class MyTask : public Task {
public:
    MyTask(int begin, int end) : begin_(begin), end_(end) {}

    virtual Any run() {
        std::cout << "tid: " << std::this_thread::get_id() << "begin!" << std::endl;

        int sum = 0;
        for (int i = begin_; i <= end_; i++) {
            sum += i;
        }

        std::cout << "tid: " << std::this_thread::get_id() << "end!" << std::endl;
        return sum;
    }

private:
    int begin_;
    int end_;
};




int main() {

    ThreadPool pool;
    pool.start(4);


    Result res1 = pool.submitTask(std::make_shared<MyTask>(1, 100));
    Result res2 = pool.submitTask(std::make_shared<MyTask>(1, 100));
    Result res3 = pool.submitTask(std::make_shared<MyTask>(1, 100));
    Result res4 = pool.submitTask(std::make_shared<MyTask>(1, 100));

    int sum1 = res1.get().cast_<int>();
    int sum2 = res2.get().cast_<int>();
    int sum3 = res3.get().cast_<int>();
    int sum4 = res4.get().cast_<int>();

    std::cout << sum1 + sum2 + sum3 + sum4 << std::endl;


    getchar();


    return 0;
}













