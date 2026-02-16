#include "RhythKeyController.h"
#include <XInput.h>

RhythKeyController::RhythKeyController() :
    potValue(0), prevPotValue(hardwareConfig.pot.centerValue) {
}

void RhythKeyController::begin() {
    // 初始化各模块
    ledController.begin();
    encoderController.begin();
    buttonMatrix.begin();

    // 初始化XInput
    XInput.begin();
    XInput.setAutoSend(true);

    // 初始化电位器
    initPotentiometer();

    // 初始化LED显示
    ledController.update(modeManager.getLEDTheme());
}

void RhythKeyController::update() {
    // 1. 扫描按钮矩阵（纯硬件读取）
    buttonMatrix.scan();

    // 2. 模式管理器处理按键事件（切换检测 + 过滤 + XInput 发送）
    modeManager.processButtons(buttonMatrix.getState(), buttonMatrix.getPrevState());

    // 3. 处理电位器
    processPotentiometer();

    // 4. 更新编码器
    encoderController.update();

    // 5. 更新LED显示（由模式管理器提供当前主题）
    ledController.update(modeManager.getLEDTheme());
}

void RhythKeyController::processPotentiometer() {
    // 读取电位器值（多次采样平均）
    int potSum = 0;
    const int sampleCount = 5;

    for (int i = 0; i < sampleCount; i++) {
        potSum += analogRead(hardwareConfig.pins.potentiometer);
    }

    potValue = potSum / sampleCount;

    // 更新左摇杆位置
    updateLeftStick(potValue);
}

void RhythKeyController::updateLeftStick(int potCurrent) {
    // 应用死区
    int potDelta = abs(potCurrent - prevPotValue);
    if (potDelta <= hardwareConfig.pot.deadzone) {
        return; // 在死区内，保持不变
    }

    // 以中心值为分界线映射到左右线性扳机
    if (potCurrent < hardwareConfig.pot.centerValue) {
        // 偏左：映射到左线性扳机
        int leftTrigger = map(potCurrent, 0, hardwareConfig.pot.centerValue - 1, 255, 0);
        XInput.setTrigger(TRIGGER_LEFT, leftTrigger);
        XInput.setTrigger(TRIGGER_RIGHT, 0);
    } else {
        // 偏右：映射到右线性扳机
        int rightTrigger = map(potCurrent, hardwareConfig.pot.centerValue, hardwareConfig.pot.maxValue, 0, 255);
        XInput.setTrigger(TRIGGER_LEFT, 0);
        XInput.setTrigger(TRIGGER_RIGHT, rightTrigger);
    }

    prevPotValue = potCurrent;
}

void RhythKeyController::initPotentiometer() {
    // 电位器引脚设为输入
    pinMode(hardwareConfig.pins.potentiometer, INPUT);

    // 初始校准
    delay(100);
    potValue = analogRead(hardwareConfig.pins.potentiometer);
    prevPotValue = potValue;
}