#include <iostream>
#include <set>
#include <string>

using std::cout;
using std::endl;
using std::ostream;
using std::set;
using std::string;

class Student {
public:
    Student(int id, string name) : _id(id), _name(name) {}

    bool operator<(const Student& stu) const { // 只读成员变量, 写成常方法
        return _id < stu._id;
    }

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

    // multiset<Student> set1;
    set<Student> set1; // 存放自定义类型, 需提供 operator< 的重载
    set1.insert(Student(1000, "张雯"));
    set1.insert(Student(1020, "李广"));

    for (auto it = set1.begin(); it != set1.end(); ++it) {
        cout << *it << endl;
    }
    return 0;
}
