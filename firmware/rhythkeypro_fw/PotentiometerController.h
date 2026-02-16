#ifndef POTENTIOMETER_CONTROLLER_H
#define POTENTIOMETER_CONTROLLER_H

#include <Arduino.h>
#include "Config.h"

class PotentiometerController {
public:
    PotentiometerController();
    ~PotentiometerController() = default;

    // 初始化电位器
    void begin();

    // 根据当前模式更新电位器映射
    void update(ControllerMode mode);

private:
    int potValue;
    int prevPotValue;

    // 读取电位器（多次采样平均）
    void readValue();

    // ONGEKI 模式：映射到左右线性扳机
    void updateTriggers();

    // VARIOUS 模式：映射到右摇杆Y轴
    void updateRightStickY();
};

#endif // POTENTIOMETER_CONTROLLER_H
