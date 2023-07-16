#include <cstring>

class String {
public:
    // 普通构造函数
    String(const char* str = nullptr) {
        /*
        nullptr 为 C++ 11引入
        NULL 是宏定义整型 0
        空指针不要用 NULL 表示, 用 nullptr
        */
        if (str != nullptr) {
            m_data = new char[strlen(str) + 1]; // strlen(str) 没计算 '\0'
            strcpy(this->m_data, str);
        }
        else {
            // m_data = nullptr;  // 这样做了后, 每次调用成员函数都需要判断是否为 nullptr
            m_data = new char[1]; // 为什么不用 m_data = new char;
            *m_data = '\0';       // 字符 '\0' 对应整数 0
        }
    }

    // 拷贝构造函数
    String(const String& other) {
        m_data = new char[strlen(other) + 1];
        strcpy(m_data, other.m_data);
    }

    // 析构函数
    ~String(void) {
        delete[] m_data;
        m_data = nullptr; // 防止野指针出现
    }

    // 赋值函数
    /*
    支持连续赋值：
    str3 = str1 = str2  从右往左赋值
    str1 = str2  -> str1.operator(str2) 返回值如果是 void 则无法支持连续赋值 str3 = void
    返回 String&, 支持连续的 operator= 赋值操作
    */
    String& operator=(const String& other) {

        if (this == &other) { // 防止自赋值
            return *this;
        }

        delete[] m_data; // 删除当前字符串占用的外部空间

        m_data = new char[strlen(other) + 1];
        strcpy(m_data, other.m_data);

        return *this;
    }

private:
    char* m_data; // 用于保存字符串
};

int main() {

    // 调用带 const char* 参数的构造函数
    String str1;
    String str2("hello");
    String str3 = "world";

    // 调用拷贝构造函数
    String str4 = str3; // 叫初始化而不是赋值, 初始化是左边对象在构造, 赋值是左边对象已经存在
    String str5(str3);

    // 调用赋值重载函数
    str1 = str2;

    return 0;
}
