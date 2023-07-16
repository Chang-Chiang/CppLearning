// VS Code 查看汇编, 在 调试控制台输入: -exec disassemble /m main
#include <iostream>
using namespace std;

int sum(int a = 10, int b = 20) { return a + b; }

int main() {
    int a = 10; // mov dword ptr[ebp-4], 0Ah
    int b = 20; // mov dword ptr[ebp-8], 14h

    int ret = sum(a, b);
    /*
    mov eax, dword ptr[ebp-8]
    push eax
    mov ecx, dword ptr[ebp-4]
    push ecx
    call sum
    */

    // int ret = sum(a);
    /*
    push 14H  # 被调函数带默认参数汇编少一条数据传送指令(mov)
    mov ecx, dword ptr[ebp-4]
    push ecx
    call sum
    */

    // int ret = sum();
    /*
    push 14H
    push 0AH
    call sum
    */

    // int ret = sum(a, 40);
    /*
    push 28H  # 传立即数, 相较于传变量, 效率提高
    mov ecx, dword ptr[ebp-4]
    push ecx
    call sum
    */

    cout << "ret: " << ret << endl;

    return 0;
}
