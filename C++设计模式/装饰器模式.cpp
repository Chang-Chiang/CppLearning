#include <iostream>

using std::cout;
using std::endl;

class Car {
public:
    virtual void show() = 0;
};

class Bmw : public Car {
public:
    void show() { cout << "宝马: 基本配置"; }
};

class Audi : public Car {
public:
    void show() { cout << "奥迪: 基本配置"; }
};

class Benz : public Car {
public:
    void show() { cout << "奔驰: 基本配置"; }
};

// ---------------------------------
class ConcreteDecorator01 : public Car {
public:
    ConcreteDecorator01(Car* p) : pCar(p) {}
    void show() {
        pCar->show();
        cout << " 定速巡航";
    }

private:
    Car* pCar;
};

class ConcreteDecorator02 : public Car {
public:
    ConcreteDecorator02(Car* p) : pCar(p) {}
    void show() {
        pCar->show();
        cout << " 自动刹车";
    }

private:
    Car* pCar;
};

class ConcreteDecorator03 : public Car {
public:
    ConcreteDecorator03(Car* p) : pCar(p) {}
    void show() {
        pCar->show();
        cout << " 车道偏离";
    }

private:
    Car* pCar;
};

int main() {
    Car* p1 = new ConcreteDecorator01(new Bmw());
    p1 = new ConcreteDecorator02(p1);
    p1 = new ConcreteDecorator03(p1);
    p1->show();
    cout << endl;

    Car* p2 = new ConcreteDecorator02(new Audi());
    p2->show();
    cout << endl;

    Car* p3 = new ConcreteDecorator03(new Benz());
    p3->show();
    cout << endl;

    return 0;
}
