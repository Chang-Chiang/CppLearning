#include <iostream>
#include <typeinfo>

using std::cout;
using std::endl;

// T 包含了所有的大的类型
template <typename T>
void func(T a) {
    cout << typeid(T).name() << endl;
}

template <typename R, typename A1, typename A2>
void func2(R (*a)(A1, A2)) {
    cout << typeid(R).name() << endl;
    cout << typeid(A1).name() << endl;
    cout << typeid(A2).name() << endl;
}

template <typename R, typename T, typename A1, typename A2>
void func3(R (T::*a)(A1, A2)) {
    cout << typeid(R).name() << endl;
    cout << typeid(T).name() << endl;
    cout << typeid(A1).name() << endl;
    cout << typeid(A2).name() << endl;
}

int sum(int a, int b) { return a + b; }

class Test {
public:
    int sum(int a, int b) { return a + b; }
};

int main() {
    func(10);          // int
    func("aaa");       // char const *
    func(sum);         // int (__cdecl*)(int, int) 函数指针类型
    func2(sum);        // int, int, int
    func(&Test::sum);  // int (__thiscall Test::*)(int, int) 指向成员方法的函数指针类型
    func3(&Test::sum); // int, class Test, int. int
    return 0;
}
