#include <cstdio>

int main() {

    const int a = 20;
    int       array[a] = {}; // int array[20] = {}

    int* p = (int*)&a;
    *p = 30; // 已经把 a 的内存的值改为 30 了

    // printf("%d %d %d \n", 20, *p, 20);  // 20 30 20
    printf("%d %d %d \n", a, *p, *(&a)); // 20 30 30

    return 0;
}
