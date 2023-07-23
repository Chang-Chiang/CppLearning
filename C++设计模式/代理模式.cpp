#include <iostream>
#include <memory>

using std::cerr;
using std::cout;
using std::endl;
using std::unique_ptr;

// #1 抽象类
class VideoSite {
public:
    virtual void freeMovie() = 0;   // 免费电影
    virtual void vipMovie() = 0;    // vip 电影
    virtual void ticketMovie() = 0; // 用券观看电影
};

// #2 委托类
class FixBugVideoSite : public VideoSite {
    virtual void freeMovie() { cout << "观看免费电影" << endl; }
    virtual void vipMovie() { cout << "观看 VIP 电影" << endl; }
    virtual void ticketMovie() { cout << "用券观看电影" << endl; }
};

// #3 代理类 代理 FixBugVideoSite
class FreeVideoSiteProxy : public VideoSite {
public:
    FreeVideoSiteProxy() { pVideo = new FixBugVideoSite(); }
    ~FreeVideoSiteProxy() { delete pVideo; }

    // 通过代理对象的 freeMovie, 来访问真正委托类对象的 freeMovie
    virtual void freeMovie() { pVideo->freeMovie(); }
    virtual void vipMovie() { cout << "普通游客, 需升级 VIP" << endl; }
    virtual void ticketMovie() { cout << "普通游客, 需买券" << endl; }

private:
    VideoSite* pVideo; // #4 组合的方式使用代理对象
    // FixBugVideoSite Video;
};

class VipVideoSiteProxy : public VideoSite {
public:
    VipVideoSiteProxy() { pVideo = new FixBugVideoSite(); }
    ~VipVideoSiteProxy() { delete pVideo; }

    // 通过代理对象的 freeMovie, 来访问真正委托类对象的 freeMovie
    virtual void freeMovie() { pVideo->freeMovie(); }
    virtual void vipMovie() { pVideo->vipMovie(); }
    virtual void ticketMovie() { cout << "普通游客, 需买券" << endl; }

private:
    VideoSite* pVideo; // #4 组合的方式使用代理对象
};

// 都是通用的 API 接口, 使用的都是基类的指针或引用
void watchMovie(unique_ptr<VideoSite>& ptr) {
    ptr->freeMovie();
    ptr->vipMovie();
    ptr->ticketMovie();
}

int main() {
    unique_ptr<VideoSite> p1(new FreeVideoSiteProxy()); // #5 用户直接访问代理对象
    unique_ptr<VideoSite> p2(new VipVideoSiteProxy());

    watchMovie(p1);
    watchMovie(p2);

    return 0;
}
