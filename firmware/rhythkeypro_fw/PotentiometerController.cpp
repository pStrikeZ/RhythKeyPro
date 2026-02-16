#include "PotentiometerController.h"
#include <XInput.h>

PotentiometerController::PotentiometerController() :
    potValue(0), prevPotValue(hardwareConfig.pot.centerValue) {
}

void PotentiometerController::begin() {
    // 电位器引脚设为输入
    pinMode(hardwareConfig.pins.potentiometer, INPUT);

    // 初始校准
    delay(100);
    potValue = analogRead(hardwareConfig.pins.potentiometer);
    prevPotValue = potValue;
}

void PotentiometerController::update(ControllerMode mode) {
    readValue();

    // 根据当前模式分发到不同的映射
    if (mode == MODE_ONGEKI) {
        updateTriggers();
        // 清除 various 模式的摇杆映射
        XInput.setJoystickY(JOY_RIGHT, 0);
    } else {
        updateRightStickY();
        // 清除 ongeki 模式的扳机映射
        XInput.setTrigger(TRIGGER_LEFT, 0);
        XInput.setTrigger(TRIGGER_RIGHT, 0);
    }
}

void PotentiometerController::readValue() {
    int potSum = 0;
    const int sampleCount = 5;

    for (int i = 0; i < sampleCount; i++) {
        potSum += analogRead(hardwareConfig.pins.potentiometer);
    }

    potValue = potSum / sampleCount;
}

void PotentiometerController::updateTriggers() {
    // 应用死区
    int potDelta = abs(potValue - prevPotValue);
    if (potDelta <= hardwareConfig.pot.deadzone) {
        return;
    }

    // 以中心值为分界线映射到左右线性扳机
    if (potValue < hardwareConfig.pot.centerValue) {
        int leftTrigger = map(potValue, 0, hardwareConfig.pot.centerValue - 1, 255, 0);
        XInput.setTrigger(TRIGGER_LEFT, leftTrigger);
        XInput.setTrigger(TRIGGER_RIGHT, 0);
    } else {
        int rightTrigger = map(potValue, hardwareConfig.pot.centerValue, hardwareConfig.pot.maxValue, 0, 255);
        XInput.setTrigger(TRIGGER_LEFT, 0);
        XInput.setTrigger(TRIGGER_RIGHT, rightTrigger);
    }

    prevPotValue = potValue;
}

void PotentiometerController::updateRightStickY() {
    // 应用死区
    int potDelta = abs(potValue - prevPotValue);
    if (potDelta <= hardwareConfig.pot.deadzone) {
        return;
    }

    // 电位器偏左→摇杆偏下（负值），偏右→摇杆偏上（正值）
    int16_t joystickY = map(potValue, 0, hardwareConfig.pot.maxValue,
        hardwareConfig.encoder.joystickMin, hardwareConfig.encoder.joystickMax);

    XInput.setJoystickY(JOY_RIGHT, joystickY);

    prevPotValue = potValue;
}
