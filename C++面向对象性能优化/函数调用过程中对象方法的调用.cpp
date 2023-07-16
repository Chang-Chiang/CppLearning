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
        ma = t.ma; // 浅拷贝
        return *this;
    }
    int getData() const { return ma; }

private:
    int ma;
};

// 不能返回局部的或者临时对象的指针(Test*)或引用(Test&)
Test GetObject(Test t) {
    int  val = t.getData();
    Test tmp(val);

    // static Test tmp(val);
    // 静态局部对象, 在数据段, 程序运行结束才析构然后释放内存,
    // 能返回指针或引用

    return tmp; // 返回对象为局部变量, 出函数消失(随函数栈的回收而回收), 不能返回指针或引用
}

int main() {
    Test t1; // 【1】Test(int) t1 构造
    Test t2; // 【2】Test(int) t2 构造

    /*
    函数调用过程中, 从实参 -> 形参, 是初始化
    Test GetObject(Test t) {  // 【3】Test(const Test&)  t1 拷贝构造形参 t
        int val = t.getData();
        Test tmp(val);        // 【4】Test(int) tmp 构造
        return tmp;
        // tmp 和 t2 是不同栈帧上的对象, 不能直接赋值
        // 先在 main 函数栈帧上构建一个临时对象(拷贝构造)
    }
    */
    t2 = GetObject(t1); // 【5】Test(const Test&) tmp 拷贝构造 main栈帧临时对象
                        // 【6】~Test() tmp 析构
                        // 【7】~Test() 形参 t 析构
                        // 【8】operator= main栈帧临时对象 给 t2 赋值
                        // 【9】~Test() main栈帧临时对象 析构
    return 0;
}
// 【10】~Test() t2 析构
// 【11】~Test() t1 析构