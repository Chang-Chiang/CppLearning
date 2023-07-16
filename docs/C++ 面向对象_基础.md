# C++ 面向对象

## 面向对象概述

+ OOP 编程

  + 对于 C 语言, 主要做的事情是各种各样的函数的定义, struct 对应 C++ 的class; 对于 C++ 则是各种类的定义, 类 即是对实体的抽象类型

  + OOP

    实体 (属性、行为)  -> ADT(abstract data type)

    对象                              <-  (实例化) 类 (属性 -> 成员变量、行为 -> 成员方法) 

  + OOP 语言的四大特征是什么？

    抽象、封装/隐藏、继承、多态

  + 类的访问限定符

    public 公有的, private 私有的, protected 保护的

## this 指针：类对象的内存大小

  一个类产生的很多对象, 每一个对象都有自己的成员变量, 但是同一个类型的对象共享一套成员方法, 成员方法区分当前操作的是哪个对象, 通过 this 指针区分; 成员方法一经编译, 方法的参数都会添加一个 this 指针, 接收调用该方法的对象的地址

```c++
#include <iostream>

using std::cout;
using std::endl;

const int NMAE_LEN = 20;

// 商品的抽象数据类型
class CGoods {  // 类名首字母大写, 驼峰命名, 首字母 C 表示是 class

// 给外部提供公有的成员方法, 来访问私有的属性
public:
    // 类体内实现的方法, 自动处理为 inline
    
    // 商品数据初始化
    void init(const char *name, double price, int amount);
    
    // 打印商品信息
    void show();
    
    // 方法名首字母小写, 驼峰式命名
    void setName(char *name) { strcpy(_name, name); }
    void setPrice(double price) { _price = price; }
    void setAmount(int amount) { _amount = amount; }
    
    const char* getName() { return _name; }
    double getPrice() { return _price; }
    int getAmount() {return _amount; }
    
// 属性一般都是私有的成员变量    
private:
    char _name[NAME_LEN];
    double _price;
    int _amount;
};  // 注意类定义结束后的分号

void CGoods::init(const char *name, double price, int amount) {
    strcpy(_name, name);
    _price = price;
    _amount = amount;
}
                  
void CGoods::show() {
    cout << "name: " << _name << endl;
    cout << "price: " << _price << endl;
    cout << "amount: " << _amount << endl;
}    
                  
int main() {
    /* 
    CGoods 可以定义无数的对象, 每一个对象都有自己的成员变量
    但是它们共享一套成员方法
    成员方法怎么知道处理哪个对象的信息？
    类的成员方法一经编译, 所有的方法参数, 都会加一个 this 指针, 接收调用该方法的对象的地址
    */
    CGoods good1;  // 类实例化了一个对象
    
    /*
    vs cmd 命令 cl *.cpp /d1reportSingleClassLayoutCGoods 查看对象内存大小
    */
    
    good1.init("面包",  10.0,  200);
    good1.show();
     
    good1.setPrice(20.5);
    good1.setAmount(100);
    good1.show();
    
    return 0;
}
```

## 顺序栈实现：引出构造、析构

```c++
#include <iostream>

using std::cout;
using std::endl;

// OOP 实现顺序栈
class SeqStack {
public:
    void init(int size = 10) {
        _pstack = new int[size];  // 堆内存的开辟
        _top = -1;
        _size = size;
    }
    
    void release() {
        delete []_pstack;
        _pstack = nullptr;
    }
    
    void push(int val) {
        if (full())  // 栈满, 扩容
            resize();
        _pstack[++_top] = val;
    }
    
    void pop() {
        if(empty())
            return;
        --_top;
    }
    
    int top() {
        return _pstack[_top];
    }
    
    bool empty() { return _top == -1; }
    bool full() { return _top == _size - 1; }
    
private:
    int *_pstack;  // 动态开辟数组, 存储顺序栈的元素
    int _top;  // 指向栈顶元素的位置
    int _size;  // 数组扩容的总大小
    
    void resize() {
        int *ptmp = new int[_size * 2];
        for (int i = 0; i < _size; ++i) {
            ptmp[i] = _pstack[i];
        }
        
        /*
        为什么不用下列语句替代 for 循环赋值
        memcpy(ptmp, _pstack, sizeof(int) * _size);
        或 realloc 函数
        涉及的是内存拷贝, 对对象而言不适合, 会产生问题
        */
        
        delete []_pstack;
        _pstack = ptmp;
        _size *= 2;
    }  
};

int main() {
    SeqStack s;
    s.init(5);  // 对象成员变量的初始化操作, 需手动调用
    
    for (int i = 0; i < 15; ++i) {
        s.push(rand() % 100);
    }
    
    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
    
    s.release();  // 释放对象成员变量占用的外部堆内存 (外部资源) , 需手动调用
    
    return 0;
}
```

