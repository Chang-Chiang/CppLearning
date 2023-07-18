#include <iostream>

using std::cout;
using std::endl;

template <typename T>
class Queue {
public:
    // 构造函数
    Queue() { _front = _rear = new QueueItem(); }

    // 析构函数
    ~Queue() {
        QueueItem* cur = _front;
        while (cur != nullptr) {
            _front = _front->_next;
            delete cur;
            cur = _front;
        }
    }

    // 入队操作
    void push(const T& val) {
        QueueItem* item = new QueueItem(val);
        _rear->_next = item;
        _rear = item;
    }

    // 出队
    void pop() {
        if (empty()) {
            return;
        }
        QueueItem* first = _front->_next;
        _front->_next = first->_next;
        if (_front->_next == nullptr) {
            _rear = _front;
        }
        delete first;
    }

    // 查看队头
    T front() const { return _front->_next->_data; }

    bool empty() const { return _front == _rear; }

private:
    // 【问题解决】产生一个 QueueItem 的对象池 (10000 个 QueueItem 节点)
    struct QueueItem {
        // 零构造，默认构造
        QueueItem(T data = T()) : _data(data), _next(nullptr) {}

        // 给 QueueItem 提供自定义内存管理
        void* operator new(size_t size) {
            if (_itemPool == nullptr) {
                _itemPool = (QueueItem*)new char[POOL_ITEM_SIZE * sizeof(QueueItem)];
                QueueItem* p = _itemPool;
                for (; p < _itemPool + POOL_ITEM_SIZE - 1; ++p) {
                    p->_next = p + 1;
                }
                p->_next = nullptr;
            }
            QueueItem* p = _itemPool;
            _itemPool = _itemPool->_next;
            return p;
        }
        void operator delete(void* ptr) {
            QueueItem* p = (QueueItem*)ptr;
            p->_next = _itemPool;
            _itemPool = p;
        }
        T                 _data;
        QueueItem*        _next;
        static QueueItem* _itemPool; // 指向对象池起始地址
        static const int  POOL_ITEM_SIZE = 100000;
    };

    QueueItem* _front; // 指向头节点
    QueueItem* _rear;  // 指向队尾
};

// 静态成员变量 _itemPool 定义
// typename 告诉编译器, 嵌套作用域下的名字 (QueueItem) 是一个类型
template <typename T>
typename Queue<T>::QueueItem* Queue<T>::QueueItem::_itemPool = nullptr;

int main() {
    Queue<int> que;

    // 【问题】短时间内大量调用 malloc, free
    for (int i = 0; i < 1000000; ++i) {
        que.push(i);
        que.pop();
    }

    cout << que.empty() << endl;

    return 0;
}
