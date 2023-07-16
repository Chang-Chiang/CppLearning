#include <iostream>

using std::cout;
using std::endl;

class Test {
public:
    void        func() { cout << "call Test::func" << endl; }
    static void static_func() { cout << "Test::ststic_func" << endl; }

    int        ma;
    static int mb;
};

int Test::mb;

int main() {
    Test  t1;
    Test* t2 = new Test();

    // ----------------------------------------

    // 指向成员变量的指针
    // int *p = &Test::ma; // 普通指针指向成员变量报错
    // 无法从 "int Test::*" 转换为 "int *"
    int Test::*p = &Test::ma;
    t1.*p = 20;  // 一定要用对象，ma 依赖对象
    cout << t1.*p << endl;

    t2->*p = 30;
    cout << t2->*p << endl;

    int* p1 = &Test::mb; // 普通指针，mb 不依赖对象
    *p1 = 40;
    cout << *p1 << endl;

    delete t2;

    // ----------------------------------------

    // 指向成员方法的指针
    // void (*pfunc)() = &Test::func;
    // 无法从 "void (_thiscall Test::* )(void)" 转换为 "void (__cdecl *)(void)"
    void (Test::*pfunc)() = &Test::func;
    (t1.*pfunc)(); // 普通成员方法调用依赖对象
    (t2->*pfunc)();

    // 如何定义函数指针指向类的 static 成员方法？
    // 定义一个普通的函数指针即可，static 成员方法不依赖对象

    delete t2;

    // ----------------------------------------

    return 0;
}
