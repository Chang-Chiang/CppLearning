# 智能指针

## 裸指针存在的问题

```c++
int main() {
    /*
    数据位于三部分:
        + data 段
        + heap 堆: 用户手动开辟, 手动释放
        + stack 栈
    */
    int* p = new int(10);
    *p = 30;

    // if ()
    // 	return -1;

    delete p; // 忘记写 或 程序中途退出 或 抛出异常，造成资源泄露
    return 0;
}

```

+ `new` 的资源忘记 `delete`，导致资源泄露
+ `new` 的资源 `delete` 多次，导致释放野指针，程序崩溃
+ 程序在 `new` 和 `delete` 间退出：
  + 满足条件正常 return
  + 异常退出

## C++ 智能指针介绍

> + 智能指针的设计就是为了解决裸指针需要手动释放资源的问题，智能指针一般定义在栈上，出作用域自动析构释放资源
> + 不带引用计数的智能指针：`auto_ptr`、`scoped_ptr`、`unique_ptr`
> + 带引用计数的智能指针：`shared_ptr`、`weak_ptr`

### auto_ptr

> + C++98，已经被 C++11 废弃
> + 最好不要使用 auto_ptr，除非应用场景非常简单
> + auto_ptr 智能指针不带引用计数，那么它处理浅拷贝的问题，是直接把前面的 auto_ptr 都置为 nullptr，只让最后一个 auto_ptr 持有资源

#### 源码设计

```c++
class auto_ptr { // wrap an object pointer to ensure destruction
public:
    typedef _Ty element_type;

    explicit auto_ptr(_Ty* _Ptr = nullptr) noexcept
        : _Myptr(_Ptr) { // construct from object pointer
    }

    /*这里是auto_ptr的拷贝构造函数，
    _Right.release()函数中，把_Right的_Myptr
    赋为nullptr，也就是换成当前auto_ptr持有资源地址
    */
    auto_ptr(auto_ptr& _Right) noexcept
        : _Myptr(_Right.release()) { // construct by assuming pointer from _Right auto_ptr
    }

    _Ty* release() noexcept { // return wrapped pointer and give up ownership
        _Ty* _Tmp = _Myptr;
        _Myptr = nullptr;
        return (_Tmp);
    }

private:
    _Ty* _Myptr; // the wrapped object pointer
};
```

#### 使用

```c++
#include <memory>
using namespace std;

// 不推荐使用 auto_ptr
// 不推荐在容器中使用 auto_ptr
int main() {
    auto_ptr<int> ptr1(new int);
    auto_ptr<int> ptr2(ptr1);
    /*
    ptr2 指向 ptr1 指向内存
    ptr1 释放, ptr1 = nullptr
    只有最后被拷贝赋值的指针(ptr2)能使用
    */

    *ptr2 = 20;
    // cout << *ptr1 << endl;  // 程序崩溃 ptr1 = nullptr

    return 0;
}
```

#### 存在问题：浅拷贝问题

+ 只有最后一个 `auto_ptr` 持有资源，原来的 `auto_ptr` 都被赋 `nullptr`，访问之前的 `auto_ptr` 会导致程序崩溃
+ :question:`auto_ptr`能不能在容器中使用。发生容器拷贝后，容器中每一个元素都进行了拷贝构造，原来容器中存的 `auto_ptr` 全被赋 `nullptr`，再次访问就是访问空指针，程序崩溃；故不要在容器中使用 `auto_ptr`

### scoped_ptr

> + C++11
> + 在 auto_ptr 的基础上禁止拷贝构造和赋值运算符重载

#### 源码设计

```c++
template <class T>
class scoped_ptr // noncopyable
{
private:
    T* px;

    /*
    私有化拷贝构造函数和赋值函数，这样scoped_ptr的智能指针
    对象就不支持这两种操作，从根本上杜绝浅拷贝的发生
    */
    scoped_ptr(scoped_ptr const&);
    scoped_ptr& operator=(scoped_ptr const&);

    typedef scoped_ptr<T> this_type;

    /*
    私有化逻辑比较运算符重载函数，不支持scoped_ptr的智能指针
    对象的比较操作
    */
    void operator==(scoped_ptr const&) const;
    void operator!=(scoped_ptr const&) const;

public:
    typedef T element_type;
    explicit scoped_ptr(T* p = 0)
        : px(p) // never throws
        {}
};
```

#### 使用

#### 存在问题

+ 与 `auto_ptr` 一样，不要在容器中使用

### unique_ptr

> + C++11
> + 禁止拷贝构造和赋值运算符重载
> + 提供带右值引用参数的拷贝构造和赋值
> + 提供 reset 重置资源、swap 交换资源等函数
> + 在使用不带引用计数的智能指针时，优先选择 unique_ptr 智能指针

#### 源码设计

#### 使用

```c++
// 示例一：右值引用的拷贝构造与赋值
unique_ptr<int> ptr(new int);
unique_ptr<int> ptr2 = std::move(ptr); // 使用了右值引用的拷贝构造
ptr2 = std::move(ptr);                 // 使用了右值引用的operator=赋值重载函数

// 示例2
unique_ptr<int> test_uniqueptr() {
    unique_ptr<int> ptr1(new int);
    return ptr1;
}

int main() {
    /*
    此处调用test_uniqueptr函数，在return ptr1代码处，
    调用右值引用的拷贝构造函数，由ptr1拷贝构造ptr
    */
    unique_ptr<int> ptr = test_uniqueptr();
    return 0;
}
```

#### 存在问题

### shared_ptr

> + 允许多个智能指针指向同一个资源的时候，引入引用计数

#### 源码设计

:question:`shared_ptr` 智能指针的引用计数在哪里存放？

答：`shared_ptr` 智能指针的资源引用计数器在内存的heap堆上

