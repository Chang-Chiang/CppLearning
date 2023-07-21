#include <iostream>
#include <map>
#include <string>

using std::cout;
using std::endl;
using std::map;
using std::ostream;
using std::string;

class Student {
public:
    Student(int id = 0, string name = "") : _id(id), _name(name) {}
    // Student(int id, string name) :_id(id), _name(name) {}

private:
    int             _id;
    string          _name;
    friend ostream& operator<<(ostream& out, const Student& stu);
};

ostream& operator<<(ostream& out, const Student& stu) {
    cout << "id: " << stu._id << " name: " << stu._name << endl;
    return out;
}

int main() {
    map<int, Student> stuMap;
    stuMap.insert({1000, Student(1000, "张雯")});
    stuMap.insert({1020, Student(1020, "李广")});
    stuMap.insert({1030, Student(1030, "高洋")});

    // 删除 stuMap.erase(it) or stuMap.erase(1020)

    // 查询 stuMap[key] or stuMap.find(key)
    cout << stuMap[1020] << endl;
    // stuMap[2000] 若不存在, 会插入 { 2000, V() }

    auto it = stuMap.begin();
    for (; it != stuMap.end(); ++it) {
        cout << "key: " << it->first << " value: " << it->second << endl;
    }
    cout << endl;

    return 0;
}
