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

    // 初始化 Rumble LED 接收器（注册 XInput 回调）
    rumbleLEDReceiver.begin();

    // 初始化LED显示
    ledController.update(modeManager.getLEDTheme());
}

void RhythKeyController::update() {
    // 1. 扫描按钮矩阵（纯硬件读取）
    buttonMatrix.scan();

    // 2. 模式管理器处理按键事件（切换检测 + 过滤 + XInput 发送）
    modeManager.processButtons(buttonMatrix.getState(), buttonMatrix.getPrevState());

    // 3. 模式切换时重置编码器位置，并清除 rumble LED 数据
    if (modeManager.consumeModeChanged()) {
        encoderController.resetPositions();
        rumbleLEDReceiver.clearData();
    }

    // 4. 处理电位器（模式感知）
    potController.update(modeManager.getMode());

    // 5. 更新编码器
    encoderController.update();

    // 6. 检查 rumble 心跳超时
    rumbleLEDReceiver.checkTimeout();

    // 7. 更新LED显示
    if (modeManager.isOngekiMode() && rumbleLEDReceiver.hasData()) {
        // ONGEKI 模式下有 rumble 数据：主题色打底 + 动态覆盖 6 颗灯
        ledController.updateWithOverlay(
            modeManager.getLEDTheme(),
            rumbleLEDReceiver.getColors(),
            DYNAMIC_LED_INDICES,
            DYNAMIC_LED_COUNT);
    } else {
        // VARIOUS 模式或无 rumble 数据：使用静态主题色
        ledController.update(modeManager.getLEDTheme());
    }
}