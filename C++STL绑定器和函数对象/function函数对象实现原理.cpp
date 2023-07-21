#include <functional>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::function;
using std::string;

template<typename Fty>
class myfunction {};

/*
template<typename R, typename A1>
class myfunction<R(A1)>
{
public:
    using PFUNC = R(*)(A1);
    myfunction(PFUNC pfunc) :_pfunc(pfunc) {}
    R operator()(A1 arg) {
        return _pfunc(arg);
    }
private:
    PFUNC _pfunc;
};

template<typename R, typename A1, typename A2>
class myfunction<R(A1)>
{
public:
    using PFUNC = R(*)(A1);
    myfunction(PFUNC pfunc) :_pfunc(pfunc) {}
    R operator()(A1 arg1, A2 arg2) {
        return _pfunc(arg1, arg2);
    }
private:
    PFUNC _pfunc;
};
*/

template<typename R, typename... A>  // typename... A 表示可变参
class myfunction<R(A...)>
{
public:
    using PFUNC = R(*)(A...);
    myfunction(PFUNC pfunc) :_pfunc(pfunc) {}
    R operator()(A... arg...) {
        return _pfunc(arg...);
    }
private:
    PFUNC _pfunc;
};

void hello(string str) { cout << str << endl; }
int sum(int a, int b) { return a + b; }

int main() {
    myfunction<void(string)> func1 = hello;
    func1("hello world!");  // func1.operator()("hello world!")

    myfunction<int(int, int)> func2(sum);
    cout << func2(10, 20) << endl;

    return 0;
}
