#include <memory>
using namespace std;

#if 0
// = default_delete<_Ty>
template <class _Ty, class _Dx>
class unique_ptr : public _Unique_ptr_base<_Ty, _Dx> { // non-copyable pointer to an object
public:
    typedef _Unique_ptr_base<_Ty, _Dx> _Mybase;
    typedef typename _Mybase::pointer  pointer;
    typedef _Ty                        element_type;
    typedef _Dx                        deleter_type;

    /*提供了右值引用的拷贝构造函数*/
    unique_ptr(unique_ptr&& _Right) noexcept
        : _Mybase(
              _Right.release(),
              _STD forward<_Dx>(_Right.get_deleter())) { // construct by moving _Right
    }

    /*提供了右值引用的operator=赋值重载函数*/
    unique_ptr& operator=(unique_ptr&& _Right) noexcept { // assign by moving _Right
        if (this != _STD addressof(_Right)) {             // different, do the move
            reset(_Right.release());
            this->get_deleter() = _STD forward<_Dx>(_Right.get_deleter());
        }
        return (*this);
    }

    /*
    交换两个unique_ptr智能指针对象的底层指针
    和删除器
    */
    void swap(unique_ptr& _Right) noexcept { // swap elements
        _Swap_adl(this->_Myptr(), _Right._Myptr());
        _Swap_adl(this->get_deleter(), _Right.get_deleter());
    }

    /*通过自定义删除器释放资源*/
    ~unique_ptr() noexcept { // destroy the object
        if (get() != pointer()) {
            this->get_deleter()(get());
        }
    }

    /*unique_ptr提供->运算符的重载函数*/
    _NODISCARD pointer operator->() const noexcept { // return pointer to class object
        return (this->_Myptr());
    }

    /*返回智能指针对象底层管理的指针*/
    _NODISCARD pointer get() const noexcept { // return pointer to object
        return (this->_Myptr());
    }

    /*提供bool类型的重载，使unique_ptr对象可以
    直接使用在逻辑语句当中，比如if,for,while等*/
    explicit operator bool() const noexcept { // test for non-null pointer
        return (get() != pointer());
    }

    /*功能和auto_ptr的release函数功能相同，最终也是只有一个unique_ptr指针指向资源*/
    pointer release() noexcept { // yield ownership of pointer
        pointer _Ans = get();
        this->_Myptr() = pointer();
        return (_Ans);
    }

    /*把unique_ptr原来的旧资源释放，重置新的资源_Ptr*/
    void reset(pointer _Ptr = pointer()) noexcept { // establish new pointer
        pointer _Old = get();
        this->_Myptr() = _Ptr;
        if (_Old != pointer()) {
            this->get_deleter()(_Old);
        }
    }
    /*
    删除了unique_ptr的拷贝构造和operator=赋值函数，
    因此不能做unique_ptr智能指针对象的拷贝构造和
    赋值，防止浅拷贝的发生
    */
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
};
#endif

// 示例2
unique_ptr<int> test_uniqueptr() {
    unique_ptr<int> ptr1(new int);
    return ptr1;
}

int main() {

    // 示例1
    unique_ptr<int> ptr(new int);
    unique_ptr<int> ptr2 = std::move(ptr); // 使用了右值引用的拷贝构造
    ptr2 = std::move(ptr);                 // 使用了右值引用的operator=赋值重载函数

    /*
    此处调用test_uniqueptr函数，在return ptr1代码处，
    调用右值引用的拷贝构造函数，由ptr1拷贝构造ptr
    */
    unique_ptr<int> ptr = test_uniqueptr();
    return 0;
}
