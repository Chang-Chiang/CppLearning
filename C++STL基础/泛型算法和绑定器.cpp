#include <algorithm>  // 包含了 C++ STL 里面的泛型算法
#include <functional> // 包含了绑定器、函数对象
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::greater;
using std::less;
using std::vector;

int main() {
    int         arr[] = {12, 4, 78, 9, 21, 43, 56, 52, 42, 31};
    vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;

    // 输出排序后元素
    sort(vec.begin(), vec.end()); // 默认快排, 从小到大排序
    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;

    // 有序容器中进行二分查找
    if (binary_search(vec.begin(), vec.end(), 21)) {
        cout << "binary_search 21 存在" << endl;
    }

    // 对有序容器, 二分查找效率更高
    auto it1 = find(vec.begin(), vec.end(), 21);
    if (it1 != vec.end()) {
        cout << "find 21 存在" << endl;
    }

    // 传入函数对象 greater, 改变容器元素排序时的比较方式
    sort(vec.begin(), vec.end(), greater<int>());
    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;

    /*
    绑定器 + 二元函数对象 => 一元函数对象
    bind1st: 把二元函数对象的 operator()(a, b) 的第一个形参绑定起来
    bind2nd: 把二元函数对象的 operator()(a, b) 的第二个形参绑定起来
    */
    // 把 48 按序插入到 vector 容器当中, 找第一个小于 48 的数字
    // find_if 需要的是一个一元函数对象
    // greater a > b, less a < b
    // 下面三句, 实现效果一样
    auto it2 = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 48));
    // auto it2 = find_if(vec.begin(), vec.end(), bind2nd(less<int>(), 48));
    // // lamda 表达式
    // auto it2 = find_if(vec.begin(), vec.end(), [](int val) -> bool { return val < 48; });
    vec.insert(it2, 48);
    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;

    // for_each 可以遍历容器的所有元素
    // 可以自行添加合适的函数对象对容器的元素进行过滤
    // lamda 表达式
    for_each(vec.begin(), vec.end(), [](int val) -> void {
        if (val % 2 == 0) {
            cout << val << " ";
        }
    });
    cout << endl;

    return 0;
}
