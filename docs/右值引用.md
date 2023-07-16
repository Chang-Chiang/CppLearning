# 右值引用

## [问题：临时对象大开销](../右值引用/字符串类设计.cpp)

```c++
#include <string.h>

#include <iostream>
using namespace std;

// 自己实现字符串对象
class CMyString {
public:
    // 构造函数
    CMyString(const char* p = nullptr) {
        cout << "CMyString(const char*)" << endl;
        if (p != nullptr) {
            _pstr = new char[strlen(p) + 1];
            strcpy(_pstr, p);
        }
        else {
            _pstr = new char[1];
            *_pstr = '\0';
        }
    }

    // 拷贝构造
    CMyString(const CMyString& str) {
        cout << "CMyString(const CMyString&)" << endl;
        _pstr = new char[strlen(str._pstr) + 1];
        strcpy(_pstr, str._pstr);
    }

    // 析构函数
    ~CMyString() {
        cout << "~CMyString()" << endl;
        delete[] _pstr;
        _pstr = nullptr;
    }

    // = 赋值运算符重载
    CMyString& operator=(const CMyString& str) {
        cout << "operator=(const CMyString&)" << endl;
        if (this == &str) {
            return *this;
        }

        delete[] _pstr;

        _pstr = new char[strlen(str._pstr) + 1];
        strcpy(_pstr, str._pstr);

        return *this;
    }

    const char* c_str() const { return _pstr; }

private:
    char* _pstr;
};

CMyString GetString(CMyString& str) {
    const char* pstr = str.c_str();
    CMyString   tmpStr(pstr);
    return tmpStr;
}

int main() {
    CMyString str1("aaaaaaaaaa"); // 【1】str1 普通构造
    CMyString str2;               // 【2】str2 普通构造

    /*
    CMyString GetString(CMyString &str) {
        const char* pstr = str.c_str();
        CMyString tmpStr(pstr);       //【3】tmpStr 普通构造
        return tmpStr;                //【4】tmpStr 拷贝构造 main栈帧临时对象(开辟空间 + 复制) -> !!!开销大
    }                                 //【5】出函数作用域 tmpStr 析构
    */
    str2 = GetString(str1); // 【6】main栈帧临时对象 赋值 str2
                            // (str2原先空间释放 + 按临时对象开辟空间 + 复制) -> !!!开销大
                            // 【7】出语句 main栈帧临时对象 析构
    cout << str2.c_str() << endl;

    return 0;
}
// 【8】str2 析构
// 【9】str1 析构
```

## [左值与右值](../右值引用/左值与右值.cpp)

+ 左值：有名字、有内存
+ 右值：没名字（临时量）、没内存

```c++
int main() {
    int  a = 10;
    int& b = a; // 左值: 有名字, 有内存
    // int &&C = a;  // 错误, 无法将左值绑定到右值引用
    // int &c = 20;  // 错误, 无法将右值绑定到左值引用

    const int& c = 20;
    /**
     * 上面语句实际做的事情：先生成临时量，然后用常引用引用临时量
     * int tmp = 20;
     * const int &c = tmp;
     */

    int&& d = 20; // 可以把一个右值绑定到一个右值引用上
    /**
     * int tmp = 20;
     * int &d = tmp;
     */

    // 临时量为右值，得用右值引用或常引用
    // CMyString&& e = CMyString("aaa");
    // const CMyString &e = CMyString("aaa");

    int& f = d; // 一个右值引用变量, 本身是一个左值，得用左值引用去引用

    return 0;
}
```

## [带右值引用参数的拷贝构造和赋值运算符重载函数](../右值引用/带右值引用参数的拷贝构造和赋值运算符重载函数.cpp)

