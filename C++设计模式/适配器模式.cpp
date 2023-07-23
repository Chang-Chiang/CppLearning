#include <iostream>

using std::cout;
using std::endl;

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
    VGAToHDMIAdapter(HDMI* p) : pHDMI(p) {}

    // 转接头
    void play() { pHDMI->play(); }

private:
    HDMI* pHDMI;
};

// 只支持 VGA 接口的电脑类
class Computer {
public:
    // 方法参数只支持 VGA 接口的指针/引用
    void playVideo(VGA* pVGA) { pVGA->play(); }
};

//
int main() {
    Computer computer;
    computer.playVideo(new TV01());
    computer.playVideo(new VGAToHDMIAdapter(new TV02()));

    return 0;
}
