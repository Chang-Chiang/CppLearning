#include <iostream>
#include <memory>
using namespace std;

class B; // 前置声明类B

class A {
public:
    A() { cout << "A()" << endl; }
    ~A() { cout << "~A()" << endl; }
    weak_ptr<B> _ptrb; // 指向B对象的弱智能指针。引用对象时，用弱智能指针
};

class B {
public:
    B() { cout << "B()" << endl; }
    ~B() { cout << "~B()" << endl; }
    weak_ptr<A> _ptra; // 指向A对象的弱智能指针。引用对象时，用弱智能指针
};

int main() {
    // 定义对象时，用强智能指针
    shared_ptr<A> ptra(new A()); // ptra指向A对象，A的引用计数为1
    shared_ptr<B> ptrb(new B()); // ptrb指向B对象，B的引用计数为1

    // A对象的成员变量_ptrb也指向B对象，B的引用计数为1，因为是弱智能指针，引用计数没有改变
    ptra->_ptrb = ptrb;

    // B对象的成员变量_ptra也指向A对象，A的引用计数为1，因为是弱智能指针，引用计数没有改变
    ptrb->_ptra = ptra;

    cout << ptra.use_count() << endl; // 打印结果:1
    cout << ptrb.use_count() << endl; // 打印结果:1

    /*
    出main函数作用域，ptra和ptrb两个局部对象析构，分别给A对象和B对象的引用计数从1减到0，
    达到释放A和B的条件，因此new出来的A和B对象被析构掉，解决了“强智能指针的交叉引用(循环引用)问题”
    */
    return 0;
}
