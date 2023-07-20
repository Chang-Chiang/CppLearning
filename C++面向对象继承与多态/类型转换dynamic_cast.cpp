// dynamic_cast : 主要用在继承结构中, 可以支持 RTTI 类型识别的上下转换
#include <iostream>

using std::cout;
using std::endl;

class Base {
public:
    virtual void func() = 0;
};

class Derive1 : public Base {
public:
    void func() { cout << "call Derive1::func" << endl; }
};

class Derive2 : public Base {
public:
    void func() { cout << "call Derive2::func" << endl; }
    // 【新需求】Derive2 实现新功能的 API 接口函数
    void derive02func() { cout << "call Derive2::derive02func" << endl; }
};

void showFunc(Base* p) {

    /*
    p->func();  // 动态绑定
    // 【新需求】对 Derive2 希望调用 derive02func -> 识别 *p 的类型 type(*p).name()
    */

    // dynamic_cast 会检查 p 指针是否指向的是一个 Derive2 类型的对象
    // p->vfptr->vftable  RTTI 信息
    // 如果是, dynamic_cast 转换类型成功, 返回 Derive2 对象的地址给 pd2, 否则返回 nullptr
    Derive2* pd2 = dynamic_cast<Derive2*>(p); // static_cast 则总是强转成功

    /*
    static_cast 编译时期的类型转换
    dynamic_cast 运行时期的类型转换, 支持 RTTI 信息识别
    */

    if (pd2 != nullptr) {
        pd2->derive02func();
    }
    else {
        p->func();
    }
}

int main() {
    Derive1 d1;
    Derive2 d2;
    showFunc(&d1); // call Derive1::func
    showFunc(&d2); // call Derive2::func

    return 0;
}
