#include <iostream>
#include <string>
#include <typeinfo>

using std::string;
using std::cin;
using std::cout;
using std::endl;

// 动物的基类
class Animal {
public:
    Animal(string name) :_name(name) {}
    virtual void bark() {}
protected:
    string _name;
};

class Cat : public Animal {
public:
    Cat(string name) :Animal(name) {}
    void bark() { cout << _name << "bark: miao miao!" << endl; }
};

class Dog : public Animal {
public:
    Dog(string name) :Animal(name) {}
    void bark() { cout << _name << "bark: wang wang!" << endl; }
};

class Pig : public Animal {
public:
    Pig(string name) :Animal(name) {}
    void bark() { cout << _name << "bark: heng heng!" << endl; }
};

/*
下面一组 bark API 接口无法做到我们软件设计要求的“开-闭”原则(高内聚, 低耦合)
软件设计有六大原则之一：“开-闭”原则, 对修改关闭, 对扩展开放
void bark(Cat &cat) {
    cat.bark();
}
void bark(Dog &dog) {
    dog.bark();
}
void bark(Pig &pig) {
    pig.bark();
}
*/

//
void bark(Animal *p) {
    p->bark();  // Animal::bark 是虚函数, 动态绑定
}

int main() {
    Cat cat("猫咪");
    Dog dog("二哈");
    Pig pig("佩奇");

    bark(&cat);
    bark(&dog);
    bark(&pig);

    return 0;
}
