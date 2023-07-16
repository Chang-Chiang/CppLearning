#include <iostream>

// 【1】全局变量, 都叫做数据
// 编译后在符号表中都会产生符号
int gdata1 = 10; // 初始化, 不为 0, 存放在 .data 段
int gdata2 = 0;  // 初始化, 为 0, 存放在 .bss 段
int gdata3;      // 未初始化, 存放在 .bss 段

// 【2】静态全局变量
static int gdata4 = 11; // 初始化, 不为 0, 存放在 .data 段
static int gdata5 = 0;  // 初始化, 为 0, 存放在 .bss 段
static int gdata6;      // 未初始化, 存放在 .bss 段

int main() {

    // 【3】局部变量, 对应产生指令
    // 编译后不会在符号表中产生符号
    int a = 12; // 产生指令 mov dword ptr[a], 0Ch  指令存放在 .text 段
                // 指令运行时, 在栈上开辟空间, 存放 a
    int b = 0;
    int c;

    // 【4】静态局部变量, 存放在数据段
    // 程序启动时不会初始化
    // 第一次运行至该点时才会初始化
    static int e = 13;           // 初始化, 不为 0, 存放在 .data 段
    static int f = 0;            // 初始化, 为 0, 存放在 .bss 段
    static int g;                // 未初始化, 存放在 .bss 段

    std::cout << c << std::endl; // 不为 0, 输出为栈上的无效值
    std::cout << g << std::endl; // 一定为 0, 内核会将 .bss 段内容置 0

    return 0;
}
