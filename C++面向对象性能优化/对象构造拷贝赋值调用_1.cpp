#include <iostream>
using namespace std;

class Test {
public:
    // 普通构造
    Test(int a = 10) : ma(a) { cout << "Test(int)" << endl; }

    // 析构
    ~Test() { cout << "~Test()" << endl; }

    // 拷贝构造
    Test(const Test& t) : ma(t.ma) { cout << "Test(const Test&)" << endl; }

    // = 赋值运算符重载
    Test& operator=(const Test& t) {
        cout << "operator=" << endl;
        ma = t.ma;
        return *this;
    }

private:
    int ma;
};

int main() {
    Test t1;      // 普通构造
    Test t2(t1);  // 拷贝构造
    Test t3 = t1; // T3 正在定义, 不是赋值, 而是调用拷贝构造

    /* ---------------■ 临时对象--------------- */
    /**
     * C++ 编译器对于【对象构造】的优化:
     * 用临时对象生成新对象的时候, 临时对象就不产生了, 直接构造新对象就可以了
    */
    // Test(20) 显式生成临时对象, 临时对象生存周期: 所在的语句
    Test t4 = Test(20); // 等价于语句 Test t4(20); 并不会先构造生成临时对象再拷贝构造生成 t4

    t4 = t2; // t4 赋值运算符重载

    /**
     * 先调用拷贝构造生成临时对象, 显式生成临时对象
     * 再用临时对象给 t4 赋值
     * 出语句临时对象析构
    */
    t4 = Test(30); // t4.operator=(cosnt Test &t), t4 赋值运算符重载

    /**
     * 显式强转, 显式生成临时对象
     * 编译器查看类 Test 有没有带 int 类型的构造函数
    */
    t4 = (Test)30; // int -> Test
    t4 = 30;       // 隐式强转, 隐式生成临时对象

    /* ---------------■ 指针临时对象--------------- */
    // Test* p = &Test(40); // 生成临时对象, 出语句后析构
    // 出语句后析构, p 指向的是一个已经析构的临时对象
    // 这里我的 g++ 编译报错

    const Test& ref = Test(50); // 生成临时对象, 出语句后不析构
    // 应用相当于临时对象的别名, 临时对象的生命周期相当于引用变量的生命周期

    return 0;
}