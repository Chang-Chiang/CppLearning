# C++ 语言基础

## 形参带默认值函数 

```c++
// VS Code 查看汇编, 在 调试控制台输入: -exec disassemble /m main
#include <iostream>
using namespace std;

int sum(int a = 10, int b = 20) { return a + b; }

int main() {
    int a = 10; // mov dword ptr[ebp-4], 0Ah
    int b = 20; // mov dword ptr[ebp-8], 14h

    int ret = sum(a, b);
    /*
    mov eax, dword ptr[ebp-8]
    push eax
    mov ecx, dword ptr[ebp-4]
    push ecx
    call sum
    */

    // int ret = sum(a);
    /*
    push 14H  # 被调函数带默认参数汇编少一条数据传送指令(mov)
    mov ecx, dword ptr[ebp-4]
    push ecx
    call sum
    */

    // int ret = sum();
    /*
    push 14H
    push 0AH
    call sum
    */

    // int ret = sum(a, 40);
    /*
    push 28H  # 传立即数, 相较于传变量, 效率提高
    mov ecx, dword ptr[ebp-4]
    push ecx
    call sum
    */

    cout << "ret: " << ret << endl;

    return 0;
}
```

+ 形参默认值从右向左给，例：`sum(int a=10, int b)`  为错误形式
+ 调用效率的问题
+ 定义处设置形参默认值，声明也可以给形参默认值
+ 形参给默认值时，不管是定义处给，还是声明处给，同一个形参只能出现一次默认值

## inline 函数

```c++
#include <iostream>

int sum(int a = 10, int b = 20) { return a + b; }

inline int sum_inline(int a = 10, int b = 20) { return a + b; }

int main() {
    int a = 10;
    int b = 20;

    int ret = sum(a, b);
    // 此处有标准的函数调用过程：参数压栈、函数栈帧的开辟和回退过程
    // 有函数调用的开销

    int ret = sum_inline(a, b);

    std::cout << "ret: " << ret << std::endl;

    return 0;
}
```

inline 内联函数：在编译过程中, 没有函数的调用开销, 在函数的调用点直接把函数的代码进行展开处理, inline 函数不再生成相应的函数符号

【注意】但是不是所有的 inline 都会被编译器处理成内联函数, 例：递归

inline 只是建议编译器把这个函数处理成内联函数

debug  版本上, inline 是不起作用的, inline 只有在 release 版本下才能出现

```bash
$ g++ -c 内联函数.cpp -O2     # release 版本
$ g++ -c 内联函数.cpp -O2 -g  # debug 版本
$ objdump -t 内联函数.o       # 查看符号表
```

+ 内联函数与普通函数的区别

  普通函数调用有开销 (展开回答) , 内联函数是在编译阶段, 在函数的调用点把函数代码直接展开, 省去开销; 如果函数在短时间内要大量调用, 且函数比较简单, 则尽量将其设为内联函数; 不是所有的 inline 都会被编译器处理成内联函数 (展开回答) ; 真的内联成功, 是不会在符号表中生成符号的; 

+ 函数调用的开销问题

  `sum(a, b)` 执行的语句实际只有, `mov` ,`add`, 但在函数调用过程中, 多出参数的压栈、函数栈帧的开辟和回退过程, 这些因调用函数产生的额外操作, 即是函数调用的开销

## 函数重载

### C 与 C++ 互相调用

+ 为什么 C++ 支持函数重载, C 语言不支持函数重载？

  C++ 代码产生函数符号的时候, 函数符号是 函数名+参数列表类型组成的; 而 C 代码产生函数符号的时候, 函数符号只由函数名决定。

+ 函数重载需要注意什么？

+ C++ 和 C 语言代码之间如何相互调用

  C++ 调用 C：把 **C 函数的声明**放在 `extern "C" { }` 里

  C 调用 C++：把 **C++ 源码 (函数的定义)** 放在 `extern "C" { }` 里

  ```c
  // .c
  int sum(int a, int b) {  // 编译过程产生函数符号 sum, .text
      return a + b;
  }
  ```

  ```c++
  // .cpp
  // int sum(int a, int b);  // 编译过程产生函数符号 sum_int_int *UND* 
  
  extern "C" {
      int sum(int a, int b);  // 告诉编译器, C 语言规则生成
  }
  int main() {
      int ret = sum(10, 20);  // 无法直接调用
      cout << "ret: " << ret << endl;
      return 0;
  }
  ```


### C++ 函数重载

