#include <memory>
using namespace std;

#if 0
template <class _Ty>
class auto_ptr { // wrap an object pointer to ensure destruction
public:
    typedef _Ty element_type;

    explicit auto_ptr(_Ty* _Ptr = nullptr) noexcept
        : _Myptr(_Ptr) { // construct from object pointer
    }

    /*这里是auto_ptr的拷贝构造函数，
    _Right.release()函数中，把_Right的_Myptr
    赋为nullptr，也就是换成当前auto_ptr持有资源地址
    */
    auto_ptr(auto_ptr& _Right) noexcept
        : _Myptr(_Right.release()) { // construct by assuming pointer from _Right auto_ptr
    }

    _Ty* release() noexcept { // return wrapped pointer and give up ownership
        _Ty* _Tmp = _Myptr;
        _Myptr = nullptr;
        return (_Tmp);
    }

private:
    _Ty* _Myptr; // the wrapped object pointer
};
#endif

// 不推荐使用 auto_ptr
// 不推荐在容器中使用 auto_ptr
int main() {
    auto_ptr<int> ptr1(new int);
    auto_ptr<int> ptr2(ptr1);
    /*
    ptr2 指向 ptr1 指向内存
    ptr1 释放, ptr1 = nullptr
    只有最后被拷贝赋值的指针(ptr2)能使用
    */

    *ptr2 = 20;
    // cout << *ptr1 << endl;  // 程序崩溃 ptr1 = nullptr

    return 0;
}
