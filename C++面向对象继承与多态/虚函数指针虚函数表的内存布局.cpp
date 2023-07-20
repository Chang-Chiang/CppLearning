#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

// 动物的基类
class Animal {
public:
    Animal(string name) : _name(name) {}
    virtual void bark() {}

protected:
    string _name;
};

class Cat : public Animal {
public:
    Cat(string name) : Animal(name) {}
    void bark() { cout << _name << "bark: miao miao!" << endl; }
};

class Dog : public Animal {
public:
    Dog(string name) : Animal(name) {}
    void bark() { cout << _name << "bark: wang wang!" << endl; }
};

int main() {
    Animal* p1 = new Cat("加菲猫");
    Animal* p2 = new Dog("二哈");

    int* p11 = (int*)p1;
    int* p22 = (int*)p2;

    int tmp = p11[0]; // p11[0] 访问的是 Cat 的前 4 个字节
    p11[0] = p22[0];  // p22[0] 访问的是 Dog 的前 4 个字节
    p22[0] = tmp;
    /*
    Cat 里的 vfptr 存放的是 Dog 的 vftable 的地址
    Dog 里的 vfptr 存放的是 Cat 的 vftable 的地址
    */

    p1->bark(); // p1 -> Cat vfptr -> Dog vftable bark
    p2->bark(); // p2 -> Dog vfptr -> Cat vftable bark

    delete p1;
    delete p2;

    return 0;
}
