#include <algorithm>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int main() {
    vector<int> vec;
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100 + 1);
    }

    sort(vec.begin(), vec.end(), [](int a, int b) -> bool { return a > b; });
    for (int val : vec) {
        cout << val << " ";
    }
    cout << endl;

    auto it = find_if(vec.begin(), vec.end(), [](int a) -> bool { return a < 65; });
    if (it != vec.end()) {
        vec.insert(it, 65);
    }
    for (int val : vec) {
        cout << val << " ";
    }
    cout << endl;

    for_each(vec.begin(), vec.end(), [](int a) {
        if (a % 2 == 0) {
            cout << a << " ";
        }
    });
    cout << endl;

    return 0;
}
