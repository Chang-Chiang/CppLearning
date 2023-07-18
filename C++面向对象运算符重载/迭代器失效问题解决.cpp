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

    for (int v : vec) {
        cout <<  v << endl;
    }
    cout << endl;

    /*----迭代器失效解决方法部分----*/
    // 【场景一】把 vec 容器中所有的偶数全部删除
    auto it = vec.begin();
    while (it != vec.end()) {
        if (*it % 2 == 0) {
            it = vec.erase(it);  // 迭代器更新
        }
        else {
            ++it;
        }
    }

    for (int v : vec) {
        cout <<  v << endl;
    }
    cout << endl;

    //【场景二】给 vec 容器中所有的偶数前添加一个比当前偶数值小 1 的数字
    // auto it = vec.begin();
    // for (; it != vec.end(); ++it) {
    //     if (*it % 2 == 0) {
    //         it = vec.insert(it, *it - 1);
    //         ++it;
    //     }
    // }
    /*----迭代器失效解决方法部分----*/

    return 0;
}
