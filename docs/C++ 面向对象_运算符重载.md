# C++ 运算符重载

意义：使对象的运算表现得和编译器内置类型一样

##  Complex 复数类

+ `comp + int` 成员运算符重载方法
+ `int + comp` 全局运算符重载方法
+ 前置 `++`、后置 `++`
+ 输入输出运算符重载

```c++
// template<typename T>

class CComplex {
public:
    
    // 构造, 可以构造三种不同的复数对象 CComplex() CComplex(20) CComplex(30, 30)
    CComplex(int r = 0, int i = 0):mreal(r), mimage(i) {}
    
    /* 定义了全局的, 屏蔽局部的
    // 加法运算符的重载函数, 指导编译器怎么做 CComplex 类对象的加法操作
    CComplex operator+(const CComplex &src) {
       
        // 局部对象, 函数运行完, 栈帧回退, 对象内存交还给系统
        // 不能返回局部对象的指针或引用
        // CComplex comp;   
        // comp.mreal = this->mreal + src.mreal;
        // comp.mimage = this->mimage + src.mimage;
        // return comp;
        
        return CComplex(this->mreal + src.mreal, this->mimage + src.mimage);
    }
    */
    
    // 后置++，返回的旧值
    CComplex operator++(int) {
        // CComplex comp = *this;
        // mreal += 1;
        // mimage += 1;
        // return comp;  // 局部对象
        
        return CComplex(mreal++, mimage++);
    }
    
    // 前置++，返回对象本身
    CComplex& operator++() {
        mreal += 1;
        mimage += 1;
        return *this;
    }
    
    void operator+=(const CComplex &src) {
        mreal += src.mreal;
        mimage += src.mimage;
    }
    
    void show() {
        cout << "real: " << mreal << "image: " << mimage << endl;
    }
    
private:
    int mreal;
    int mimage;
    
    // 定义为友元函数, 可以访问对象私有成员变量
    friend CComplex operatot+(const CComplex &lhs, const CComplex &rhs);
    friend ostream& operator<<(ostream &out, const CComplex &src);
    friend istream& operator>>(istream &in, CComplex &src);

};  // 类定义结束

// 定义在类外的函数, 无法访问对象的私有成员变量, 故在类中声明为友元 friend 函数
CComplex operatot+(const CComplex &lhs, const CComplex &rhs) {
    return CComplex(lhs.mreal + rhs.mreal, lhs.mimage + rhs.mimage);
}

ostream& operator<<(ostream &out, const CComplex &src) {
    out <<　"mreal: " << src.mreal << "mimage: " << src.mimage << endl;
    return out;
}

istream& operator>>(istream &in, CComplex &src) {
    in >> src.mreal >> src.mimage;
    return in;
}	



int main() {
    CComplex comp1(10, 10);
    CComplex comp2(20, 20);
    
    // comp1.operator+(comp2)  加法运算符的重载函数
    CComplex comp3 = comp1 + comp2;
    comp3.show();
    
    CComplex comp4 = comp1 + 20;  // comp1.operator+(20)  int -> CComplex(int)
    comp4.show();
    
    // 编译器做对象运算的时候, 会调用对象的运算符重载函数 (优先调用成员方法) 
    // 如果没有成员方法, 就在全局作用域找合适的运算符重载函数
    // ::operator+(30, comp1)
    CComplex comp5 = 30 + comp1;  
    comp5.show();
    
    // operator++()  不带参数, 表示 前置++
    // operator++(int)  带 int 参数, 表示 后置++
    comp5 = comp1++;  // ++ -- 单目运算符
    comp1.show();
    comp5.show();
    
    comp5 = ++comp1;
    comp1.show();
    comp5.show();
    
    comp1 += comp2;
    comp1.show();
    cout << comp1 << endl;  // 输出运算符重载  
    
    cin >> comp1 >> comp2;
    cout << compq << comp2 << endl;
     
    return 0;
}
```

## STL string 类支持的操作

