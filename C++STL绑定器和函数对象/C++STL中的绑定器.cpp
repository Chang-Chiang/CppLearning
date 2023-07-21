#include <algorithm>
#include <ctime> // time() 函数
#include <functional>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::greater;
using std::vector;

template <typename Container>
void showContainer(Container& con) {
    // Container::iterator it = con.begin();
    // Container 类型还未实例化, 编译器不知道 Container::iterator 是类型还是变量

    typename Container::iterator it = con.begin();
    // 告诉编译器 Container::iterator 是类型

    for (; it != con.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}

int main() {
    vector<int> vec;
    srand(time(nullptr));
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100 + 1);
    }

    showContainer(vec);
    sort(vec.begin(), vec.end()); // 默认小到大排序
    showContainer(vec);

    // greater 二元函数对象
    sort(vec.begin(), vec.end(), greater<int>()); // 重载 从大到小排序
    showContainer(vec);

    /*
    把 70 按顺序插入到 vec 容器当中
    operator()(const T &val) 需要 一元函数对象
    greater a > b
    less    a < b
    绑定器 + 二元函数对象 => 一元函数对象
    bool operator()(const _Ty &_Left, const _Ty &_Right)
    bind1st + greater => bool operator()(70, const _Ty &_Right)
    bind2nd + less    => bool operator()(const _Ty &_Left, 70)
    */
    auto it1 = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 70));
    // auto it1 = find_if(vec.begin(), vec.end(), bind2nd(less<int>(), 70));
    if (it1 != vec.end()) {
        vec.insert(it1, 70);
    }
    showContainer(vec);

    return 0;
}
