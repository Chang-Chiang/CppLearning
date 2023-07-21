# 绑定器和函数对象

```c++
#include <iostream>
#include <vector>
#include <functional>  // 函数对象
#include <algorithm>  // 泛型算法
#include <ctime>
```

## C++ STL 中的绑定器

+ bind1st : operator() 的第一个形参变量绑定成一个确定的值

+ bind2nd : operator() 的第一个形参变量绑定成一个确定的值

+ 应用实例

  ```c++
  template<typename Container>
  void showContainer(Container &con) {
      // Container::iterator it = con.begin();  
      // Container 类型还未实例化, 编译器不知道 Container::iterator 是类型还是变量
      
      typename Container::iterator it = con.begin();  
      // 告诉编译器 Container::iterator 是类型
      
      for (; it != con.end(); ++it) {
          cout << *it << " ";
      }
      cout << endl;
  }
  
  int main() {
      vector<int> vec;
      srand(time(nullptr));
      for (int i = 0; i < 20; ++i) {
          vec.push_back(rand()%100 + 1);
      }
      
      showContainer(vec);
      sort (vec.begin(), vec.end());  // 默认小到大排序 
      showContainer(vec);
      
      // greater 二元函数对象
      sort (vec.begin(), vec.end(), greater<int>());  // 重载 从大到小排序
      showContainer(vec);
      
      /*
      把 70 按顺序插入到 vec 容器当中
      operator()(const T &val) 需要 一元函数对象
      greater a > b
      less    a < b
      绑定器 + 二元函数对象 => 一元函数对象
      bool operator()(const _Ty &_Left, const _Ty &_Right)
      bind1st + greater => bool operator()(70, const _Ty &_Right)
      bind2nd + less    => bool operator()(const _Ty &_Left, 70)
      */
      auto it1 = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 70));
      // auto it1 = find_if(vec.begin(), vec.end(), bind2nd(less<int>(), 70));
      if (it1 != vec.end()) {
          vec.insert(it1, 70);
      }
  	showContainer(vec);
      
      return 0;
  }
  ```

+ 绑定器底层原理实现

  ```c++
  template<typename Iterator, typename Compare>  // 迭代器类型, 函数对象
  Iterator my_find_if(Iterator first, Iterator last, Compare comp) {
      for (; first != last; ++first) {
          if (comp(*first)) {  // comp.operator()(*first) 
              return first;
          }
      }
      return last;
  }
  
  template<typename Compare, typename T>
  class _mybind1st {  // 绑定器是函数对象的一个应用
  public:
      _mybind1st(Compare comp, T val) :_comp(comp), _val(val) {}
      bool operator()(const T &second) {
          return _comp(_val, second);
      }
  private:
      Compare _comp;
      T _val;
  };
  
  template<typename Compare, typename T>  // 二元函数对象, 元素类型
  _mybind1st<Compare, T> mybind1st(Compare comp, const T &val) {
      // 直接使用函数模板, 好处是, 可以进行类型的推演
      return _mybind1st<Compare, T>(comp, val);  // 返回一元函数对象
  }
  ```


## bind 和 function 机制

C++ 11 从 Boost 库中引入了 bind 绑定器和 function 函数对象机制

C++11 bind绑定器返回的结果还是一个函数对象

+ 例子

  ```c++
  #include <iostream>
  #include <string>
  #include <functional>
  
  void hello(string str) { cout << str << endl; }
  int sum(int a, int b) { return a + b; }
  class Test {
  public:
      int sum(int a, int b) { return a + b; }
  };
  
  int main() {
      
      // bind 是函数模板, 可以自动推演模板类型参数
      bind(hello, "hello, bind!")();
      cout << bind(sum, 10, 20)() << endl;
      cout << bind(&Test::sum, Test(), 10, 20)() << endl;
      
      // 参数占位符
      bind(hello, placeholders::_1)("hello bind 2!");  // namespace place_holders
      cout << bind(sum, placeholders::_1, placeholders::_2)(200, 300) << endl;
      
      // 绑定器出了语句, 无法继续使用 => 通过function 把 bind 返回的绑定器 binder 复用
      function<void(string)> func1 = bind(hello, placeholders::_1);
      func1("hello china!");
      funnc1("hello hubei!")
      
      return 0;
  }
  ```

