#include "ModeManager.h"

ModeManager::ModeManager() : currentMode(MODE_ONGEKI), modeChanged(false) {
    // 初始化长按状态
    for (uint8_t i = 0; i < 2; i++) {
        longPressState[i].pressStartTime = 0;
        longPressState[i].tracking = false;
        longPressState[i].pressSent = false;
    }
}

void ModeManager::processButtons(const bool currentState[3][6], const bool prevState[3][6]) {
    for (uint8_t row = 0; row < 3; row++) {
        for (uint8_t col = 0; col < 6; col++) {
            // 切换键：检测模式切换
            if (row == TOGGLE_ROW && col == TOGGLE_COL) {
                handleToggle(currentState[row][col], prevState[row][col]);
                continue;
            }

            // ONGEKI 模式下检查是否为长按按键
            if (currentMode == MODE_ONGEKI) {
                int8_t lpIndex = getLongPressIndex(row, col);
                if (lpIndex >= 0) {
                    handleLongPressButton(currentState[row][col], row, col, lpIndex);
                    continue;
                }
            }

            // 普通按键：通过过滤层发送事件
            sendButtonEvent(currentState[row][col], prevState[row][col], row, col);
        }
    }
}

void ModeManager::handleToggle(bool currentState, bool previousState) {
    // 下降沿触发（低电平有效，HIGH -> LOW = 按下）
    if (currentState == LOW && previousState == HIGH) {
        // 切换前如果当前模式有屏蔽按键，先释放
        if (currentMode == MODE_VARIOUS) {
            releaseMaskedKeys();
        }

        // 循环切换到下一个模式
        currentMode = static_cast<ControllerMode>((currentMode + 1) % MODE_COUNT);
        modeChanged = true;

        // 切换后如果新模式有屏蔽按键，释放它们
        if (currentMode == MODE_VARIOUS) {
            releaseMaskedKeys();
        }

        // 切换模式时重置所有长按状态
        for (uint8_t i = 0; i < gameConfig.longPress.count; i++) {
            if (longPressState[i].pressSent) {
                uint16_t buttonCode = gameConfig.buttonMap
                    [gameConfig.longPress.buttons[i].row]
                    [gameConfig.longPress.buttons[i].col];
                if (buttonCode != 0) {
                    XInput.release(buttonCode);
                }
            }
            longPressState[i].tracking = false;
            longPressState[i].pressSent = false;
        }
    }
}

void ModeManager::sendButtonEvent(bool currentState, bool previousState, uint8_t row, uint8_t col) {
    uint16_t buttonCode = gameConfig.buttonMap[row][col];

    // 按钮代码为 0 表示未映射，跳过
    if (buttonCode == 0) return;

    // various 模式下，被屏蔽的按键不发送事件
    if (currentMode == MODE_VARIOUS && isButtonMasked(row, col)) {
        // 如果屏蔽期间按键被按下，强制释放
        if (currentState == LOW) {
            XInput.release(buttonCode);
        }
        return;
    }

    // 正常状态变化检测（低电平有效）
    if (currentState == LOW && previousState == HIGH) {
        // 按键按下
        XInput.press(buttonCode);
    } else if (currentState == HIGH && previousState == LOW) {
        // 按键松开
        XInput.release(buttonCode);
    }
}

int8_t ModeManager::getLongPressIndex(uint8_t row, uint8_t col) const {
    for (uint8_t i = 0; i < gameConfig.longPress.count; i++) {
        if (gameConfig.longPress.buttons[i].row == row &&
            gameConfig.longPress.buttons[i].col == col) {
            return i;
        }
    }
    return -1;
}

void ModeManager::handleLongPressButton(bool currentState, uint8_t row, uint8_t col, uint8_t index) {
    uint16_t buttonCode = gameConfig.buttonMap[row][col];
    if (buttonCode == 0) return;

    LongPressState& state = longPressState[index];

    if (currentState == LOW) {
        // 按键处于按下状态
        if (!state.tracking) {
            // 刚按下，开始追踪
            state.tracking = true;
            state.pressStartTime = millis();
            state.pressSent = false;
        } else if (!state.pressSent) {
            // 持续按住，检查是否达到长按阈值
            if (millis() - state.pressStartTime >= gameConfig.longPress.holdDuration) {
                XInput.press(buttonCode);
                state.pressSent = true;
            }
        }
    } else {
        // 按键松开
        if (state.pressSent) {
            // 长按已触发过，立即发送释放
            XInput.release(buttonCode);
        }
        // 重置追踪状态
        state.tracking = false;
        state.pressSent = false;
    }
}

bool ModeManager::isButtonMasked(uint8_t row, uint8_t col) const {
    for (uint8_t i = 0; i < gameConfig.sideKeyMask.count; i++) {
        const auto& maskBtn = gameConfig.sideKeyMask.buttons[i];
        if (maskBtn.row == row && maskBtn.col == col) {
            return true;
        }
    }
    return false;
}

void ModeManager::releaseMaskedKeys() {
    // 根据屏蔽列表释放对应按键的 XInput 映射
    for (uint8_t i = 0; i < gameConfig.sideKeyMask.count; i++) {
        const auto& maskBtn = gameConfig.sideKeyMask.buttons[i];
        uint16_t buttonCode = gameConfig.buttonMap[maskBtn.row][maskBtn.col];
        if (buttonCode != 0) {
            XInput.release(buttonCode);
        }
    }
}

const CRGB* ModeManager::getLEDTheme() const {
    return (currentMode == MODE_ONGEKI) ?
        gameConfig.ledTheme.ongeki :
        gameConfig.ledTheme.various;
}
