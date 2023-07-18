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

        // erase(it);  里做的是 verify(it._ptr, _last);
    	// insert(it, val);  里做的是 verify(it._ptr, _last);
        verify(_last - 1, _last);

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
        friend class vector<T, Alloc>;
        iterator(vector<T, Alloc>* pvec, T* ptr = nullptr) : _ptr(ptr), _pVec(pvec) {
            Iterator_Base* itb = new Iterator_Base(this, pVec->_head._next);
            _pVec->_head._next = itb;
        }

        bool operator!=(const iterator& it) const {

            // 检查迭代器的有效性
            if (_pVec == nullptr || _pVec != it._pVec) {
                throw "iterator incompatable!";
                return false;
            }
            return _ptr != it._ptr;
        }

        void operator++() {

            // 检查迭代器的有效性
            if (_pVec == nullptr) {
                throw "iterator invalid!";
            }
            _ptr++;
        }

        T& operator*() {

            // 检查迭代器的有效性
            if (_pVec == nullptr) {
                throw "iterator invalid!";
            }
            return *_ptr;
        }

        const T& operator*() const {

            // 检查迭代器的有效性
            if (_pVec == nullptr) {
                throw "iterator invalid!";
            }
            return *_ptr;
        } // 不能解引用后赋值

        // iterator(T* ptr = nullptr) : _ptr(ptr) {}
        // bool operator!=(const iterator& it) const { return _ptr != it._ptr; }
        // void operator++() { _ptr++; }
        // T&   operator*() { return *_ptr; }
        // int data = *it; *it = 20; 解引用读取、赋值
        // 所以不设为常方法
        // const T& operator*() const { return *_ptr; } // 不能解引用后赋值

    private:
        T* _ptr;

        // 当前迭代器是哪个容器对象
        vector<T, Alloc>* _pVec;
    };

public:
    // iterator begin() { return iterator(_first); }
    // iterator end() { return iterator(_last); }
    iterator begin() { return iterator(this, _first); }
    iterator end() { return iterator(this, _last); }

    // 检查迭代器失效
    void verify(T* first, T* last) {
        Iterator_Base* pre = &this->head;
        Iterator_Base* it = this->head._next;

        while (it != nullptr) {
            if (it->_cur->_ptr >= first && it->_cur->_ptr <= last) {
                // 迭代器失效, 把 iterator 持有的容器指针置 nullptr
                it->_cur->_pVec = nullptr;

                // 删除当前迭代器节点, 继续判断后面的迭代器节点是否失效
                pre->_next = it->_next;
                delete it;
                it = pre->_next;
            }
            else {
                pre = it;
                it = it->_next;
            }
        }
    }

private:
    T*    _first;     // 指向数组起始位置
    T*    _last;      // 指向数组中有效元素的后继位置
    T*    _end;       // 指向数组空间的后继位置
    Alloc _allocator; // 定义容器的空间配置器对象

    // 容器迭代器失效增加代码
    // 链表存储迭代器
    struct Iterator_Base {
        Iterator_Base(iterator* c = nullptr, Iterator_Base* n = nullptr) : _cur(c), _next(n) {}
        iterator*      _cur;
        Iterator_Base* _next;
    } Iterator_Base _head;

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
    vector<int> vec;
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100 + 1);
    }

    auto it1 = vec.end();
    // vec.pop_back();  // 执行后, 无法比较
    auto it2 = vec.end();

    cout << (it1 != it2) << endl;

    return 0;
}
