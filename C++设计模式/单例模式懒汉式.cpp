// 懒汉式单例模式
class Singleton {
public:
    // #3 获取类的唯一实例对象的接口方法
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

private:
    static Singleton* instance; // #2 定义一个唯一的类的实例对象

    // #1 构造函数私有化
    Singleton() {}

    // 删除默认拷贝构造, =赋值操作符重载
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

Singleton* Singleton::instance = nullptr; // 静态成员变量在类外初始化

int main() {
    Singleton* p1 = Singleton::getInstance();
    Singleton* p2 = Singleton::getInstance();
    Singleton* p3 = Singleton::getInstance();

    return 0;
}
