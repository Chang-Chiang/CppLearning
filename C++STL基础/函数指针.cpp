#include <iostream>
using namespace std;

// 解决方案: 使用 C 的函数指针
template <typename T>
bool mygreater(T a, T b) {
    return a > b;
}

template <typename T>
bool myless(T a, T b) {
    return a < b;
}

// 库函数模板
template <typename T, typename Compare>
bool compare(T a, T b, Compare comp) {
    // 通过函数指针调用函数, 是没有办法内联的, 效率很低, 因为有函数调用开销
    return comp(a, b);
}

int main() {
    cout << compare(10, 20, mygreater<int>) << endl; // 传入的是函数的地址
    cout << compare(10, 20, myless<int>) << endl;

    return 0;
}
