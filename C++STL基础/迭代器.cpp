#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int main() {
    vector<int> vec;
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100);
    }

    // auto -> vector<int>::iterator
    auto it1 = vec.begin();
    // vector<int>::const_iterator it1 = vec.begin();
    /*
    class const_iterator {}
    class iterator : public const_iterator {}
    */
    for (; it1 != vec.end(); ++it1) {
        cout << *it1 << " ";
        if (*it1 % 2 == 0) {
            *it1 = 0; // const_iterator 返回常引用, 不能修改
        }
    }
    cout << endl;

    // rbegin() 返回的是最后一个元素的反向迭代器表示
    // rend() 返回的是首元素前驱位置的迭代器表示
    // vector<int>::reverse_iterator rit = vec.rbegin();
    auto rit = vec.rbegin();
    for (; rit != vec.rend(); ++rit) {
        cout << *rit << " ";
    }
    cout << endl;

    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
