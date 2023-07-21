#include <iostream>
#include <set>

using std::cout;
using std::endl;
using std::set;

int main() {
    set<int> set1;
    for (int i = 0; i < 20; ++i) {
        set1.insert(rand() % 20 + 1);
    }

    for (int v : set1) {  // 有序
        cout << v << " "; // 对红黑树中序遍历的结果
    }
    cout << endl;

    return 0;
}
