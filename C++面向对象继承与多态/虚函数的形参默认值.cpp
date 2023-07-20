#include <iostream>

using std::cout;
using std::endl;

class Base {
public:
    virtual void show(int i = 10) { cout << "call Base:: show i: " << i << endl; }
};

class Derive : public Base {
public:
    void show(int i = 20) { cout << "call Derive:: show i: " << i << endl; }
};

int main() {

    Base* p = new Derive(); // 需要虚析构函数

    /*
    函数的调用过程：
    先参数压栈, 编译阶段编译器只能看到 Base::show(int i = 10)
    push 0Ah  # 函数调用, 参数压栈是在编译时期就确定好的
    mov eax, dword ptr[p]  # 动态绑定是在运行时
    mov ecx, dword ptr[eax]
    call ecx
    */

    p->show(); // 动态绑定 p -> Derive vfptr -> Derive vftable
    // call Derive:: show i: 10

    delete p;

    return 0;
}
