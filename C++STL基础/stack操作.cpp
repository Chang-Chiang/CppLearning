#include <iostream>
#include <stack>

using std::cout;
using std::endl;
using std::stack;

/*
+ push  入栈
+ pop   出栈
+ top   查看栈顶元素
+ empty 判断栈空
+ size  返回元素个数
*/
int main() {
    stack<int> s1;

    for (int i = 0; i < 20; ++i) {
        s1.push(rand() % 100 + 1);
    }

    cout << s1.size() << endl;

    while (!s1.empty()) {
        cout << s1.top() << " ";
        s1.pop();
    }
    cout << endl;

    return 0;
}
