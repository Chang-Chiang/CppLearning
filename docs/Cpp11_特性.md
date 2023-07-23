# C++11 特性

## 关键字和语法

  + auto: 根据右值, 推导出右值的类型, 然后左边变量的类型也就已知了

  + nullptr: 给指针专用(能够和整数进行区别)  #define NULL 0

  + foreach: 可以遍历数组、容器等, 底层就是通过指针或者迭代器来实现的

    ```c++
    for (Type val : container) {
        cout << val << " ";
    }
    ```

  + 右值引用: 对象优化, move 移动语义函数 和 forward 类型完美转发函数

  + 模板的一个新特性: ```typename... A ```表示可变参

## 绑定器和函数对象

+ function: 函数对象
+ bind: 绑定器
+ lamda 表达式

## 智能指针

+ shared_ptr 
+ weak_ptr

## 容器

+ unordered_set 和 unordered_map
+ array: 数组 => 尽量使用 vector
+ forward_list: 前向链表 => 尽量使用 list

## C++ 语言级别支持的多线程编程

+ 以前 windows 平台 createThread, Linux 平台 pthread_create, clone