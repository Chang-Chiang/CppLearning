# 对象构造、拷贝、赋值、析构方法调用

问：

+ C++ 效率不如 C 效率高？

## 减少不相关的函数调用

C、C++ 都属于本地编译型语言，本地经过编译器编译链接产生可执行文件才能去运行。语言效率低主要在于开发者，开发者写的代码效率低，C++ 调用的函数导致效率低。开发者应尽量避免这些不相关的函数调用。

## [掌握类的成员方法调用的时机](../对象构造拷贝赋值析构方法调用/对象构造拷贝赋值调用_1.cpp)

```c++
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
    Test t1;             // t1 调用普通构造
    Test t2(t1);         // t2 调用拷贝构造
    Test t3 = t1;        // t3 调用拷贝构造
    Test t4 = Test(20);  // t4 调用普通构造（临时对象的构造会被编译器优化掉）
    t4 = t2;             // t4 调用赋值运算符重载
    t4 = Test(30);       // 临时对象调用普通构造（显式），t4 调用赋值运算符重载，临时对象调用析构
    t4 = (Test)30;       // 临时对象调用普通构造（显式），t4 调用赋值运算符重载，临时对象调用析构
    t4 = 30;             // 临时对象调用普通构造（隐式），t4 调用赋值运算符重载，临时对象调用析构
    
    // Test* p = &Test(40); 
    // 临时对象调用普通构造, 出语句后析构，p 变成野指针。这里我的编译器报错
    
    const Test& ref = Test(50); // 临时对象调用普通构造, 出语句后不析构

    return 0;
}
// ref 的临时对象、t4、t3、t2、t1 析构
```

## [全局变量、静态局部变量、局部变量构造时机](../对象构造拷贝赋值析构方法调用/对象构造拷贝赋值调用_2.cpp)

+ 全局变量先构造
+ 再局部变量
+ 对静态的局部变量，程序运行时已分配好内存（数据段），到到第一次运行到才初始化

```c++
#include <iostream>
using namespace std;

class Test {
public:
    // 三种构造方式 Test(), Test(10), Test(10, 10)
    Test(int a = 5, int b = 5) : ma(a), mb(b) { cout << "Test(int, int)" << endl; }

    ~Test() { cout << "~Test()" << endl; }

    Test(const Test& src) : ma(src.ma), mb(src.mb) { cout << "Test(const Test&)" << endl; }

    void operator=(const Test& src) {
        ma = src.ma;
        mb = src.mb;
    }

private:
    int ma;
    int mb;
};

Test t1(10, 10);                    // 【1】Test(int, int)
int  main() {
    Test        t2(20, 20);        // 【3】Test(int, int)
    Test        t3 = t2;           // 【4】Test(const Test&)
    static Test t4 = Test(30, 30); // 【5】Test(int, int)
                                   // （临时对象构造新对象时，临时对象自己的构造被优化掉）
    t2 = Test(40, 40);   // 【6】Test(int, int), operator=, ~Test()
    t2 = (Test)(50, 50); // 【7】Test(int, int), operator=, ~Test()
    //  即为 t2 = (Test)50 ，逗号表达式结果为最后一个 (50, 50) 即 50

    t2 = 60;                           // 【8】Test(int, int), operator=, ~Test()
    Test*       p1 = new Test(70, 70); // 【9】Test(int, int) 堆上的对象, delete 才释放
    Test*       p2 = new Test[2];      // 【10】Test(int, int), Test(int, int)
    Test*       p3 = &Test(80, 80);    // 【11】Test(int, int), ~Test()
    const Test& p4 = Test(90, 90);     // 【12】Test(int, int)
    delete p1;                         // 【13】 ~Test()
    delete[] p2;                       // 【14】 ~Test(), ~Test()
}
// 析构: p4, t3, t2
Test t5(100, 100); // 【2】Test(int, int)
// 析构: t4, t5, t1 在数据段, 程序结束才析构
```

## [函数调用过程中对象方法的调用](../对象构造拷贝赋值析构方法调用/函数调用过程中对象方法的调用.cpp)

+ 对象初始化：是调用构造函数
+ 对象赋值：是两个对象都已经存在了

```c++
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
```

## [函数调用过程中的对象优化](../对象构造拷贝赋值析构方法调用/函数调用过程中的对象优化.cpp)

+ **函数参数传递**过程中，对象优先按引用传递, 不要按值传递. 优化【3】【7】
  + 值传递过程中，实参向形参传值，会调用对象的拷贝构造
  + 传引用相当于传对象的地址，优化掉形参的拷贝构造、以及出函数作用域后形参的析构函数
+ **函数返回对象**的时候，应该优先返回一个临时对象，而不是返回一个定义过的对象. 优化【4】【6】
  + 返回临时对象，临时对象拷贝构造 main栈帧上的新对象
  + 优化掉返回临时对象到 main 栈帧临时对象的拷贝构造、临时对象出函数作用域后的析构函数
+ 接收返回值是对象的时候，优先按初始化的方式接收，不要按赋值的方式接收. 优化【5】【8】【9】
  + 用 main 栈帧上的**临时对象拷贝构造新对象**（临时对象不会构造），优化掉函数返回临时对象拷贝构造 main 栈帧上的临时对象、赋值运算符重载、语句后 main 栈帧临时对象的析构

【1】Test(int) t1 构造

【2】Test(int) t2 构造

~~【3】Test(const Test&)  t1 拷贝构造 t~~

~~【4】Test(int) tmp 构造~~

【5】~~Test(const Test&)  tmp 拷贝构造~~ ，~~Test(int) 构造main栈帧临时对象~~

~~【6】~Test() tmp 析构~~ 

~~【7】~Test() 形参 t 析构~~

~~【8】operator= main栈帧临时对象 给 t2 赋值~~

~~【9】~Test() main栈帧临时对象 析构~~

【10】~Test() t2 析构

【11】~Test() t1 析构

```c++
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
