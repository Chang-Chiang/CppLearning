int main() {
    /*
    数据位于三部分:
        + data 段
        + heap 堆: 用户手动开辟, 手动释放
        + stack 栈
    */
    int* p = new int(10);
    *p = 30;

    // if ()
    // 	return -1;

    delete p; // 忘记写 或 程序中途退出 或 抛出异常，造成资源泄露
    return 0;
}