```c++
#include <iostream>

using std::cout;
using std::endl;

// #if 0 ... #endif 屏蔽块内代码
#if 0
bool compare(int a, int b) {
    cout << "compare_int_int" << endl;
    return a > b;
}

bool compare(double a, double b) {
    cout << "compare_double_double" << endl;
    return a > b;
}

bool compare(const char *a, const char *b) {
    cout << "compare_char*_char*" << endl;
    return strcmp(a, b) > 0;
}

int data = 10;
int main() {
    
    int data = 20;
    //int a = data;  // a = 20
    int a = ::data;  // a = 10
    
    // 局部进行函数声明后, 函数调用均调用该函数
    // bool compare(int a, int b);  // 函数声明 
    
    compare(10, 20);
    compare(10.0, 20.0);
    compare("aaa", "bbb");
    
    return 0;
}
#endif
```

函数重载：

+ 一组函数, 其中函数名相同, 参数列表的个数或者类型不同, 那么这一组函数就称作函数重载。

+ 【注意】一组函数要称得上重载, 一定得在同一个作用域

+ const 或者 volatile 的时候, 怎么影响形参类型的

  ```c++
  #include <iostream>
  #include <typeinfo>
  
  using std::cout;
  using std::endl;
  
  void func(int a) {}  // 形参类型为 int
  void func(const int a) {}  // 形参类型为 int
  
  int main() {
      int a = 10;
      const int b = 10;
      cout << typeid(a).name() << endl;  // int
      cout << typeid(b).name() << endl;  // int
      
      return 0;
  }
  ```

### 小结

+ 一组函数, 函数名相同, 参数列表也相同, 仅仅是返回值不同, 不叫重载

+ 【面试题】请你解释一下, 什么是多态？

  + 静态的多态：编译时期, 函数重载、模板
  + 动态的多态：运行时期

+ 什么是函数重载？

+ 跨语言

  ```c++
  // 编译器内置的宏 __FILE__, __LINE__
  // 只要是 C++ 编译器, 都内置了 __cplusplus 这个宏名
  
  // C、C++ 编译器编译时按 C 规则生成函数符号
  #ifdef __cplusplus
  extern "C" {
  #endif
      
      int sum(int a, int b) {
          return a + b;
      }
      
  #ifdef __cplusplus
  }
  #endif
  ```

## const

### const 在 C 和 C++ 中的区别

+ 在 C++ 中, const 修饰的量, 叫常量; 
+ 与普通变量的区别：

  + 编译方式不同：所有出现常量名字的地方都会用常量初值替换

  + 不能作为左值
+ const 修饰的量（常量）常出现的错误是：

  + 常量不能再作为左值, 即直接修改常量的值
  + 不能把常量的地址泄露给一个普通的指针或者普通的引用变量, 即间接修改常量的值

### const 和一级指针的结合

```c++
const int *p;
int const* p;
int *const p;
const int *const p;
```

const 修饰的是离它最近的类型，**看 `const` 在 `*` 左侧还是右侧**

+ `const int *p; `

  const 修饰 int, 表达式 *p 是 const

  可以修改指针 (可以任意指向不同的 int 类型的内存) 

  但不能修改指针指向的值 (不能通过指针间接修改指向的内存的值) 

  不允许 `*p = 20`, 允许 `p = &b`

+ `int const* p;`

  与 `const int *p; ` 一样

+ `int *const p;`

  const 修饰 int*, 表示指针 p 是 const 

  不能再指向其它内存

  但是可以通过指针解引用修改指向的内存的值

  允许 `*p = 20`, 不允许 `p = &b`

+ `const int *const p;`

  第一个 const 修饰的是 int, 表达式 *p 是 const, 不允许 `*p = 20`

  第二个 const 修饰的是 int*, 表达式 p 是 const, 不允许 `p = &b`

+ 总结 const 和指针的类型转换公式

  + `int* <- const int*`             错误
  + `const int* <-  int*`           允许
  + `int** <- const int**`         错误
  + `const int** <- int**`         错误
  + `int** <- int *const *`       错误
  + `int *const * <- int**`     允许

```c++
int main() {
    
    int a = 10;
    const int *p = &a;
    int *q = p;  // 禁止 int* <- const int*
    cout << typeid(p).name << endl;  // int const *
    
    int *p1 = nullptr;  // 不要用 NULL, NULL 就是 0
    int *const p2 = nullptr;
    cout << typeid(p1).name() << endl;  // int *
    cout << typeid(p2).name() << endl;  // int *
    // const 如果右边没有指针 * 的话, const 是不参与类型的
    
    
    // 普通变量、指针
    int b = 20;
    int *q1 = &b;
    const int *q2 = &a;  // 允许 const int * <- int* 
    int *const q3 = &a;  // 允许 int * <- int* 
    int *q4 = q3;  // 允许 int * <- int* 
    
    // 常量、常量指针
    const int a = 10;
    const int *p = &a;
    
    // int *p = &a;  // error

    return 0;
}
```

