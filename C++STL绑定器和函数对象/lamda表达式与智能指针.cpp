#include <cstdio>
#include <functional>
#include <memory>

using std::function;
using std::unique_ptr;

// 智能指针自定义删除器 delete p; 对 FILE* 应该是 fclose(FILE*)
int main() {

    unique_ptr<FILE, function<void(FILE*)>> ptr1(
        fopen("data.txt", "w"), [](FILE* pf) { fclose(pf); });
    return 0;
}