```c++
// stl 库的 string 类
#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;


int main() {
    
    // 字符串构造
    string str1;  // 默认构造
    // 也有提供 string(const char*) 构造
    string str2 = "aaa";  // 拷贝构造
    string str3 = "bbb";
    
    // 字符串拼接, 全局的操作符重载 +
    string str4 = str2 + str3;  
    string str5 = str2 + "ccc";
    string str6 = "ddd" + str2;
    
    // 字符串输出
    cout << "str6: " << str6 << endl;
    
    // 字符串比较
    if (str5 > str6) {
        cout << str5 << ">" << str6 << endl;
    }
    else {
        cout << str5 << "<" << str6 << endl;
    }
    
    // 字符串长度方法
    int len = str.length();
    
    // 字符串取下标, 运算符重载 []
    for (int i = 6; i < len; ++i) {
        cout << str6[i] << " ";
    }
    cout << endl;
    
    // 字符串 转 char*, string -> char*
    char buf[1024] = { 0 };  // C 风格字符串数组
    strcpy(buf, str6.c_str());  // 非标准库 str6.c_str() 返回成 C语言的 char*
    cout << "buf: " << buf << endl;
    
    return 0;
}
```

## 仿 STL 实现 String 类

+ 需求分析
  + 普通构造函数
  + 拷贝构造函数
  + 字符串长度计算
  + 操作符重载, 字符串拼接 +
  + 操作符重载, 字符串赋值 = 
  + 操作符重载, 字符串大小比较 >, <
  + 操作符重载, 取下标 []

+ 仿 STL 实现 string 类

```cpp
// 自己实现字符串对象
class String {
public:
    
    // 构造函数
    String(const char *p = nullptr) {
        if (p != nullptr){
            _pstr = new char[strlen(p) + 1];
            strcpy(_pstr, p);
        }
        else {
            // _pstr = nullptr;  // 不要这么做, 否则后面每次都需要判断是否为空
            _pstr = new char[1]; 
            *_pstr = '\0';
        }
    }
    
    // 拷贝构造
    String(const String &str) {
        _pstr = new char[strlen(str._pstr) + 1];
        strcpy(_pstr, str._pstr);
    }
    
    // 析构函数
    ~String() {
        delete []_pstr;
        _pstr = nullptr;
    }
    
    // = 赋值运算符重载
    String& operator=(const String &str) {
        if (this == &src)
            return *this;
        
        delete []_pstr;
        
        _pstr = new char[strlen(str._pstr) + 1];
        strcpy(_pstr, str._pstr);
        
        return *this;
    }
    
    // > < == 比较运算符重载
    // 只涉及读不涉及写的方法最好写为常方法, 普通对象、常对象都可以调用
    bool operator>(const String &str)const {
        return strcmp(_pstr, str._pstr) > 0;
    }
    bool operator<(const String &str)const {
        return strcmp(_pstr, str._pstr) < 0;
    }
    bool operator==(const String &str)const {
        return strcmp(_pstr, str._pstr) == 0;
    }
    
    // 字符串有效字符个数
    int length(cosnt) { return strlen(_pstr); }
    
    // [] 取下标运算符重载
    // char ch = str6[6]; str6[6] = '7';
    char& operator[](int index) { return _pstr[index]; }
    // char ch = str6[6]; 返回常对象, 不允许修改 str6[6] = '7';
    const char& operator[](int index)const { return _pstr[index]; }
    
    // 返回 C 类型字符串
    const char* c_str()const { return _pstr; }
    
private:
    char *_pstr;  //成员变量应该是一个指针
    friend String operator+(const String &lhs, const String &rhs);
    friend ostream& operator<<(ostream & out, const String &str);
};

// 全局的重载函数, +
String operator+(const String &lhs, const String &rhs) {
    
    // 效率很低, 两次构造析构
    // char *ptmp = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
    // strcpy(ptmp, lhs.pstr);
    // strcat(ptmp, rhs._pstr);
    // String tmp(ptmp);  
    // delete []ptmp;
    // return tmp;
    
    // 一次 new delete
    String tmp;
    tmp._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1]
    strcpy(tmp, lhs.pstr);
    strcat(tmp, rhs._pstr);
    return tmp;   
}

// << 输出运算符重载   
ostream& operator<<(ostream & out, const String &str) {
    out << str._pstr;
    return out;
}   
```

## string 类的容器迭代器

+ 迭代器功能：提供一种统一的方式, 透明的 (无需知道底层数据结构) 访问容器内部的元素的值
+ 泛型算法参数接收的都是迭代器
+ 泛型算法, 是全局的函数, 给所有容器用
+ 故, 泛型算法有一套方式, 能够统一的遍历所有容器的元素 -> 迭代器

```c++
#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main() {
    string str1 = "hello world";  
    // str1 叫容器吗？叫容器, 底层放了一组 char 类型字符
    
    // 容器迭代器类型
    // string::iterator it = str1.begin();
    auto it = str1.begin();
    for (; it != str1.end(); ++it) {  
    // it != str1.end(), 不考虑底层是数组或是链表实现
        cout << *it << " ";
    }
    cout << endl;
    
    // C++11 foreach 的方式来遍历容器内部元素的值
    // 底层通过迭代器遍历
    for (char ch : str1) {
        cout << ch << " ";
    }
    cout << endl;
    
    return 0;
}
```