## 构造函数和析构函数

+ 函数的名字和类名一样
+ 没有返回值

```c++
#include <iostream>

using std::cout;
using std::endl;

// OOP 实现顺序栈
class SeqStack {
public:
    
    // 构造函数, 是可以带参数的, 因此可以提供多个构造函数, 叫做构造函数的重载
    // SeqStack s1;  // 默认构造, 参数是 10
    // SeqStack s2(20);  // 参数是 20
    SeqStack(int size = 10){
        cout << this << "SeqStack()" << endl;
        _pstack = new int[size];  // 堆内存的开辟
        _top = -1;
        _size = size;
    }
    
    // 析构函数, 是不带参数的, 所以析构函数只能有一个
    ~SeqStack() {
        cout << this << "~SeqStack()" << endl;
        delete []_pstack;
        _pstack = nullptr;
    }
    
    void push(int val) {
        if (full())  // 栈满, 扩容
            resize();
        _pstack[++_top] = val;
    }
    
    void pop() {
        if(empty())
            return;
        --_top;
    }
    
    int top() {
        return _pstack[_top];
    }
    
    bool empty() { return _top == -1; }
    bool full() { return _top == _size - 1; }
    
private:
    int *_pstack;  // 动态开辟数组, 存储顺序栈的元素
    int _top;  // 指向栈顶元素的位置
    int _size;  // 数组扩容的总大小
    
    void resize() {
        int *ptmp = new int[_size * 2];
        for (int i = 0; i < _size; ++i) {
            ptmp[i] = _pstack[i];
        }
        
        /*
        为什么不用下列语句替代 for 循环赋值
        memcpy(ptmp, _pstack, sizeof(int) * _size);
        或 realloc 函数
        涉及的是内存拷贝, 对对象而言不适合, 会产生问题
        */
        
        delete []_pstack;
        _pstack = ptmp;
        _size *= 2;
    }  
};


SeqStack gs;  // 全局对象, .data 上, 程序结束时析构
int main() {
    
    /*
    内存无非分为三块
    .data  数据段
    heap
    stack
    */
    
    // 开辟在 heap 上
    SeqStack *ps = new SeqStack(60);  // mallocn内存开辟 + SeqStack(60) 对象构造
    ps->push(70);
    ps->push(80);
    ps->pop();
    cout << ps->top() << endl;
    delete ps;  // 先调用 ps->~SeqStack() + 然后 free(ps), delete 和 free 的区别
    
    SeqStack s;  // stack 上, 开辟内存 + 调用构造函数
    // s.init(5);  // 对象成员变量的初始化操作, 需手动调用
    
    for (int i = 0; i < 15; ++i) {
        s.push(rand() % 100);
    }
    
    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
    
    // s.release();  // 释放对象成员变量占用的外部堆内存 (外部资源) , 需手动调用
    
    SeqStack s1(50);
    s1.~SeqStack();  // 析构函数调用以后, 对象不存在了, 但是其内存还在, 函数结束后释放
    s1.push(30);  // 堆内存的非法访问
    
    return 0;
}
```

## 对象深拷贝和浅拷贝

如果发生浅拷贝, 一定要自定义拷贝构造函数和赋值重载函数

