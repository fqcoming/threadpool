


#include <iostream>
#include <functional>
#include <thread>
#include <future>
#include <chrono>

using namespace std;







int sum1(int a, int b) {
    return a + b;
}












int main() {

    // packaged_task 支持异步执行，而 function 不支持异步执行
    packaged_task<int(int, int)> task(sum1);
    future<int> res = task.get_future();

    // task(10, 20); // 在当前线程执行

    thread t(move(task), 10, 20);
    t.detach();

    cout << res.get() << endl;

    return 0;
}












