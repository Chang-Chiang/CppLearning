# C++ 语言级别的多线程编程

好处: 代码可以跨平台

+ thread/mutex/condition_variable
+ lock_quard/unique_lock
+ atomic 基于 CAS 操作的原子类型, 线程安全的
+ sleep_for

## thread 类编写 C++ 多线程程序

+ 怎么创建启动一个线程

  ```std::thread```定义一个线程对象, 传入线程所需要的线程函数和参数, 线程自动开启

+ 子线程如何结束

  子线程函数运行完成, 线程就结束了

+ 主线程如何处理子线程

  + t.join() 等待 t 线程结束, 当前线程继续往下运行
  + t.detach() 把 t 线程设置为分离线程, 主线程结束, 整个进程结束, 所有子线程都自动结束

```c++
void threadHandler1(int time) {
    
    // 让子线程睡眠 2 s
    std::this_thread::sleep_for(std::chrono::seconds(time));
    cout << "hello thread1!" << endl;
}

void threadHandler2(int time) {
    
    // 让子线程睡眠 2 s
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
```

## 线程间互斥

> 模拟车站三个窗口卖票的程序

多线程程序, 竞态条件: 多线程程序执行的结果是一致的, 不会随着 CPU 对线程不同的调用顺序, 而产生不同的运行结果

- 问题代码一

  代码问题分析：加锁位置有问题，第一个线程加锁后进入 while 循环，其他线程无法获取锁阻塞，导致只有一个窗口能卖票

  ```c++
  // 模拟卖票的线程函数
  void sellTicket(int index) {
      mtx.lock();
      while (ticketCount > 0) {
          // 临界区代码段, 保证是原子操作, 线程间互斥操作
          cout << "窗口: " << index << "卖出第: " << ticketCount << "张票!" << endl;
          // cout << ticketCount << endl;
          ticketCount--;  // 不是线程安全操作
          // mov eax, ticketCount
          // sub eax, 1
          // mov ticketCount, eax
  
          // 模拟卖票时间 100 ms
          std::this_thread::sleep_for(std::chrono::milliseconds(100));  
  
      }
      mtx.unlock();          
  }
  ```

- 问题代码二

  修改加锁位置

  代码问题分析：只剩一张票时，即 ticketCount == 1，线程①进入 while 循环后加锁卖票，此时另一个线程②也进入 while 循环尝试获取锁阻塞，前面线程①卖完票后释放锁，后面线程②获取锁开始卖票，但现在 ticketCount == 0

  ```c++
  // 模拟卖票的线程函数
  void sellTicket(int index) {
      
      while (ticketCount > 0) {
          mtx.lock();
          // 临界区代码段, 保证是原子操作, 线程间互斥操作
          cout << "窗口: " << index << "卖出第: " << ticketCount << "张票!" << endl;
          // cout << ticketCount << endl;
          ticketCount--;  // 不是线程安全操作
          // mov eax, ticketCount
          // sub eax, 1
          // mov ticketCount, eax
  
          mtx.unlock();  
          // 模拟卖票时间 100 ms
          std::this_thread::sleep_for(std::chrono::milliseconds(100));  
      }            
  }
  ```

- 完整代码

    ```c++
    // 模拟车站三个窗口卖票的程序
    
    #include <iostream>
    #include <thread>
    #include <list>
    #include <mutex>
    
    int ticketCount = 100;  // 车站有 100 张车票, 三个窗口一起卖票
    std::mutex mtx;  // 全局的一把互斥锁
    
    // 模拟卖票的线程函数
    void sellTicket(int index) {
    
        while (ticketCount > 0) {  // 锁 + 双重判断
    
            mtx.lock()
            if (ticketCount > 0) {
                // 临界区代码段, 保证是原子操作, 线程间互斥操作
                cout << "窗口: " << index << "卖出第: " << ticketCount << "张票!" << endl;
                // cout << ticketCount << endl;
                ticketCount--;  // 不是线程安全操作
                // mov eax, ticketCount
                // sub eax, 1
                // mov ticketCount, eax
    
            }
            mtx.unlock()
    
            // 模拟卖票时间 100 ms
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  
        }   
    }
    
    int main() {
        list<std::thread> tlist;
        for (int i = 1; i <= 3; i++) {
            tlist.push_back(std::thread(sellTicket, i));
        }
    
        for (std::thread &t : tlist) {
            t.join();
        }
    
        cout << "所有窗口卖票结束!" << endl;
    
        return 0;
    }
    ```

## lock_guard 与 unique_lock 

