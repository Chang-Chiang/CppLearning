#include <iostream>

using std::cout;
using std::endl;

class Base {
public:
    virtual void show() { cout << "call Base:: show" << endl; }
};

class Derive : public Base {
private:
    void show() { cout << "call Derive:: show" << endl; }
};

int main() {

    Base* p = new Derive();

    // 成员方法能不能调用 (方法的访问权限是不是 public) 是在编译阶段就需要确定好的
    // 编译阶段只能看到 Base::show 是 public
    p->show(); // "call Derive:: show"
    // 最终能调用到 Derive::show 是在运行时期才确定的

    delete p;

    return 0;
}
