#include <iostream>
#include <unordered_set>

using std::cout;
using std::endl;
using std::unordered_set;

/*
增加: insert(val)
遍历: iterator 自己搜索, 调用 find 成员方法
删除: erase(key) erase(it)
*/
int main() {

    // unordered_multiset<int> set1;
    unordered_set<int> set1; // 不会存储 key 值重复的元素
    for (int i = 0; i < 50; ++i) {
        set1.insert(rand() % 20 + 1);
        // 与 vector/deque/list insert(it, val) (线性表) 不同, 只需要一个参数
    }

    cout << set1.size() << endl;    // 返回容器中元素个数
    cout << set1.count(15) << endl; // 返回 key 为 15 的元素的个数

    // 迭代器遍历集合容器
    auto it1 = set1.begin();
    for (; it1 != set1.end(); ++it1) {
        cout << *it1 << " ";
    }
    cout << endl;

    // 按 key 值删除元素
    set1.erase(20);

    // 迭代器遍历删除, 只删除一次
    for (it1 = set1.begin(); it1 != set1.end(); ++it1) {
        if (*it1 == 30) {
            it1 = set1.erase(it1); // 调用 erase, it1 迭代器就失效了
        }
    }

    // 迭代器遍历删除, 连续删除
    for (it1 = set1.begin(); it1 != set1.end();) {
        if (*it1 == 30) {
            it1 = set1.erase(it1); // 调用 erase, it1 迭代器就失效了
        }
        else {
            ++it1;
        }
    }

    // find 查询
    it1 = set1.find(20); // 不存在则返回末尾迭代器
    if (it1 != set1.end()) {
        set1.erase(it1);
    }

    for (int v : set1) {
        cout << v << " ";
    }
    cout << endl;

    return 0;
}
