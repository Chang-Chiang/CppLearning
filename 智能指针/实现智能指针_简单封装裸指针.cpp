#include <iostream>
using namespace std;

template <typename T>
class CSmartPtr {
public:
    CSmartPtr(T* ptr = nullptr) : mptr(ptr) {}
    ~CSmartPtr() { delete mptr; }
    T&       operator*() { return *mptr; }
    const T& operator*() const { return *mptr; }
    T*       operator->() { return mptr; } // (ptr.operator->())->test();
    const T* operator->() const { return mptr; }

private:
    T* mptr;
};

int main() {
    CSmartPtr<int> ptr(new int);
    /*其它的代码...*/
    *ptr = 20;
    cout << *ptr << endl;

    /*
    由于ptr是栈上的智能指针对象，
    不管是函数正常执行完，
    还是运行过程中出现异常，
    栈上的对象都会自动调用析构函数，
    在析构函数中进行了 delete 操作，保证释放资源
    */
    return 0;
}

// 问：智能指针能否定义在堆上？
// CSmartPtr<int> *p = new CSmartPtr<int>(new int);  // p 是裸指针, 无意义
