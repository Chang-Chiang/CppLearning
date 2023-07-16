#include <iostream>
#include <typeinfo> // typeid

int main() {

    int        a = 10;
    const int* p = &a;
    // int*       q = p;                            // 禁止 int* <- const int*
    std::cout << typeid(p).name() << std::endl;    // int const *

    int*       p1 = nullptr;                     // 不要用 NULL, NULL 就是 0
    int* const p2 = nullptr;
    std::cout << typeid(p1).name() << std::endl; // int *
    std::cout << typeid(p2).name() << std::endl; // int *
    // const 如果右边没有指针 * 的话, const 是不参与类型的

    // 普通变量、指针
    int        b = 20;
    int*       q1 = &b;
    const int* q2 = &a; // 允许 const int * <- int*
    int* const q3 = &a; // 允许 int * <- int*
    int*       q4 = q3; // 允许 int * <- int*

    // 常量、常量指针
    // const int  a = 10;
    // const int* p = &a;

    // int *p = &a;  // error

    return 0;
}
