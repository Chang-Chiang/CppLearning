#include <iostream>

using std::cout;
using std::endl;

// 定义容器的空间配置器，和 C++ 标准库的 allocator 实现一样
template <typename T>
struct Allocator {
    // 负责内存开辟
    T* allocate(size_t size) { return (T*)malloc(sizeof(T) * size); }

    // 负责内存释放
    void deallocate(void* p) { free(p); }

    // 负责对象构造
    void construct(T* p, const T& val) {
        // 定位 new
        new (p) T(val);
    }

    // 负责对象析构
    void destroy(T* p) {
        // ~T() 代表了 T 类型的析构函数
        p->~T();
    }
};

/*
容器底层内存开辟、内存释放、对象构造、对象析构
都通过 allocator 空间配置器来实现
*/
// Allocator 是模板名, Allocator<T> 才是类型名
template <typename T, typename Alloc = Allocator<T>>
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

    // 向容器末尾添加元素
    void push_back(const T& val) {
        if (full()) {
            expand();
        }
        // *_last++ = val;

        // _last 指针指向的内存构造一个值为 val 的对象
        _allocator.construct(_last, val);
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

public:
    T& operator[](int index) {
        if (index < 0 || index >= size()) {
            throw "OutOfRangeException";
        }
        return _first[index];
    }

    // 迭代器一般实现成容器的嵌套类型
    // 不同容器底层数据结构不一样，具体迭代方式也不一样
    class iterator {
    public:
        iterator(T* ptr = nullptr) : _ptr(ptr) {}
        bool operator!=(const iterator& it) const { return _ptr != it._ptr; }
        void operator++() { _ptr++; }
        T&   operator*() { return *_ptr; }
        // int data = *it; *it = 20; 解引用读取、赋值
        // 所以不设为常方法

        const T& operator*() const { return *_ptr; } // 不能解引用后赋值

    private:
        T* _ptr;
    };

public:
    iterator begin() { return iterator(_first); }
    iterator end() { return iterator(_last); }

public:
    // 自定义 vector 容器 insert 方法的简单实现
    // 1. 不考虑扩容
    // 2. 不考虑 it._ptr 的指针合法性
    iterator insert(iterator it, const T& val) {
        verify(it._ptr - 1, _last); // 若考虑扩容, 则是 verify(_first - 1, _last);
        T* p = _last;
        while (p > it._ptr) {
            _allocator.construct(p, *(p - 1));
            _allocator.destroy(p - 1);
            p--;
        }
        _allocator.construct(p, val);
        _last++;
        return iterator(this, p);
    }

    // 自定义 vector 容器 erase 方法的简单实现
    iterator erase(iterator it) {
        verify(it._ptr - 1, _last);
        T* p = it._ptr;
        while (p < _last - 1) {
            _allocator.destroy(p);
            _allocator.construct(p, *(p + 1));
            p++;
        }
        _allocator.destroy(p);
        _last--;
        return iterator(this, it._ptr);
    }

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

int main() {
    vector<int> vec(200);
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100 + 1);
    }

    for (int v : vec) {
        cout << v << endl;
    }
    cout << endl;

    /*---- 迭代器失效部分 ----*/

    // 把 vec 容器中所有的偶数全部删除
    auto it = vec.begin();
    for (; it != vec.end(); ++it) {
        if (*it % 2 == 0) {

            // 迭代器失效
            // 第一次调用 erase 以后, 迭代器 it 就失效了
            vec.erase(it);
            // insert(it, val) 添加到迭代器指定位置
            // break;  // 只删除一个偶数
        }
    }

    // 给 vec 容器中所有的偶数前添加一个比当前偶数值小 1 的数字
    // auto it = vec.begin();
    // for (; it != vec.end(); ++it) {
    //     if (*it % 2 == 0) {

    //         // 迭代器在第一次 insert 后, iterator 就失效了
    //         vec.insert(it, *it - 1);
    //         // break;  // 只添加一次
    //     }
    // }

    /*---- 迭代器失效部分 ----*/

    return 0;
}
