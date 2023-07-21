# C++ 模板编程

## 简介

模板的意义：对类型也可以进行参数化

+ 函数模板

  主体是 【模板】, 是不进行编译的, 因为类型还不知道

+ 模板的实例化

  在函数的调用点实例化, 编译器用用户指定的类型, 从原模版实例化一份函数代码出来

+ 模板函数

  用模板从指定类型实例化出来, 真正需要进行代码编译的函数

+ 模板类型参数

  typename/class 定义模板类型参数

+ 模板非类型参数

  + 都是常量, 只能使用, 而不能修改
  + 必须是整数类型 (整数、地址、引用都可以) 

+ 模板的实参推演

  可以根据用户传入的实参的类型, 来推导出模板类型参数的具体类型

+ 模板的特例化 (专用化) 

  特殊的实例化, 不是编译器提供的, 而是开发者提供的。对于某些类型来说, 依赖编译器默认实例化的模板代码, 代码处理逻辑是有错误的

+ 模板代码的定义

  + 多文件通常将函数定义、声明分开

  + 对模板而言, 若将定义、声明分开, 会出现链接错误：模板是不进行编译的

  + 模板代码是不能在一个文件中定义, 在另外一个文件中使用的, 模板代码调用之前, 一定要看到模板定义的地方！这样的话, 模板才能进行正常的实例化, 产生能够被编译器编译的代码

  + 所以, 模板代码都是放在头文件当中的, 然后在源文件中直接进行 #include 包含

  + 或者

    ```c++
    // 告诉编译器, 进行指定类型的模板实例化
    template bool compare<int>(int, int);
    template bool compare<double>(double, double);
    ```

+ 函数模板、模板的特例化、非模板函数的重载 之间的关系

  很多书上将这三者称为函数重载, 说法错误, 函数重载的定义是：函数名相同, 参数列表不同。但是模板的特例化、非模板函数 函数名不一样！

## 函数模板

```c++
// 函数模板
template<typename T>  // 定义一个模板参数列表
bool compare(T a, T b) {  // compare 是一个函数模板, 模板名称
    cout << "template compare" << endl;
    return a > b;
}

// 针对 compare 函数模板, 提供 const char* 类型的特例化版本
template<>
bool compare<const char*>(const char *a, const char *b) {  
    cout << "compare<const char*>" << endl;
    return strcmp(a, b) > 0;
}

// 非模板函数, 即普通函数
bool compare(const char *a, const char *b) {  
    cout << "normal compare" << endl;
    return strcmp(a, b) > 0;
}
/*
在函数调用点, 编译器用用户指定的类型, 从原模版实例化一份函数代码出来
模板函数：用模板从指定类型实例化出来, 真正需要进行代码编译的函数
bool compare<int>(int a, int b) {
	cout << "template compare" << endl;
    return a > b;
}
*/

int main() {
    
    // 函数的调用点
    compare<int>(10, 20);  // compare<int> 模板名 + 参数列表 才是函数名 
    compare<double>(10.5, 20.5);
    
    compare(20, 30);  // 函数模板的实参推演
    
    // compare(30, 40.5);  // 错误 -> 定义新的模板参数列表 或者指定参数 compare<int>(30, 40.5);
    
    /*
    模板的特例化：
    对于某些类型来说, 依赖编译器默认实例化的模板代码, 代码处理逻辑是有错误的
    */
    compare("aaa", "bbb");  // 实参推演为 const char*, 比较的是两个字符串的地址
    // 希望是 strcmp(a, b) > 0;
    // 当存在普通函数时, 优先调用普通函数 (把 compare 处理成函数的名字) , 编译器省去实参推演
    // 没有的话, 才去找 compare 模板
    
    compare<const char*>("aaa", "bbb");  // 调用模板函数
    
    return 0;
}
```

## 模板的非类型参数

```c++
// 模板的非类型参数

// 冒泡排序
template<typename T, int SIZE>  // T 为 模板类型参数, SIZE 为 模板非类型参数
void sort(T *arr) {
    for (int i = 0; i < SIZE - 1; ++i) {
        for (int j = 0; j < SIZE - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main() {
    int arr[] = {12, 5, 7, 89, 32, 21, 35};
    const int size = sizeof(arr) / sizeof(arr[0]);
    sort<int, size>(arr);
    
    for (int val : arr) {
        cout << val << " ";
    }
    
    return 0;
}
```

## 类模板

类模板实例化得到模板类

