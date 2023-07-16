#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

bool compare(int a, int b) {
    cout << "compare_int_int" << endl;
    return a > b;
}

bool compare(double a, double b) {
    cout << "compare_double_double" << endl;
    return a > b;
}

bool compare(const char* a, const char* b) {
    cout << "compare_char*_char*" << endl;
    return strcmp(a, b) > 0;
}

int data = 10;
int main() {

    int data = 20;
    // int a = data;  // a = 20
    int a = ::data; // a = 10

    // 局部进行函数声明后, 函数调用均调用该函数
    // bool compare(int a, int b); // 函数声明

    compare(10, 20);
    compare(10.0, 20.0);
    compare("aaa", "bbb");

    return 0;
}
