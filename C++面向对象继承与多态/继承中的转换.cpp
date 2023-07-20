#include <iostream>

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

    Base   b(10);
    Derive d(20);

    b = d; // 类型从下到上的转换, 允许
    // d = b;  // 类型从上到下的转换, 不允许

    Base* pb = &d; // 类型从下到上的转换, 允许
    pb->show();    // "Base::show()"
    // ((Derive*)pb)->show();  // 类型强转 "Derive::show()"
    pb->show(10); // "Base::show(int)"

    // Derive *pd = &b;  // 类型从上到下的转换, 不允许

    // 类型强转, 不安全, 涉及了内存的非法访问, 实际内存中没有派生类对象
    Derive* pd = (Derive*)&b;
    pd->show(); // "Derive::show()"

    return 0;
}
