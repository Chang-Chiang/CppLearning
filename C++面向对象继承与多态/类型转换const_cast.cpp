// const_cast : 去掉常量属性的一个类型转换
int main() {
    const int a = 10;

    int* p1 = (int*)&a; // C 风格
    // char *p1 = (char*)&a;  // 允许
    // double *p1 = (double*)&a;  // 允许, 引发错误：内存实际 4 个字节有效, 指针实际解引用 8 个字节

    int* p2 = const_cast<int*>(&a); // C++ 风格
    // char *p2 = const_cast<char*>(&a);  // 禁止
    // double *p2 = const_cast<double*>(&a);  // 禁止

    // const_cast<这里面必须是指针或引用类型 eg. int*, int& >
    // int b = const_cast<int>(a);  // 禁止

    return 0;
}
