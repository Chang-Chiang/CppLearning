#include <memory>
using namespace std;

template <class T>
class scoped_ptr // noncopyable
{
private:
    T* px;

    /*
    私有化拷贝构造函数和赋值函数，这样scoped_ptr的智能指针
    对象就不支持这两种操作，从根本上杜绝浅拷贝的发生
    */
    scoped_ptr(scoped_ptr const&);
    scoped_ptr& operator=(scoped_ptr const&);

    typedef scoped_ptr<T> this_type;

    /*
    私有化逻辑比较运算符重载函数，不支持scoped_ptr的智能指针
    对象的比较操作
    */
    void operator==(scoped_ptr const&) const;
    void operator!=(scoped_ptr const&) const;

public:
    typedef T element_type;
    explicit scoped_ptr(T* p = 0)
        : px(p) // never throws
    {
#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)
        boost::sp_scalar_constructor_hook(px);
#endif
    }

#ifndef BOOST_NO_AUTO_PTR
    /*支持从auto_ptr构造一个scoped_ptr智能指针对象，
    但是auto_ptr因为调用release()函数，导致其内部指
    针为nullptr*/
    explicit scoped_ptr(std::auto_ptr<T> p) BOOST_NOEXCEPT : px(p.release()) {
    #if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)
        boost::sp_scalar_constructor_hook(px);
    #endif
    }

#endif
    /*析构函数，释放智能指针持有的资源*/
    ~scoped_ptr() // never throws
    {
#if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)
        boost::sp_scalar_destructor_hook(px);
#endif
        boost::checked_delete(px);
    }
};