```c++
// 顺序栈
template<typename T=int>
class SeqStack {  // 是一个模板名称, 而不是类名称
public:
    // 构造函数和析构函数不用加 <T> 参数列表
    // 其他出现模板的地方都加上类型参数列表
    SeqStack(int size = 10)
        :_pstack = new T[size]
    	, _top(0)
        , _size(size)
    {}
    
    // 析构函数, 是不带参数的, 所以析构函数只能有一个
    ~SeqStack() {
        delete []_pstack;
        _pstack = nullptr;
    }
    
    SeqStack(const SeqStack<T> &stack)
        :_top(stack._top)
        , _size(stack._size){

        _pstack = new T[stack._size];  // 堆内存的开辟
        for (int i = 0; i < _top; ++i) {  
            _pstack[i] = stack._pstack[i];   
        }
    }
    
    SeqStack<T>& operator=(const SeqStack<T> &stack){

        if (this == &stack)
            return *this;
        
        // 需要先释放当前对象占用的外部资源
        delete []_pstack;
        
        _top = stack._top;
        _size = stack._size;
        _pstack = new T[stack._size];  // 堆内存的开辟
        for (int i = 0; i < _top; ++i) {              
            _pstack[i] = stack._pstack[i];   
        }

        return *this;
    }
    
    void push(const T &val);
    
    void pop() {
        if(empty())
            return;
        --_top;
    }
    
    T top()const {  // 只涉及读, 不涉及写, 尽量设计成常方法
        if (empty())
            throw "stack is empty";  // 抛异常也代表函数逻辑结束
        return _pstack[_top - 1];
    }
    
    bool empty()const { return _top == 0; }
    bool full()const { return _top == _size; }
    
private:
    T *_pstack;  // 动态开辟数组, 存储顺序栈的元素
    int _top;  // 指向栈顶元素的位置
    int _size;  // 数组扩容的总大小
    
    // 顺序栈底层数组按 2 倍的方式扩容
    void expand() {
        T *ptmp = new T[_size * 2];
        for (int i = 0; i < _top; ++i) {
            ptmp[i] = _pstack[i];
        }
        
        delete []_pstack;
        _pstack = ptmp;
        _size *= 2;
    }  
};

template<typename T>
void SeqStack<T>::push(const T &val) {
        if (full())  // 栈满, 扩容
            expand();
        _pstack[_top++] = val;
}

int main() {
    
    // 类模板的选择性实例化
    // 模板类, class SeqStack<int> 从模板实例化的一个类型
    SeqStack<int> s1;  // 实例化构造、析构
    s1.push(20);  // 成员方法只有在调用到, 在实例化过程中才会产生
    s1.pop();
    cout << s1.top() << endl;
    
    return 0;
}
```

## vector 容器模板实现

```c++
template<typename T>
class vector {
public:
    // 构造
    vector(int size = 10) {
        _first = new T[size];
        _last = _first;
        _end = _first + size;
    }
    
    // 析构
    ~vector() {
        delete []_first;
        _first = _last = _end = nullptr;
    }
    
    // 拷贝构造
    vector(const vector<T> &rhs) {
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
    vector<T>& operator=(const vector<T> &rhs) {
        if (this == &rhs)
            return *this;
        
        delete []_first;
        
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
    void push_back(const T &val) {
        if (full())
            expand();
        *_last++ = val;
    }
    
    // 从容器末尾删除元素
    void pop_back() {
        if (empty())
            return;
        --_last;
    }
    
    // 返回容器末尾的元素值
    T back()const {
        return *(_last - 1);
    }
    
    bool full()const { return _last == _end; }
    bool empty()const { return _first == _last; }
    int size()const { return _last - _first; }
    
private:
    T *_first;  // 指向数组起始位置
    T *_last;  // 指向数组中有效元素的后继位置
    T *_end;  // 指向数组空间的后继位置
    
    // 容器的二倍扩容
    void expand() {
        int size = _end - _first;
        T *ptmp = new T[2 * size];
        
        for (int i = 0; i < size; ++i) {
            ptmp[i] = _first[i];
        }
        
        delete []_first;
        _first = ptmp;
        _last = _first + size;
        _end = _first + 2 * size;
        
    }
};


int main() {
    vector<int> vec;
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100);
    }
    
    vec.pop_back();
    
    while (!vec.empty()) {
        cout << vec.back() << " ";
        vec.pop_back();
    }
    
    cout << endl;
    
    return 0;
}
```

## 容器空间配置器 allocator

按上述实现 vector 存在的问题

```c++
template<typename T>
class vector {
public:
    // 构造
    vector(int size = 10) {
        // 需求：需要把内存开辟和对象构造分开处理
        _first = new T[size];  // new 一个数组, 不仅开辟空间, 还会构造对象
        _last = _first;
        _end = _first + size;
    }
    
    // 析构
    ~vector() {
        // 需求：析构容器有效的元素, 然后释放 _first 指向的堆内存
        delete []_first;  
        _first = _last = _end = nullptr;
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

/*
运行结果：构造函数、析构函数调用了十次
Test()
Test()
Test()
-------------
Test()
Test()
···
Test()
-------------
-------------
~Test()
~Test()
···
~Test()
*/
```

