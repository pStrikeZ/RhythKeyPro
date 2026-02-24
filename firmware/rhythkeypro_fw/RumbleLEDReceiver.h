#ifndef RUMBLE_LED_RECEIVER_H
#define RUMBLE_LED_RECEIVER_H

#include <Arduino.h>
#include <XInput.h>
#include <FastLED.h>
#include "Config.h"

// 音击灯效 Rumble 接收器
// 通过 XInput 震动通道接收 PC 端编码的 LED 颜色数据。
// 协议：0 = 忽略（系统重置无效），1–125 = base-5 编码颜色，126 = 清除覆盖层，>126 = 忽略
// 超时机制：500ms 无有效数据自动恢复默认主题色（应对 wrapper 被强制终止）

static const unsigned long RUMBLE_TIMEOUT_MS = 500;  // 心跳超时
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

    // 主循环中调用：检查心跳超时，超时则自动清除覆盖层
    void checkTimeout();

private:
    volatile CRGB colors[DYNAMIC_LED_COUNT];
    volatile bool dataValid;
    volatile unsigned long lastReceiveTime;  // 上次收到有效数据的时间戳

    // XInput 回调（静态，ISR 上下文）
    static void onReceive(uint8_t packetType);

    // 解码单侧 3 灯的 base-5 值
    static void decodeBase5(uint8_t value, volatile CRGB* out);
};

// 全局实例（回调需要访问）
extern RumbleLEDReceiver rumbleLEDReceiver;

#endif // RUMBLE_LED_RECEIVER_H
