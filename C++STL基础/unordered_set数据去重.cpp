#include <iostream>
#include <unordered_set>

using std::cout;
using std::endl;
using std::unordered_set;

// 去重
int main() {
    const int ARR_LEN = 100;
    int       arr[ARR_LEN] = {0};
    for (int i = 0; i < ARR_LEN; ++i) {
        arr[i] = rand() % 20 + 1;
    }

    // 上面的 100 个整数中, 把数字进行去重打印
    unordered_set<int> set;
    for (int v : arr) { // O(N)
        set.insert(v);  // O(1)
    }
    for (int v : set) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
