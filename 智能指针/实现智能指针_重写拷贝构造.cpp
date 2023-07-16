template <typename T>
class CSmartPtr {
public:
    CSmartPtr(T* ptr = nullptr) : mptr(ptr) {}
    ~CSmartPtr() { delete mptr; }

    /*
    解决浅拷贝， 重新开辟空间
    */
    CSmartPtr(const CSmartPtr<T>& src) { mptr = new T(*src.mptr); }

    T&       operator*() { return *mptr; }
    const T& operator*() const { return *mptr; }

    T*       operator->() { return mptr; } // (ptr.operator->())->test();
    const T* operator->() const { return mptr; }

private:
    T* mptr;
};

int main() {
    CSmartPtr<int> ptr1(new int);
    CSmartPtr<int> ptr2(ptr1);

    *ptr1 = 20;
    *ptr2 = 30;
    // 需求: ptr1, ptr2管理同一块资源
    // 但目前新写的拷贝构造 是两块资源

    return 0;
}
