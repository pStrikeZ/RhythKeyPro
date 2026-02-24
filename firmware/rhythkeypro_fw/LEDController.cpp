#include "LEDController.h"

LEDController::LEDController() : leds(nullptr), initialized(false) {
}

LEDController::~LEDController() {
    if (leds) {
        delete[] leds;
    }
}

void LEDController::begin() {
    if (initialized) return;

    leds = new CRGB[hardwareConfig.led.count];
    if (!leds) return;

    FastLED.addLeds<WS2812, 7, GRB>(leds, hardwareConfig.led.count)
        .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(hardwareConfig.led.brightness);

    initialized = true;
}

void LEDController::update(const CRGB* theme) {
    if (!initialized || !leds) return;

    applyTheme(theme);
    show();
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

    show();
}

void LEDController::setBrightness(uint8_t brightness) {
    if (!initialized) return;

    hardwareConfig.led.brightness = brightness;
    FastLED.setBrightness(brightness);
    show();
}

void LEDController::applyTheme(const CRGB* theme) {
    if (!leds || !theme) return;

    for (uint8_t i = 0; i < hardwareConfig.led.count; i++) {
        leds[i] = theme[i];
    }
}

void LEDController::show() {
    if (initialized) {
        FastLED.show();
    }
}