#include <iostream>

using std::cout;
using std::endl;

// OOP 实现顺序栈
class SeqStack {
public:
    void init(int size = 10) {
        _pstack = new int[size]; // 堆内存的开辟
        _top = -1;
        _size = size;
    }

    void release() {
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

int main() {
    SeqStack s;
    s.init(5); // 对象成员变量的初始化操作, 需手动调用

    for (int i = 0; i < 15; ++i) {
        s.push(rand() % 100);
    }

    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }

    s.release(); // 释放对象成员变量占用的外部堆内存 (外部资源) , 需手动调用

    return 0;
}
