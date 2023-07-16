#include <iostream>
#include <cstring>

// 日期类
class CDate {
public:
    // 自定义构造函数, 编译器就不会再产生默认构造函数
    CDate(int y, int m, int d) {
        _year = y;
        _month = m;
        _day = d;
    }
    void show() { std::cout << _year << "/" << _month << "/" << _day << std::endl; }

private:
    int _year;
    int _month;
    int _day;
};

// CDate 信息 是 CGoods 商品信息的一部分 a part of...  组合的关系
class CGoods {

public:
    // "CDate" 没有合适的默认构造函数可用
    CGoods(const char* n, int a, double p, int y, int m, int d)
        : _date(y, m, d)
        ,         // CDate _data(y, m, d);  // 指定对象的构造方式
        _amount(a)
        ,         // int amount = a;  // 定义时初始化
        _price(p) // 1 构造函数的初始化列表, 先执行
    {
        // 2 当前类类型构造函数体, 后执行
        strcpy(_name, n);
        // _amount = a;  // int _amount; _amount = a;  // 先定义后赋值
        // _data = CDate(y, m, d);  // 未指定构造函数形式
    }

    void show() {
        std::cout << "name: " << _name << std::endl;
        std::cout << "amount: " << _amount << std::endl;
        std::cout << "price: " << _price << std::endl;
        _date.show();
    }

private:
    char   _name[20];
    double _price;
    int    _amount;
    CDate  _date; // 成员对象, 对象的生成： 1. 分配内存 2. 调用构造函数
};

int main() {

    CGoods good("商品", 100, 35.0, 2019, 5, 12);
    good.show();

    return 0;
}