+ 自定义 String 类的迭代器
  + 构造函数
  + != 不等于运算符重载
  + 前置 ++ 运算符重载
  + `*` 解引用运算符重载
  + 成员变量取决于容器底层数据结构

```c++
// 自己实现字符串对象
class String {
public:
    ···
    // 嵌套类, String 字符串类型提供迭代器实现    
    class iterator {
    public:
        iterator(char *p = nullptr) :_p(p){}
        bool operator!=(const iterator &it) {
            return _p != it._p;
        }
        void operator++() { ++_p; }
        char& operator*() { return *_p; }
    private:  
        char *_p;
    };
    
    // 返回容器底层首元素的迭代器的表示
    iterator begin() { return iterator(_pstr); }
    // 返回容器末尾元素后继位置的迭代器的表示
    iterator end() { return iterator(_pstr + length()); }
private:
    ···
};
```

## vector 容器的迭代器实现

+ vector 容器自定义实现

```c++
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
```

+ vector 迭代器实现

```c++
template<typename T, typename Alloc = Allocator<T>>  
class vector {
public:
    ···
    
    T& operator[](int index) { 
        if (index < 0 || index >= size()) {
            throw "OutOfRangeException";
        }
        return _first[index];
    }
    
    // 迭代器一般实现成容器的嵌套类型
    class iterator {
    public:  
        iterator(T *ptr = nullptr) :_ptr(ptr) {}
        bool operator!=(const iterator &it)const {
            return _ptr != it._ptr;
        }
        void operator++() { _ptr++; }
        T& operator*() { return *_ptr; }  
        // int data = *it; *it = 20; 解引用读取、赋值
        // 所以不设为常方法
        
        const T& operator*()const { return *_ptr; }  // 不能解引用后赋值
        
    private:
        T *_ptr;
    };
    
    iterator begin() { return iterator(_first); }
    iterator end() { return iterator(_last); }
    
private:
    T *_first;  // 指向数组起始位置
    T *_last;  // 指向数组中有效元素的后继位置
    T *_end;  // 指向数组空间的后继位置
    Alloc _allocator;  // 定义容器的空间配置器对象
};

int main() {
    vector<int> vec;
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100 + 1);
    }
    
    // [] 下标访问并不适用于所有容器
    int size = vec.size();
    for (int i = 0; i < size; ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;
    
    // 迭代器遍历适用于所有容器
    // vector<int>::iterator it = vec.begin();
    auto it = vec.begin();
    for (; it ! = vec.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
    
    // 底层原理, 就是通过容器的迭代器来实现容器遍历
    for (int val : vec) {
        cout << val << " ";
    }
    cout << endl;
    
    return 0;
}
```

## 迭代器的失效——问题

+ 迭代器为什么会失效

  + 当容器调用 erase 方法后, 当前位置到容器末尾元素的所有迭代器全部失效 

  + 当容器调用 insert 方法后, 当前位置到容器末尾元素的所有迭代器全部失效 , 首元素

    首元素 -> 插入点/删除点 -> 末尾元素：首元素到插入点, 迭代器依然有效; 插入点到末尾元素, 迭代器全部失效

  + 对 insert 来说, 如果引起容器内存扩容, 原来容器的所有迭代器全部失效

  + 不同容器的迭代器是不能进行比较运算的

+ 迭代器失效后, 问题怎么解决

  + 对插入/删除点的迭代器进行更新操作

```c++
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

int main() {
    vector<int> vec;
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100 + 1);
    }
    
    for (int v : vec) {
        cout <<  v << endl;
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
    auto it = vec.begin();
    for (; it != vec.end(); ++it) {
        if (*it % 2 == 0) {
            
            // 迭代器在第一次 insert 后, iterator 就失效了
            vec.insert(it, *it - 1);  
			// break;  // 只添加一次
        }
    }
    
    /*---- 迭代器失效部分 ----*/
    
    
    /*----迭代器失效解决方法部分----*/
    auto it = vec.begin();
    while (it != vec.end()) {
        if (*it % 2 == 0) {
            it = vec.erase(it);  // 迭代器更新   
        }
        else {
            ++it;
        }
    }
    
    for (int v : vec) {
        cout <<  v << endl;
    }
    cout << endl;
    
    auto it = vec.begin();
    for (; it != vec.end(); ++it) {
        if (*it % 2 == 0) {
            it = vec.insert(it, *it - 1); 
            ++it;
        }
    }
    
    /*----迭代器失效解决方法部分----*/
    
    return 0;    
}
```

