#include <iostream>
#include <queue>

using std::cout;
using std::endl;
using std::priority_queue;

/*
+ push  入队
+ pop   出队
+ top   查看队顶元素
+ empty 判断队空
+ size  返回元素个数
*/
int main() {
    priority_queue<int> pque;

    for (int i = 0; i < 20; ++i) {
        pque.push(rand() % 100 + 1);
    }

    cout << pque.size() << endl;

    while (!pque.empty()) {
        cout << pque.top() << " ";
        pque.pop();
    }
    cout << endl;

    return 0;
}
