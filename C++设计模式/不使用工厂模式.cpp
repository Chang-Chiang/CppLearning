#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

class Car {
public:
    Car(string name) : _name(name) {}
    virtual void show() = 0;

protected:
    string _name;
};

class Bmw : public Car {
public:
    Bmw(string name) : Car(name) {}
    void show() { cout << "获取了一辆宝马汽车: " << _name << endl; }
};

class Audi : public Car {
public:
    Audi(string name) : Car(name) {}
    void show() { cout << "获取了一奥迪汽车: " << _name << endl; }
};

int main() {
    Car* p1 = new Bmw("X1");
    Car* p2 = new Audi("A6");

    return 0;
}
