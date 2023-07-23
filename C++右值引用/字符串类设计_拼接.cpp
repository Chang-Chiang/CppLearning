#include <cstring>
#include <iostream>
#include <vector>
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
        cout << "CMyString(CMyString&&)" << endl;
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
        cout << "operator=(CMyString&&)" << endl;
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
    char*            _pstr;
    friend CMyString operator+(const CMyString& lhs, const CMyString& rhs);
    friend ostream&  operator<<(ostream& out, const CMyString& str);
};
CMyString operator+(const CMyString& lhs, const CMyString& rhs) {

    /**
     * // 【注】new 没有对应的 delete, 内存泄漏
     * // 为了 new 有对应的 delete，不能直接 return CMyString(tmp); 返回临时对象
     * // 需要先构造临时对象，再 delete[] tmp; 再 return
     * CMyString *tmp = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
     * strcpy(tmp, lhs._pstr);
     * strcat(tmp, rhs._pstr);
     * CMyString tmpStr(tmp);
     * delete[] tmp;
     * return tmpStr;
     * // return CMyString(tmp);
     */

    CMyString tmpStr;
    tmpStr._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
    strcpy(tmpStr._pstr, lhs._pstr);
    strcat(tmpStr._pstr, rhs._pstr);
    return tmpStr;
}
ostream& operator<<(ostream& out, const CMyString& str) {
    out << str._pstr;
    return out;
}

CMyString GetString(CMyString& str) {
    const char* pstr = str.c_str();
    CMyString   tmpStr(pstr);
    return tmpStr;
}

// int main() {
//     CMyString str1 = "hello";
//     CMyString str2 = "world!";
//     CMyString str3 = str1 + str2;
//     /*
//     调用 str1.operator+()
//         + tmpStr 普通构造
//         + tmpStr 带右值引用的拷贝构造给 str3
//         + tmpStr 析构
//     */

//     cout << str3 << endl;
//     return 0;
// }

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
