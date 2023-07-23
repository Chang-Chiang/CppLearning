// 饿汉式单例模式
// 一定是线程安全的
// 缺点: 一定会调用构造函数, 构造操作太多时, 资源浪费
class Singleton {
public:
    // #3 获取类的唯一实例对象的接口方法
    static Singleton* getInstance() { return &instance; }

private:
    static Singleton instance; // #2 定义一个唯一的类的实例对象

    // #1 构造函数私有化
    Singleton() {}

    // 删除默认拷贝构造, =赋值操作符重载
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

// 静态成员变量，在数据段
Singleton Singleton::instance; // 静态成员变量在类外定义

int main() {
    Singleton* p1 = Singleton::getInstance();
    Singleton* p2 = Singleton::getInstance();
    Singleton* p3 = Singleton::getInstance();

    return 0;
}
