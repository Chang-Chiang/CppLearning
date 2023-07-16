/**
 * @file 内联函数.cpp
 * @author Chang Chiang (Chang_Chiang@outlook.com)
 * @brief inline 内联函数，在编译过程中，没有函数的调用开销，在函数的调用点把函数的代码进行展开处理了
 * inline 内联函数不再生成相应的函数符号
 * @version 0.1
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <iostream>

int sum(int a = 10, int b = 20) { return a + b; }

inline int sum_inline(int a = 10, int b = 20) { return a + b; }

int main() {
    int a = 10;
    int b = 20;

    int ret = sum(a, b);
    // 此处有标准的函数调用过程：参数压栈、函数栈帧的开辟和回退过程
    // 有函数调用的开销

    int ret = sum_inline(a, b);

    std::cout << "ret: " << ret << std::endl;

    return 0;
}
