#include <iostream>
#include <unordered_map>
#include <utility> // pair

using std::cout;
using std::endl;
using std::pair;
using std::unordered_map;

// 查重
int main() {
    const int ARR_LEN = 100;
    int       arr[ARR_LEN] = {0};
    for (int i = 0; i < ARR_LEN; ++i) {
        arr[i] = rand() % 20 + 1;
    }

    // 上面的 100 个整数中, 统计哪些数字重复了, 并且统计数字重复次数
    unordered_map<int, int> map1;
    for (int k : arr) {
        /*
        auto it = map1.find(k);
        if (it == map1.end()) {
            map1.insert({k, 1});  // k 未出现
        }
        else {
            it->second++;
        }
        */

        map1[k]++;
    }

    // for_each 遍历
    // for (pair<int, int> p : map1) {  // 发生拷贝
    // for (pair<int, int> &p : map1) {  // 报错
    for (const pair<int, int>& p : map1) {
        if (p.second > 1) {
            cout << "key: " << p.first << " count: " << p.second << endl;
        }
    }

    // 迭代器遍历
    auto it = map1.begin();
    for (; it != map1.end(); ++it) {
        if (it->second > 1) {
            cout << "key: " << it->first << " count: " << it->second << endl;
        }
    }

    return 0;
}
