#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int main() {
    vector<int> vec;                   // vector<string> vec
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100 + 1); // 1~100 间的随机数
    }

    // operator[] 运算符重载函数进行遍历
    int size = vec.size();
    for (int i = 0; i < size; ++i) {
        cout << vec[i] << " ";
    }
    cout << endl;

    // 把 vec 容器中所有的偶数全部删除
    auto it2 = vec.begin();
    // for (; it2 != vec.end(); ) {
    while (it2 != vec.end()) {
        if (*it2 % 2 == 0) {
            vec.erase(it2);
        }
        else {
            ++it2;
        }
    }

    // 通过迭代器遍历 vector 容器
    auto it1 = vec.begin();
    for (; it1 != vec.end(); ++it1) {
        cout << *it1 << " ";
    }
    cout << endl;

    // 给 vector 容器中所有的奇数前面都添加一个(奇数 - 1) 的偶数
    for (it1 = vec.begin(); it1 != vec.end(); ++it1) {
        if (*it1 % 2 != 0) {
            vec.insert(it1, *it1 - 1);
            ++it1;
        }
    }

    // 通过迭代器遍历 vector 容器
    for (it1 = vec.begin(); it1 != vec.end(); ++it1) {
        cout << *it1 << " ";
    }
    cout << endl;

    return 0;
}
