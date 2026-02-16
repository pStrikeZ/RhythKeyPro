#include "ButtonMatrix.h"

ButtonMatrix::ButtonMatrix() {
    // 初始化按钮状态（低电平有效，初始为 HIGH）
    memset(buttonState, HIGH, sizeof(buttonState));
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
    // 保存上一帧状态
    memcpy(prevButtonState, buttonState, sizeof(buttonState));

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

        // 第5列(索引4)是A2，第6列(索引5)是A3，特殊处理
        if (col == 4) pin = A2;
        if (col == 5) pin = A3;

        buttonState[rowIndex][col] = digitalRead(pin);
    }
}