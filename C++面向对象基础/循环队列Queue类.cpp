#include <iostream>

using std::cout;
using std::endl;

class Queue {
public:
    // Queue(int size = 5) {
    //     _pQue = new int[size];
    //     _front = _rear = 0;
    //     _size = size;
    // }

    // Queue(const Queue&) = delete;  // 不使用默认构造函数
    // Queue& operator=(const Queue&) = delete;  // 不使用默认赋值重载函数

    // 普通构造
    Queue(int size = 5) {
        _pQue = new int[size];
        _front = _rear = 0;
        _size = size;
    }

    // 拷贝构造
    Queue(const Queue& src) {
        _size = src._size;
        _front = src._front;
        _rear = src._rear;

        _pQue = new int[_size];
        for (int i = _front; i != _rear; i = (i + 1) % _size) {
            _pQue[i] = src._pQue[i];
        }
    }

    // 赋值重载
    Queue& operator=(const Queue& src) {
        if (this == &src) {
            return *this;
        }

        delete[] _pQue;
        _size = src._size;
        _front = src._front;
        _rear = src._rear;
        _pQue = new int[_size];
        for (int i = _front; i != _rear; i = (i + 1) % _size) {
            _pQue[i] = src._pQue[i];
        }

        return *this;
    }

    // 析构函数
    ~Queue() {
        delete[] _pQue;
        _pQue = nullptr;
    }

    // 入队操作
    void push(int val) {
        if (full()) {
            resize();
        }
        _pQue[_rear] = val;
        _rear = (_rear + 1) % _size;
    }

    // 出队操作
    void pop() {
        if (empty()) {
            return;
        }
        _front = (_front + 1) % _size;
    }

    // 获取队头元素
    int front() { return _pQue[_front]; }

    bool full() { return (_rear + 1) % _size == _front; }
    bool empty() { return _front == _rear; }

private:
    int* _pQue;  // 申请队列的数组空间
    int  _front; // 指示队头的位置
    int  _rear;  // 指示队尾的位置
    int  _size;  // 队列扩容的总大小

    void resize() {
        int* ptmp = new int[2 * _size];
        int  index = 0;
        for (int i = _front; i != _rear; i = (i + 1) % _size) {
            ptmp[index++] = _pQue[i];
        }

        delete[] _pQue;
        _pQue = ptmp;
        _front = 0;
        _rear = index;
        _size *= 2;
    }
};

int main() {
    Queue queue;

    for (int i = 0; i < 20; ++i) {
        queue.push(rand() % 100);
    }

    while (!queue.empty()) {
        cout << queue.front() << " ";
        queue.pop();
    }

    cout << endl;

    Queue queue1 = queue;
    queue1 = queue;

    return 0;
}
