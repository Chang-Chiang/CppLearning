#include <iostream>

using std::cout;
using std::endl;

class A {
public:
    A(int data) : ma(data) { cout << "A()" << endl; }
    ~A() { cout << "~A()" << endl; }

protected:
    int ma;
};
// ================================================= //
class B : public A {
public:
    B(int data) : A(data), mb(data) { cout << "B()" << endl; }
    ~B() { cout << "~B()" << endl; }

protected:
    int mb;
};

class C : public A {
public:
    C(int data) : A(data), mc(data) { cout << "C()" << endl; }
    ~C() { cout << "~C()" << endl; }

protected:
    int mc;
};
// ================================================= //
class D
    : public B
    , public C {
public:
    D(int data) : B(data), C(data), md(data) { cout << "D()" << endl; }
    ~D() { cout << "~D()" << endl; }

protected:
    int md;
};

int main() {

    D d(10);

    /*
    输出：
    A()
    B()
    A()
    C()
    D()
    ~D()
    ~C()
    ~A()
    ~B()
    ~A()
    */

    return 0;
}
