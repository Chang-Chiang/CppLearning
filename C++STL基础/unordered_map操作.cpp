#include <iostream>
#include <string>
#include <unordered_map>
#include <utility> // make_pair

using std::cout;
using std::endl;
using std::make_pair;
using std::string;
using std::unordered_map;

/*
[key, value]
struct pair {
  first; => key
  second; => value
};
*/

int main() {

    // unordered_multimap<int, string> map1;
    unordered_map<int, string> map1;
    map1.insert(make_pair(1000, "张三"));
    map1.insert({1010, "李四"});
    map1.insert({1020, "王五"});
    map1.insert({1000, "王凯"});

    map1.erase(1020);

    /*
    map 的 operatorp[]
    1. 查询
    2. 如果 key 不存在, 它会插入一对数据 [key, string()]
    V& operator[](const K &key) {
      insert({key, V()});
    }
    */
    // map1[2000];  // 插入元素
    map1[2000] = "刘硕";  // 等价于 map1.insert({2000, "刘硕"});
    map1[1000] = "张三2"; // 修改

    cout << map1.size() << endl;
    // map operator[](key) => value
    cout << map1[1000] << endl; // 张三

    // find 查询
    auto it1 = map1.find(1010);
    if (it1 != map1.end()) {
        // it1 -> pair 对象
        cout << "key: " << it1->first << " value: " << it1->second << endl;
    }

    return 0;
}
