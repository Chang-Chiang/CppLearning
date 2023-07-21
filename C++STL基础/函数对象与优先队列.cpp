#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using std::cout;
using std::endl;
using std::greater;
using std::priority_queue;
using std::vector;

int main() {
    // priority_queue<int> que1;  // 底层默认是大根堆, 想使用小根堆怎么办
    using MinHeap = priority_queue<int, vector<int>, greater<int>>; // 底层使用小根堆
    MinHeap que2;
    for (int i = 0; i < 10; ++i) {
        que2.push(rand() % 100);
    }

    // 输出 从小到大
    while (!que2.empty()) {
        cout << que2.top() << " ";
        que2.pop();
    }
    cout << endl;

    return 0;
}
