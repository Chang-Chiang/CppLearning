// 模板的非类型参数

#include <iostream>
using std::cout;
using std::endl;

// 冒泡排序
// T 为 模板类型参数, SIZE 为 模板非类型参数
template<typename T, int SIZE>
void sort(T *arr) {
    for (int i = 0; i < SIZE - 1; ++i) {
        for (int j = 0; j < SIZE - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int main() {
    int arr[] = {12, 5, 7, 89, 32, 21, 35};
    const int size = sizeof(arr) / sizeof(arr[0]);
    sort<int, size>(arr);

    for (int val : arr) {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
