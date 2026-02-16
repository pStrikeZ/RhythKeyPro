#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include <Arduino.h>
#include <XInput.h>
#include <FastLED.h>
#include "Config.h"

class ModeManager {
public:
    ModeManager();
    ~ModeManager() = default;

    // 处理按钮矩阵的所有事件（含切换键检测 + 过滤 + XInput 发送）
    void processButtons(const bool currentState[3][6], const bool prevState[3][6]);

    // 获取当前模式对应的 LED 主题色数组
    const CRGB* getLEDTheme() const;

    // 获取当前模式
    ControllerMode getMode() const { return currentMode; }
    bool isOngekiMode() const { return currentMode == MODE_ONGEKI; }

private:
    ControllerMode currentMode;

    // 切换键在矩阵中的坐标
    static const uint8_t TOGGLE_ROW = 0;
    static const uint8_t TOGGLE_COL = 5;

    // 处理模式切换键
    void handleToggle(bool currentState, bool previousState);

    // 判断按键是否被当前模式屏蔽
    bool isButtonMasked(uint8_t row, uint8_t col) const;

    // 模式切换时释放所有被屏蔽的按键
    void releaseMaskedKeys();

    // 发送单个按键的 XInput 事件
    void sendButtonEvent(bool currentState, bool previousState, uint8_t row, uint8_t col);
};

#endif // MODE_MANAGER_H
