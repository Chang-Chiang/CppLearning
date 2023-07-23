// 引入强弱智能指针

#include <iostream>
#include <memory>
#include <thread>
using namespace std;

class A {
public:
    A() { cout << "A()" << endl; }
    ~A() { cout << "~A()" << endl; }
    void testA() { cout << "这是 A 的一个方法" << endl; }
};

// 子线程
void handler01(weak_ptr<A> pw) {
    std::this_thread::sleep_for(std::chrono::seconds(2));

    shared_ptr<A> sp = pw.lock();
    if (sp != nullptr) {
        sp->testA();
    }
    else {
        cout << "A 对象已经析构, 不能再访问！" << endl;
    }
}

// main 线程
int main() {
    {
        shared_ptr<A> p(new A());
        thread        t1(handler01, weak_ptr<A>(p));
        t1.detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(20));

    return 0;
}
