#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::ostream;
using std::istream;

// template<typename T>

class CComplex {
public:
    // 构造, 可以构造三种不同的复数对象 CComplex() CComplex(20) CComplex(30, 30)
    CComplex(int r = 0, int i = 0) : mreal(r), mimage(i) {}

    /* 定义了全局的, 屏蔽局部的
    // 加法运算符的重载函数, 指导编译器怎么做 CComplex 类对象的加法操作
    CComplex operator+(const CComplex &src) {

        // 局部对象, 函数运行完, 栈帧回退, 对象内存交还给系统
        // 不能返回局部对象的指针或引用
        // CComplex comp;
        // comp.mreal = this->mreal + src.mreal;
        // comp.mimage = this->mimage + src.mimage;
        // return comp;

        return CComplex(this->mreal + src.mreal, this->mimage + src.mimage);
    }
    */

    // 后置++，返回的旧值
    CComplex operator++(int) {
        // CComplex comp = *this;
        // mreal += 1;
        // mimage += 1;
        // return comp;  // 局部对象

        return CComplex(mreal++, mimage++);
    }

    // 前置++，返回对象本身
    CComplex& operator++() {
        mreal += 1;
        mimage += 1;
        return *this;
    }

    void operator+=(const CComplex& src) {
        mreal += src.mreal;
        mimage += src.mimage;
    }

    void show() { cout << "real: " << mreal << "image: " << mimage << endl; }

private:
    int mreal;
    int mimage;

    // 定义为友元函数, 可以访问对象私有成员变量
    friend CComplex operator+(const CComplex& lhs, const CComplex& rhs);
    friend ostream& operator<<(ostream& out, const CComplex& src);
    friend istream& operator>>(istream& in, CComplex& src);

}; // 类定义结束

// 定义在类外的函数, 无法访问对象的私有成员变量, 故在类中声明为友元 friend 函数
CComplex operator+(const CComplex& lhs, const CComplex& rhs) {
    return CComplex(lhs.mreal + rhs.mreal, lhs.mimage + rhs.mimage);
}

ostream& operator<<(ostream& out, const CComplex& src) {
    out << "mreal: " << src.mreal << "mimage: " << src.mimage << endl;
    return out;
}

istream& operator>>(istream& in, CComplex& src) {
    in >> src.mreal >> src.mimage;
    return in;
}

int main() {
    CComplex comp1(10, 10);
    CComplex comp2(20, 20);

    // comp1.operator+(comp2)  加法运算符的重载函数
    CComplex comp3 = comp1 + comp2;
    comp3.show();

    CComplex comp4 = comp1 + 20; // comp1.operator+(20)  int -> CComplex(int)
    comp4.show();

    // 编译器做对象运算的时候, 会调用对象的运算符重载函数 (优先调用成员方法)
    // 如果没有成员方法, 就在全局作用域找合适的运算符重载函数
    CComplex comp5 = 30 + comp1;
    comp5.show();

    // operator++()  不带参数, 表示 前置++
    // operator++(int)  带 int 参数, 表示 后置++
    comp5 = comp1++; // ++ -- 单目运算符
    comp1.show();
    comp5.show();

    comp5 = ++comp1;
    comp1.show();
    comp5.show();

    comp1 += comp2;
    comp1.show();
    cout << comp1 << endl; // 输出运算符重载

    cin >> comp1 >> comp2;
    cout << comp1 << comp2 << endl;

    return 0;
}