+ 浅拷贝与深拷贝

  ```c++
  class SeqStack {
  public:
      /*
      ···
      */
      
      // 默认的拷贝构造做的是内存的数据拷贝 -> 浅拷贝
      // 浅拷贝不一定有错, 关键是对象如果占用外部资源, 那么浅拷贝就会出现问题
      /*
      SeqStack(const SeqStack &src){
          _pstack = src._pstack;  // 堆内存的开辟
          _top = src._top;
          _size = src._size;
      }
      */   
      
      // 自定义 拷贝构造函数
      SeqStack(const SeqStack &src){
          
          cout << "SeqStack(const SeqStack &src)" << endl;
          
          // 深拷贝
          _pstack = new int[src._size];  // 堆内存的开辟
          for (int i = 0; i <= src._top; ++i) {  
          // i <= src._top, 因为 _top 从 -1 开始
              
              _pstack[i] = src._pstack[i];   
          }
          /*
          for 循环实现拷贝, 而不是使用 memcpy 或 realloc 的原因：
          拷贝的数据类型是整型 -> 没有问题
          拷贝的数据类型是类创建的对象, 对象的成员变量中有指针指向外部资源 -> 浅拷贝, 有问题
          */
          
          _top = src._top;
          _size = src._size;
      }
      
      // 自定义 赋值重载函数
      SeqStack& operator=(const SeqStack &src){
          
          cout << "Soperator=" << endl;
          
          // 防止自己给自己赋值, 避免将自己释放后内存非法访问
          if (this == &src)
              // return *this;
              return;
          
          // 需要先释放当前对象占用的外部资源
          delete []_pstack;
          
          _pstack = new int[src._size];  // 堆内存的开辟
          for (int i = 0; i <= src._top; ++i) {              
              _pstack[i] = src._pstack[i];   
          }
          
          _top = src._top;
          _size = src._size;
          
          // return *this;
      }
  
  private:
      
  };
  
  int main() {
      
      SeqStack s;  
      // 没有提供任何构造函数的时候, 编译器会生成默认构造函数、默认析构函数, 是空函数
      
      SeqStack s1(10);
      SeqStack s2 = s1;  // 1 默认拷贝构造函数 -> 做直接内存数据拷贝, 浅拷贝
      // SeqStack s3(s1);  // 2 拷贝构造函数, 与 1 一样
      
      // s2.operator=(s1);  // 操作符重载
      s2 = s1;  // 默认赋值函数 -> 做直接内存数据拷贝, 浅拷贝
  }
  ```

+ String 类的创建

  ```c++
  class String {
  public:
      
      // 普通构造函数
      String(const char *str = nullptr) {
      /*
      nullptr 为 C++ 11引入
      NULL 是宏定义整型 0
      空指针不要用 NULL 表示, 用 nullptr
      */
          if (str != nullptr) {
              m_data = new char[strlen(str) + 1];  // strlen(str) 没计算 '\0'
              strcpy(this->m_data, str);
          }
          else {
              // m_data = nullptr;  // 这样做了后, 每次调用成员函数都需要判断是否为 nullptr
              m_data = new char[1];  // 为什么不用 m_data = new char;
              *m_data = '\0';  // 字符 '\0' 对应整数 0
          }  
      }
      
      // 拷贝构造函数
      String(const String &other) {
          m_data = new char[strlen(other) + 1];
          strcpy(m_data, other.m_data);
      }
      
      // 析构函数
      ~String(void) {
          delete []m_data;
          m_data = nullptr;  // 防止野指针出现
      }
      
      // 赋值函数   
      /* 
      支持连续赋值：
      str3 = str1 = str2  从右往左赋值
      str1 = str2  -> str1.operator(str2) 返回值如果是 void 则无法支持连续赋值 str3 = void
      返回 String&, 支持连续的 operator= 赋值操作
      */
      String& operator=(const String &other) {
          
          if (this == &other) {  // 防止自赋值
              return *this;
          }
          
          delete []m_data;  // 删除当前字符串占用的外部空间
          
          m_data = new char[strlen(other) + 1];
          strcpy(m_data, other.m_data);
          
          return *this;
      }
          
  private:
      char *m_data;  // 用于保存字符串
  };
  
  int main() {
      
      // 调用带 const char* 参数的构造函数
      String str1;
      String str2("hello");
      String str3 = "world";
      
      // 调用拷贝构造函数
      String str4 = str3;  // 叫初始化而不是赋值, 初始化是左边对象在构造, 赋值是左边对象已经存在 
      String str5(str3);
      
      // 调用赋值重载函数
      str1 = str2;  
      
      return 0;
  }
  ```

