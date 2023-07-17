// stl 库的 string 类
#include <cstring>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {

    // 字符串构造
    string str1; // 默认构造
    // 也有提供 string(const char*) 构造
    string str2 = "aaa"; // 拷贝构造
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
    int len = str6.length();

    // 字符串取下标, 运算符重载 []
    for (int i = 0; i < len; ++i) {
        cout << str6[i] << " ";
    }
    cout << endl;

    // 字符串 转 char*, string -> char*
    char buf[1024] = {0};      // C 风格字符串数组
    strcpy(buf, str6.c_str()); // 非标准库 str6.c_str() 返回成 C语言的 char*
    cout << "buf: " << buf << endl;

    return 0;
}
