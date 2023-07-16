int main() {
    int  a = 10;
    int& b = a; // 左值: 有名字, 有内存
    // int &&C = a;  // 错误, 无法将左值绑定到右值引用
    // int &c = 20;  // 错误, 无法将右值绑定到左值引用

    const int& c = 20;
    /**
     * 上面语句实际做的事情：先生成临时量，然后用常引用引用临时量
     * int tmp = 20;
     * const int &c = tmp;
     */

    int&& d = 20; // 可以把一个右值绑定到一个右值引用上
    /**
     * int tmp = 20;
     * int &d = tmp;
     */

    // 临时量为右值，得用右值引用或常引用
    // CMyString&& e = CMyString("aaa");
    // const CMyString &e = CMyString("aaa");

    int& f = d; // 一个右值引用变量, 本身是一个左值，得用左值引用去引用

    return 0;
}
