/*
lamda 表达式只能使用在语句中,
如果想跨语句使用之前定义好的 lamda 表达式 怎么办
用什么类型表示 lamda 表达式？
lamda 表达式是函数对象, 用 function 类型表示函数对象类型
*/
#include <functional>
#include <iostream>
#include <map>

using std::cout;
using std::endl;
using std::function;
using std::map;

int main() {

    map<int, function<int(int, int)>> caculateMap;
    caculateMap[1] = [](int a, int b) -> int { return a + b; };
    caculateMap[2] = [](int a, int b) -> int { return a - b; };
    caculateMap[3] = [](int a, int b) -> int { return a * b; };
    caculateMap[4] = [](int a, int b) -> int { return a / b; };

    cout << "10 + 15: " << caculateMap[1](10, 15) << endl;
    return 0;
}