#### 使用

```c++
#include <iostream>
#include <memory>
using namespace std;

class B; // 前置声明类B

class A {
public:
    A() { cout << "A()" << endl; }
    ~A() { cout << "~A()" << endl; }
    shared_ptr<B> _ptrb; // 指向B对象的智能指针
};

class B {
public:
    B() { cout << "B()" << endl; }
    ~B() { cout << "~B()" << endl; }
    shared_ptr<A> _ptra; // 指向A对象的智能指针
};

int main() {
    shared_ptr<A> ptra(new A()); // ptra指向A对象，A的引用计数为1
    shared_ptr<B> ptrb(new B()); // ptrb指向B对象，B的引用计数为1
    ptra->_ptrb = ptrb; // A对象的成员变量_ptrb也指向B对象，B的引用计数为2
    ptrb->_ptra = ptra; // B对象的成员变量_ptra也指向A对象，A的引用计数为2

    cout << ptra.use_count() << endl; // 打印A的引用计数结果:2
    cout << ptrb.use_count() << endl; // 打印B的引用计数结果:2

    /*
    出 main 函数作用域，ptra 和 ptrb 两个局部对象析构，分别给A对象和B对象的引用计数从2减到1，
    达不到释放A和B的条件（释放的条件是 A和B的引用计数为0），因此造成两个new出来的A和B对象无法释放，
    导致内存泄露，这个问题就是“强智能指针的交叉引用(循环引用)问题”
    */
    return 0;
}
```

#### 存在问题

+ 交叉引用造成内存泄露

### weak_ptr

> + weak_ptr 不会改变资源的引用计数，只是一个观察者的角色，通过观察 shared_ptr 来判定资源是否存在
> + weak_ptr 持有的引用计数，不是资源的引用计数，而是同一个资源的观察者的计数
> + weak_ptr 没有提供常用的指针操作，无法直接访问资源，需要先通过 lock 方法提升为shared_ptr 强智能指针，才能访问资源
> + **定义对象时，用强智能指针 shared_ptr，在其它地方引用对象时，使用弱智能指针 weak_ptr**

#### 源码设计

#### 使用

```c++
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
```

#### 存在问题

## 自己实现智能指针

### 对裸指针的简单封装

```c++
#include <iostream>
using namespace std;

template <typename T>
class CSmartPtr {
public:
    CSmartPtr(T* ptr = nullptr) : mptr(ptr) {}
    ~CSmartPtr() { delete mptr; }
    T&       operator*() { return *mptr; }
    const T& operator*() const { return *mptr; }
    T*       operator->() { return mptr; } // (ptr.operator->())->test();
    const T* operator->() const { return mptr; }

private:
    T* mptr;
};

int main() {
    CSmartPtr<int> ptr(new int);
    /*其它的代码...*/
    *ptr = 20;
    cout << *ptr << endl;

    /*
    由于ptr是栈上的智能指针对象，
    不管是函数正常执行完，
    还是运行过程中出现异常，
    栈上的对象都会自动调用析构函数，
    在析构函数中进行了 delete 操作，保证释放资源
    */
    return 0;
}

// 问：智能指针能否定义在堆上？
// CSmartPtr<int> *p = new CSmartPtr<int>(new int);  // p 是裸指针, 无意义
```

### 重写拷贝构造：解决浅拷贝问题

> 拷贝构造默认浅拷贝，析构时同一块资源释放两次

```c++
template <typename T>
class CSmartPtr {
public:
    CSmartPtr(T* ptr = nullptr) : mptr(ptr) {}
    ~CSmartPtr() { delete mptr; }

    /*
    解决浅拷贝， 重新开辟空间
    */
    CSmartPtr(const CSmartPtr<T>& src) { mptr = new T(*src.mptr); }

    T&       operator*() { return *mptr; }
    const T& operator*() const { return *mptr; }

    T*       operator->() { return mptr; } // (ptr.operator->())->test();
    const T* operator->() const { return mptr; }

private:
    T* mptr;
};

int main() {
    CSmartPtr<int> ptr1(new int);
    CSmartPtr<int> ptr2(ptr1);

    *ptr1 = 20;
    *ptr2 = 30;
    // 需求: ptr1, ptr2管理同一块资源
    // 但目前新写的拷贝构造 是两块资源

    return 0;
}
```

### 自定义删除器

+ **背景**：
  + 智能指针的删除器 deletor
  + 智能指针: 能够保证资源绝对的释放. 释放资源方式 delete ptr;
  + 但是释放数组 delete[], 释放文件等不能用 delete
  + 删除器: 指导智能指针正确地释放资源

```c++
/*
unique_ptr, shared_ptr 的析构函数里面是一个函数对象的调用
智能指针的默认删除器:
template<typename T>
class default_delete {
public:
    void operator()(T *ptr) {
        delete ptr;
    }
};
*/

#include <iostream>
#include <memory>
using namespace std;

template <typename T>
class MyDeletor {
public:
    void operator()(T* ptr) const {
        cout << "call MyDeletor.operator()" << endl;
        delete[] ptr;
    }
};

template <typename T>
class MyFileDeletor {
public:
    void operator()(T* ptr) const {
        cout << "call MyFileDeletor.operator()" << endl;
        fclose(ptr);
    }
};

int main() {
    // unique_ptr<int> ptr1(new int[100]);  // 默认为 delete, 但需要的是 delete[]
    unique_ptr<int, MyDeletor<int>>       ptr1(new int[100]);
    unique_ptr<FILE, MyFileDeletor<FILE>> pre2(fopen("data.txt", "w"));
    return 0;
}
```

## 参考

+ [深入掌握C++智能指针](https://blog.csdn.net/QIANGWEIYUAN/article/details/88562935)