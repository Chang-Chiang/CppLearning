#include <functional>
#include <iostream>
#include <set>

using std::cout;
using std::endl;
using std::greater;
using std::set;

int main() {
    set<int, greater<int>> set1;
    for (int i = 0; i < 10; ++i) {
        set1.insert(rand() % 100);
    }

    // 从大到小
    for (int v : set1) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