```c++
// 模拟卖票的线程函数
void sellTicket(int index) {
    
    while (ticketCount > 0) {  // 锁 + 双重判断
        
        {
            lock_guard<std::mutex> lock(mtx);  // 栈上的局部对象, 出作用域析构。 类似scoped_ptr
            // 保证所有线程都能释放锁, 防止死锁问题的发生
            
            // unique_lock<std::mutex> lock(mtx);  // 类似unique_ptr
                
            if (ticketCount > 0) {
                cout << "窗口: " << index << "卖出第: " << ticketCount << "张票!" << endl;
                ticketCount--;  // 不是线程安全操作

            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));  
    }   
}
```

## 线程间同步通信

多线程编程两个问题：

+ 线程间的互斥: 竞态条件, 发生竞态条件的代码段称为临界区代码段, 临界区代码保证原子操作, 利用互斥锁 mutex 或 轻量级的无锁实现 CAS
+ 线程间的同步通信

```c++
// 生产者消费者线程模型

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>  // C++ STL 所有的容器都不是线程安全的

std::mutex mtx;  // 定义 互斥锁, 做线程间的互斥操作
std::condition_variable cv;  // 定义条件变量, 做线程间的通信

// 生产者生产一个物品, 通知消费者消费一个
// 消费完了, 消费者再通知生产者继续生产物品
class Queue {
public:
    // 生产物品
    void put(int val) {

        unique_lock<std::mutex> lck(mtx);
        
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
        
        cout << "生产者 生产: " << val << " 号物品" << endl;
    }
    
    // 消费物品
    int get() {

        unique_lock<std::mutex> lck(mtx);
        
        while (que.empty()) {
            // 消费者线程发现 que 为空, 通知生产者线程先生产物品
            // 进入等待状态, 把 mtx 互斥锁释放
            cv.wait(lck)
        }
        
        int val = que.front();
        que.pop();
        
        cv.notify_all();  // 通知其他线程, 消费结束, 继续生产
        
        cout << "消费者 消费: " << val << " 号物品" << endl;
        return val;
    }
private:
    queue<int> que;
};

// 生产者线程
void producer(Queue *que) {
    for (int i = 1; i <= 10; ++i) {
        que->put(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }
}

// 消费者线程
void consumer(Queue *que) {
    for (int i = 1; i <= 10; ++i) {
        que->get();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    }
}

int main() {
    
    Queue que;  // 两个线程共享的队列
    std::thread t1(producer, &que);
    std::thread t2(consumer, &que);
    
    t1.join();
    t2.join();
    return 0;
}
```

## unique_lock, condition_variable 总结

+ lock_guard 和 unique_lock
+ condition_variable, wait, notify_all

```c++
std::mutex mtx;
std::condition_variable cv;

int main() {
    mtx.lock();
    mtx.unlock();
    
    {
        // 不可能用在函数参数传递或者返回过程中
        // lock_guard 不能进行拷贝构造、赋值
        // 只能用于简单的临界区代码段的互斥操作中
        lock_guard<std::mutex> guard(mtx);
    }
    
    // 不仅可以用于简单的临界区代码段的互斥操作中
    // 还能用于函数调用过程中
    unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);  // 1. 使线程进入等待状态 2. lck.unlock() 把互斥锁 mtx 释放
    
    // ----------------------------
    
    // 通知在 cv 上等待的线程, 条件成立了, 起来干活了
    // 其他在 cv 上等待的线程, 从等待状态变为阻塞状态, 获取互斥锁后, 线程继续往下执行
    cv.notify_all();
        
    return 0;
}
```

## 原子类型

- 互斥锁比较重，适用于临界区代码做的事情复杂、多 的情况
- 系统理论上，用 CAS 保证 ++、-- 操作的原子特性就足够（也叫无锁操作）

```c++
#include <iostream>
#include <thread>
#include <atomic>  // 包含了很多原子类型
#include <list>

// volatile 防止多线程对于共享变量进行缓存
volatile std::atomic_bool isReady = false;
volatile std::atomic_int mycount = 0;

void task() {
    while (!isReady) {
        std::this_thread::yield();  // 线程让出当前的 CPU 时间片, 等待下一次调度
    }
    
    for (int i = 0; i < 100; ++i) {
        count++;
    }
}

int main() {
    list<std::thread> tlist;
    
    for (int i = 0; i < 10; ++i) {
        tlist.push_back(std::thread(task));
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(3)); 
    
    cout << "count: " << mycount << endl;
    isReady = true;
    for (std::thread &t : tlist) {
        t.join();
    }
    
    return 0;
}
```
