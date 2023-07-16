#include <iostream>
using namespace std;

/*
多个智能指针可以管理同一个资源
带引用计数: 给每一个对象资源, 匹配一个引用计数
*/

// 对资源进行引用计数的类
template <typename T>
class RefCnt {
public:
    RefCnt(T* ptr = nullptr) : mptr(ptr) {
        if (mptr != nullptr) {
            mcount = 1;
        }
    }
    void addRef() { mcount++; }
    int  delRef() { return --mcount; }

private:
    T*  mptr;
    int mcount; // 非线程安全, 实际库里面定义类型为 atomic_int CAS
};

template <typename T>
class CSmartPtr { // 类似于库里的 shared_ptr
public:
    CSmartPtr(T* ptr = nullptr) : mptr(ptr) { mpRefCnt = new RefCnt<T>(mptr); }
    ~CSmartPtr() {
        if (0 == mpRefCnt->delRef()) {
            delete mptr;
            mptr = nullptr;
        }
    }

    T& operator*() { return *mptr; }
    T* operator->() { return mptr; } // (ptr.operator->())->test();

    CSmartPtr(const CSmartPtr<T>& src) : mptr(src.mptr), mpRefCnt(src.mpRefCnt) {
        if (mptr != nullptr) {
            mpRefCnt->addRef();
        }
    }

    CSmartPtr<T>& operator=(const CSmartPtr<T>& src) {
        if (this == &src) {
            return *this;
        }
        if (0 == mpRefCnt->delRef()) {
            delete mptr;
        }
        mptr = src.mptr;
        mpRefCnt = src.mpRefCnt;
        mpRefCnt->addRef();
        return *this;
    }

private:
    T*         mptr;     // 指向资源的指针
    RefCnt<T>* mpRefCnt; // 指向该资源引用计数对象的指针
};

int main() {
    CSmartPtr<int> ptr1(new int);
    CSmartPtr<int> ptr2(ptr1);
    CSmartPtr<int> ptr3;
    ptr3 = ptr2;
    *ptr1 = 20;
    cout << *ptr2 << " " << *ptr3 << endl;
    return 0;
}
