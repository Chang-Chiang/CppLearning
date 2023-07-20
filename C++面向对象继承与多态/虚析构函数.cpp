#include <iostream>

using std::cout;
using std::endl;

class Base {
public:
    Base(int data) : ma(data) { cout << "Base()" << endl; }

    // 虚析构函数
    virtual ~Base() { cout << "~Base()" << endl; }

    virtual void show() { cout << "call Base::show()" << endl; }

protected:
    int ma;
};

class Derive : public Base {
public:
    Derive(int data) : Base(data), mb(data) { cout << "Derive()" << endl; }

    // 基类的析构函数是 virtual 虚函数, 那么派生类的析构函数自动成为虚函数
    ~Derive() { cout << "~Derive()" << endl; }

private:
    int mb;
};

int main() {
    // Derive d(10);
    // Base *pb = &d;
    // pb->show();

    Base* pb = new Derive(10); // 开辟内存 + 构造对象  "Base()" "Derive()"
    pb->show();                // 动态绑定 "call Base::show()"

    delete pb;                 // 析构对象 + 内存释放
    // 基类析构函数定义为普通函数时, "~Base()" 派生类的析构函数没有被调用到, 内存泄漏！
    // 如果 Derive 类的成员变量有指针指向外部堆内存
    // 未析构对象直接释放对象内存, 外部堆内存资源未被释放

    /*
    编译器首先看 pb 类型, pb 是 Base 类, 调用 Base::~Base()
    Base::~Base() 是普通函数 (非虚函数) , 调用析构函数就是静态绑定 call Base::~Base()
    Base::~Base() 是虚函数, 调用析构函数就是动态绑定,
    所以要将基类的析构函数定义为 虚析构函数
    */

    return 0;
}