## muduo 网络库线程池

```c++
#include <thread>

// 线程类
class Thread {
public:
    Thread(function<void()> func) :_func(func) { }
    
    // 【自定义给 no】
    // Thread(function<void(int)> func, int no) :_func(func), _no(no) { }
    
    thread start() {
        thread t(_func);  // _func()
        
        // 【自定义给 no】
        // thread t(_func, _no);  // _func(_no)
        
        return t;
    }
private:
    function<void()> _func;
    // int _no;  // 【自定义给 no】
};

// 线程池类
class ThreadPool {
public:
    ThreadPool() {}
    ~ThreadPool() {
        // 释放 Thread 对象占用的堆资源
        for (int i = 0; i < _pool.size(); ++i) {
            delete _pool[i];
        }
    }
    
    // 开启线程池
    void startPool (int size) {
        for (int i = 0; i < size; ++i) {
            _pool.push_back(new Thread(bind(&ThreadPool::runInThread, this, i)));
            
            // 【自定义给 no】
            // _pool.push_back(new Thread(bind(&ThreadPool::runInThread, this), i));
        }
        
        for (int i = 0; i < size; ++i) {
            _handler.push_back(_pool[i]->start());
        }
        
        // 等待所有子线程运行完成, 再结束主线程
        for (thread &t : _handler) {
            t.join();
        }
    }
    
private:
    vector<Thread*> _pool;
    vector<thread> _handler;
    
    // 把 runInThread 这个成员方法充当线程函数
    void runInThread(int id) {
        cout << "call runInThread! id: " << id << endl;
        
    }
};

int main() {
    ThreadPool pool;
    pool.startPool(10);
    return 0;
}
```

## 模板的完全特例化、非完全(部分)特例化

特例化还叫做特殊的实例化

```c++
template<typename T>
bool compare(T a, T b) {
    cout << "template compare" << endl;
    return a > b;
}

// 字符串比较特例化, 完全特例化
template<>
bool compare<const char*>(const char*a, const char*b) {
    cout << "compare<const char*>" << endl;
    return strcmp(a, b) > 0;
}

int main() {
    compare(10, 20);  // 模板实参推演, 通过实参类型推断形参 T 为 int
    compare("aaa", "bbb");  // 对两个字符串做大于比较, 实际比较的是地址, 需做特例化
    return 0;
}
```

```c++
template<typename T>
class Vector {
public:
    Vector() { cout << "call Vector template init" << endl; }
};

// 对 char* 类型提供的完全特例化版本
template<>
class Vector<char*> {
public:
    Vector() { cout << "call Vector<char*> init" << endl; }
};

// 对指针类型提供的部分特例化版本, 知道是指针, 但不知道指针的具体类型
template<typename Ty>
class Vector<Ty*> {
public:
    Vector() { cout << "call Vector<Ty*> init" << endl; }
};

// 针对函数指针类型(有一个返回值, 有两个形参变量)提供的部分特例化
template<typename R, typename A1, typename A2>
class Vector<R(*)(A1, A2)> {
public:
    Vector() { cout << "call Vector<R(*)(A1, A2)> init" << endl; }    
};

// 针对函数类型(有一个返回值, 有两个形参变量)提供的部分特例化
template<typename R, typename A1, typename A2>
class Vector<R(A1, A2)> {
public:
    Vector() { cout << "call Vector<R(A1, A2)> init" << endl; }    
};

int sum(int a, int b) { return a + b; }

int main() {
    Vector<int> vec1;  // call Vector template init
    Vector<char*> vec2;  // call Vector<char*> init
    Vector<int*> vec3;  // call Vector<Ty*> init
    Vector<int(*)(int, int)> vec4;  // 函数指针类型 call Vector<R(*)(A1, A2)> init
    Vector<int(int, int)> vec4;  // 函数类型 all Vector<R(A1, A2)> init
    
    // 注意区分函数类型和函数指针类型
    // 函数指针类型
    typedef int(*PFUNC1)(int, int);
    PFUNC1 pfunc1 = sum;
    cout << pfunc1(10, 20) << endl;
    
    // 函数类型
    typedef int PFUNC2(int, int);
    PFUNC2 *pfunc2 = sum;
    cout << (*pfunc2)(10, 20) << endl;
    
    return 0;
}
```