### const 和 二级指针的结合

+ `const int **q`

  与 `const` 最近的类型是 `int`，`**q` 为 `const`

+ `int *const *q`

  与 `const` 最近的类型是 `int*`，`*q` 为 `const`

+ `int **const q`

  与 `const` 最近的类型是 `int**`，`q` 为 `const`

```c++
int main() {
    int a = 10;
    
    int *p = &a;
    const int **q = &p;  // 错误 const int ** <- int **
    // *q <=> p  // 常量的地址泄露给了普通指针
    // const int b = 20;
    // *q = &b; 
    
    // 修改方式 1
    const int *p = &a;
    const int **q = &p;  
    
    // 修改方式 2
    int *p = &a;
    const int *const*q = &p;
    
    return 0;
}
```

+ 题目

  + 错误代码

    ```c++
    // 错误
    int a = 10;
    const int *p = &a;  // const int* <-  int*
    int *q = p;  // int* <- const int* 错误
     
    // 正确
    int a = 10;
    int *const p = &a;    // int* <- int*
    int *q = p;  // int* <- int*
    
    // 正确
    int a = 10;
    int *const p = &a;  // int* <- int*
    int *const q = p;  // int* <- int*
     
    // 正确
    int a = 10;
    int *const p = &a;  // int* <- int*
    const int *a = P;  // const int* <- int*
    ```

  + 错误代码

    ```c++
    // 错误
    int a= 10;
    int *p = &a;
    const int **q = &p;
     
    // 正确
    int a = 10;
    int *p = &a;
    int *const*q = &p;  // int* const* <- int**
       
    // 正确
    int a = 10;
    int *p = &a;
    int **const q = &p;  // int** <- int**
      
    // 错误
    int a = 10;
    int *const p = &a;  // int* <- int*
    int **q = &p;  // int** <- int* const* 错误
    
    // 错误
    int a = 10;
    const int *p = &a;
    int *const* q = &p;  // int *const* <- const int**
    ```

### const、一级指针、引用的结合

```c++
int main() {
    
    // 写一句代码, 在内存的 0x0018ff44 处写一个 4 字节的 10
    // int *p = (int*)0x0018ff44;  // 指针变量
    // int *&&p = (int*)0x0018ff44;  // 引用变量
    // int *const &p = (int*)0x0018ff44;  // 引用变量
    
    int a = 10;
    int *p = &a;
    // int **q = &p;  // 二级指针
    // int *&q = p;  // 引用变量
    
    // 判断下面语句对错
    // const int *&q = p;  // 错误，typeid(q).name()
    // 将引用变量还原至指针进行判断 
    // const int **q = &p;  // const int ** <- int ** 错误 
    
    return 0;
}
```

```c++
// 正确
int a = 10;
int *p = &a;
int *&q = p;  // int **q = &p

// 错误
int a = 10;
int *const p = &a;
int *&q = p;  // int **q = &p; 常量泄露给普通指针

// 错误
int a = 10;
const int *p = &a;
int *&q = p;  // int **q = &p;  int** <- const int**  错误

// 错误
int a = 10;
int *p = &a;
const int *&q = p;  // const int **q = &p;  const int ** <- int ** 错误 
```

## C++ 引用

### 左值引用、右值引用

引用是一种更安全的指针

+ **引用和指针的区别？** 

  + 引用是必须初始化的, 指针可以不初始化
  + 引用只有一级引用, 没有多级引用; 指针可以有一级指针, 也可以有多级指针
  + 定义指针变量和定义引用变量, 在汇编指令上是一模一样的; 通过引用变量修改所引用内存的值, 和通过指针解引用修改指针指向的内存的值, 其底层指令也是一模一样的

+ 左值引用、右值引用

  + `int &&c = 20;` 右值引用, 专门用来引用右值类型, 指令上, 可以自动产生临时量, 然后直接引用临时量 `c = 40;` 还可以通过右值引用直接修改临时量内存
  + 右值引用变量, 本身是一个左值, 只能用左值引用来引用它
  + 不能用一个右值引用变量, 来引用一个左值

  ```c++
  int main() {
      int a = 10;  // 左值, 它有内存, 有名字, 值可以修改
      int &b = a;  // lea eax, [a]
                   // mov dword ptr[b], eax
      
      // int &c = 20;  // 错误 20 是右值：没内存 (立即数, 放在 CPU 寄存器) , 没名字
      
      
      // C++11 提供了右值引用
      // c 可以作为左值, 允许 c = 30; 修改临时量内存
      int &&c = 20;  // mov dword ptr[ebp-30h], 14h  # 20 放在一个临时量内存
                     // lea eax, [ebp-30h]
                     // mov dword ptr[c], eax
      
      int &e = c;  // 一个右值引用变量, 本身是一个左值
      
      // d 不能作为左值, 禁止 d = 30; 不允许修改临时量内存
      const int &d = 20;  // 允许
      /* 相当于
  	int temp = 20;
      temp 地址存到 d
      */
      
      return 0;
  }
  ```

