# 强弱智能指针应用实例

## 多线程访问共享对象的线程安全问题

```c++
// 多线程访问共享对象的线程安全问题

#include <iostream>
#include <thread>
using namespace std;

class A {
public:
    A() { cout << "A()" << endl; }
    ~A() { cout << "~A()" << endl; }
    void testA() { cout << "这是 A 的一个方法" << endl; }
};

// 子线程
void handler01(A* q) {
    // 子线程睡眠 2s，主线程已经退出，q 已经析构，无法再去访问其方法
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // q 访问 A 对象的时候, 需要侦测一下 A 对象是否存活
    q->testA();
}

// main 线程
int main() {
    A*     p = new A();
    thread t1(handler01, p);

    // std::this_thread::sleep_for(std::chrono::seconds(2));

    delete p;

    return 0;
}
// 多线程访问共享对象的线程安全问题

#include <thread>
class A {
public:
    A() { cout << "A()" << endl; }
    ~A() { cout << "~A()" << endl; }
    void testA() { cout << "这是 A 的一个方法" << endl; } 
};

// 子线程
void handler01(A *q) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // q 访问 A 对象的时候, 需要侦测一下 A 对象是否存活
    q->testA();
}

// main 线程
int main() {
    A *p = new A();
    thread t1(handler01, p);
    
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    
    delete p;
    
    return 0;
}
```

## 引入强弱智能指针

```c++
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
```

