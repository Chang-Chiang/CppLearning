#include <iostream>
using namespace std;

// 解决方案: C++ 函数对象的版本实现
template <typename T>
class mygreater {
public:
    bool operator()(T a, T b) { // 两个参数, 叫做 二元函数对象
        return a > b;
    }
};

template <typename T>
class myless {
public:
    bool operator()(T a, T b) { // 二元函数对象
        return a < b;
    }
};

// 库函数模板
template <typename T, typename Compare>
bool compare(T a, T b, Compare comp) {
    return comp(a, b); // 编译过程明确调用的是什么函数, 可以内联, 效率高
}

int main() {
    cout << compare(10, 20, mygreater<int>()) << endl; // 传入的是对象
    cout << compare(10, 20, myless<int>()) << endl;

    return 0;
}
