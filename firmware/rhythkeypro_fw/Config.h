#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <XInput.h>
#include <FastLED.h>

// 硬件配置结构体
struct HardwareConfig {
    // 引脚定义
    struct Pins {
        uint8_t row[3] = {4, 5, 6};
        uint8_t col[6] = {10, 14, 15, 16, A2, A3};
        uint8_t encoder[4] = {2, 3, 8, 9}; // ENC1_A, ENC1_B, ENC2_A, ENC2_B
        uint8_t led = 7;
        uint8_t potentiometer = A0;
    } pins;

    // LED配置
    struct LED {
        uint8_t count = 12;
        uint8_t brightness = 80;
        EOrder colorOrder = GRB;
    } led;

    // 电位器配置
    struct Potentiometer {
        int deadzone = 3;
        int centerValue = 512;
        int maxValue = 1023;
    } pot;

    // 编码器配置
    struct Encoder {
        int16_t joystickMax = 32767;
        int16_t joystickMin = -32768;
        int16_t joystickResetHigh = -32768;
        int16_t joystickResetLow = 32767;
        int16_t step = 2048;
    } encoder;
};

// 游戏配置结构体
struct GameConfig {
    // 按钮映射 (行, 列) -> XInput按钮
    uint16_t buttonMap[3][6] = {
        // 第一行：未使用的按键, 未使用的按键, 左摇杆按下, 右摇杆按下, 未使用的按键, 模式切换
        {0, 0, BUTTON_L3, BUTTON_R3, 0, 0},
        // 第二行：右下, 右红, 右绿, 右蓝, 右菜单键, 右侧键
        {BUTTON_A, BUTTON_X, BUTTON_Y, BUTTON_B, BUTTON_START, BUTTON_RB},
        // 第三行：左侧键, 左菜单键, 左红, 左绿, 左蓝, 左下
        {BUTTON_LB, BUTTON_BACK, DPAD_LEFT, DPAD_UP, DPAD_RIGHT, DPAD_DOWN}
    };

    // LED主题色彩
    struct LEDTheme {
        CRGB ongeki[12] = {
            CRGB(128,0,128),    // 1 紫色，左侧键
            CRGB::Red,          // 2 红色，左菜单键
            CRGB::Red,          // 3 红色，左红
            CRGB::Green,        // 4 绿色，左绿
            CRGB::Blue,         // 5 蓝色，左蓝
            CRGB::White,        // 6 白色，左下
            CRGB::White,        // 7 白色，右下
            CRGB::Red,          // 8 红色，右红
            CRGB::Green,        // 9 绿色，右绿
            CRGB::Blue,         // 10 蓝色，右蓝
            CRGB::Yellow,       // 11 黄色，右菜单键
            CRGB(128,0,128)     // 12 紫色，右侧键
        };

        CRGB various[12] = {
            CRGB::Black,        // 1 熄灭，左侧键
            CRGB::Black,        // 2 熄灭，左菜单键
            CRGB::White,        // 3 白色，左红
            CRGB::White,        // 4 白色，左绿
            CRGB::White,        // 5 白色，左蓝
            CRGB(0,128,128),    // 6 青色，左下
            CRGB(0,128,128),    // 7 青色，右下
            CRGB::White,        // 8 白色，右红
            CRGB::White,        // 9 白色，右绿
            CRGB::White,        // 10 白色，右蓝
            CRGB::Black,        // 11 熄灭，右菜单键
            CRGB::Black         // 12 熄灭，右侧键
        };
    } ledTheme;

    // 侧键屏蔽按键列表
    struct SideKeyMask {
        struct MaskButton {
            uint8_t row;
            uint8_t col;
        };

        MaskButton buttons[4] = {
            {2, 0}, // 左侧键
            {2, 1}, // 左菜单键
            {1, 4}, // 右菜单键
            {1, 5}  // 右侧键
        };

        uint8_t count = 4;
    } sideKeyMask;
};

// 枚举定义
enum ControllerMode {
    MODE_ONGEKI = 0,
    MODE_VARIOUS,
    MODE_COUNT  // 哨兵值，必须始终在最后
};

enum ButtonIndex {
    BTN_LEFT = 0,
    BTN_LEFT_MENU,
    BTN_LEFT_RED,
    BTN_LEFT_GREEN,
    BTN_LEFT_BLUE,
    BTN_LEFT_BOTTOM,
    BTN_RIGHT_BOTTOM,
    BTN_RIGHT_RED,
    BTN_RIGHT_GREEN,
    BTN_RIGHT_BLUE,
    BTN_RIGHT_MENU,
    BTN_RIGHT
};

enum EncoderDirection {
    DIR_NONE = 0,
    DIR_CLOCKWISE = 1,
    DIR_COUNTER_CLOCKWISE = -1
};

// 全局配置实例
extern HardwareConfig hardwareConfig;
extern GameConfig gameConfig;

#endif // CONFIG_H