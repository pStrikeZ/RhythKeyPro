#include "BrightnessManager.h"

BrightnessManager::BrightnessManager()
    : currentBrightness(DEFAULT_BRIGHTNESS),
      savedBrightness(DEFAULT_BRIGHTNESS),
      lastChangeTime(0),
      dirty(false) {
}

void BrightnessManager::begin() {
    uint8_t magic = EEPROM.read(EEPROM_MAGIC_ADDR);

    if (magic == EEPROM_MAGIC_VALUE) {
        // 魔数匹配：EEPROM 中有合法的亮度数据，直接读取
        currentBrightness = EEPROM.read(EEPROM_BRIGHTNESS_ADDR);
    } else {
        // 魔数不匹配（新芯片或首次使用）：写入默认值
        currentBrightness = DEFAULT_BRIGHTNESS;
        EEPROM.update(EEPROM_MAGIC_ADDR, EEPROM_MAGIC_VALUE);
        EEPROM.update(EEPROM_BRIGHTNESS_ADDR, currentBrightness);
    }

    savedBrightness = currentBrightness;

    // 应用加载的亮度到 FastLED
    FastLED.setBrightness(currentBrightness);
}

void BrightnessManager::increase() {
    // 边界检查：防止溢出
    if (currentBrightness <= 255 - BRIGHTNESS_STEP) {
        currentBrightness += BRIGHTNESS_STEP;
    } else {
        currentBrightness = 255;
    }

    FastLED.setBrightness(currentBrightness);
    FastLED.show();

    lastChangeTime = millis();
    dirty = true;
}

void BrightnessManager::decrease() {
    // 边界检查：最低不低于 BRIGHTNESS_STEP，防止 LED 完全熄灭
    if (currentBrightness > BRIGHTNESS_STEP * 2) {
        currentBrightness -= BRIGHTNESS_STEP;
    } else {
        currentBrightness = BRIGHTNESS_STEP;
    }

    FastLED.setBrightness(currentBrightness);
    FastLED.show();

    lastChangeTime = millis();
    dirty = true;
}

void BrightnessManager::tick() {
    // 非阻塞检查：有脏数据且距上次调节已超过延迟时间
    if (dirty && (millis() - lastChangeTime >= SAVE_DELAY_MS)) {
        EEPROM.update(EEPROM_BRIGHTNESS_ADDR, currentBrightness);
        savedBrightness = currentBrightness;
        dirty = false;
    }
}
