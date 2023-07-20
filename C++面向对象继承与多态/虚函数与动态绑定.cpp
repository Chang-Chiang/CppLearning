#include <iostream>

using std::cout;
using std::endl;

class Base {
public:
    Base(int data = 10) :ma(data) {}
    virtual void show() { cout << "Base::show()" << endl; }
    virtual void show(int) { cout << "Base::show(int)" << endl; }
protected:
    int ma;
};

class Derive : public Base {
public:
    Derive(int data = 20) :Base(data), mb(data) {}
    void show() { cout << "Derive::show()" << endl; }
private:
    int mb;
};

int main() {
    Derive d(50);
    Base *pb = &d;

    /*
    + pb 是 Base 类型, 调用  Base::show() 如果发现 show 是普通函数, 就进行静态绑定, call Base::show()
    + pb 是 Base 类型, 调用  Base::show() 如果发现 show 是虚函数, 就进行动态绑定
    	  mov eax, dword ptr[pb]  # 对象前四个字节 (vfptr 虚函数表的地址) 放到 eax 寄存器
    	  mov ecx, dword ptr[eax]  # 虚函数表的四个字节, 即虚函数的地址放到 ecx
    	  call ecx  # 虚函数的地址  动态 (运行时期) 的绑定 (函数的调用)
    */
    pb->show();
    pb->show(10);  // Base::show(int) 是一个虚函数, 此时就得动态绑定

    cout << sizeof(Base) << endl;  // 4 + 4(vfptr) = 8
    cout << sizeof(Derive) << endl;  // 8 + 4(vfptr) = 12

    cout << typeid(pb).name() << endl;  // class Base*

    // pb 的类型：Base
    // 如果 Base 没有虚函数, *pb 识别的就是编译时期的类型 *pb -> class Base
    // 如果 Base 有虚函数, *pb 识别的是运行时期的类型, 即 RTTI 类型 pb -> d(vfptr) -> Derive vftable
    cout << typeid(*pb).name() << endl;  // class Derive

    return 0;
}
