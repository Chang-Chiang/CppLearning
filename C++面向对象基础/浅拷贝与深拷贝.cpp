#include <iostream>
using std::cout;
using std::endl;
class SeqStack {
public:
    /*
    ···
    */
    SeqStack(int size = 10) {
        cout << this << "SeqStack()" << endl;
        _pstack = new int[size]; // 堆内存的开辟
        _top = -1;
        _size = size;
    }

    // 默认的拷贝构造做的是内存的数据拷贝 -> 浅拷贝
    // 浅拷贝不一定有错, 关键是对象如果占用外部资源, 那么浅拷贝就会出现问题
    /*
    SeqStack(const SeqStack &src){
        _pstack = src._pstack;  // 堆内存的开辟
        _top = src._top;
        _size = src._size;
    }
    */

    // 自定义 拷贝构造函数
    SeqStack(const SeqStack& src) {

        cout << "SeqStack(const SeqStack &src)" << endl;

        // 深拷贝
        _pstack = new int[src._size]; // 堆内存的开辟
        for (int i = 0; i <= src._top; ++i) {
            // i <= src._top, 因为 _top 从 -1 开始

            _pstack[i] = src._pstack[i];
        }
        /*
        for 循环实现拷贝, 而不是使用 memcpy 或 realloc 的原因：
        拷贝的数据类型是整型 -> 没有问题
        拷贝的数据类型是类创建的对象, 对象的成员变量中有指针指向外部资源 -> 浅拷贝, 有问题
        */

        _top = src._top;
        _size = src._size;
    }

    // 自定义 赋值重载函数
    SeqStack& operator=(const SeqStack& src) {

        cout << "Soperator=" << endl;

        // 防止自己给自己赋值, 避免将自己释放后内存非法访问
        if (this == &src) {
            return *this;
            // return;
        }

        // 需要先释放当前对象占用的外部资源
        delete[] _pstack;

        _pstack = new int[src._size]; // 堆内存的开辟
        for (int i = 0; i <= src._top; ++i) {
            _pstack[i] = src._pstack[i];
        }

        _top = src._top;
        _size = src._size;

        return *this;
    }

private:
    int* _pstack; // 动态开辟数组, 存储顺序栈的元素
    int  _top;    // 指向栈顶元素的位置
    int  _size;   // 数组扩容的总大小
};

int main() {

    SeqStack s;
    // 没有提供任何构造函数的时候, 编译器会生成默认构造函数、默认析构函数, 是空函数

    SeqStack s1(10);
    SeqStack s2 = s1; // 1 默认拷贝构造函数 -> 做直接内存数据拷贝, 浅拷贝
    // SeqStack s3(s1);  // 2 拷贝构造函数, 与 1 一样

    // s2.operator=(s1);  // 操作符重载
    s2 = s1; // 默认赋值函数 -> 做直接内存数据拷贝, 浅拷贝
}
