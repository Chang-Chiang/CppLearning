#include <iostream>

using std::cout;
using std::endl;

// OOP 实现顺序栈
class SeqStack {
public:
    // 构造函数, 是可以带参数的, 因此可以提供多个构造函数, 叫做构造函数的重载
    // SeqStack s1;  // 默认构造, 参数是 10
    // SeqStack s2(20);  // 参数是 20
    SeqStack(int size = 10) {
        cout << this << "SeqStack()" << endl;
        _pstack = new int[size]; // 堆内存的开辟
        _top = -1;
        _size = size;
    }

    // 析构函数, 是不带参数的, 所以析构函数只能有一个
    ~SeqStack() {
        cout << this << "~SeqStack()" << endl;
        delete[] _pstack;
        _pstack = nullptr;
    }

    void push(int val) {
        if (full()) { // 栈满, 扩容
            resize();
        }
        _pstack[++_top] = val;
    }

    void pop() {
        if (empty()) {
            return;
        }
        --_top;
    }

    int top() { return _pstack[_top]; }

    bool empty() { return _top == -1; }
    bool full() { return _top == _size - 1; }

private:
    int* _pstack; // 动态开辟数组, 存储顺序栈的元素
    int  _top;    // 指向栈顶元素的位置
    int  _size;   // 数组扩容的总大小

    void resize() {
        int* ptmp = new int[_size * 2];
        for (int i = 0; i < _size; ++i) {
            ptmp[i] = _pstack[i];
        }

        /*
        为什么不用下列语句替代 for 循环赋值
        memcpy(ptmp, _pstack, sizeof(int) * _size);
        或 realloc 函数
        涉及的是内存拷贝, 对对象而言不适合, 会产生问题
        */

        delete[] _pstack;
        _pstack = ptmp;
        _size *= 2;
    }
};

SeqStack gs; // 全局对象, .data 上, 程序结束时析构
int      main() {

    /*
    内存无非分为三块
    .data  数据段
    heap
    stack
    */

    // 开辟在 heap 上
    SeqStack* ps = new SeqStack(60); // mallocn内存开辟 + SeqStack(60) 对象构造
    ps->push(70);
    ps->push(80);
    ps->pop();
    cout << ps->top() << endl;
    delete ps;  // 先调用 ps->~SeqStack() + 然后 free(ps), delete 和 free 的区别

    SeqStack s; // stack 上, 开辟内存 + 调用构造函数
    // s.init(5);  // 对象成员变量的初始化操作, 需手动调用

    for (int i = 0; i < 15; ++i) {
        s.push(rand() % 100);
    }

    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }

    // s.release();  // 释放对象成员变量占用的外部堆内存 (外部资源) , 需手动调用

    SeqStack s1(50);
    s1.~SeqStack(); // 析构函数调用以后, 对象不存在了, 但是其内存还在, 函数结束后释放
    // s1.push(30); // 段错误，堆内存的非法访问

    return 0;
}
