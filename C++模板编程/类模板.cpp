#include <iostream>
// 顺序栈
template <typename T = int>
class SeqStack { // 是一个模板名称, 而不是类名称
public:
    // 构造函数和析构函数不用加 <T> 参数列表
    // 其他出现模板的地方都加上类型参数列表
    SeqStack(int size = 10) : _pstack(new T[size]), _top(0), _size(size) {}

    // 析构函数, 是不带参数的, 所以析构函数只能有一个
    ~SeqStack() {
        delete[] _pstack;
        _pstack = nullptr;
    }

    SeqStack(const SeqStack<T>& stack) : _top(stack._top), _size(stack._size) {

        _pstack = new T[stack._size]; // 堆内存的开辟
        for (int i = 0; i < _top; ++i) {
            _pstack[i] = stack._pstack[i];
        }
    }

    SeqStack<T>& operator=(const SeqStack<T>& stack) {

        if (this == &stack) {
            return *this;
        }

        // 需要先释放当前对象占用的外部资源
        delete[] _pstack;

        _top = stack._top;
        _size = stack._size;
        _pstack = new T[stack._size]; // 堆内存的开辟
        for (int i = 0; i < _top; ++i) {
            _pstack[i] = stack._pstack[i];
        }

        return *this;
    }

    void push(const T& val);

    void pop() {
        if (empty()) {
            return;
        }
        --_top;
    }

    T top() const {                 // 只涉及读, 不涉及写, 尽量设计成常方法
        if (empty()) {
            throw "stack is empty"; // 抛异常也代表函数逻辑结束
        }
        return _pstack[_top - 1];
    }

    bool empty() const { return _top == 0; }
    bool full() const { return _top == _size; }

private:
    T*  _pstack; // 动态开辟数组, 存储顺序栈的元素
    int _top;    // 指向栈顶元素的位置
    int _size;   // 数组扩容的总大小

    // 顺序栈底层数组按 2 倍的方式扩容
    void expand() {
        T* ptmp = new T[_size * 2];
        for (int i = 0; i < _top; ++i) {
            ptmp[i] = _pstack[i];
        }

        delete[] _pstack;
        _pstack = ptmp;
        _size *= 2;
    }
};

template <typename T>
void SeqStack<T>::push(const T& val) {
    if (full()) { // 栈满, 扩容
        expand();
    }
    _pstack[_top++] = val;
}

int main() {

    // 类模板的选择性实例化
    // 模板类, class SeqStack<int> 从模板实例化的一个类型
    SeqStack<int> s1; // 实例化构造、析构
    s1.push(20);      // 成员方法只有在调用到, 在实例化过程中才会产生
    s1.pop();
    std::cout << s1.top() << std::endl;

    return 0;
}
