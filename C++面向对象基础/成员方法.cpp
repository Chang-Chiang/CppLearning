#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

class CDate {
public:
    CDate(int y, int m, int d) {
        _year = y;
        _month = m;
        _day = d;
    }
    void show() const { cout << _year << "/" << _month << "/" << _day << endl; }

private:
    int _year;
    int _month;
    int _day;
};

class CGoods {

public:
    CGoods(const char* n, int a, double p, int y, int m, int d)
        : _date(y, m, d), _amount(a), _price(p) {
        strcpy(_name, n);
        _count++; // 记录所有产生的新对象的数量
    }

    // 普通的成员方法, 打印商品的私有信息, CGoods *this
    void show() {
        cout << "name: " << _name << endl;
        cout << "amount: " << _amount << endl;
        cout << "price: " << _price << endl;
        _date.show();
    }

    // 重载关系
    // 常成员方法, const CGoods *this
    // 只要是只读操作的成员方法, 一律实现成 const 常成员方法
    void show() const {
        cout << "name: " << _name << endl;
        cout << "amount: " << _amount << endl;
        cout << "price: " << _price << endl;
        _date.show();
    }

    // static 静态成员方法, 打印所有商品共享的信息, 没有 this 指针
    static void showCGoodsCount() { cout << "所有商品的种类数量是：" << _count << endl; }

private:
    char   _name[20];
    double _price;
    int    _amount;
    CDate  _date;

    static int _count;
    /*
    声明, 用来记录商品对象的总数量
    不属于对象, 而是属于类级别
    .bss 段
    */
};

// static 成员变量一定要在类外进行定义并且初始化
int CGoods::_count = 0;

int main() {

    // 普通成员方法
    CGoods good1("商品1", 100, 35.0, 2019, 5, 12);
    good1.show();

    // CGoods good1("商品1", 100, 35.0, 2019, 5, 12);
    // good1.show();

    // 静态成员方法, 用类名调用
    CGoods::showCGoodsCount(); // 统计所有商品的总数量

    const CGoods good3("非卖商品3", 100, 35.0, 2019, 5, 12);
    good3.show();
    /*
    实参 const CGoods* -> 形参 const CGoods*
    */
}
