#include <iostream>
#include <typeinfo>

using std::cout;
using std::endl;

class Base {
public:
    Base(int data = 10) : ma(data) {}
    void show() { cout << "Base::show()" << endl; }
    void show(int) { cout << "Base::show(int)" << endl; }

protected:
    int ma;
};

class Derive : public Base {
public:
    Derive(int data = 20) : Base(data), mb(data) {}
    void show() { cout << "Derive::show()" << endl; }

private:
    int mb;
};

int main() {

    Derive d(50);
    Base*  pb = &d;
    pb->show();                         // 静态绑定 "Base::show()"
    pb->show(10);                       // 静态绑定 "Base::show(int)"

    cout << sizeof(Base) << endl;       // 4
    cout << sizeof(Derive) << endl;     // 8

    cout << typeid(pb).name() << endl;  // class Base*
    cout << typeid(*pb).name() << endl; // class Base

    return 0;
}
