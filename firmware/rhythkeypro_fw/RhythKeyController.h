#ifndef RHYTHKEY_CONTROLLER_H
#define RHYTHKEY_CONTROLLER_H

#include <Arduino.h>
#include "Config.h"
#include "LEDController.h"
#include "EncoderController.h"
#include "PotentiometerController.h"
#include "ButtonMatrix.h"
#include "ModeManager.h"

class RhythKeyController {
public:
    RhythKeyController() = default;
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
    PotentiometerController potController;
    ButtonMatrix buttonMatrix;
    ModeManager modeManager;
};

#endif // RHYTHKEY_CONTROLLER_H