// 生产者消费者线程模型

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue> // C++ STL 所有的容器都不是线程安全的
#include <thread>

std::mutex              mtx; // 定义 互斥锁, 做线程间的互斥操作
std::condition_variable cv;  // 定义条件变量, 做线程间的通信

// 生产者生产一个物品, 通知消费者消费一个
// 消费完了, 消费者再通知生产者继续生产物品
class Queue {
public:
    // 生产物品
    void put(int val) {
        std::unique_lock<std::mutex> lck(mtx);

        while (!que.empty()) {
            // que 不为空, 生产者应该通知消费者去消费, 消费完了, 再继续生产
            // 生产者线程进入等待状态, 并且把 mtx 互斥锁释放掉
            cv.wait(lck);
        }
        que.push(val);

        /*
        notify_one: 通知另外的一个线程
        notify_all: 通知其他所有线程
        其他线程得到该通知, 就会从等待状态变为阻塞状态, 获取互斥锁才能继续执行
        */
        cv.notify_all();

        std::cout << "生产者 生产: " << val << " 号物品" << std::endl;
    };

    // 消费物品
    int get() {
        std::unique_lock<std::mutex> lck(mtx);

        while (que.empty()) {
            // 消费者线程发现 que 为空, 通知生产者线程先生产物品
            // 进入等待状态, 把 mtx 互斥锁释放
            cv.wait(lck);
        }

        int val = que.front();
        que.pop();

        cv.notify_all(); // 通知其他线程, 消费结束, 继续生产

        std::cout << "消费者 消费: " << val << " 号物品" << std::endl;
        return val;
    }

private:
    std::queue<int> que;
};

// 生产者线程
void producer(Queue* que) {
    for (int i = 1; i <= 10; ++i) {
        que->put(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// 消费者线程
void consumer(Queue* que) {
    for (int i = 1; i <= 10; ++i) {
        que->get();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {

    Queue       que; // 两个线程共享的队列
    std::thread t1(producer, &que);
    std::thread t2(consumer, &que);

    t1.join();
    t2.join();
    return 0;
}
