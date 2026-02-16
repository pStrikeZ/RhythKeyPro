#include "RhythKeyController.h"
#include <XInput.h>

void RhythKeyController::begin() {
    // 初始化各模块
    ledController.begin();
    encoderController.begin();
    potController.begin();
    buttonMatrix.begin();

    // 初始化XInput
    XInput.begin();
    XInput.setAutoSend(true);

    // 初始化LED显示
    ledController.update(modeManager.getLEDTheme());
}

void RhythKeyController::update() {
    // 1. 扫描按钮矩阵（纯硬件读取）
    buttonMatrix.scan();

    // 2. 模式管理器处理按键事件（切换检测 + 过滤 + XInput 发送）
    modeManager.processButtons(buttonMatrix.getState(), buttonMatrix.getPrevState());

    // 3. 处理电位器（模式感知）
    potController.update(modeManager.getMode());

    // 4. 更新编码器
    encoderController.update();

    // 5. 更新LED显示（由模式管理器提供当前主题）
    ledController.update(modeManager.getLEDTheme());
}