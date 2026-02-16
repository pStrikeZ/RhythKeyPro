#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <FastLED.h>
#include "Config.h"

class LEDController {
public:
    LEDController();
    ~LEDController();

    // 初始化LED
    void begin();

    // 更新LED显示（接收当前模式对应的主题色数组）
    void update(const CRGB* theme);

    // 设置LED亮度
    void setBrightness(uint8_t brightness);

    // 获取LED数量
    uint8_t getLEDCount() const { return hardwareConfig.led.count; }

private:
    CRGB* leds;
    bool initialized;

    // 应用LED主题
    void applyTheme(const CRGB* theme);

    // 显示更新
    void show();
};

#endif // LED_CONTROLLER_H