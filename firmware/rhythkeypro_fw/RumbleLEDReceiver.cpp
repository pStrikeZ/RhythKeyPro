#include "RumbleLEDReceiver.h"

// 全局实例
RumbleLEDReceiver rumbleLEDReceiver;

RumbleLEDReceiver::RumbleLEDReceiver() : dataValid(false) {
    for (uint8_t i = 0; i < DYNAMIC_LED_COUNT; i++) {
        colors[i] = CRGB::Black;
    }
}

void RumbleLEDReceiver::begin() {
    XInput.setReceiveCallback(onReceive);
}

void RumbleLEDReceiver::clearData() {
    dataValid = false;
}

void RumbleLEDReceiver::decodeBase5(uint8_t value, volatile CRGB* out) {
    out[2] = RUMBLE_COLOR_TABLE[value % 5]; value /= 5;
    out[1] = RUMBLE_COLOR_TABLE[value % 5]; value /= 5;
    out[0] = RUMBLE_COLOR_TABLE[value];
}

void RumbleLEDReceiver::onReceive(uint8_t packetType) {
    // 只处理 rumble 包
    if (packetType != (uint8_t)XInputReceiveType::Rumble) return;

    uint8_t left  = XInput.getRumbleLeft();
    uint8_t right = XInput.getRumbleRight();

    // 双 0 = 无信号，清除数据让主循环回落到静态主题色
    if (left == 0 && right == 0) {
        rumbleLEDReceiver.dataValid = false;
        return;
    }

    // 分别处理左右（值 0 = 无信号该侧不更新，1–125 = 有效数据）
    if (left >= 1 && left <= 125) {
        decodeBase5(left - 1, rumbleLEDReceiver.colors);
    }
    if (right >= 1 && right <= 125) {
        decodeBase5(right - 1, rumbleLEDReceiver.colors + 3);
    }

    rumbleLEDReceiver.dataValid = true;
}
