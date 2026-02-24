#ifndef RUMBLE_LED_RECEIVER_H
#define RUMBLE_LED_RECEIVER_H

#include <Arduino.h>
#include <XInput.h>
#include <FastLED.h>
#include "Config.h"

// 音击灯效 Rumble 接收器
// 通过 XInput 震动通道接收 PC 端编码的 LED 颜色数据。
// 协议：0 = 无信号（维持当前色），1–125 = base-5 编码颜色（减 1 后解码），>125 = 忽略
class RumbleLEDReceiver {
public:
    RumbleLEDReceiver();
    ~RumbleLEDReceiver() = default;

    // 初始化并注册 XInput 回调
    void begin();

    // 是否有有效的 LED 数据
    bool hasData() const { return dataValid; }

    // 获取解码后的 6 色数组（左红/左绿/左蓝/右红/右绿/右蓝）
    const CRGB* getColors() const { return colors; }

    // 清除数据（模式切换时调用）
    void clearData();

private:
    volatile CRGB colors[DYNAMIC_LED_COUNT];
    volatile bool dataValid;

    // XInput 回调（静态，ISR 上下文）
    static void onReceive(uint8_t packetType);

    // 解码单侧 3 灯的 base-5 值
    static void decodeBase5(uint8_t value, volatile CRGB* out);
};

// 全局实例（回调需要访问）
extern RumbleLEDReceiver rumbleLEDReceiver;

#endif // RUMBLE_LED_RECEIVER_H
