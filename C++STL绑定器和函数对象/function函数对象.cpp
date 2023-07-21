#include <functional>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::function;
using std::string;

void hello(string str) { cout << str << endl; }
int  sum(int a, int b) { return a + b; }

int main() {
    function<void(string)> func1 = hello;
    func1("hello world!");
    return 0;
}
