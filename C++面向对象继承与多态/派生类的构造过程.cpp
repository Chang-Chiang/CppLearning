#include <iostream>

using std::cout;
using std::endl;

class Base {
public:
    Base(int data) : ma(data) { cout << "Base()" << endl; }
    ~Base() { cout << "~Base()" << endl; }

protected:
    int ma;
};

class Derive : public Base {
public:
    Derive(int data) : Base(data), mb(data) { cout << "Derive()" << endl; }
    ~Derive() { cout << "~Derive()" << endl; }

private:
    int mb;
};

int main() {
    Derive d(20);
    return 0;
}
