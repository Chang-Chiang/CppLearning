int main() {

    // 写一句代码, 在内存的 0x0018ff44 处写一个 4 字节的 10
    // int *p = (int*)0x0018ff44;  // 指针变量
    // int *&&p = (int*)0x0018ff44;  // 引用变量
    // int *const &p = (int*)0x0018ff44;  // 引用变量

    int  a = 10;
    int* p = &a;
    // int **q = &p;  // 二级指针
    // int *&q = p;  // 引用变量

    // 判断下面语句对错
    // const int*& q = p; // 错误，typeid(q).name()
    // 将引用变量还原至指针进行判断
    // const int **q = &p;  // const int ** <- int ** 错误

    return 0;
}
