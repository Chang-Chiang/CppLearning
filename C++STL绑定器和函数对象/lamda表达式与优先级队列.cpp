#include <functional>
#include <queue>
#include <vector>

using std::function;
using std::priority_queue;
using std::vector;

// 优先级队列

class Data {
public:
    Data(int val1 = 10, int val2 = 10) : ma(val1), mb(val2) {}
    // bool operator>(const Data &data)const { return ma > data.ma; }
    // bool operator<(const Data &data)const { return ma < data.ma; }
    int ma;
    int mb;
};

int main() {
    // priority_queue<Data> queue;
    // queue.push(Data(10, 20));  // 需定义 < 运算符重载
    // queue.push(Data(15, 15));
    // queue.push(Data(20, 10));

    using FUNC = function<bool(Data&, Data&)>;
    priority_queue<Data, vector<Data>, FUNC> maxHeap(
        [](Data& d1, Data& d2) -> bool { return d1.ma > d2.ma; });
    maxHeap.push(Data(10, 20)); // 需定义 < 运算符重载
    maxHeap.push(Data(15, 15));
    maxHeap.push(Data(20, 10));

    return 0;
}
