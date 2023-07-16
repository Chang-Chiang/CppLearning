// main.cpp

// 引用 sum.cpp 文件里面定义的全局变量、函数
extern int gdata;         // 生成符号, gdata 对符号的引用 *UND*
int        sum(int, int); // 生成符号, sum 对符号的引用 *UND*

int data = 20;            // data .data

int main() {              // main .text
    int a = gdata;
    int b = data;

    int ret = sum(a, b);

    return 0;
}
