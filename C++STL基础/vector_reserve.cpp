#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

int main() {
    vector<int> vec; // 默认, 底层开辟空间 0 个, 扩容 0 1 2 4 8 16 32 64···
    vec.reserve(20); // 给 vector 容器预留空间  1 0 0 20
    // vec.resize(20);  // 1 20(int()) 0 40

    cout << vec.empty() << endl; // 1
    cout << vec.size() << endl;  // 0

    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100 + 1); // 1~100 间的随机数
    }

    cout << vec.empty() << endl; // 0
    cout << vec.size() << endl;  // 20

    return 0;
}