利用容器的空间配置器 allocator 重写 vector 类

做 4 件事

+ 内存开辟
+ 内存释放
+ 对象构造
+ 对象析构

```c++
/*
C++ 标准库的 vector
template<class _Ty, class _Alloc = allocator<_Ty>>
class vector
*/

// 定义容器的空间配置器 (和 C++ 标准库的 allocator 实现一样) 
template<typename T>
class Allocator {
public:
    
    // 负责内存开辟
    T* allocate(size_t size) {
        return (T*)malloc(sizeof(T) * size);
    }
    
    // 负责内存释放
    void deallocate(void *p) {
        free(p);
    }
    
    // 负责对象构造
    void construct(T *p, const T &val) {
        new (p) T(val);  // 定位 new, 在指定内存上构造值为 val 的对象
    }
    
    //负责对象析构
    void destroy(T *p) {
        p->~T();  // ~T() 代表了 T 类型的析构函数
    }
    
};


/*
容器底层内存开辟、内存释放、对象构造、对象析构
都通过 allocator 空间配置器来实现
*/
template<typename T, typename Alloc = Allocator<T>>  // Allocator 是模板名, Allocator<T> 才是类型名
class vector {
public:
    // 构造
    vector(int size = 10, const Alloc &alloc = Allocator<T>())
        :_allocator(alloc) {
        // _first = new T[size];
        _first = _allocator.allocate(size);  // 仅开辟内存
        _last = _first;
        _end = _first + size;
    }
    
    // 析构
    ~vector() {
        // delete []_first;
        
        // 把 _first 指针指向的数组的有效元素进行析构操作
        for (T *p = _first; p != _last; ++p) {
            _allocator.destroy(p);  
        }
        
        _allocator.deallocate(_first);  // 释放堆上的数组内存
        _first = _last = _end = nullptr;
    }
    
    // 拷贝构造
    vector(const vector<T> &rhs) {
        int size = rhs._end - rhs._first;
        // _first = new T[size];
        
        _first = _allocator.allocate(size);  // 开辟空间
        
        int len = rhs._last - rhs._first;
        for (int i = 0; i < len; ++i) {
            // _first[i] = rhs._first[i];
            _allocator.construct(_first + i, rhs._first[i]);
        }
        
        _last = _first + len;
        _end = _first + size;
    }
    
    // 赋值重载
    vector<T>& operator=(const vector<T> &rhs) {
        if (this == &rhs)
            return *this;
        
        // delete []_first;
        
        // 把 _first 指针指向的数组的有效元素进行析构操作
        for (T *p = _first; p != _last; ++p) {
            _allocator.destroy(p);  
        }
        
        _allocator.deallocate(_first);  // 释放堆上的数组内存
        
        int size = rhs._end - rhs._first;
        // _first = new T[size];
        
        _first = _allocator.allocate(size);  // 开辟空间
        
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
    void push_back(const T &val) {
        if (full())
            expand();
        // *_last++ = val;
        
        // _last 指针指向的内存构造一个值为 val 的对象
        _allocator.construct(_last, val);
        _last++;
    }
    
    // 从容器末尾删除元素
    void pop_back() {
        if (empty())
            return;
        // --_last;
        
        // 不仅 --last, 还需要析构删除的元素
        --_last;
        _allocator.destroy(_last)
    }
    
    // 返回容器末尾的元素值
    T back()const {
        return *(_last - 1);
    }
    
    bool full()const { return _last == _end; }
    bool empty()const { return _first == _last; }
    int size()const { return _last - _first; }
    
private:
    T *_first;  // 指向数组起始位置
    T *_last;  // 指向数组中有效元素的后继位置
    T *_end;  // 指向数组空间的后继位置
    Alloc _allocator;  // 定义容器的空间配置器对象
    
    // 容器的二倍扩容
    void expand() {
        int size = _end - _first;
        // T *ptmp = new T[2 * size];
        
        T *ptmp = _allocator.allocate(2 * size);
        
        for (int i = 0; i < size; ++i) {
            // ptmp[i] = _first[i];
            _allocator.construct(ptmp + i, _first[i]);
        }
        
        // delete []_first;
        
        for (T *p = _first; p != _last; ++p) {
            _allocator.destroy(p);  
        }
        _allocator.deallocate(_first);
        
        _first = ptmp;
        _last = _first + size;
        _end = _first + 2 * size;
        
    }
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

/*
运行结果：
Test()
Test()
Test()
-------------
Test(const Test&)
Test(const Test&)
Test(const Test&)
-------------
~Test()
-------------
~Test()
~Test()
~Test()
~Test()
~Test()
*/
```