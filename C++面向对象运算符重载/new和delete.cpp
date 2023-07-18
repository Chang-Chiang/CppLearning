#include <iostream>
#include <new>

using std::bad_alloc;
using std::cin;
using std::cout;
using std::endl;

// 先调用 opeartor new 开辟空间
// 再调用对象的构造函数 (初始化)
void* operator new(size_t size) {
    void* p = malloc(size);
    if (p == nullptr) {
        throw bad_alloc();
    }
    cout << "operator new addr: " << p << endl;
    return p;
}

// delete p
// 先调用 p 指向对象的析构函数
// 再调用 operator delete 释放内存空间
void operator delete(void* ptr) {
    cout << "operator delete addr: " << ptr << endl;
    free(ptr);
}

void* operator new[](size_t size) {
    void* p = malloc(size);
    if (p == nullptr) {
        throw bad_alloc();
    }
    cout << "operator new[] addr: " << p << endl;
    return p;
}

void operator delete[](void* ptr) {
    cout << "operator delete[] addr: " << ptr << endl;
    free(ptr);
}

class Test {
public:
    Test(int data = 10) { cout << "Test()" << endl; }
    ~Test() { cout << "~Test()" << endl; }

private:
    int ma;
};

int main() {
    try {
        int* p = new int;
        delete p;
        int* q = new int[10];
        delete[] q;

        Test* p1 = new Test();
        delete p1;
        Test* p2 = new Test[5];
        cout << "p2: " << p2 << endl;
        delete[] p2;
    }

    catch (const bad_alloc& err) {
        cout << err.what() << endl;
    }

    return 0;
}
