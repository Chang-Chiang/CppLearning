#include <atomic> // 包含了很多原子类型
#include <iostream>
#include <list>
#include <thread>

// volatile 防止多线程对于共享变量进行缓存
volatile std::atomic_bool isReady = false;
volatile std::atomic_int  mycount = 0;

void task() {
    while (!isReady) {
        std::this_thread::yield(); // 线程让出当前的 CPU 时间片, 等待下一次调度
    }

    for (int i = 0; i < 100; ++i) {
        mycount++;
    }
}

int main() {
    std::list<std::thread> tlist;

    for (int i = 0; i < 10; ++i) {
        tlist.push_back(std::thread(task));
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    isReady = true;
    for (std::thread& t : tlist) {
        t.join();
    }

    std::cout << "count: " << mycount << std::endl;

    return 0;
}
