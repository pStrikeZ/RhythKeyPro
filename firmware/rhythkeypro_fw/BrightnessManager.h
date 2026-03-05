#ifndef BRIGHTNESS_MANAGER_H
#define BRIGHTNESS_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include <FastLED.h>

class BrightnessManager {
public:
    BrightnessManager();
    ~BrightnessManager() = default;

    // 从 EEPROM 加载亮度并应用到 FastLED
    void begin();

    // 亮度增加/减少（仅更新内存，不写 EEPROM）
    void increase();
    void decrease();

    // 每帧调用：非阻塞检查是否需要延迟保存到 EEPROM
    void tick();

    // 获取当前亮度
    uint8_t getBrightness() const { return currentBrightness; }

private:
    static const uint8_t EEPROM_MAGIC_ADDR = 0;    // 魔数标记地址（用于区分未初始化）
    static const uint8_t EEPROM_BRIGHTNESS_ADDR = 1; // 亮度值存储地址
    static const uint8_t EEPROM_MAGIC_VALUE = 0xA5;  // 魔数：标记 EEPROM 已被初始化
    static const uint8_t BRIGHTNESS_STEP = 16;       // 每次调节步进量
    static const uint8_t DEFAULT_BRIGHTNESS = 80;    // 默认亮度（首次使用）
    static const unsigned long SAVE_DELAY_MS = 3000;  // 防抖延迟保存时间（毫秒）

    uint8_t currentBrightness;   // 当前内存中的亮度值
    uint8_t savedBrightness;     // 上次写入 EEPROM 的亮度值
    unsigned long lastChangeTime; // 最后一次调节的时间戳
    bool dirty;                   // 是否有未保存的变更
};

#endif // BRIGHTNESS_MANAGER_H