## 迭代器失效——原理

```c++
template<typename T, typename Alloc = Allocator<T>>  
class vector {
public:
    ···
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
        
        // erase(it);  里做的是 verify(it._ptr, _last);
    	// insert(it, val);  里做的是 verify(it._ptr, _last);
        verify()
        --_last;
        _allocator.destroy(_last)
    }
         
    ···
    T& operator[](int index) { 
        if (index < 0 || index >= size()) {
            throw "OutOfRangeException";
        }
        return _first[index];
    }
    
    // 迭代器一般实现成容器的嵌套类型
    class iterator {
    public:  
        
        friend class vector<T, Alloc>;
        
        iterator(vector<T, Alloc> *pvec, T *ptr = nullptr) 
            :_ptr(ptr), _pVec(pvec) {   
        	Iterator_Base *itb = new Iterator_Base(this, pVec->_head._next);
            _pVec->_head._next = itb;   
        }
        
        bool operator!=(const iterator &it)const {
            
            // 检查迭代器的有效性
            if (_pVec == nullptr || _pVec != ut._pVec) {
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
        // int data = *it; *it = 20; 解引用读取、赋值
        // 所以不设为常方法
        
        const T& operator*()const { 
            
            // 检查迭代器的有效性
            if (_pVec == nullptr) {
                throw "iterator invalid!";
            }
            return *_ptr; 
        }  // 不能解引用后赋值
        
    private:
        T *_ptr;
        
        // 当前迭代器是哪个容器对象
        vector<T, Alloc> * _pVec;
    };
    
    iterator begin() { return iterator(this, _first); }
    iterator end() { return iterator(this, _last); }
    
    void verify(T *first, T *last) {
        Iterator_Base *pre = &this->head;
        Iterator_Base *it = this->head._next;
        
        while (it != nullptr) {
            if (it->_cur->_ptr >= first && it->_cur->_ptr <= last) {
                // 迭代器失效, 把 iterator 持有的容器指针置 nullptr
                it->_cur->_pVec = nullptr;
                
                // 删除当前迭代器节点, 继续判断后面的迭代器节点是否失效
                pre->_next = ut->_next;
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
    T *_first;  // 指向数组起始位置
    T *_last;  // 指向数组中有效元素的后继位置
    T *_end;  // 指向数组空间的后继位置
    Alloc _allocator;  // 定义容器的空间配置器对象
    
    // 容器迭代器失效增加代码
    struct Iterator_Base {
        Iterator_Base(iterator *c=nullptr, Iterator_Base *n=nullptr)
            :_cur(c), _next(n) {}
        iterator *_cur;
        Iterator_Base *_next;
    }
    Iterator_Base _head;
};


int main() {
    vector<int> vec;
    for (int i = 0; i < 20l ++i) {
        vec.push_back(rand() % 100 + 1);
    }
    
    auto it1 = vec.end;
    // vec.pop_back();  // 执行后, 无法比较
    auto it2 = vec.end();
    
    cout << (it1 != it2) << endl;
    
    return 0;
}
```

## 迭代器失效——实现 vector 的 insert, erase 方法

```c++
template<typename T, typename Alloc = Allocator<T>>  
class vector {
public:
    ···
    // 自定义 vector 容器 insert 方法的简单实现
    // 1. 不考虑扩容
    // 2. 不考虑 it._ptr 的指针合法性
    iterator insert(iterator it, const T &val) {
        verify(it._ptr - 1, _last);  // 若考虑扩容, 则是 verify(_first - 1, _last);
        T *p = _last;
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
        T *p = it._ptr;
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
};


int main() {
    vector<int> vec(200);
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100 + 1);
    }
    
    // 拷贝 5.3 节代码进行测试
}
```

## operator new 和 operator delete

+ malloc 和 new 的区别

  + malloc 按字节开辟内存, new 开辟内存时需要指定类型 例 new int[]。所以 malloc 开辟内存返回的都是 void`*`, operator new -> int`*`
  + malloc 只负责开辟空间, new 不仅有 malloc 的功能, 还可以进行数据的初始化
  + malloc 开辟内存失败返回 nullptr 指针, new 抛出的时 bad_alloc 类型的异常

+ free 和 delete 的区别

  delete: 先调用析构函数, 再 free 释放内存

  当 delete 是一个普通指针, 如 delete (int*)p, 则和 free(p) 没有区别

