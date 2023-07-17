#include <cstring>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::ostream;

// 自己实现字符串对象
class String {
public:
    // 构造函数
    String(const char* p = nullptr) {
        if (p != nullptr) {
            // strlen 计算的是 p 指向字符串有效字符的个数
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
    String(const String& str) {
        _pstr = new char[strlen(str._pstr) + 1];
        strcpy(_pstr, str._pstr);
    }

    // 析构函数
    ~String() {
        delete[] _pstr;
        _pstr = nullptr;
    }

    // = 赋值运算符重载
    String& operator=(const String& str) {
        if (this == &str) {
            return *this;
        }

        delete[] _pstr;

        _pstr = new char[strlen(str._pstr) + 1];
        strcpy(_pstr, str._pstr);

        return *this;
    }

    // > < == 比较运算符重载
    // 只涉及读不涉及写的方法最好写为常方法, 普通对象、常对象都可以调用
    bool operator>(const String& str) const { return strcmp(_pstr, str._pstr) > 0; }
    bool operator<(const String& str) const { return strcmp(_pstr, str._pstr) < 0; }
    bool operator==(const String& str) const { return strcmp(_pstr, str._pstr) == 0; }

    // 字符串有效字符个数
    int length() const { return strlen(_pstr); }

    // [] 取下标运算符重载
    // char ch = str6[6]; str6[6] = '7';
    char& operator[](int index) { return _pstr[index]; }

    // [] 取下标运算符重载 常方法
    // char ch = str6[6]; 返回常对象, 不允许修改 str6[6] = '7';
    const char& operator[](int index) const { return _pstr[index]; }

    // 返回 C 类型字符串
    const char* c_str() const { return _pstr; }

public:
    // 嵌套类, String 字符串类型提供迭代器实现
    class iterator {
    public:
        iterator(char* p = nullptr) : _p(p) {}
        bool  operator!=(const iterator& it) { return _p != it._p; }
        void  operator++() { ++_p; }
        char& operator*() { return *_p; }

    private:
        char* _p;
    };

    // 返回容器底层首元素的迭代器的表示
    iterator begin() { return iterator(_pstr); }
    // 返回容器末尾元素后继位置的迭代器的表示
    iterator end() { return iterator(_pstr + length()); }

private:
    char*           _pstr; // 成员变量应该是一个指针
    friend String   operator+(const String& lhs, const String& rhs);
    friend ostream& operator<<(ostream& out, const String& str);
};

// 全局的重载函数, +
String operator+(const String& lhs, const String& rhs) {

    // 效率很低, 两次构造析构
    // char *ptmp = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
    // strcpy(ptmp, lhs._pstr);
    // strcat(ptmp, rhs._pstr);
    // String tmp(ptmp);
    // delete []ptmp;
    // return tmp;

    // 一次 new delete
    String tmp;
    tmp._pstr = new char[strlen(lhs._pstr) + strlen(rhs._pstr) + 1];
    strcpy(tmp._pstr, lhs._pstr);
    strcat(tmp._pstr, rhs._pstr);
    return tmp;
}

// << 输出运算符重载
ostream& operator<<(ostream& out, const String& str) {
    out << str._pstr;
    return out;
}


int main() {
    String str1 = "hello world";
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
