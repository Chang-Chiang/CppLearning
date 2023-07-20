#include <iostream>

using std::cout;
using std::endl;

class Base {
public:
    Base(int data) : ma(data) { cout << "Base()" << endl; }

    // 虚析构函数
    virtual ~Base() { cout << "~Base()" << endl; }

    virtual void show() { cout << "Base::show()" << endl; }

protected:
    int ma;
};

class Derive : public Base {
public:
    Derive(int data) : Base(data), mb(data) { cout << "Derive()" << endl; }

    // 基类的析构函数是 virtual 虚函数, 那么派生类的析构函数自动成为虚函数
    ~Derive() { cout << "~Derive()" << endl; }

    void show() { cout << "Derive::show()" << endl; }

private:
    int mb;
};

int main() {
    Base   b(10);
    Derive d(10);

    // 静态绑定, 用对象本身调用虚函数, 是静态绑定
    b.show(); // 虚函数 call Base::show()
    d.show(); // 虚函数 call Base::show()

    // 动态绑定, 必须由指针 (引用) 调用虚函数, 才会发生动态绑定
    Base* pb1 = &b;
    pb1->show();
    Base* pb2 = &d;
    pb2->show();
    // mov ecx, dword ptr [pb2]
    // mov eax, dword ptr [edx]
    // call eax

    // 动态绑定
    Base& rb1 = b;
    rb1.show();
    Base& rb2 = d;
    rb2.show();

    // 动态绑定
    Derive* pd1 = &d;
    pd1->show();
    Derive& rd1 = d;
    rd1.show();

    Derive* pd2 = (Derive*)&b; //
    pd2->show(); // 动态绑定, 但访问的是基类的虚函数表 vftable Base::show()

    return 0;
}
