#include <iostream>

using std::cout;
using std::endl;

template <typename T>
class Vector {
public:
    Vector() { cout << "call Vector template init" << endl; }
};

// 对 char* 类型提供的完全特例化版本
template <>
class Vector<char*> {
public:
    Vector() { cout << "call Vector<char*> init" << endl; }
};

// 对指针类型提供的部分特例化版本, 知道是指针, 但不知道指针的具体类型
template <typename Ty>
class Vector<Ty*> {
public:
    Vector() { cout << "call Vector<Ty*> init" << endl; }
};

// 针对函数指针类型(有一个返回值, 有两个形参变量)提供的部分特例化
template <typename R, typename A1, typename A2>
class Vector<R (*)(A1, A2)> {
public:
    Vector() { cout << "call Vector<R(*)(A1, A2)> init" << endl; }
};

// 针对函数类型(有一个返回值, 有两个形参变量)提供的部分特例化
template <typename R, typename A1, typename A2>
class Vector<R(A1, A2)> {
public:
    Vector() { cout << "call Vector<R(A1, A2)> init" << endl; }
};

int sum(int a, int b) { return a + b; }

int main() {
    Vector<int>               vec1; // call Vector template init
    Vector<char*>             vec2; // call Vector<char*> init
    Vector<int*>              vec3; // call Vector<Ty*> init
    Vector<int (*)(int, int)> vec4; // 函数指针类型 call Vector<R(*)(A1, A2)> init
    Vector<int(int, int)>     vec5; // 函数类型 all Vector<R(A1, A2)> init

    // 注意区分函数类型和函数指针类型
    // 函数指针类型
    typedef int (*PFUNC1)(int, int);
    PFUNC1 pfunc1 = sum;
    cout << pfunc1(10, 20) << endl;

    // 函数类型
    typedef int PFUNC2(int, int);
    PFUNC2*     pfunc2 = sum;
    cout << (*pfunc2)(10, 20) << endl;

    return 0;
}
