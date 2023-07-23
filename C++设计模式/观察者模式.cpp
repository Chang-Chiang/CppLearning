#include <iostream>
#include <list>
#include <unordered_map>

using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::unordered_map;

// 主题有更改, 应该及时通知相应的观察者, 去处理相应的事件

// 观察者抽象类
class Observer {
public:
    // 处理消息的接口
    virtual void handle(int msgid) = 0;
};

// 观察者实例类
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
    void dispatch(int msgid) {
        auto it = _subMap.find(msgid);
        if (it != _subMap.end()) {
            for (Observer* pObser : it->second) {
                pObser->handle(msgid);
            }
        }
    }

private:
    unordered_map<int, list<Observer*>> _subMap;
};

int main() {
    Subject   subject;
    Observer* p1 = new Observer1();
    Observer* p2 = new Observer2();
    Observer* p3 = new Observer3();

    subject.addObserver(p1, 1);
    subject.addObserver(p1, 2);
    subject.addObserver(p2, 2);
    subject.addObserver(p3, 1);
    subject.addObserver(p3, 3);

    int msgid = 0;
    for (;;) {
        cout << "输入消息 id: ";
        cin >> msgid;
        if (msgid == -1) {
            break;
        }
        subject.dispatch(msgid);
    }

    return 0;
}
