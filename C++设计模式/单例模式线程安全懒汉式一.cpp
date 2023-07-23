// 线程安全的懒汉单例模式 【1】

#include <mutex>

std::mutex mtx;

// 懒汉式单例模式, 是不是线程安全的 ?
class Singleton {
public:
    /*
    // 是不是可重入函数 ? 不是可重入函数 !
    static Singleton* getInstance() {
        if (instance == nullptr) {

            // 开辟内存
            // 给 instance 赋值
            // 构造对象
            instance = new Singleton();
        }
        return &instance;
    }
    */

    static Singleton* getInstance() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> guard(mtx);
            if (instance == nullptr) {
                instance = new Singleton();
            }
        }
        return instance;
    }

private:
    // static Singleton *instance;
    static Singleton* volatile instance;
    Singleton() {}

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

Singleton* volatile Singleton::instance = nullptr;
