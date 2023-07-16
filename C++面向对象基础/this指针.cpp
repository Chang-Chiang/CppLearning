#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

const int NAME_LEN = 20;

// 商品的抽象数据类型
class CGoods { // 类名首字母大写, 驼峰命名, 首字母 C 表示是 class

    // 给外部提供公有的成员方法, 来访问私有的属性

public:
    // 类体内实现的方法, 自动处理为 inline

    // 商品数据初始化
    void init(const char* name, double price, int amount);

    // 打印商品信息
    void show();

    // 方法名首字母小写, 驼峰式命名
    void setName(char* name) { strcpy(_name, name); }
    void setPrice(double price) { _price = price; }
    void setAmount(int amount) { _amount = amount; }

    const char* getName() { return _name; }
    double      getPrice() { return _price; }
    int         getAmount() { return _amount; }

private:
    // 属性一般都是私有的成员变量
    char   _name[NAME_LEN];
    double _price;
    int    _amount;
}; // 注意类定义结束后的分号

void CGoods::init(const char* name, double price, int amount) {
    strcpy(_name, name);
    _price = price;
    _amount = amount;
}

void CGoods::show() {
    cout << "name: " << _name << endl;
    cout << "price: " << _price << endl;
    cout << "amount: " << _amount << endl;
}

int main() {
    /*
    CGoods 可以定义无数的对象, 每一个对象都有自己的成员变量
    但是它们共享一套成员方法
    成员方法怎么知道处理哪个对象的信息？
    类的成员方法一经编译, 所有的方法参数, 都会加一个 this 指针, 接收调用该方法的对象的地址
    */
    CGoods good1; // 类实例化了一个对象

    /*
    vs cmd 命令 cl *.cpp /d1reportSingleClassLayoutCGoods 查看对象内存大小
    */

    good1.init("面包", 10.0, 200);
    good1.show();

    good1.setPrice(20.5);
    good1.setAmount(100);
    good1.show();

    return 0;
}
