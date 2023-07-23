#include <iostream>
using std::cout;
using std::endl;

// 定义容器的空间配置器 (和 C++ 标准库的 allocator 实现一样)
template <typename T>
class Allocator {
public:
    // 负责内存开辟
    T* allocate(size_t size) { return (T*)malloc(sizeof(T) * size); }

    // 负责内存释放
    void deallocate(void* p) { free(p); }

    // // 负责对象构造
    // // 接收左值
    // void construct(T* p, const T& val) {
    //     new (p) T(val); // 定位 new, 在指定内存上构造值为 val 的对象
    // }

    // // 负责对象构造
    // // 接收右值
    // void construct(T *p, const T &&val) {
    //     new (p) T(std::move(val));  // val 强转成右值引用类型
    // }

    // 模板实现
    template<typename Ty>
    void construct(T *p, Ty &&val) {
        new (p) T(std::forward<Ty>(val));
    }

    // 负责对象析构
    void destroy(T* p) {
        p->~T(); // ~T() 代表了 T 类型的析构函数
    }
};

/*
容器底层内存开辟、内存释放、对象构造、对象析构
都通过 allocator 空间配置器来实现
*/
template <typename T, typename Alloc = Allocator<T>> // Allocator 是模板名, Allocator<T> 才是类型名
class vector {
public:
    // 构造
    vector(int size = 10, const Alloc& alloc = Allocator<T>()) : _allocator(alloc) {
        // _first = new T[size];
        _first = _allocator.allocate(size); // 仅开辟内存
        _last = _first;
        _end = _first + size;
    }

    // 析构
    ~vector() {
        // delete []_first;

        // 把 _first 指针指向的数组的有效元素进行析构操作
        for (T* p = _first; p != _last; ++p) {
            _allocator.destroy(p);
        }

        _allocator.deallocate(_first); // 释放堆上的数组内存
        _first = _last = _end = nullptr;
    }

    // 拷贝构造
    vector(const vector<T>& rhs) {
        int size = rhs._end - rhs._first;
        // _first = new T[size];

        _first = _allocator.allocate(size); // 开辟空间

        int len = rhs._last - rhs._first;
        for (int i = 0; i < len; ++i) {
            // _first[i] = rhs._first[i];
            _allocator.construct(_first + i, rhs._first[i]);
        }

        _last = _first + len;
        _end = _first + size;
    }

    // 赋值重载
    vector<T>& operator=(const vector<T>& rhs) {
        if (this == &rhs) {
            return *this;
        }

        // delete []_first;

        // 把 _first 指针指向的数组的有效元素进行析构操作
        for (T* p = _first; p != _last; ++p) {
            _allocator.destroy(p);
        }

        _allocator.deallocate(_first); // 释放堆上的数组内存

        int size = rhs._end - rhs._first;
        // _first = new T[size];

        _first = _allocator.allocate(size); // 开辟空间

        int len = rhs._last - rhs._first;
        for (int i = 0; i < len; ++i) {
            // _first[i] = rhs._first[i];
            _allocator.construct(_first + i, rhs._first[i]);
        }

        _last = _first + len;
        _end = _first + size;

        return *this;
    }

    // // 向容器末尾添加元素
    // // 接收左值
    // void push_back(const T& val) {
    //     if (full()) {
    //         expand();
    //     }
    //     // *_last++ = val;

    //     // _last 指针指向的内存构造一个值为 val 的对象
    //     _allocator.construct(_last, val);
    //     _last++;
    // }

    // // 向容器末尾添加元素
    // // 接收右值, 一个右值引用变量本身还是一个左值
    // void push_back(const T &&val) {
    //     if (full())
    //         expand();
    //     _allocator.construct(_last, std::move(val));  // val 强转成右值引用类型
    //     _last++;
    // }

    template<typename Ty>
    void push_back(Ty &&val) {  // 函数模板的类型推演 + 引用折叠
        /* 引用折叠
        CMyString& &&val -> CMyString &val
        CMyString&& &&val -> CMyString &&val
        */
        if (full())
            expand();

        // move 移动语义, 得到右值类型
        // forward 类型的完美转发, 能够识别左值和右值类型 <- 通过模板的非完全特例化实现
        _allocator.construct(_last, std::forward<Ty>(val));
        _last++;
    }

    // 从容器末尾删除元素
    void pop_back() {
        if (empty()) {
            return;
        }
        // --_last;

        // 不仅 --last, 还需要析构删除的元素
        --_last;
        _allocator.destroy(_last);
    }

    // 返回容器末尾的元素值
    T back() const { return *(_last - 1); }

    bool full() const { return _last == _end; }
    bool empty() const { return _first == _last; }
    int  size() const { return _last - _first; }

private:
    T*    _first;     // 指向数组起始位置
    T*    _last;      // 指向数组中有效元素的后继位置
    T*    _end;       // 指向数组空间的后继位置
    Alloc _allocator; // 定义容器的空间配置器对象

    // 容器的二倍扩容
    void expand() {
        int size = _end - _first;
        // T *ptmp = new T[2 * size];

        T* ptmp = _allocator.allocate(2 * size);

        for (int i = 0; i < size; ++i) {
            // ptmp[i] = _first[i];
            _allocator.construct(ptmp + i, _first[i]);
        }

        // delete []_first;

        for (T* p = _first; p != _last; ++p) {
            _allocator.destroy(p);
        }
        _allocator.deallocate(_first);

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
