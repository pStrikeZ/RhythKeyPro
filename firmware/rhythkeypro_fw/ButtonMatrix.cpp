#include "ButtonMatrix.h"

ButtonMatrix::ButtonMatrix() : sideKeyEnabled(true) {
    // 初始化按钮状态
    memset(buttonState, HIGH, sizeof(buttonState)); // 低电平有效，所以初始化为HIGH
    memset(prevButtonState, HIGH, sizeof(prevButtonState));
}

void ButtonMatrix::begin() {
    // 初始化行引脚为输出
    for (uint8_t i = 0; i < 3; i++) {
        pinMode(hardwareConfig.pins.row[i], OUTPUT);
    }

    // 初始化列引脚为输入上拉
    for (uint8_t i = 0; i < 6; i++) {
        pinMode(hardwareConfig.pins.col[i], INPUT_PULLUP);
    }
}

void ButtonMatrix::scan() {
    // 扫描所有行
    for (uint8_t row = 0; row < 3; row++) {
        scanRow(row);
    }
}

void ButtonMatrix::scanRow(uint8_t rowIndex) {
    // 设置所有行为高电平
    for (uint8_t i = 0; i < 3; i++) {
        digitalWrite(hardwareConfig.pins.row[i], HIGH);
    }

    // 当前行设为低电平
    digitalWrite(hardwareConfig.pins.row[rowIndex], LOW);

    // 扫描所有列
    for (uint8_t col = 0; col < 6; col++) {
        uint8_t pin = hardwareConfig.pins.col[col];
        bool currentState = digitalRead(pin);

        // 第5列(索引4)是A2，第6列(索引5)是A3，特殊处理
        if (col == 4) pin = A2;
        if (col == 5) pin = A3;

        currentState = digitalRead(pin);

        // 特殊处理第5列(行0为功能键)
        if (rowIndex == 0 && col == 5) {
            handleToggleKey(currentState, prevButtonState[rowIndex][col]);
        } else {
            checkXInputButton(currentState, prevButtonState[rowIndex][col], rowIndex, col);
        }

        // 更新前一状态
        prevButtonState[rowIndex][col] = currentState;
    }
}

void ButtonMatrix::checkXInputButton(bool currentState, bool previousState, uint8_t row, uint8_t col) {
    uint16_t buttonCode = getButtonCode(row, col);

    // 如果按钮代码为0，跳过
    if (buttonCode == 0) return;

    // 检查侧键屏蔽
    if (!sideKeyEnabled && isButtonMasked(row, col)) {
        // 如果当前是按下状态，强制释放
        if (currentState == LOW) {
            XInput.release(buttonCode);
        }
        return;
    }

    // 检测状态变化 (低电平有效)
    if (currentState == LOW && previousState == HIGH) {
        // 按键按下
        XInput.press(buttonCode);
    } else if (currentState == HIGH && previousState == LOW) {
        // 按键松开
        XInput.release(buttonCode);
    }
}

void ButtonMatrix::handleToggleKey(bool currentState, bool previousState) {
    if (currentState == LOW && previousState == HIGH) {
        sideKeyEnabled = !sideKeyEnabled;

        // 关闭时强制释放所有侧键和菜单键
        if (!sideKeyEnabled) {
            releaseSideKeys();
        }
    }
}

void ButtonMatrix::setSideKeyEnabled(bool enabled) {
    sideKeyEnabled = enabled;

    if (!enabled) {
        releaseSideKeys();
    }
}

bool ButtonMatrix::isButtonMasked(uint8_t row, uint8_t col) const {
    for (uint8_t i = 0; i < gameConfig.sideKeyMask.count; i++) {
        const auto& maskBtn = gameConfig.sideKeyMask.buttons[i];
        if (maskBtn.row == row && maskBtn.col == col) {
            return true;
        }
    }
    return false;
}

void ButtonMatrix::releaseSideKeys() {
    // 释放左侧键
    XInput.release(BUTTON_LB);
    // 释放左菜单键
    XInput.release(BUTTON_BACK);
    // 释放右菜单键
    XInput.release(BUTTON_START);
    // 释放右侧键
    XInput.release(BUTTON_RB);
}

uint16_t ButtonMatrix::getButtonCode(uint8_t row, uint8_t col) const {
    if (row < 3 && col < 6) {
        return gameConfig.buttonMap[row][col];
    }
    return 0;
}