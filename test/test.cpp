
#include <iostream>
using namespace std;


class Any {
public:

    Any() = default;
    ~Any() = default;

    Any(const Any&) = delete;
    Any& operator=(const Any&) = delete;

    // 当删除了拷贝构造和拷贝赋值，需要使用移动默认构造和赋值时需要显示声明默认构造和声明
    Any(Any&&) = default;
    Any& operator=(Any&&) = default;

};



// g++ -o test test.cpp
int main() {

    Any any;
    Any any1(move(any));




    return 0;
}