```c++
#include <cstring>
#include <iostream>
using namespace std;

// 自己实现字符串对象
class CMyString {
public:
    // 构造函数
    CMyString(const char* p = nullptr) {
        cout << "CMyString(const char*)" << endl;
        if (p != nullptr) {
            _pstr = new char[strlen(p) + 1];
            strcpy(_pstr, p);
        }
        else {
            _pstr = new char[1];
            *_pstr = '\0';
        }
    }

    // 析构函数
    ~CMyString() {
        cout << "~CMyString()" << endl;
        delete[] _pstr;
        _pstr = nullptr;
    }

    // 带左值引用参数的拷贝构造
    CMyString(const CMyString& str) {
        cout << "CMyString(const CMyString&)" << endl;
        _pstr = new char[strlen(str._pstr) + 1];
        strcpy(_pstr, str._pstr);
    }

    // 带右值引用参数的拷贝构造
    // str 引用的就是一个临时对象
    CMyString(CMyString&& str) {
        cout << "CMyString(const CMyString&&)" << endl;
        _pstr = str._pstr;
        str._pstr = nullptr;
    }

    // 带左值引用参数的 = 赋值运算符重载
    CMyString& operator=(const CMyString& str) {
        cout << "operator=(const CMyString&)" << endl;
        if (this == &str) {
            return *this;
        }

        delete[] _pstr;

        _pstr = new char[strlen(str._pstr) + 1];
        strcpy(_pstr, str._pstr);

        return *this;
    }

    // 带右值引用参数的 = 赋值运算符重载
    // str 引用的就是一个临时对象
    CMyString& operator=(CMyString&& str) { 
        cout << "operator=(const CMyString&&)" << endl;
        if (this == &str) {
            return *this;
        }

        delete[] _pstr;

        _pstr = str._pstr;
        str._pstr = nullptr;

        return *this;
    }

    const char* c_str() const { return _pstr; }

private:
    char* _pstr;
};

CMyString GetString(CMyString& str) {
    const char* pstr = str.c_str();
    CMyString   tmpStr(pstr);
    return tmpStr;
}

int main() {
    CMyString str1("aaaaaaaaaa"); // 【1】str1 普通构造
    CMyString str2;               // 【2】str2 普通构造

    /*
    CMyString GetString(CMyString &str) {
        cosnt char* pstr = str.c_str();
        CMyString tmpStr(pstr);       //【3】tmpStr 普通构造
        return tmpStr;                //【4】tmpStr 【带右值引用参数】拷贝构造 main栈帧临时对象
    }                                 //【5】出函数作用域 tmpStr 析构
    */
    str2 = GetString(str1); // 【6】main栈帧临时对象 【带右值引用参数】赋值 str2
                            // 【7】出语句 main栈帧临时对象 析构
    cout << str2.c_str() << endl;

    return 0;
}
// 【8】str2 析构
// 【9】str1 析构

/************************************************************/
int main() {
    CMyString str1 = "aaa"; // 【1】str1 普通构造

    vector<CMyString> vec;
    vec.reserve(10);

    // 【2】str1 在底层数组 拷贝构造 产生一个新对象
    vec.push_back(str1);

    // 【3】普通构造 临时对象
    // 【4】临时对象 在底层数组 带右值引用参数的拷贝构造 产生一个新对象
    vec.push_back(CMyString("bbb"));
    //【5】临时对象 析构

    return 0;
}
// 【6】析构
// 【7】析构
// 【8】析构
```

## vector 底层是如何添加对象的

> + vec.push_back 传入对象（左值）时，在底层数组拷贝构造产生一个新对象
>
> + vec.push_back 传入临时对象（右值）时，在底层数组，调用带右值引用参数的拷贝构造产生一个新对象
>
> :question:是如何实现的

### 直观想法：将形参设置为右值引用

:bug:一个右值引用变量本身还是一个左值，分配器构造时匹配的还是接收左值的函数

```c++
template<typename T>
class Allocator {
public:
    ···
    void construct(T *p, const T &val) {  // 接收左值
        new (p) T(val);  
    }

    void construct(T *p, const T &&val) {  // 接收右值
        new (p) T(val);  
    }
};

template<typename T>
class vector {
public:
    ···
    void push_back(const T &val) {  // 接收左值
        if (full())
            expand();
        _allocator.construct(_last, val);
        _last++;
    }
    void push_back(const T &&val) {  // 接收右值, 一个右值引用变量本身还是一个左值
        if (full())
            expand();
        _allocator.construct(_last, val);  // 匹配的还是接收左值的 construct()
        _last++;
    }
    ···
private:
    ···
};

int main() {
    CMyString str1 = "aaa";           
    vector<CMyString> vec;

    vec.push_back(str1);              
    vec.push_back(CMyString("bbb"));  // 使用自定义 vector 不能匹配到右值引用的构造
    return 0;
}       
```

### std::move 移动语义：将变量强转成右值引用类型

```c++
template<typename T>
class Allocator {
public:
    ···
    void construct(T *p, const T &val) {  // 接收左值
        new (p) T(val);  
    }

    void construct(T *p, const T &&val) {  // 接收右值
        new (p) T(std::move(val));  // val 强转成右值引用类型 
    }
};

template<typename T>
class vector {
public:
    ···
    void push_back(const T &val) {  // 接收左值
        if (full())
            expand();
        _allocator.construct(_last, val);
        _last++;
    }
    void push_back(const T &&val) {  // 接收右值, 一个右值引用变量本身还是一个左值
        if (full())
            expand();
        _allocator.construct(_last, std::move(val));  // val 强转成右值引用类型
        _last++;
    }
    ···
private:
    ···
};

int main() {
    CMyString str1 = "aaa";           
    vector<CMyString> vec;

    vec.push_back(str1);              
    vec.push_back(CMyString("bbb"));  // 匹配到右值引用的构造
    return 0;
}   
```

## std::forward 完美转发，模板实现

```c++
template<typename T>
class Allocator {
public:
    ···
	template<typename Ty>
    void construct(T *p, Ty &&val) { 
        new (p) T(std::forward<Ty>(val));  
    }
};

template<typename T>
class vector {
public:
    ···
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
    ···
private:
    ···
};
```

## 参考

+ [C++11 - 右值引用_大秦坑王的博客-CSDN博客](https://blog.csdn.net/QIANGWEIYUAN/article/details/88653747)