### 引用的实例

```c++
int main() {
    
    int a = 10;
    int *p = &a;
    // lea eax, [a]            ; a 的地址放到 eax 寄存器
    // mov dword ptr [p], eax  ; eax 寄存器存的值放到 p 的内存
    int &b = a;
    // lea eax, [a]            ; a 的地址放到 eax 寄存器
    // mov dword ptr [b], eax  ; eax 寄存器存的值放到 b 的内存
    
    // a *p b 属于同一块内存
    
    *p = 20;
    // mov eax, dword ptr [p]
    // mov dword ptr [eax], 14h
    
    // 20 20 20
    cout << a << " " << *p << " " << b << endl;
    
    b = 30;
    // mov eax, dword ptr [b]
    // mov dword ptr [eax], 1Eh
    
    // 30 30 30
    cout << a << " " << *p << " " << b << endl;
    
    return 0;
}
```

```c++
void swap(int &x, int &y) {
    int temp = &x;
    &x = &y;
    &y = temp;
}

int main() {
    int a = 10;
    int b = 20;
    
    // swap(a, b);  // 形参变量值交换, 但实参变量值未改变
    // void swap(int a, int b) {
    //    int temp = a;
    //    a = b;
    //    b = temp;
	// }
    
    // swap(&a, &b);
    // void swap(int *x, int *y) {
    //    int temp = *x;
    //    *x = *y;
    //    *y = temp;
    // }
    
    swap(a, b);
    
    return 0;
}
```

```c++
int main() {
    int array[5] = {};
    int *p = array;
    
    // 定义一个引用变量, 来引用 array 数组
    // int (*q)[5] = &array;  // 定义指针变量
    int (&q)[5] = array;
    
    cout << sizeof(array) << endl;  // 32位 20
    cout << sizeof(p) << endl;  // 32位 4
    cout << sizeof(q) << endl;  // 32位 20
    
    return 0;
}
```

## new 和 delete

malloc 和 free, 称作 C 的库函数

new 和 delete, 称作运算符

+ new 和 delete
+ new 和 malloc 的区别是什么？
  + malloc 只管开辟内存, 不管初始化; new 不仅可以做内存开辟, 还可以做内存初始化操作
  + malloc 开辟内存失败, 是通过返回值和 nullptr 作比较; 而 new 开辟内存失败, 是通过抛出 bad_alloc 类型的异常来判断
  + new 开辟内存的时候是指定类型的，返回值不需要进行类型强转；malloc 只是按字节开辟内存，返回是 void*，需要对返回值进行类型转换
+ delete 和 free 的区别是什么？
  + free 不管是释放单个元素内存还是数组内存，是一个标准的函数调用，只需要传入内存的起始地址；delete 释放数组内存需要加 []

+ new 有多少种？
  + `int *p1 = new int(20);`
  + `int *p2 = new (nothrow)int;` 不抛出异常, nullptr 判断, 与 malloc 一样
  + `const int *p3 = new const int(40);` 堆上生成常量对象
  + `int data = 0; int *p4 = new (&data) int(50); cout << data << endl;  // data 50`定位 new,  在指定内存上

```c++
int main() {
    
    // C 方式
    int *p = (int*)malloc(sizeof(int));  
    // malloc 按字节开辟内存
    // 返回 void*,  需对返回值类型进行强转
    // 只管开辟内存, 不管初始化
    
    if (p == nullptr) {
        return -1;
	}
    
    *p = 20;
    free(p);
    
    // C++ 方式
    try {
        int *p1 = new int(20);
    }
    catch(const bad_alloc &e) {
        
    }
    
    // int *p1 = new int(20);
    // delete p1;
    
    // 开辟数组内存
    int *q = (int*)malloc(sizeof(int) * 20);
    if (q == nullptr) {
        return -1;
    }
    free(q);
    
    
    // int *q1 = new int[20];  // 不初始化
    int *q1 = new int[20]();  // 20 个 int, 初始化为 0
    delete[]q1;
    
    return 0;
}
```
