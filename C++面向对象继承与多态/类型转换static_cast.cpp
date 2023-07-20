// static_cast : 提供编译器认为安全的类型转换, 没有任何联系的类型之间的转换就被否定
int main() {

    int  a = 10;
    char b = static_cast<char>(a); // 允许

    int* p = nullptr;
    // short* b = static_cast<short*>(p);  // 禁止
    // double* b = static_cast<double*>(p);  // 禁止
    // double* b = (double*)p;  // C 允许

    // 【问题】基类类型和派生类类型, 能不能用 static_cast
    // 【答】当然可以

    return 0;
}
