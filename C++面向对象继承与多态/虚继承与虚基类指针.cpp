#include <iostream>

using std::cout;
using std::endl;

class A {
public:

private:
    int ma;
};

class B1 : public A {
public:

private:
    int mb;
};

class B2 : virtual public A {
public:

private:
    int mb;
};

int main() {
    int  a = 1;
    int* p = &a;
    cout << sizeof(p) << endl;  // 8

    cout << sizeof(A) << endl;  // 4
    cout << sizeof(B1) << endl; // 4 + 4
    cout << sizeof(B2) << endl; // 4 + 4 + 8

    return 0;
}
