# C++ STL

## 简介

STL, standard template library 标准模板库, 

#### 一、标准容器

1. 顺序容器
   + [vector](##vector)
   + [deque](##deque)
   + [list](##list)
2. [容器适配器](##容器适配器)
   + stack
   + queue
   + priority_queue
3. 关联容器
   + 无序关联容器, 底层数据结构基于链式哈希表, 增删查 $O(1)$ 的时间复杂度
     + unordered_set
     + unordered_multiset
     + unordered_map
     + unordered_multimap
   + 有序关联容器, 底层数据结构基于红黑树, 增删查 $O(log_2 n)$ 的时间复杂度,  (树的层数、树的高度) 
     + set
     + multiset
     + map
     + multimap
4. C++11 array, forward_list

#### 二、近容器

+ 数组
+ string
+ bitset (位容器)

#### 三、迭代器

+ iterator 和 const_iterator
+ reverse_iterator 和 const_reverse_iterator 

#### 四、函数对象(类似 C 的函数指针)

+ greater
+ less

#### 五、泛型算法

+ sort
+ find
+ find_if
+ binary_search
+ for_each

## vector

+ vector: 向量容器

+ 底层数据结构：动态开辟的数组, 每次以原来空间大小的 2 倍进行扩容

+ 容器中, 对象的构造析构、内存的开辟释放, 是通过容器的空间配置器 allocator 实现的, allocate, deallocate, construct, destroy

+ 操作

  ```c++
  vector<inv> vec;
  ```

  + 增加

    ```c++
    vec.push_back(20);  // 末尾添加元素, 时间复杂度 O(1), 可能导致容器扩容
    vec.insert(it, 20);  // 在指定位置 (it 迭代器指向的位置) 添加一个元素 20, 时间复杂度 O(N), 可能导致容器扩容
    ```

  + 删除

    ```c++
    vec.pop_back();  // 末尾删除元素 O(1)
    vec.erase(it);  // 删除 it迭代器 指向位置元素, O(N)
    ```

  + 查询

    ```c++
    // operator[]  下标的随机访问 vec[5]  O(1)
    // iterator 迭代器进行遍历
    // find, for_each 泛型算法
    // foreach => 通过 iterator 遍历容器
    ```

  + 注意

    对容器进行连续插入或删除操作 (insert/erase), 一定要更新迭代器, 否则第一次 insert 或 erase 完成, 迭代器就失效了。

+ 常用方法

  ```c++
  // size()     返回容器有效元素个数
  // empty()    判断容器是否为空
  // reverse()  给 vector 预留空间, 只给容器底层开辟指定大小的内存空间, 并不会添加新的元素
  // resize()   重置大小, 容器扩容用, 不仅给容器底层开辟指定大小的内存空间, 还会添加新的元素
  // swap       两个容器进行元素交换
  ```

+ 例：增删查

    ```c++
    #include <iostream>
    #include <vector>
    
    int main() {
        vector<int> vec;  // vector<string> vec
        for (int i = 0; i < 20; ++i) {
            vec.push_back(rand()%100 + 1);  // 1~100 间的随机数
        }
    
        // operator[] 运算符重载函数进行遍历
        int size = vec.size();
        for (int i = 0; i < size; ++i) {
            cout << vec[i] << " ";
        }
        cout << endl;
    
        // 把 vec 容器中所有的偶数全部删除
        auto it2 = vec.begin();
        // for (; it2 != vec.end(); ) {
        while (it2 != vec.end()) {
            if (*it2 % 2 == 0) {
                vec.erase(it2);
            } 
            else {
                ++it2;
            }
        }
    
        // 通过迭代器遍历 vector 容器
        auto it1 = vec.begin();
        for (; it1 != vec.end(); ++it1) {
            cout << *it1 << " ";
        }
        cout << endl;
    
        // 给 vector 容器中所有的奇数前面都添加一个(奇数 - 1) 的偶数
        for (it1 = vec.begin(); it1 != vec.end(); ++it1) {
            if (*it1 % 2 != 0) {
                vec.insert(it1, *it1 - 1);
                ++it1;
            }
        }
    
        // 通过迭代器遍历 vector 容器
        for (it1 = vec.begin(); it1 != vec.end(); ++it1) {
            cout << *it1 << " ";
        }
        cout << endl;
    
        return 0;
    }
    ```

+ 例：reserve

  ```c++
  #include <iostream>
  #include <vector>
  
  int main() {
      vector<int> vec;  // 默认, 底层开辟空间 0 个, 扩容 0 1 2 4 8 16 32 64···
      vec.reserve(20);  // 给 vector 容器预留空间  1 0 0 20
      // vec.resize(20);  // 1 20(int()) 0 40
      
      cout << vec.empty() << endl;  // 1
      cout << vec.size() << endl;  // 0
      
      for (int i = 0; i < 20; ++i) {
          vec.push_back(rand()%100 + 1);  // 1~100 间的随机数
      }
      
      cout << vec.empty() << endl;  // 0
      cout << vec.size() << endl;  // 20
      
      return 0;
  }
  ```

## deque

+ deque: 双端队列容器

+ 底层数据结构: 动态开辟的二维数组，一维数组从 2 开始，以 2 倍的方式进行扩容，每次扩容后，原来第二维的数组，从新的第一维数组的下标 $oldsize/2$ 开始存放，上下都预留相同的空行，方便支持 deque 的首尾元素添加

  ```c++
  #define MAP_SIZE 2;
  #define QUE_SIZE 4096/sizeof(T)
  ```

+ 操作

  ```c++
  #include <deque>
  deque<int> deq;
  ```

  + 增加

    ```c++
    deq.push_back(20);  // 从末尾添加元素 O(1)
    deq.push_front(20);  // 从首部添加元素 O(1)  对比 vec.insert(vec.begin(), 20);  O(N)
    deq.insert(it, 20);  // it 指向位置添加元素 O(N)
    ```

  + 删除

    ```c++
    deq.pop_back();  // 从末尾删除元素 O(1)
    deq.pop_front();  // 从首部删除元素 O(1)
    deq.erase(it);  // 从 it 指向的位置删除元素 O(N)
    ```

  + 查询搜索

    ```c++
    // 通过迭代器 iterator
    // 连续的 insert 和 erase 一定要考虑迭代器失效的问题
    ```

## list

+ list: 链表容器

+ 底层数据结构: 双向的循环链表

+ 操作与 deque 一样

  ```c++
  #include <list>
  list<int> mylist;
  ```

  + 增加

    ```c++
    mylist.push_back(20);  // 从末尾添加元素 O(1)
    mylist.push_front(20);  // 从首部添加元素 O(1) 
    mylist.insert(it, 20);  // it 指向位置添加元素 O(1)
    // 链表进行 insert 的时候, 先要进行一个 query 查询操作
    // 对于链表而言, 查询操作效率慢
    ```

  + 删除
  
    ```c++
    mylist.pop_back();  // 从末尾删除元素 O(1)
    mylist.pop_front();  // 从首部删除元素 O(1)
    mylist.erase(it);  // 从 it 指向的位置删除元素 O(1)
    ```

  + 查询搜索
  
    ```c++
    // 通过迭代器 iterator
    // 连续的 insert 和 erase 一定要考虑迭代器失效的问题
    ```

## 顺序容器对比

+ 容器的纵向考察: 

  + vector 特点: 动态数组, 内存是连续的, 2 倍的方式进行扩容. 

  + deque 特点: 动态开辟的二维数组空间, 第二维是固定长度的数组空间, 扩容的时候是第一维的数组进行 2 倍扩容.

  + 【问题】deque 底层内存是否是连续的？

    并不是, 每一个第二维是连续的, 分段连续的

+ vector 和 deque 之间的区别 (容器的横向考察)
  + 底层数据结构
  + 前中后插入删除元素的时间复杂度: deque 前中后 $O(1)$, vector 中后 $O(1)$ 前 $O(N)$
  + 对于内存的使用效率: vector 效率低, vector 需要的内存空间必须是连续的; deque 可以分块进行数据存储, 不需要内存空间必须是连续的
  + 在中间 insert 或 erase, vector 和 deque 哪一个的效率好一点: 时间复杂度都是 $O(N)$, vector 内存完全连续, 方便移动 。由于deque 的第二维内存空间不是连续的, 所以在 deque 中间进行元素的 insert 或 erase, 造成元素移动的时候比 vector 慢
+ vector 和 list 之间的区别 (容器的横向考察)
  + 底层数据结构: 
    + 数组: 增删查 O(N), 随机访问 $O(1)$ 
    + 双向循环链表: 增删 $O(1)$ 但要考虑搜索的时间, 查 $O(N)$, 随机访问 $O(N)$. 

## 容器适配器

+ 对容器适配器的理解？
  + 适配器底层没有自己的数据结构，它是另外一个容器的封装，它的方法全部由底层以来的容器进行实现
  + 没有实现自己的迭代器

```c++
template<typename T, typename Container=deque<T>>
class Stack {
public:
    void push(const T &val) { con.push_back(val); }
    void pop() { con.pop_back(); }
    T top() const { return con.back(); }
private:
    Container con;
};
```

+ stack

  ```c++
  #include <iostream>
  #include <stack>
  
  /*
  + push  入栈
  + pop   出栈
  + top   查看栈顶元素
  + empty 判断栈空
  + size  返回元素个数
  */
  int main() {
      stack<int> s1;
      
      for (int i = 0; i < 20; ++i) {
          s1.push(rand() % 100 + 1);
      }
      
      cout << s1.size() << endl;
      
      while (!s1.empty()) {
          cout << s1.top() << " ";
          s1.pop();
      }
      return 0;
  }
  ```

+ queue

  ```c++
  #include <iostream>
  #include <queue>
  
  /*
  + push  入栈
  + pop   出栈
  + front 查看队头元素
  + back  查看队尾元素
  + empty 判断队空
  + size  返回元素个数
  */
  int main() {
      queue<int> que;
      
      for (int i = 0; i < 20; ++i) {
          que.push(rand() % 100 + 1);
      }
      
      cout << que.size() << endl;
      
      while (!que.empty()) {
          cout << que.front() << " ";
          que.pop();
      }
      return 0;
  }
  ```

+ priority_queue 

  底层数据结构默认是一个大根堆

  ```c++
  #include <iostream>
  #include <queue>
  
  /*
  + push  入队
  + pop   出队
  + top   查看队顶元素
  + empty 判断队空
  + size  返回元素个数
  */
  int main() {
      priority_queue<int> pque;
      
      for (int i = 0; i < 20; ++i) {
          pque.push(rand() % 100 + 1);
      }
      
      cout << pque.size() << endl;
      
      while (!pque.empty()) {
          cout << pque.top() << " ";
          pque.pop();
      }
      return 0;
  }
  ```

+ 问题

  为什么 stack 依赖 deque, queue 依赖 deque, priority_queue 依赖 vector?

  答: 

  + **vector 的初始内存使用效率太低, 没有deque 好**:  vector 扩容操作(0-1-2-4-8-···), 以存放 int 为例, deque 第二维一开始存放 $4096/sizeof(int) =1024$
  + 对于 queue 来说, 需要支持尾部插入, 头部删除, 都是 O(1), deque 满足, 但是 vector 头部删除 O(N), 如果 queue 依赖 vector, 其底层出队效率很低
  + vector 需要大片的连续内存, 而 deque 只需分段的内存, 当存储大量数据时, 显然 deque 对于内存的利用率更好一些
  + priority_queue 底层默认把数据组成一个大根堆结构, 是需要在一个内存连续的数组上构建一个大根堆或者小根堆的 

## 无序关联容器

```c++
#include <unordered_set>
#include <unordered_map>
```

90% 的情况都在使用无序关联容器, 大部分的应用场景只要求增删查的时间复杂度, 另一些除了要求增删查的时间复杂度, 还对元素顺序有要求 (负载均衡的一致性, 哈希算法), 只能选择有序关联容器

+ 无序关联容器

  单重表示不允许 key 重复, 多重表示允许 key 重复

  + unordered_set, 单重集合
  + unordered_multiset, 多重集合
  + unordered_map, 单重映射表
  + unordered_multimap, 多重映射表

+ 底层: 链式哈希表

+ 增删查时间复杂度: $O(1)$

+ 分类

  + 集合 set: 存的关键字 key
  + 映射表 map: 存的键值对 [key， value] 

+ set

    ```c++
    #include <iostream>
    #include <unordered_set>
    
    /*
    增加: insert(val)
    遍历: iterator 自己搜索, 调用 find 成员方法
    删除: erase(key) erase(it)
    */
    int main() {
    
        // unordered_multiset<int> set1;
        unordered_set<int> set1;  // 不会存储 key 值重复的元素
        for (int i = 0; i < 50; ++i) {
            set1.insert(rand() % 20 + 1);  
            // 与 vector/deque/list insert(it, val) (线性表) 不同, 只需要一个参数
        }
    
        cout << set1.size() << endl;  // 返回容器中元素个数
        cout << set1.count(15) << endl;  // 返回 key 为 15 的元素的个数
    
        // 迭代器遍历集合容器
        auto it1 = set1.begin();
        for (; it1 != set1.end(); ++it1) {
            cout << *it1 << " ";
        }
        cout << endl;
    
        // 按 key 值删除元素
        set1.erase(20);  
    
        // 迭代器遍历删除, 只删除一次
        for (it1 = set1.begin(); it1 != set1.end(); ++it1) {
            if (*it1 == 30) {
                it1 = set1.erase(it1);  // 调用 erase, it1 迭代器就失效了
            }
        }
    
        // 迭代器遍历删除, 连续删除
        for (it1 = set1.begin(); it1 != set1.end(); ) {
            if (*it1 == 30) {
                it1 = set1.erase(it1);  // 调用 erase, it1 迭代器就失效了
            }
            else {
                ++it1;
            }
        }
    
        // find 查询
        it1 = set1.find(20);  // 不存在则返回末尾迭代器
        if (it1 != set1.end()) {
            set1.erase(it1);
        }
    
        for (int v : set1) {
            cout << v << " ";
        }
        cout << endl;
    
        return 0;
    }
    ```

+ map

  用法示例

  ```c++
  #include <iostream>
  #include <string>
  #include <unordered_map>

  /*
  [key, value]
  struct pair {
    first; => key
    second; => value
  };
  */

  int main() {

      // unordered_multimap<int, string> map1;
      unordered_map<int, string> map1;
      map1.insert(make_pair(1000, "张三"));
      map1.insert({1010, "李四"});
      map1.insert({1020, "王五"});
      map1.insert({1000, "王凯"});

      map1.erase(1020);  

      /*
      map 的 operatorp[]
      1. 查询
      2. 如果 key 不存在, 它会插入一对数据 [key, string()]
      V& operator[](const K &key) {
        insert({key, V()});
      }
      */
      // map1[2000];  // 插入元素
      map1[2000] = "刘硕";  // 等价于 map1.insert({2000, "刘硕"});
      map1[1000] = "张三2";  // 修改

      cout << map1.size() << endl;
      // map operator[](key) => value
      cout << map1[1000] << endl;  // 张三

      // find 查询
      auto it1 = map1.find(1010);
      if (it1 != map1.end()) {
          // it1 -> pair 对象
          cout << "key: " << it1->first << " value: " << it1->second << endl;
      }

      return 0;
  }
  ```

+ 处理海量数据查重复，去重复时，会用到哈希表

    ```c++
    #include <iostream>
    #include <unordered_map>
    
    // 查重
    int main() {
        const int ARR_LEN = 100;
        int arr[ARR_LEN] = { 0 };
        for (int i = 0; i < ARR_LEN; ++i) {
        	arr[i] = rand() % 20 + 1;
    	}
    
        // 上面的 100 个整数中, 统计哪些数字重复了, 并且统计数字重复次数
        unordered_map<int, int> map1;
        for (int k : arr) {
            /*
            auto it = map1.find(k);
            if (it == map1.end()) {
            	map1.insert({k, 1});  // k 未出现
            }
            else {
           		it->second++;
            }
            */
    
            map1[k]++;
        }
    
        // for_each 遍历
        // for (pair<int, int> p : map1) {  // 发生拷贝
        // for (pair<int, int> &p : map1) {  // 报错
        for (const pair<int, int> &p : map1) {
            if (p.second > 1) {
                cout << "key: " << p.first << "count: " << p.second << endl;
            }
        }
    
        // 迭代器遍历
        auto it = map1.begin();
            for (; it != map1.end(); ++it) {
                if (it->second > 1) {
                cout << "key: " << it->first << " count: " << it->second << endl;
            }
        }
    
        return 0;
    }
    ```

    ```c++
    #include <iostream>
    #include <unordered_set>
    
    // 去重
    int main() {
        const int ARR_LEN = 100;
        int arr[ARR_LEN] = { 0 };
        for (int i = 0; i < ARR_LEN; ++i) {
        	arr[i] = rand() % 20 + 1;
        }
    
        // 上面的 100 个整数中, 把数字进行去重打印
        unordered_set<int> set;
        for (int v : arr) {  // O(N)
        	set.insert(v);  // O(1)
        }
        for (int v : set) {
        	cout << v << " ";
        }
        cout << endl;
    
        return 0;
    }
    ```

## 有序关联容器

```c++
#include <set>
#include <map>
```

+ 有序关联容器
  + set
  + multiset
  + map
  + multimap

+ 底层: 红黑树
+ 增删查时间复杂度: $O(log_2 n)$, (树的层数、树的高度) 
+ set: 使用示例 & 存储自定义类型

    ```c++
    #include <iostream>
    #include <set>
    #include <map>
    
    int main() {
        set<int> set1;
        for (int i = 0; i < 20; ++i) {
            set1.insert(rand()%20 + 1);
        }
    
        for (int v : set1) {  // 有序
            cout << v << " ";  // 对红黑树中序遍历的结果
        }
        cout << endl;
    
        return 0;
    }
    ```

    ```c++
    #include <iostream>
    #include <string>
    #include <set>
    #include <map>
    
    class Student {
    public:
        Student(int id, string name) :_id(id), _name(name) {}
        
        bool operator<(const Student &stu)const {  // 只读成员变量, 写成常方法
            return _id < stu._id;
        }
    private:
        int _id;
        string _name;
        friend ostream& operator<<(ostream &out, const Student &stu);
    };
    ostream& operator<<(ostream &out, const Student &stu) {
        cout << "id: " << stu._id << " name: " << stu._name << endl;
        return out;
    }
    
    int main() {
    
        // multiset<Student> set1;
        set<Student> set1;  // 存放自定义类型, 需提供 operator< 的重载
        set1.insert(Student(1000, "张雯"));
        set1.insert(Student(1020, "李广"));
    
        for (auto it = set1.begin(); it != set1.end(); ++it) {
            cout << *it << endl;
        }
        return 0;
    }
    ```

+ map

  ```c++
  #include <iostream>
  #include <string>
  #include <map>
  
  class Student {
  public:
      Student(int id=0, string name="") :_id(id), _name(name) {}
      // Student(int id, string name) :_id(id), _name(name) {}
  
  private:
      int _id;
      string _name;
      friend ostream& operator<<(ostream &out, const Student &stu);
  };
  ostream& operator<<(ostream &out, const Student &stu) {
      cout << "id: " << stu._id << "name: " << stu._name << endl;
      return out;
  }
  
  int main() {
      map<int, Student> stuMap;
      stuMap.insert({1000, Student(1000, "张雯")});
      stuMap.insert({1020, Student(1020, "李广")});
      stuMap.insert({1030, Student(1030, "高洋")});
      
      // 删除 stuMap.erase(it) or stuMap.erase(1020) 
      
      // 查询 stuMap[key] or stuMap.find(key)
      cout << stuMap[1020] << endl;
      // stuMap[2000] 若不存在, 会插入 { 2000, V() }
      
      auto it = stuMap.begin();
      for (; it != stuMap.end(); ++it) {
          cout << "key: " << it->first << " value: " << it->second << endl;
      } 
      cout << endl;
      
      return 0;
  }
  ```

## 迭代器

+ const_iterator (基类):  常量的正向迭代器, 只能读容器的元素, 不能修改容器的元素

+ iterator (派生类): 普通的正向迭代器, 既可以读容器的元素, 又可以修改容器的元素

+ const_reverse_iterator: 常量的反向迭代器

+ reverse_iterator: 反向迭代器

```c++
#include <iostream>
#include <vector>

int main() {
    vector<int> vec;
    for (int i = 0; i < 20; ++i) {
        vec.push_back(rand() % 100);
    }
    
    // auto -> vector<int>::iterator
    auto it1 = vec.begin();
    // vector<int>::const_iterator it1 = vec.begin();
    /*
    class const_iterator {}
    class iterator : public const_iterator {}
    */
    for (; it1 != vec.end(); ++it1) {
        cout << *it1 << " ";
        if (*it1 % 2 == 0) {
            *it1 = 0;  // const_iterator 返回常引用, 不能修改
        }
    }
    cout << endl;
    
    // rbegin() 返回的是最后一个元素的反向迭代器表示
    // rend() 返回的是首元素前驱位置的迭代器表示
    // vector<int>::reverse_iterator rit = vec.rbegin();
    auto rit = vec.rbegin();
    for (; rit != vec.rend(); ++rit) {
        cout << *rit << " ";
    }
    cout << endl;
    
    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;
}
```

## 函数对象

+ 函数对象, 类似 C 语言里面的函数指针

+ 定义: 把有 operator() 小括号运算符重载函数的对象, 称作函数对象, 或者称作仿函数

  ```c
  // C 函数指针
  int sum(int a, int b) {
      return a + b;
  }
  
  int ret = sum(10, 20);
  ```

  ```c++
  // C++ 函数对象
  class Sum {
  public:
      int operator()(int a, int b) {
          return a + b;
      }
  };
  
  Sum sum;
  int ret = sum(10, 20);  // sum.operator()(10, 20)
  ```

+ 函数对象相对于 C 函数指针的优点

  + 通过函数对象调用 opeartor(), 可以省去函数的调用开销, 比通过函数指针调用函数 (不能够 inline 内联调用) 效率高
  + 因为函数对象是用类生成的, 所以可以添加相关的成员变量, 用来记录函数对象使用时更多的信息 

  ```c++
  #include <iostream>
  
  // 存在问题, 不能自定义作 大于 小于 比较
  template<typename T>
  bool compare(T a, T b) {
      return a > b;
  }
  
  int main() {
      cout << compare(10, 20) << endl;
      cout << compare('b', 'y') << endl;
      
      return 0;
  }
  ```

  ```c
  #include <iostream>
  
  // 解决方案: 使用 C 的函数指针
  template<typename T>
  bool mygreater(T a, T b) {
      return a > b;
  }
  
  template<typename T>
  bool myless(T a, T b) {
      return a < b;
  }
  
  // 库函数模板
  template<typename T, typename Compare>
  bool compare(T a, T b, Compare comp) {
      // 通过函数指针调用函数, 是没有办法内联的, 效率很低, 因为有函数调用开销
      return comp(a, b);
  }
  
  int main() {
      cout << compare(10, 20, mygreater<int>) << endl;  // 传入的是函数的地址
      cout << compare(10, 20, myless<int>) << endl;
      
      return 0;
  }
  ```

  ```c++
  #include <iostream>
  
  // 解决方案: C++ 函数对象的版本实现
  template<typename T>
  class mygreater {
  public:
      bool operator()(T a, T b) {  // 两个参数, 叫做 二元函数对象
          return a > b;
      }
  }
  
  template<typename T>
  class myless {
  public:
      bool operator()(T a, T b) {  // 二元函数对象
          return a < b;
      }
  }
  
  // 库函数模板
  template<typename T, typename Compare>
  bool compare(T a, T b, Compare comp) {
      return comp(a, b);  // 编译过程明确调用的是什么函数, 可以内联, 效率高
  }
  
  int main() {
      cout << compare(10, 20, mygreater<int>()) << endl;  // 传入的是对象
      cout << compare(10, 20, myless<int>()) << endl;
      
      return 0;
  }
  ```

+ 例子: 优先级队列

  ```c++
  #include <iostream>
  #include <queue>
  
  int main() {
      priority_queue<int> que1;  // 底层默认是大根堆, 想使用小根堆怎么办
      for (int i = 0; i < 10; ++i) {
          que1.push(rand() % 100);
      }
      
      // 输出 从大到小
      while (!que1.empty()) {
          cout << que1.top() << " ";
          que1.pop();
      }
      cout << endl;
      
      return 0;
  }
  ```

  ```c++
  #include <iostream>
  #include <queue>
  #include <vector>
  
  int main() {
      using MinHeap = priority_queue<int, vector<int>, greater<int>>;  // 底层使用小根堆
      MinHeap que2;  
      for (int i = 0; i < 10; ++i) {
          que2.push(rand() % 100);
      }
      
      // 输出 从小到大
      while (!que2.empty()) {
          cout << que2.top() << " ";
          que2.pop();
      }
      cout << endl;
      
      return 0;
  }
  ```

+ set

  ```c++
  #include <iostream>
  #include <set>
  
  int main() {
      set<int, greater<int>> set1;
      for (int i = 0; i < 10; ++i) {
          set1.insert(rand() % 100);
      }
      
      // 从大到小
      for (int v : set1) {
          cout << v << " ";
      }
      cout << endl;
      
      return 0;
  }
  ```

## 泛型算法和绑定器

```c++
#include <algorithm>  // 包含了 C++ STL 里面的泛型算法
#include <functional>  // 包含了绑定器、函数对象
```

+ 泛型算法: template + 迭代器 + 函数对象
  + 算法是用 模板template 实现的
  + 算法是给所有容器用的, 所以接收的是容器的迭代器
  + 算法功能是可以更改的, 通过传入不同的 函数对象 更改算法功能
+ 特点
  + 泛型算法的参数接收的都是迭代器
  + 泛型算法的参数还可以接收函数对象

```c++
#include <algorithm>  
#include <functional>
#include <vector>

int main() {
    int arr[] = {12, 4, 78, 9, 21, 43, 56, 52, 42, 31};
    vector<int> vec(arr, arr + sizeof(arr)/sizeof(arr[0]));
    
    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;
    
    // 输出排序后元素
    sort(vec.begin(), vec.end());  // 默认快排, 从小到大排序
    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;
    
    // 有序容器中进行二分查找
    if (binary_search(vec.begin(), vec.end(), 21)) {
        cout << "binary_search 21 存在" << endl;
    }
    
    // 对有序容器, 二分查找效率更高
    auto it1 = find(vec.begin(), vec.end(), 21);
    if (it1 != vec.end()) {
        cout << "find 21 存在";
    }
    
    // 传入函数对象 greater, 改变容器元素排序时的比较方式
    sort(vec.begin(), vec.end(), greater<int>()); 
    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;
    
    /*
    绑定器 + 二元函数对象 => 一元函数对象
    bind1st: 把二元函数对象的 operator()(a, b) 的第一个形参绑定起来
    bind2nd: 把二元函数对象的 operator()(a, b) 的第二个形参绑定起来
    */
    // 把 48 按序插入到 vector 容器当中, 找第一个小于 48 的数字
    // find_if 需要的是一个一元函数对象
    // greater a > b, less a < b
    // 下面三句, 实现效果一样
    auto it2 = find_if(vec.begin(), vec.end(), bind1st(greater<int>(), 48));
    auto it2 = find_if(vec.begin(), vec.end(), bind2nd(less<int>(), 48));
    // lamda 表达式
    auto it2 = find_if(vec.begin(), vec.end(), [](int val)->bool { return val < 48;});  
    vec.insert(it2, 48);
    for (int v : vec) {
        cout << v << " ";
    }
    cout << endl;
    
    // for_each 可以遍历容器的所有元素
    // 可以自行添加合适的函数对象对容器的元素进行过滤
    // lamda 表达式
    for_each(
        vec.begin(), 
        vec.end(),
        [](int val)->void {
            if (val % 2 == 0) {
                cout << val << " ";
            }
        }
    );
    cout << endl;
    
    return 0;
}
```