#include <iostream>
#include <memory>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;

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

enum CarType { BMW, AUDI };

class SimpleFactory {
public:
    Car* createCar(CarType ct) { // 缺点, 不封闭, 不符合软件设计的 "开闭" 原则
        switch (ct) {
            case BMW:
                return new Bmw("X1");
            case AUDI:
                return new Audi("A6");
            default:
                cerr << "传入工厂的参数不正确: " << ct << endl;
                break;
        }
        return nullptr;
    }
};

int main() {
    SimpleFactory* factory = new SimpleFactory();
    Car*           p1 = factory->createCar(BMW);
    Car*           p2 = factory->createCar(AUDI);

    p1->show();
    p2->show();
    delete p1;
    delete p2;
    delete factory;

    // -------------------------------------
    // 使用智能指针, 不用手动释放资源
    // unique_ptr<SimpleFactory> factory(new SimpleFactory());
    // unique_ptr<Car>           p1(factory->createCar(BMW));
    // unique_ptr<Car>           p2(factory->createCar(AUDI));

    // p1->show();
    // p2->show();

    return 0;
}
