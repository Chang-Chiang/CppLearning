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
    CMyString& operator=(CMyString&& str) { // 临时对象
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
