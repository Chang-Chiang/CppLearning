#include <functional>
#include <iostream>
#include <string>
using namespace std;

void hello(string str) { cout << str << endl; }
int  sum(int a, int b) { return a + b; }

class Test {
public:
    int sum(int a, int b) { return a + b; }
};

int main() {

    // bind 是函数模板, 可以自动推演模板类型参数
    bind(hello, "hello, bind!")();
    cout << bind(sum, 10, 20)() << endl;
    cout << bind(&Test::sum, Test(), 10, 20)() << endl;

    // 参数占位符
    bind(hello, placeholders::_1)("hello bind 2!"); // namespace place_holders
    cout << bind(sum, placeholders::_1, placeholders::_2)(200, 300) << endl;

    // 绑定器出了语句, 无法继续使用 => 通过function 把 bind 返回的绑定器 binder 复用
    function<void(string)> func1 = bind(hello, placeholders::_1);
    func1("hello china!");
    func1("hello hubei!");

    return 0;
}
