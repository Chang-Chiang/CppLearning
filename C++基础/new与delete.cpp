#include <cstdlib> // malloc free
#include <new>     // bad_alloc

int main() {

    // C 方式
    int* p = (int*)malloc(sizeof(int));
    // malloc 按字节开辟内存
    // 返回 void*,  需对返回值类型进行强转
    // 只管开辟内存, 不管初始化

    if (p == nullptr) {
        return -1;
    }

    *p = 20;
    free(p);

    // C++ 方式
    try {
        int* p1 = new int(20);
    }
    catch (const std::bad_alloc& e) {
    }

    // int *p1 = new int(20);
    // delete p1;

    // 开辟数组内存
    int* q = (int*)malloc(sizeof(int) * 20);
    if (q == nullptr) {
        return -1;
    }
    free(q);

    // int *q1 = new int[20];  // 不初始化
    int* q1 = new int[20](); // 20 个 int, 初始化为 0
    delete[] q1;

    return 0;
}