+ 循环队列 Queue 类

  ```c++
  class Queue {
  public:
      Queue(int size = 5) {
          _pQue = new int[size];
          _front = _rear = 0;
          _size = size;
      }
      
      // Queue(const Queue&) = delete;  // 不使用默认构造函数
      // Queue& operator=(const Queue&) = delete;  // 不使用默认赋值重载函数
      
      // 普通构造
      Queue(int size = 5) {
          _pQue = new int[size];
          _front = _rear = 0;
          _size = size;
      }
      
      // 拷贝构造
      Queue(const Queue &src) {
          _size = src.size;
          _front = src._front;
          _rear = src._rear;
          
          _pQue = new int[_size];
          for (int i = _front; i != _rear; i = (i + 1) % _size) {
              _pQue[i] = src.pQue[i];
          }
      }
      
      // 赋值重载
      Queue& operator=(const Queue &src) {
  		if (this == &src)
              return *this;
          
          delete []_pQue; 
          _size = src.size;
          _front = src._front;
          _rear = src._rear;
          _pQue = new int[_size];
          for (int i = _front; i != _rear; i = (i + 1) % _size) {
              _pQue[i] = src.pQue[i];
          }
          
          return *this;
      }
      
      // 析构函数
      ~Queue() {
          delete []_pQue;
          _pQue = nullptr;
      }
      
      void push(int val) {  // 入队操作
          if (full())
              resize();
          _pQue[_rear] = val;
          _rear = (_rear + 1) % _size;
      }
      
      void pop() {  // 出队操作
          if (empty())
              return;
          _front = (_front + 1) % _size;
      }
      
      int front() {  // 获取队头元素
          return _pQue[_front];
      }
      
      bool full() { return (_rear + 1) % _size == _front; }
      bool empty() { return _front == _rear; }
      
  private:
      int *_pQue;  // 申请队列的数组空间
      int _front;  // 指示队头的位置
      int _rear;  // 指示队尾的位置
      int _size;  // 队列扩容的总大小
      
      void resize() {
          int *ptmp = new int[2 * _size];
          int index = 0;
          for (int i = _front; i != rear; i = (i + 1) % _size) {
              ptmp[index++] = _pQue[i];
          }
          
          delete []_pQue;
          _pQue = ptmp;
          _front = 0;
          _rear = index;
          _size *= 2;
      }
  };
  
  int main() {
      Queue queue;
      
      for (int i = 0; i < 20; ++i) {
          queue.push(rand() % 100);
      }
      
      while (!queue.empty()) {
          cout << queue.front() << " ";
          queue.pop();
      }
      
      cout << endl;
      
      Queue queue1 = queue;
      queue1 = queue;
      
      return 0;
  }
  ```

## 构造函数的初始化列表

可以指定当前成员变量的初始化方式, 尤其对于成员对象而言

```c++
// 日期类
class CDate {
public:
    // 自定义构造函数, 编译器就不会再产生默认构造函数
    CDate(int y, int m, int d) {
        _year = y;
        _month = m;
        _day = d;
    }
    void show() {
        cout << _year << "/" << _month << "/" << _day << endl
    }
private:
    int _year;
    int _month;
    int _day;    
};

// CDate 信息 是 CGoods 商品信息的一部分 a part of...  组合的关系
// 构造函数的初始化列表：可以指定当前对象成员变量的初始化方式
class CGoods { 

public:
    // "CDate" 没有合适的默认构造函数可用
    CGoods(const char *n, int a, double p, int y, int m, int d)
        :_data(y, m, d),  // CDate _data(y, m, d);  // 指定对象的构造方式
        _amount(a),   // int amount = a;  // 定义时初始化
        _price(p)  // 1 构造函数的初始化列表, 先执行
        {
            // 2 当前类类型构造函数体, 后执行
            strcpy(_name, n);
            // _amount = a;  // int _amount; _amount = a;  // 先定义后赋值
            // _data = CDate(y, m, d);  // 未指定构造函数形式
        }
    
    void show() {
        cout << "name: " << _name << endl;
        cout << "amount: " << _amount << endl;
        cout << "price: " << _price << endl;   
        _data.show();
    }
private:
    char _name[20];
    double _price;
    int _amount;
    CDate _date;  // 成员对象, 对象的生成： 1. 分配内存 2. 调用构造函数
};

int main() {
    
    CGoods good("商品", 100,  35.0,  2019,  5,  12);
    good.show();
    
    return 0; 
}
```

```c++
// 面试题，问 ma, mb 的值
class Test {
public:
    /*
    构造函数初始化列表指定初始化方式
    若未指定, 则默认初始化
    成员变量初始化顺序, 为成员变量的定义顺序, 和构造函数初始化列表中出现的先后顺序无关
    ma 先初始化 ma = mb;  // ma = -858993460
    mb 后初始化 mb = data;  // mb = 10
    */
    Test(int data = 10) : mb(data), ma(mb) {}
    void show() {
        cout << "ma: " << ma << "mb: " << mb << endl;
    }
private:  
    int ma;
    int mb;
}

int main() {
    
    Test t;  // 开辟栈空间, windows 将栈空间初始化为 0XCCCCCCCC, -858993460
    t.show();
    return 0;
}
```

## 成员方法：普通、static、const

