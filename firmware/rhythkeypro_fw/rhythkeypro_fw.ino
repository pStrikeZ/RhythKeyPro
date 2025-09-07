// RhythKey v4 XInput固件 - 重构版本
// 支持游戏列表：
// 音击/SOUND VOLTEX/DJMAX RESPECT V/osu! mania

#include <XInput.h>
#include <FastLED.h>

// 包含自定义头文件
#include "Config.h"
#include "LEDController.h"
#include "EncoderController.h"
#include "ButtonMatrix.h"
#include "RhythKeyController.h"

// 全局控制器实例
RhythKeyController rhythKey;

void setup() {
    // 初始化主控制器
    rhythKey.begin();
}

void loop() {
    // 主循环更新
    rhythKey.update();
}