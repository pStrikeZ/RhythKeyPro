#include "LEDController.h"
#include <string.h>  // memcmp, memcpy

LEDController::LEDController() : leds(nullptr), prevLeds(nullptr), initialized(false) {
}

LEDController::~LEDController() {
    if (leds) {
        delete[] leds;
    }
    if (prevLeds) {
        delete[] prevLeds;
    }
}

void LEDController::begin() {
    if (initialized) return;

    leds = new CRGB[hardwareConfig.led.count];
    prevLeds = new CRGB[hardwareConfig.led.count];
    if (!leds || !prevLeds) return;

    // 初始化 prevLeds 为全黑之外的值，确保首次 show 一定触发
    memset(prevLeds, 0xFF, sizeof(CRGB) * hardwareConfig.led.count);

    FastLED.addLeds<WS2812, 7, GRB>(leds, hardwareConfig.led.count)
        .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(hardwareConfig.led.brightness);

    initialized = true;
}

void LEDController::update(const CRGB* theme) {
    if (!initialized || !leds) return;

    applyTheme(theme);
    showIfDirty();
}

void LEDController::updateWithOverlay(const CRGB* theme, const volatile CRGB* overlay,
                                       const uint8_t* indices, uint8_t count) {
    if (!initialized || !leds) return;

    // 先铺底主题色
    applyTheme(theme);

    // 覆盖动态位置（volatile CRGB 不能直接赋值，需逐字段拷贝）
    for (uint8_t i = 0; i < count; i++) {
        uint8_t idx = indices[i];
        if (idx < hardwareConfig.led.count) {
            leds[idx].r = overlay[i].r;
            leds[idx].g = overlay[i].g;
            leds[idx].b = overlay[i].b;
        }
    }

    showIfDirty();
}

void LEDController::setBrightness(uint8_t brightness) {
    if (!initialized) return;

    hardwareConfig.led.brightness = brightness;
    FastLED.setBrightness(brightness);
    // 亮度变化不体现在 leds 数组中，需要强制刷新
    FastLED.show();
    if (prevLeds) {
        memcpy(prevLeds, leds, sizeof(CRGB) * hardwareConfig.led.count);
    }
}

void LEDController::applyTheme(const CRGB* theme) {
    if (!leds || !theme) return;

    for (uint8_t i = 0; i < hardwareConfig.led.count; i++) {
        leds[i] = theme[i];
    }
}

void LEDController::showIfDirty() {
    if (!initialized || !prevLeds) return;

    // 比较当前 leds 和上次快照，相同则跳过
    if (memcmp(leds, prevLeds, sizeof(CRGB) * hardwareConfig.led.count) == 0) {
        return;
    }

    FastLED.show();
    memcpy(prevLeds, leds, sizeof(CRGB) * hardwareConfig.led.count);
}