+ 普通的成员方法

  编译器会添加一个 this 形参变量

  + 属于类的作用域

  + 调用该方法时, 需要依赖一个对象

     (常对象是无法调用的, 因为常对象的实参类型 const CGoods* , 而普通成员方法的形参为  CGoods*) 

  + 可以访问任意对象的私有成员变量

+ static 静态成员方法

  不会生成 this 形参

  + 属于类的作用域
  + 用类名作用域调用静态成员方法
  + 可以任意访问对象的私有成员, 仅限不依赖对象的成员 (只能调用其他的 static 静态成员) 

+ const 常成员方法

  生成 const CGoods*

  + 属于类的作用域
  + 调用依赖一个对象, 普通对象或者常对象都可以
  + 可以任意访问对象的私有成员, 但只能读, 而不能写

```c++
class CDate {
public:
    
    CDate(int y, int m, int d) {
        _year = y;
        _month = m;
        _day = d;
    }
    void show()const {
        cout << _year << "/" << _month << "/" << _day << endl
    }
private:
    int _year;
    int _month;
    int _day;    
};


class CGoods { 

public:
    
    CGoods(const char *n, int a, double p, int y, int m, int d)
        :_data(y, m, d),  
        _amount(a), 
        _price(p)  
        {
            strcpy(_name, n);
            _count++;  // 记录所有产生的新对象的数量
        }
    
    // 普通的成员方法, 打印商品的私有信息, CGoods *this
    void show() {
        cout << "name: " << _name << endl;
        cout << "amount: " << _amount << endl;
        cout << "price: " << _price << endl;   
        _data.show();
    }
    
    // 重载关系
    // 常成员方法, const CGoods *this
    // 只要是只读操作的成员方法, 一律实现成 const 常成员方法
    void show()const {
        cout << "name: " << _name << endl;
        cout << "amount: " << _amount << endl;
        cout << "price: " << _price << endl;   
        _data.show();
    }
    
    // static 静态成员方法, 打印所有商品共享的信息, 没有 this 指针
    static void showCGoodsCount() {
        cout << "所有商品的种类数量是：" << _count << endl;
    }

private:
    char _name[20];
    double _price;
    int _amount;
    CDate _date;
    
    static int _count;  
    /*
    声明, 用来记录商品对象的总数量
    不属于对象, 而是属于类级别
    .bss 段
    */
};


// static 成员变量一定要在类外进行定义并且初始化
int CGoods::_count = 0

int main() {
    
    // 普通成员方法
    CGoods good1("商品1",  100,  35.0,  2019,  5,  12);
    goods1.show();
    
    CGoods good1("商品1",  100,  35.0,  2019,  5,  12);
    goods1.show();
    
    // 静态成员方法, 用类名调用
    CGoods::showCGoodsCount();  // 统计所有商品的总数量
    
    const CGoods good3("非卖商品3",  100,  35.0,  2019,  5,  12);
    good3.show();
    /*
    实参 const CGoods* -> 形参 const CGoods*
    */
}
```

## 指向类成员 (成员变量、成员方法) 的指针

```c++
#include <iostream>

using std::cout;
using std::endl;

class Test {
public:
    void        func() { cout << "call Test::func" << endl; }
    static void static_func() { cout << "Test::ststic_func" << endl; }

    int        ma;
    static int mb;
};

int Test::mb;

int main() {
    Test  t1;
    Test* t2 = new Test();

    // ----------------------------------------

    // 指向成员变量的指针
    // int *p = &Test::ma; // 普通指针指向成员变量报错
    // 无法从 "int Test::*" 转换为 "int *"
    int Test::*p = &Test::ma;
    t1.*p = 20;  // 一定要用对象，ma 依赖对象
    cout << t1.*p << endl;

    t2->*p = 30;
    cout << t2->*p << endl;

    int* p1 = &Test::mb; // 普通指针，mb 不依赖对象
    *p1 = 40;
    cout << *p1 << endl;

    delete t2;

    // ----------------------------------------

    // 指向成员方法的指针
    // void (*pfunc)() = &Test::func;
    // 无法从 "void (_thiscall Test::* )(void)" 转换为 "void (__cdecl *)(void)"
    void (Test::*pfunc)() = &Test::func;
    (t1.*pfunc)(); // 普通成员方法调用依赖对象
    (t2->*pfunc)();

    // 如何定义函数指针指向类的 static 成员方法？
    // 定义一个普通的函数指针即可，static 成员方法不依赖对象

    delete t2;

    // ----------------------------------------

    return 0;
}

```