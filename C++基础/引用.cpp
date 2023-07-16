int main() {
    int  a = 10; // 左值, 它有内存, 有名字, 值可以修改
    int& b = a;  // lea eax, [a]
                 // mov dword ptr[b], eax

    // int &c = 20;  // 错误 20 是右值：没内存 (立即数, 放在 CPU 寄存器) , 没名字

    // C++11 提供了右值引用
    // c 可以作为左值, 允许 c = 30; 修改临时量内存
    int&& c = 20; // mov dword ptr[ebp-30h], 14h  # 20 放在一个临时量内存
                  // lea eax, [ebp-30h]
                  // mov dword ptr[c], eax

    int& e = c;   // 一个右值引用变量, 本身是一个左值

    // d 不能作为左值, 禁止 d = 30; 不允许修改临时量内存
    const int& d = 20; // 允许
    /* 相当于
    int temp = 20;
    temp 地址存到 d
    */

    return 0;
}
