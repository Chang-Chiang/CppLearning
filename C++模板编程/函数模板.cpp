#include <cstring>
#include <iostream>
using std::cout;
using std::endl;
// 函数模板
template <typename T>    // 定义一个模板参数列表
bool compare(T a, T b) { // compare 是一个函数模板, 模板名称
    cout << "template compare" << endl;
    return a > b;
}

// 针对 compare 函数模板, 提供 const char* 类型的特例化版本
template <>
bool compare<const char*>(const char* a, const char* b) {
    cout << "compare<const char*>" << endl;
    return strcmp(a, b) > 0;
}

// 非模板函数, 即普通函数
bool compare(const char* a, const char* b) {
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
    compare<int>(10, 20); // compare<int> 模板名 + 参数列表 才是函数名
    compare<double>(10.5, 20.5);

    compare(20, 30); // 函数模板的实参推演

    compare(30, 40.5); // 错误 -> 定义新的模板参数列表 或者指定参数 compare<int>(30, 40.5);

    /*
    模板的特例化：
    对于某些类型来说, 依赖编译器默认实例化的模板代码, 代码处理逻辑是有错误的
    */
    compare("aaa", "bbb"); // 实参推演为 const char*, 比较的是两个字符串的地址
    // 希望是 strcmp(a, b) > 0;
    // 当存在普通函数时, 优先调用普通函数 (把 compare 处理成函数的名字) , 编译器省去实参推演
    // 没有的话, 才去找 compare 模板

    compare<const char*>("aaa", "bbb"); // 调用模板函数

    return 0;
}