## 模板的实参推演

```c++
#include <typeinfo>

// T 包含了所有的大的类型
template<typename T>  
void func(T a) {
    cout << typeid(T).name() << endl;
}

template<typename R, typename A1, typename A2>  
void func2(R (*a)(A1, A2)) {
    cout << typeid(R).name() << endl;
    cout << typeid(A1).name() << endl;
    cout << typeid(A2).name() << endl;
}

template<typename R, typename T, typename A1, typename A2>  
void func3(R (T::*a)(A1, A2)) {
    cout << typeid(R).name() << endl;
    cout << typeid(T).name() << endl;
    cout << typeid(A1).name() << endl;
    cout << typeid(A2).name() << endl;
}

int sum(int a, int b) { return a + b; }

class Test {
public: 
    int sum(int a, int b) { return a + b; }
};

int main() {
    func(10);  // int
    func("aaa");  // char const *
    func(sum);  // int (__cdecl*)(int, int) 函数指针类型
    func2(sum);  // int, int, int
    func(&Test::sum);  // int (__thiscall Test::*)(int, int) 指向成员方法的函数指针类型
    func3(&Test::sum);  // int, class Test, int. int
    return 0;
}
```

## function 函数对象类型的实现原理

```c++
#include <iostream>
#include <string>
#include <functional>

void hello(string str) { cout << str << endl; }
int sum(int a, int b) { return a + b; }

int main() {
    function<void(string)> func1 = hello;
    func1("hello world!");
    return 0;
}

//-------------------------------------------------

template<typename Fty>
class myfunction {};

/*
template<typename R, typename A1>
class myfunction<R(A1)>
{
public:
    using PFUNC = R(*)(A1);
    myfunction(PFUNC pfunc) :_pfunc(pfunc) {}
    R operator()(A1 arg) {
        return _pfunc(arg);
    }
private:
    PFUNC _pfunc;
};

template<typename R, typename A1, typename A2>
class myfunction<R(A1)>
{
public:
    using PFUNC = R(*)(A1);
    myfunction(PFUNC pfunc) :_pfunc(pfunc) {}
    R operator()(A1 arg1, A2 arg2) {
        return _pfunc(arg1, arg2);
    }
private:
    PFUNC _pfunc;
};
*/

template<typename R, typename... A>  // typename... A 表示可变参
class myfunction<R(A...)>
{
public:
    using PFUNC = R(*)(A...);
    myfunction(PFUNC pfunc) :_pfunc(pfunc) {}
    R operator()(A... arg...) {
        return _pfunc(arg...);
    }
private:
    PFUNC _pfunc;
};

int main() {
    myfunction<void(string)> func1 = hello;
    func1("hello world!");  // func1.operator()("hello world!")
    
    myfunction<int(int, int)> func2(sum);
    cout << func2(10, 20) << endl;
    
    return 0;
}
```

## lamda 表达式

+ 底层依赖函数对象的机制实现

+ 函数对象使用场景

  + 泛型算法, 参数传递过程中
  + 比较性质
  + 自定义操作
  + 优先级队列

+ 函数对象缺点: 得先定义一个类

