#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

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
