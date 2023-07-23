// 模拟车站三个窗口卖票的程序

#include <iostream>
#include <list>
#include <mutex>
#include <thread>

using std::cout;
using std::endl;
using std::list;

int        ticketCount = 100; // 车站有 100 张车票, 三个窗口一起卖票
std::mutex mtx;               // 全局的一把互斥锁

// 模拟卖票的线程函数
void sellTicket(int index) {

    while (ticketCount > 0) { // 锁 + 双重判断

        mtx.lock();
        if (ticketCount > 0) {
            // 临界区代码段, 保证是原子操作, 线程间互斥操作
            cout << "窗口: " << index << "卖出第: " << ticketCount << " 张票!" << endl;
            ticketCount--; // 不是线程安全操作
            // mov eax, ticketCount
            // sub eax, 1
            // mov ticketCount, eax
        }
        mtx.unlock();

        // 模拟卖票时间 100 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    list<std::thread> tlist;
    for (int i = 1; i <= 3; i++) {
        tlist.push_back(std::thread(sellTicket, i));
    }

    for (std::thread& t : tlist) {
        t.join();
    }

    cout << "所有窗口卖票结束!" << endl;

    return 0;
}