+ 问题：new 和 delete 能混用吗？C++ 为什么区分单个元素和数组的内存分配和释放呢？

  + new delete 配合使用, new[] delete[] 配合使用
  + 对于普通的编译器内置类型, 可以混用
  + 自定义的类类型, 有析构函数, 为了调用正确的析构函数, 那么开辟对象数组的时候, 会多开辟 4 个字节, 记录对象的个数

```c++
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

// 先调用 opeartor new 开辟空间
// 再调用对象的构造函数 (初始化) 
void* operator new(size_t size) {
    void *p = malloc(size);
    if (p == nullptr)
        throw bad_alloc();
    cout << "operator new addr: " << p << endl;
    return p;
}

// delete p
// 先调用 p 指向对象的析构函数
// 再调用 operator delete 释放内存空间
void operator delete(void *ptr) {
    cout << "operator delete addr: " << ptr << endl;
    free(ptr);
}

void* operator new[](size_t size) {
    void *p = malloc(size);
    if (p == nullptr)
        throw bad_alloc();
    cout << "operator new[] addr: " << p << endl;
    return p;
}

void operator delete[](void *ptr) {
    cout << "operator delete[] addr: " << ptr << endl;
    free(ptr);
}

class Test {
public:
    Test(int data = 10) {cout << "Test()" << endl; } 
    ~Test() {cout << "~Test()" << endl; }
private:
    int ma;
};


int main() {
    try {
        int *p = new int;
    	delete p;
        int *q = new int[10];
    	delete []q;
        
        Test *p1 = new Test();
        delete p1; 
        Test *p2 = new Test[5];
        cout << "p2: " << p2 << endl;
        delete []p2;
    }
    
    catch (const bad_alloc &err) {
        cout << err.what() << endl;
    }
    
    return 0;
}
```

## new 和 delete 重载, 实现对象池 

+ 运算符重载
  + 设计成 成员方法
  + 设计成 全局方法

```c++
template<typename T>
class Queue {
public:
    
    // 构造函数
    Queue() {
        _front = _rear = new QueueItem();
    }
    
    // 析构函数
    ~Queue() {
        QueueItem *cur = _front;
        while (cur != nullptr) {
            _front = _front->_next;
            delete cur;
            cur = _front;
        }
    }
    
    // 入队操作
    void push(const T &val) {
        QueueItem *item = new QueueItem(val);
        rear->_next = item;
        _rear = item;
    }
    
    // 出队
    void pop() {
        if (empty())
            return;
        QueueItem *first = _front->next;
        _front->next = first->next;
        if(_front->next == nullptr) {
            _rear = _front;
        }
        delete first;
    }
    
    // 查看对头
    T front()const {
        return _front->_next->_data;
    }
    
    bool empty()const { return _front == _rear; }
    
private:
    
    // 【问题解决】产生一个 QueueItem 的对象池 (10000 个 QueueItem 节点) 
    struct QueueItem {
        QueueItem(T data = T()) : _data(data), _next(nullptr) {}
        
        // 给 QueueItem 提供自定义内存管理
        void* operator new(size_t size) {
            if (_itemPool == nullptr) {
                _itemPool = (QueueItem*)new char[POOL_ITEM_SIZE * sizeof(QueueItem)];
                QueueItem *p = _itemPool;
                for (; p < _itemPool POOL_ITEM_SIZE - 1; ++p) {
                    p->_next = p + 1;
                }
                p->_next = nullptr;
            }
            QueueItem *p = _itemPool;
            _itemPool = _itemPool->_next;
            return p;
        }
        void operator delete(oid *ptr) {
            QueueItem *p = (QueueItem*)ptr;
            p->_next = _itemPool;
            _itemPool = p;
        }
        T _data;
        QueueItem *_next;
        static QueueItem *_itemPool;
        static const int POOL_ITEM_SIZE = 1000000;
    }
    
    QueueItem *_front;  // 指向头节点
    QueueItem *_rear;  // 指向队尾
};

// 静态成员变量 _itemPool 定义
// typename 告诉编译器, 嵌套作用域下的名字 (QueueItem) 是一个类型
template<typename T>
typename Queue<T>::QueueItem *Queue<T>::QueueItem::itemPool = nullptr;

int main() {
    Queue<int> que;
    
    // 【问题】短时间内大量调用 malloc, free
    for (int i = 0; i < 1000000; ++i) {  
        que.push(i);
        que.pop();
    }
    cout << que.empty() << endl;
    
    return 0;
}
```