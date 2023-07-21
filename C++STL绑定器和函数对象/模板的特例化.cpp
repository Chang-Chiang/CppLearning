#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

template <typename T>
bool compare(T a, T b) {
    cout << "template compare" << endl;
    return a > b;
}

// 字符串比较特例化, 完全特例化
template <>
bool compare<const char*>(const char* a, const char* b) {
    cout << "compare<const char*>" << endl;
    return strcmp(a, b) > 0;
}

int main() {
    compare(10, 20);       // 模板实参推演, 通过实参类型推断形参 T 为 int
    compare("aaa", "bbb"); // 对两个字符串做大于比较, 实际比较的是地址, 需做特例化
    return 0;
}
