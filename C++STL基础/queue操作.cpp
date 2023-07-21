#include <iostream>
#include <queue>

using std::cout;
using std::endl;
using std::queue;

/*
+ push  入栈
+ pop   出栈
+ front 查看队头元素
+ back  查看队尾元素
+ empty 判断队空
+ size  返回元素个数
*/
int main() {
    queue<int> que;

    for (int i = 0; i < 20; ++i) {
        que.push(rand() % 100 + 1);
    }

    cout << que.size() << endl;

    while (!que.empty()) {
        cout << que.front() << " ";
        que.pop();
    }
    cout << endl;

    return 0;
}
