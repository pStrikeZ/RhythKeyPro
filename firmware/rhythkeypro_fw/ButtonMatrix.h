#ifndef BUTTON_MATRIX_H
#define BUTTON_MATRIX_H

#include <Arduino.h>
#include <XInput.h>
#include "Config.h"

class ButtonMatrix {
public:
    ButtonMatrix();
    ~ButtonMatrix() = default;

    // 初始化按钮矩阵
    void begin();

    // 扫描按钮矩阵
    void scan();

    // 处理切换键
    void handleToggleKey(bool currentState, bool previousState);

    // 设置侧键启用状态
    void setSideKeyEnabled(bool enabled);

    // 获取侧键状态
    bool isSideKeyEnabled() const { return sideKeyEnabled; }

    // 检查按键是否被屏蔽
    bool isButtonMasked(uint8_t row, uint8_t col) const;

private:
    bool buttonState[3][6];
    bool prevButtonState[3][6];
    bool sideKeyEnabled;

    // 检查并更新XInput按钮
    void checkXInputButton(bool currentState, bool previousState, uint8_t row, uint8_t col);

    // 释放侧键
    void releaseSideKeys();

    // 扫描单行按钮
    void scanRow(uint8_t rowIndex);

    // 获取按钮映射
    uint16_t getButtonCode(uint8_t row, uint8_t col) const;
};

#endif // BUTTON_MATRIX_H