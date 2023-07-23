#include <iostream>
#include <thread>

using std::cout;
using std::endl;

void threadHandler1(int time) {

    // 让子线程睡眠 time s
    std::this_thread::sleep_for(std::chrono::seconds(time));
    cout << "hello thread1!" << endl;
}

void threadHandler2(int time) {

    // 让子线程睡眠 time s
    std::this_thread::sleep_for(std::chrono::seconds(time));
    cout << "hello thread2!" << endl;
}

int main() {

    // 创建了一个线程对象, 传入一个线程函数, 新线程就开始运行了
    std::thread t1(threadHandler1, 2);
    std::thread t2(threadHandler2, 3);

    // 主线程等待子线程结束, 主线程继续往下运行
    t1.join();
    t2.join();

    // 把子线程设置为分离线程
    // t1.detach();

    cout << "main thread done!" << endl;

    // 主线程运行完成, 查看如果当前进程还有未运行完成的子线程
    // 线程就会异常终止
    return 0;
}
