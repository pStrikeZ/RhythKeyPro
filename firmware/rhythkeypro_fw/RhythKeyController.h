#ifndef RHYTHKEY_CONTROLLER_H
#define RHYTHKEY_CONTROLLER_H

#include <Arduino.h>
#include "Config.h"
#include "LEDController.h"
#include "EncoderController.h"
#include "ButtonMatrix.h"
#include "ModeManager.h"

class RhythKeyController {
public:
    RhythKeyController();
    ~RhythKeyController() = default;

    // 初始化控制器
    void begin();

    // 主循环更新
    void update();

    // 获取当前模式
    ControllerMode getMode() const { return modeManager.getMode(); }
    bool isOngekiMode() const { return modeManager.isOngekiMode(); }

private:
    LEDController ledController;
    EncoderController encoderController;
    ButtonMatrix buttonMatrix;
    ModeManager modeManager;

    // 电位器相关
    int potValue;
    int prevPotValue;

    // 处理电位器输入
    void processPotentiometer();

    // 更新左摇杆位置（基于电位器）
    void updateLeftStick(int potValue);

    // 初始化电位器
    void initPotentiometer();
};

#endif // RHYTHKEY_CONTROLLER_H