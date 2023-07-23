/*
智能指针的删除器 deletor
智能指针: 能够保证资源绝对的释放. 释放资源方式 delete ptr;
但是释放数组 delete[], 释放文件等不能用 delete
删除器: 指导智能指针正确地释放资源
*/

/*
unique_ptr, shared_ptr 的析构函数里面是一个函数对象的调用
智能指针的默认删除器:
template<typename T>
class default_delete {
public:
    void operator()(T *ptr) {
        delete ptr;
    }
};
*/

#include <functional>
#include <iostream>
#include <memory>
using namespace std;

template <typename T>
class MyDeletor {
public:
    void operator()(T* ptr) const {
        cout << "call MyDeletor.operator()" << endl;
        delete[] ptr;
    }
};

template <typename T>
class MyFileDeletor {
public:
    void operator()(T* ptr) const {
        cout << "call MyFileDeletor.operator()" << endl;
        fclose(ptr);
    }
};

int main() {
    // unique_ptr<int> ptr1(new int[100]);  // 默认为 delete, 但需要的是 delete[]
    unique_ptr<int, MyDeletor<int>>       ptr1(new int[100]);
    unique_ptr<FILE, MyFileDeletor<FILE>> pre2(fopen("data.txt", "w"));

    // lamda 表达式重构
    // unique_ptr<int, function<void(int*)>>   ptr1(new int[100], [](int* p) -> void {
    //     cout << "call lamda release new int[100]" << endl;
    //     delete[] p;
    //   });
    // unique_ptr<FILE, function<void(FILE*)>> ptr2(fopen("data.txt", "w"), [](FILE* p) -> void {
    //     cout << "call lamda release new fopen" << endl;
    //     fclose(p);
    // });

    return 0;
}