+ lamda 表达式的语法

  ```c++
  // [捕获外部变量](形参列表)->返回值 {操作代码};
  // 如果 lamda 返回值不需要, 那么 "->返回值" 可以省略
  // []: 表示不捕获任何外部变量
  // [=]: 以传值的方式捕获外部的所有变量
  // [&]: 以传引用的方式捕获外部所有变量
  // [this]: 捕获外部的 this 指针
  // [=, &a]: 以传值的方式捕获外部的所有变量, 但是 a 变量
  // [a, b]: 以传值的方式捕获外部的 a, b
  
  template<typename T = void>
  class TestLamda01 {
  public:
      TestLamda01() {}
      void operator()() {
          cout << "hello world!" << endl;
      }
  };
  
  template<typename T = void>
  class TestLamda02 {
  public:
      int operator()(int a, int b)const {
          return a + b;
      }
  };
  
  template<typename T = int>
  class TestLamda03 {
  public:
      TestLamda03(int a, int b) :ma(a), mb(b) {}
      int operator()()const {
          int tmp = ma;
          ma = mb;
          mb = tmp;
      }
  private:
      mutable int ma;
      mutable int mb;
  };
  
  template<typename T = int>
  class TestLamda04 {
  public:
      TestLamda04(int &a, int &b) :ma(a), mb(b) {}
      int operator()()const {
          int tmp = ma;
          ma = mb;
          mb = tmp;
      }
  private:
      int &ma;
      int &mb;
  };
  
  int main() {
      auto func1 = []()->void { cout << "hello world!" << endl; };
      // auto func1 = []() { cout << "hello world!" << endl; };
      func1();
      
      TestLamda01<> t1;
      t1();
      
      auto func2 = [](int a, int b)->int { return a + b; };
      cout << func2(20, 30) << endl;
      
      TestLamda02<> t2;
      t2();
      
      int a = 10;
      int b = 20;
      auto func3 = [a, b]()mutable {
          int tmp = a;
          a = b;
          b = tmp;
      };
      func3();
      cout << "a: " << a << "b: " << b << endl;
      
      TestLamda03<> t3(a, b);
      t3();
      
      auto func4 = [&a, &b]() {
          int tmp = a;
          a = b;
          b = tmp;
      };
      func4();
      cout << "a: " << a << "b: " << b << endl;
      
      TestLamda04<> t4(a, b);
      t4();
      
      return 0;
  }
  ```

+ 基本用法

  ```c++
  int main() {
      vector<int> vec;
      for (int i = 0; i < 20; ++i) {
          vec.push_back(rand()%100 + 1);
      }
      
      sort(vec.begin(), vec.end(), 
           [](int a, int b)->bool {
               return a > b;
           });
      for (int val : vec) {
          cout << val << " ";
      }
      cout << endl;
      
      auto it = find_if(vec.begin(), vec.end(), 
                        [](int a)->bool {
                            return a< 65;
                        });
      if (it != vec.end()) {
          vec.insert(it, 65);
      }
      for (int val : vec) {
          cout << val << " ";
      }
      cout << endl;
      
      for_each(vec.begin(), vec.end(), 
                        [](int a) {
                            if (a % 2 == 0)
                                cout << a << " ";
                        });
      cout << endl;
      
      return 0;
  }
  ```

## lamda 表达式的应用实践

```c++
/*
lamda 表达式只能使用在语句中, 
如果想跨语句使用之前定义好的 lamda 表达式 怎么办
用什么类型表示 lamda 表达式？
lamda 表达式是函数对象, 用 function 类型表示函数对象类型
*/
#include <functional>
#include <map>

int main() {
    
    map<int, function<int(int, int)>> caculateMap;
    caculateMap[1] = [](int a, int b)->int { return a + b; };
    caculateMap[2] = [](int a, int b)->int { return a - b; };
    caculateMap[3] = [](int a, int b)->int { return a * b; };
    caculateMap[4] = [](int a, int b)->int { return a / b; };
    
    cout << "10 + 15: " << caculateMap[1](10, 15) << endl;
    return 0;
}
```

```c++
// 智能指针自定义删除器 delete p; 对 FILE* 应该是 fclose(FILE*)
int main() {
    
    unique_ptr<FILE, function<void(FILE*)>> 
        ptr1(fopen("data.txt", "w"), [](FILE *pf) { fclose(pf); });
    return 0;
}
```

```c++
// 优先级队列

class Data {
public:
	Data(int val1 = 10, int val2 = 10) :ma(val1), mb(val2) {}
    // bool operator>(const Data &data)const { return ma > data.ma; }
    // bool operator<(const Data &data)const { return ma < data.ma; }
    int ma;
    int mb;
};

int main() {
    // priority_queue<Data> queue;
    // queue.push(Data(10, 20));  // 需定义 < 运算符重载
    // queue.push(Data(15, 15));
    // queue.push(Data(20, 10));
    
    using FUNC = function<bool(Data&, Data&)>;
    priority_queue<Data, vector<Data>, FUNC> 
        maxHeap([](Data &d1, Data &d2)->bool {
            return d1.ma > d2.ma;
        });
    maxHeap.push(Data(10, 20));  // 需定义 < 运算符重载
    maxHeap.push(Data(15, 15));
    maxHeap.push(Data(20, 10));
    
    return 0;
}
```

## 