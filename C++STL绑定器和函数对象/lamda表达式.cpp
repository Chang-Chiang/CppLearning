#include <iostream>

using std::cout;
using std::endl;

// [捕获外部变量](形参列表)->返回值 {操作代码};
// 如果 lamda 返回值不需要, 那么 "->返回值" 可以省略
// []: 表示不捕获任何外部变量
// [=]: 以传值的方式捕获外部的所有变量
// [&]: 以传引用的方式捕获外部所有变量
// [this]: 捕获外部的 this 指针
// [=, &a]: 以传值的方式捕获外部的所有变量, 但是 a 变量
// [a, b]: 以传值的方式捕获外部的 a, b

template <typename T = void>
class TestLamda01 {
public:
    TestLamda01() {}
    void operator()() { cout << "hello world!" << endl; }
};

template <typename T = void>
class TestLamda02 {
public:
    int operator()(int a, int b) const { return a + b; }
};

template <typename T = int>
class TestLamda03 {
public:
    TestLamda03(int a, int b) : ma(a), mb(b) {}
    void operator()() const {
        int tmp = ma;
        ma = mb;
        mb = tmp;
    }

private:
    mutable int ma;
    mutable int mb;
};

template <typename T = int>
class TestLamda04 {
public:
    TestLamda04(int& a, int& b) : ma(a), mb(b) {}
    void operator()() const {
        int tmp = ma;
        ma = mb;
        mb = tmp;
    }

private:
    int& ma;
    int& mb;
};

int main() {
    auto func1 = []() -> void { cout << "hello world!" << endl; };
    // auto func1 = []() { cout << "hello world!" << endl; };
    func1();

    TestLamda01<> t1;
    t1();

    auto func2 = [](int a, int b) -> int { return a + b; };
    cout << func2(20, 30) << endl;

    TestLamda02<> t2;
    t2(10, 20);

    int  a = 10;
    int  b = 20;
    auto func3 = [a, b]() mutable {
        int tmp = a;
        a = b;
        b = tmp;
    };
    func3();
    cout << "a: " << a << " b: " << b << endl;

    TestLamda03<> t3(a, b);
    t3();

    auto func4 = [&a, &b]() {
        int tmp = a;
        a = b;
        b = tmp;
    };
    func4();
    cout << "a: " << a << " b: " << b << endl;

    TestLamda04<> t4(a, b);
    t4();

    return 0;
}
