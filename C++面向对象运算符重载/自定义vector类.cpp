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
        _allocator.destroy(_last)
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
