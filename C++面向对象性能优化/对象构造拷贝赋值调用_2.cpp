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