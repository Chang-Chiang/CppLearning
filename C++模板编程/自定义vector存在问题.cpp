#include <iostream>

using std::cin;
using std::cout;
using std::endl;

template <typename T>
class vector {
public:
    // 构造
    vector(int size = 10) {
        // 需求：需要把内存开辟和对象构造分开处理
        _first = new T[size]; // new 一个数组, 不仅开辟空间, 还会构造对象
        _last = _first;
        _end = _first + size;
    }

    // 析构
    ~vector() {
        // 需求：析构容器有效的元素, 然后释放 _first 指向的堆内存
        delete[] _first;
        _first = _last = _end = nullptr;
    } // 拷贝构造
    vector(const vector<T>& rhs) {
        int size = rhs._end - rhs._first;
        _first = new T[size];

        int len = rhs._last - rhs._first;
        for (int i = 0; i < len; ++i) {
            _first[i] = rhs._first[i];
        }

        _last = _first + len;
        _end = _first + size;
    }

    // 赋值重载
    vector<T>& operator=(const vector<T>& rhs) {
        if (this == &rhs) {
            return *this;
        }

        delete[] _first;

        int size = rhs._end - rhs._first;
        _first = new T[size];

        int len = rhs._last - rhs._first;
        for (int i = 0; i < len; ++i) {
            _first[i] = rhs._first[i];
        }

        _last = _first + len;
        _end = _first + size;

        return *this;
    }

    // 向容器末尾添加元素
    void push_back(const T& val) {
        if (full()) {
            expand();
        }
        *_last++ = val;
    }

    // 从容器末尾删除元素
    void pop_back() {
        if (empty()) {
            return;
        }
        --_last;
    }

    // 返回容器末尾的元素值
    T back() const { return *(_last - 1); }

    bool full() const { return _last == _end; }
    bool empty() const { return _first == _last; }
    int  size() const { return _last - _first; }

private:
    T* _first; // 指向数组起始位置
    T* _last;  // 指向数组中有效元素的后继位置
    T* _end;   // 指向数组空间的后继位置

    // 容器的二倍扩容
    void expand() {
        int size = _end - _first;
        T*  ptmp = new T[2 * size];

        for (int i = 0; i < size; ++i) {
            ptmp[i] = _first[i];
        }

        delete[] _first;
        _first = ptmp;
        _last = _first + size;
        _end = _first + 2 * size;
    }
};

class Test {
public:
    Test() { cout << "Test()" << endl; }
    Test(const Test&) { cout << "Test(const Test&)" << endl; }
    ~Test() { cout << "~Test()" << endl; }
};

int main() {

    Test t1, t2, t3;

    cout << "-------------" << endl;

    vector<Test> vec;

    // 期望：在容器底层内存构造一个新对象
    // 实际做的是拷贝
    vec.push_back(t1);
    vec.push_back(t2);
    vec.push_back(t3);

    cout << "-------------" << endl;

    // 删除对象时, 未将对象析构 (可能占用了外部资源, 必须析构)
    // 不能用 delete(不仅析构对象, 还释放内存) -> 容器的堆内存, 不能释放
    // 需求：只需要析构对象, 要把对象的析构和内存释放分离开
    vec.pop_back();

    cout << "-------------" << endl;

    return 0;
}
