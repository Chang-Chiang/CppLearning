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

    // for (int v : vec) {
    //     cout <<  v << endl;
    // }
    // cout << endl;

    /*---- 迭代器失效部分 ----*/

    // 【场景一】原意：把 vec 容器中所有的偶数全部删除
    auto it = vec.begin();
    for (; it != vec.end(); ++it) {
        if (*it % 2 == 0) {

            // 迭代器失效
            // 第一次调用 erase 以后, 迭代器 it 就失效了
            vec.erase(it);
            // insert(it, val) 添加到迭代器指定位置
            // break;  // 只删除一个偶数
        }
    }

    // 【场景二】原意：给 vec 容器中所有的偶数前添加一个比当前偶数值小 1 的数字
    // auto it = vec.begin();
    // for (; it != vec.end(); ++it) {
    //     if (*it % 2 == 0) {

    //         // 迭代器在第一次 insert 后, iterator 就失效了
    //         vec.insert(it, *it - 1);
	// 		// break;  // 只添加一次
    //     }
    // }

    /*---- 迭代器失效部分 ----*/

    return 0;
}
