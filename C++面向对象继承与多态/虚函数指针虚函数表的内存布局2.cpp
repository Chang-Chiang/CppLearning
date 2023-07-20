#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

class Base {
public:
    Base() {
        /*
        左括号到第一行代码之间的指令：
        push ebp
        mov ebp, esp
        sub esp, 4Ch
        rep stos esp<->ebp 0xCCCCCCCC (windows VS 编译器下)

        vfptr <- &Base::vftable  # 栈帧准备好后, 源代码运行前, 将 vftable 写入 vfptr
        */
        cout << "call Base()" << endl;
        clear();
    }

    void clear() { memset(this, 0, sizeof(*this)); }

    virtual void show() { cout << "call Base::show()" << endl; }
};

class Derive : public Base {
public:
    Derive() {
        /*
        左括号到第一行代码之间的指令：
        push ebp
        mov ebp, esp
        sub esp, 4Ch
        rep stos esp<->ebp 0xCCCCCCCC (windows VS 编译器下)

        vfptr <- &Derive::vftable  # 栈帧准备好后, 源代码运行前, 将 vftable 写入 vfptr
        */
        cout << "Call Derive()" << endl;
    }

    void show() { cout << "call Derive::show()" << endl; }
};

int main() {

    // /******* 运行出错部分 *******/
    // Base* pb1 = new Base(); // 构造后, vfptr 被置0
    // /*
    // mov eax, dword ptr[p]
    // mov ecx, dword ptr[eax]  # eax 为 0x00000000 地址, 不是 Base::vftable
    // call ecx
    // */
    // pb1->show(); // 动态绑定, 0 地址不能读不能写, 调用出错
    // delete pb1;
    // /******* 运行出错部分 *******/

    // ---------------------

    Base* pb2 = new Derive(); //
    /*
    先调用基类构造(基类 vftable 地址写入 vfptr), clear() 后vfptr 清 0
    再调用派生类构造(派生类 vftable 地址写入 vfptr), vfptr 有效
    vfptr 里面存储的是 vftable 的地址
    */
    pb2->show(); // 动态绑定
    delete pb2;
    /* 正常运行 */

    return 0;
}
