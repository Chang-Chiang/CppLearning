#include <iostream>
using namespace std;

class Test {
public:
    Test(int a = 10) : ma(a) { cout << "Test(int)" << endl; }
    ~Test() { cout << "~Test()" << endl; }
    Test(const Test& t) : ma(t.ma) { cout << "Test(const Test&)" << endl; }
    Test& operator=(const Test& t) {
        cout << "operator=" << endl;
        ma = t.ma; // 浅拷贝
        return *this;
    }
    int getData() const { return ma; }

private:
    int ma;
};

// 【优化一】：传引用
// Test GetObject(Test t) {
Test GetObject(Test& t) { // #【1】
    int val = t.getData();

    // Test tmp(val);
    // return tmp;

    // 【优化二】：返回临时对象
    return Test(val); // #【2】 函数内临时对象到 main 栈帧上临时对象的拷贝构造
    // 返回临时对象, 临时对象拷贝构造 main栈帧上的新对象
    // 编译器优化, 不产生临时对象(出函数后也就不需析构了), 直接构造 main栈帧上的新对象
}

int main() {
    Test t1;
    // Test t2;
    // t2 = GetObject(t1);
    // 【优化三】：初始化方式接收返回值
    Test t2 = GetObject(t1); // #【3】 main 栈帧上的临时对象拷贝构造同类型新对象 t2
    return 0;
}