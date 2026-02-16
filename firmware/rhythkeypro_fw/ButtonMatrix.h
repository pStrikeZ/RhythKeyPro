#ifndef BUTTON_MATRIX_H
#define BUTTON_MATRIX_H

#include <Arduino.h>
#include "Config.h"

// 纯扫描层：只负责读取矩阵引脚状态，不处理任何业务逻辑
class ButtonMatrix {
public:
    ButtonMatrix();
    ~ButtonMatrix() = default;

    // 初始化按钮矩阵引脚
    void begin();

    // 扫描按钮矩阵，更新内部状态
    void scan();

    // 获取当前 / 上一帧的按键状态（供 ModeManager 消费）
    const bool (*getState() const)[6] { return buttonState; }
    const bool (*getPrevState() const)[6] { return prevButtonState; }

private:
    bool buttonState[3][6];
    bool prevButtonState[3][6];

    // 扫描单行引脚
    void scanRow(uint8_t rowIndex);
};

#endif // BUTTON_MATRIX_H