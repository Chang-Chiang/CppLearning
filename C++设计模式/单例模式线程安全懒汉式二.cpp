// 线程安全的懒汉单例模式 【2】
class Singleton {
public:
    static Singleton* getInstance() {
        // g++ -o 单例模式 单例模式.cpp -g
        // gdb 单例模式
        // 函数静态局部变量的初始化, 在汇编指令上已经自动添加线程互斥指令了
        static Singleton instance;
        return &instance;
    }

private:
    Singleton() {}

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};
