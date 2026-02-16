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