# 设计模式

> 参考: [C++设计模式 - 单例模式_大秦坑王的博客-CSDN博客](https://blog.csdn.net/QIANGWEIYUAN/article/details/88544524?spm=1001.2014.3001.5502)

## 单例模式

一个类不管创建多少次对象, 永远只能得到该类型一个对象的实例, 常用到的比如日志模块、数据库模块

+ 饿汉式单例模式: 还没有获取实例对象, 实例对象就已经产生了
+ 懒汉式单例模式: 唯一的实例对象, 直到第一次获取它的时候, 才产生

### 饿汉式单例模式

```c++
// 饿汉式单例模式
// 一定是线程安全的
// 缺点: 一定会调用构造函数, 构造操作太多时, 资源浪费
class Singleton {
public:
    static Singleton* getInstance() {  // #3 获取类的唯一实例对象的接口方法
        return &instance;
    }
private:
    static Singleton instance;  // #2 定义一个唯一的类的实例对象
    Singleton() {  // #1 构造函数私有化
        
    }
    
    // 删除默认拷贝构造, =赋值操作符重载
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

// 静态成员变量，在数据段
Singleton Singleton::instance;  // 静态成员变量在类外定义

int main() {
    Singleton *p1 = Singleton::getInstance();
    Singleton *p2 = Singleton::getInstance();
    Singleton *p3 = Singleton::getInstance();
    // p1、p2、p3 地址一样
    
    return 0;
}
```

### 懒汉式单例模式

```c++
// 懒汉式单例模式
class Singleton {
public:
    static Singleton* getInstance() {  // #3 获取类的唯一实例对象的接口方法
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
private:
    static Singleton *instance;  // #2 定义一个唯一的类的实例对象
    Singleton() {  // #1 构造函数私有化
        
    }
    
    // 删除默认拷贝构造, =赋值操作符重载
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

Singleton Singleton::instance = nullptr;  // 静态成员变量在类外初始化

int main() {
    Singleton *p1 = Singleton::getInstance();
    Singleton *p2 = Singleton::getInstance();
    Singleton *p3 = Singleton::getInstance();
    
    return 0;
}
```

### 线程安全的懒汉单例模式一

```c++
// 线程安全的懒汉单例模式 【1】

#include <mutex>

std::mutex mtx;

// 懒汉式单例模式, 是不是线程安全的 ?
class Singleton {
public:
    
    /*
    // 是不是可重入函数 ? 不是可重入函数 !
    static Singleton* getInstance() {  
        if (instance == nullptr) {
            
            // 开辟内存
            // 给 instance 赋值
            // 构造对象
            instance = new Singleton();
        }
        return &instance;
    }
	*/
        
    static Singleton* getInstance() {  
        if (instance == nullptr) {
            lock_guard<std::mutex> guard(mtx);
            if (instance == nullptr) {
                instance = new Singleton();
            } 
        }
        return instance;
    }   

private:
    // static Singleton *instance;  
    static Singleton *volatile instance;  
    Singleton() { 
        
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

Singleton *volatile Singleton::instance = nullptr; 
```

### 线程安全的懒汉单例模式二

```c++
// 线程安全的懒汉单例模式 【2】
class Singleton {
public:
    static Singleton* getInstance() { 
        // g++ -o run 单例模式.cpp -g 
        // gdb run
        // 函数静态局部变量的初始化, 在汇编指令上已经自动添加线程互斥指令了
    	static Singleton instance;  
        return &instance;
    }
private: 
    Singleton() { 
        
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};
```

## 工厂模式

工厂模式, 主要是封装了对象的创建

+ 简单工厂 Simple Factory
  + 把对象的创建封装在一个接口函数里面, 通过传入不同的标识, 返回创建的对象
  + 客户不用自己负责 new 对象, 不用了解对象创建的详细过程
  + 缺点：提供创建对象实例的接口函数不闭合, 不能对修改关闭
+ 工厂方法 Factory Method
  + Factory 基类, 提供了一个纯虚函数 (创建产品), 定义派生类 (具体产品的工厂) 负责创建对应的产品
  + 可以做到不同的产品, 在不同的工厂里面创建, 能够对现有产品, 以及产品的修改关闭
  + 实际上, 很多产品是有关联关系的, 属于一个产品簇, 不应该放在不同的工厂里面去创建, 这样一是不符合实际的产品对象创建逻辑, 二是工厂类太多了, 不好维护
+ 抽象工厂 Abstract Factory
  + 把有关联关系的, 属于一个产品簇的所有产品创建的接口函数, 放在一个抽象工厂里面
  + 派生类 (具体产品的工厂) 应该负责创建该产品簇里面所有的产品

```c++
class Car {
public:
    Car(string name) :_name(name) {}
    virtual void show() = 0;
protected:
    string _name;
};

class Bmw : public Car {
public:
    Bmw(string name) :Car(name) {}
    void show() {
        cout << "获取了一辆宝马汽车: " << _name << endl;
    }
};

class Audi : public Car {
public:
    Audi(string name) :Car(name) {}
    void show() {
        cout << "获取了一奥迪汽车" << _name << endl;
    }
};

int main() {
    Car *p1 = new Bmw("X1");
    Car *p2 = new Audi("A6");
    
    return 0;
}
```

### 简单工厂

```c++
enum CarType { BMW, AUDI };
class SimpleFactory {
public:
    Car* createCar() {  // 缺点, 不封闭, 不符合软件设计的 "开闭" 原则
        switch (ct) {
            case BMW:
                return new Bmw("X1");
            case AUDI:
                return new Audi("A6");
            default:
                cerr << "传入工厂的参数不正确: " << ct << endl;
                break;
        }
        return nullptr;
    }
};

int main() {
    SimpleFactory *factory = new SimpleFactory();
    Car *p1 = factory->createCar(BMW);
    Car *p2 = factory->createCar(AUDI);
    
    p1->show();
    p2->show();
    delete p1;
    delete p2;
    delete factory;
    
    // -------------------------------------
    // 使用智能指针, 不用手动释放资源
    unique_ptr<SimpleFactory> factory(new SimpleFactory());
    unique_ptr<Car> p1(factory->createCar(BMW));
    unique_ptr<Car> p2(factory->createCar(AUDI));
    
    p1->show();
    p2->show();
    
    return 0;
}
```

### 工厂方法

```c++
class Factory {
public:
    virtual Car* createCar(string name) = 0;  // 工厂方法
};

class BMWFactory : public Factory {
public:
    Car* createCar(string name) {
        return new Bmw(name);
    }
};

class AUDIFactory : public Factory {
public:
    Car* createCar(string name) {
        return new Audi(name);
    }
};

int main() {
    unique_ptr<Factory> bmwfty(new BMWFactory());
    unique_ptr<Factory> audifty(new AUDIFactory());
    unique_ptr<Car> p1(bmwfty->createCar("X6"));
    unique_ptr<Car> p2(audifty->createCar("A8"));
    
    p1->show();
    p2->show();
    
    return 0;
}
```

### 抽象工厂

考虑产品簇(有关联关系的系列产品), 对每一个产品都需要定义一个类, 导致类急剧增多

```c++
// 系列产品 1
class Car {
public:
    Car(string name) :_name(name) {}
    virtual void show() = 0;
protected:
    string _name;
};

class Bmw : public Car {
public:
    Bmw(string name) :Car(name) {}
    void show() {
        cout << "获取了一辆宝马汽车: " << _name << endl;
    }
};

class Audi : public Car {
public:
    Audi(string name) :Car(name) {}
    void show() {
        cout << "获取了一奥迪汽车" << _name << endl;
    }
};

// 系列产品 2
class Light {
public:
    virtual void show() = 0;
};

class BmwLight : public Light {
public:
    void show() { cout << "BMW light!" << endl; }
};

class AudiLight : public Light {
public:
    void show() { cout << "AUDI light!" << endl; }
};
```

抽象工厂: 对一组有关联关系的产品簇提供产品对象的统一创建

```c++
class AbstractFactory {
public:
    virtual Car* createCar(string name) = 0;  // 工厂方法, 创建汽车
    virtual Light* createCarLight() = 0;  // 工厂方法, 创建汽车关联的产品, 车灯
};

class BMWFactory : public AbstractFactory {
public:
    Car* createCar(string name) {
        return new Bmw(name);
    }
    Light* createCarLight() {
        return new BmwLight();
    }
};

class AUDIFactory : public AbstractFactory {
public:
    Car* createCar(string name) {
        return new Audi(name);
    }
    Light* createCarLight() {
        return new AudiLight();
    }
};

int main() {
    unique_ptr<AbstractFactory> bmwfty(new BMWFactory());
    unique_ptr<AbstractFactory> audifty(new AUDIFactory());
    unique_ptr<Car> p1(bmwfty->createCar("X6"));
    unique_ptr<Car> p2(audifty->createCar("A8"));
    unique_ptr<Light> l1(bmwfty->createCarLight());
    unique_ptr<Light> l2(audifty->createCarLight());
    
    p1->show();
    p2->show();
    l1->show();
    l2->show();
    
    return 0;
}
```

## 代理模式

proxy 代理模式: 通过代理类, 来控制实际对象（委托类）的访问权限

```c++
class VideoSite {  // #1 抽象类
public:
    virtual void freeMovie() = 0;  // 免费电影
    virtual void vipMovie() = 0;  // vip 电影
    virtual void ticketMovie() = 0;  // 用券观看电影
};

class FixBugVideoSite : public VideoSite {  // #2 委托类
    virtual void freeMovie() {
        cout << "观看免费电影" << endl;
    }
    virtual void vipMovie() {
        cout << "观看 VIP 电影" << endl;
    }
    virtual void ticketMovie() {
        cout << "用券观看电影" << endl;
    }
};

// 代理 FixBugVideoSite
class FreeVideoSiteProxy : public VideoSite {  // #3 代理类
public:
    FreeVideoSiteProxy() { pVideo = new FixBugVideoSite(); }
    ~FreeVideoSiteProxy() { delete pVideo; }
    virtual void freeMovie() {
        pVideo->freeMovie();  // 通过代理对象的 freeMovie, 来访问真正委托类对象的 freeMovie
    }
    virtual void vipMovie() {
        cout << "普通游客, 需升级 VIP" << endl;
    }
    virtual void ticketMovie() {
        cout << "普通游客, 需买券" << endl;
    } 
private:
    VideoSite *pVideo;  // #4 组合的方式使用代理对象
    // FixBugVideoSite Video;
};

// 都是通用的 API 接口, 使用的都是基类的指针或引用
void watchMovie(unique_ptr<VideoSite> &ptr) {
    ptr->freeMovie();
    ptr->vipMovie();
    ptr->ticketMovie();
}

int main() {
    unique_ptr<VideoSite> p1(new FreeVideoSiteProxy());  // #5 用户直接访问代理对象
    unique_ptr<VideoSite> p1(new VipVideoSiteProxy());
    
    watchMovie(p1);
    watchMovie(p2);
    
    return 0;
}
```

## 装饰器模式

问题: 为了增强现有类的功能, 通过实现子类的方式, 重写接口, 是可以完成功能扩展的, 但是代码中有太多的子类添加进来了

Decorator: 主要是增加现有类的功能

```c++
class Car {
public:
    virtual void show() = 0;
};

class Bmw : public Car {
public:
    void show() { cout << "宝马: 基本配置"; }
};

class Audi : public Car {
public:
    void show() { cout << "奥迪: 基本配置"; }
};

class Benz : public Car {
public:
    void show() { cout << "奔驰: 基本配置"; }
};

// ---------------------------------
class ConcreteDecorator01 : public Car {
public: 
    ConcreteDecorator01(Car *p) :pCar(p) {}
    void show() { 
        pCar->show();
        cout << " 定速巡航";
    }
private:
    Car *pCar;
};

class ConcreteDecorator02 : public Car {
public: 
    ConcreteDecorator02(Car *p) :pCar(p) {}
    void show() { 
        pCar->show();
        cout << " 自动刹车";
    }
private:
    Car *pCar;
};

class ConcreteDecorator03 : public Car {
public: 
    ConcreteDecorator03(Car *p) :pCar(p) {}
    void show() { 
        pCar->show();
        cout << " 车道偏离";
    }
private:
    Car *pCar;
};

int main() {
    Car *p1 = new ConcreteDecorator01(new Bmw());
    p1 = new ConcreteDecorator02(p1);
    p1 = new ConcreteDecorator03(p1);
    p1->show();
    cout << endl;
    
    Car *p2 = new ConcreteDecorator02(new Audi());
    p2->show();
    cout << endl;
    
    Car *p3 = new ConcreteDecorator03(new Benz());
    p3->show();
    cout << endl;
    
    return 0;
}
```

## 适配器模式

让不兼容的接口可以在一起工作

```c++
// VGA 接口类
class VGA {  
public:
    virtual void play() = 0;
};

// TV01 表示支持 VGA 接口的投影仪
class TV01 : public VGA {
public:
    void play() { cout << "VGA 连接投影仪播放" << endl; }
};
// -------------------------------------------------
// 进了一批新的投影仪, 但是新的投影仪只支持 HDMI 接口
// HDMI 接口类
class HDMI {  
public:
    virtual void play() = 0;
};

// TV02 表示支持 HDMI 接口的投影仪
class TV02 : public HDMI {
public:
    void play() { cout << "HDMI 连接投影仪播放" << endl; }
};
// ------------------------------------------------------

/*
方法1: 换一个支持 HDMI 接口的电脑 => 代码重构
方法2: 转接头 VGA 转 HDMI => 添加适配器类
*/
class VGAToHDMIAdapter : public VGA {
public:
    VGAToHDMIAdapter(HDMI *p) :pHDMI(p) {}
    
    // 转接头
    void play() { pHDMI->play(); }
private:
    HDMI *pHDMI;
};
// 只支持 VGA 接口的电脑类
class computer {
public:
    // 方法参数只支持 VGA 接口的指针/引用
    void playVideo(VGA *pVGA) { pVGA->play(); }
};

// 
int main() {
    Computer computer;
    computer.playVideo(new TV01());
    computer.playVideo(new VGAToHDMIAdapter(new TV02()));
    
    return 0;
}
```

## 观察者模式

又叫 观察者-监听者模式, 发布-订阅模式

主要关注的是对象的一对多的关系, 也就是多个对象都依赖一个对象, 当该对象的状态发生改变时, 其他对象都能够接收到相应的通知。

例: 通过一组数据生成 曲线图 (对象 1)/柱状图 (对象 2)/圆饼图 (对象 3), 当数据对象改变时, 对象 1,2,3 应该及时收到相应的通知。

```c++
#include <unordered_map>
#include <list>
// 主题有更改, 应该及时通知相应的观察者, 去处理相应的事件

// 观察者抽象类
class Observer {
public:
    // 处理消息的接口
    virtual void handle(int msgid) = 0;
};

// 观察者实例
class Observer1 : public Observer {
public:
    void handle(int msgid) {
        switch (msgid) {
            case 1:
                cout << "Observer1 recv 1 msg!" << endl;
                break;
            case 2:
                cout << "Observer1 recv 2 msg!" << endl;
                break;
            default:
                cout << "Observer1 recv unknow msg!" << endl;
                break;
        }
    }
};
class Observer2 : public Observer {
public:
    void handle(int msgid) {
        switch (msgid) {
            case 2:
                cout << "Observer2 recv 2 msg!" << endl;
                break;
            default:
                cout << "Observer2 recv unknow msg!" << endl;
                break;
        }
    }
};
class Observer3 : public Observer {
public:
    void handle(int msgid) {
        switch (msgid) {
            case 1:
                cout << "Observer3 recv 1 msg!" << endl;
                break;
            case 3:
                cout << "Observer3 recv 3 msg!" << endl;
                break;
            default:
                cout << "Observer3 recv unknow msg!" << endl;
                break;
        }
    }
};

class Subject {
public:
    // 给主题增加观察者对象
    void addObserver(Observer* obser, int msgid) {
        _subMap[msgid].push_back(obser);
        /*
        auto it = _subMap.find(msgid);
        if (it != _subMap.end()) {
            it->second.push_back(obser);
        }
        else {
            list<Observer*> lis;
            lis.push_back(obser);
            _subMap.insert({msgid, lis});
        }
        */
    }
    
    // 主题监测发生改变, 通知相应的观察者对象处理事件
    void dispatch(int msg) {
        auto it = _subMap.find(msgid);
        if (it != _subMap.end()) {
            for (Observer *pObser : it->second) {
                pObser->handle(msgid);
            }
        }
    }
private:
    unordered_map<int, list<Observer*>> _subMap;
    
};

int main() {
    Subject subject;
    Observer *p1 = new Observer1();
    Observer *p2 = new Observer2();
    Observer *p3 = new Observer3();
    
    subject.addObserver(p1, 1);
    subject.addObserver(p1, 2);
    subject.addObserver(p2, 2);
    subject.addObserver(p3, 1);
    subject.addObserver(p3, 3);
    
    int msgid = 0;
    for (;;) {
        cout << "输入消息 id: ";
        cin >> msgid;
        if (msgid == -1)
            break;
        subject.dispatch(msgid);
    }
    
    return 0;
}
```
