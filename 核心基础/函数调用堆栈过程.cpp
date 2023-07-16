// VS Code 查看汇编, 在 调试控制台输入: -exec disassemble /m main
int sum(int a, int b) {
    int temp = 0;
    temp = a + b;
    return temp;
}

int main() {
    int a = 10;
    int b = 20;
    int ret = sum(a, b);
    return 0;
}
