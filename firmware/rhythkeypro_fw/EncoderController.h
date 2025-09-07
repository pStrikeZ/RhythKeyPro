#ifndef ENCODER_CONTROLLER_H
#define ENCODER_CONTROLLER_H

#include <Arduino.h>
#include "Config.h"

class EncoderController {
public:
    EncoderController();
    ~EncoderController() = default;

    // 初始化编码器
    void begin();

    // 更新编码器状态
    void update();

    // 获取摇杆位置
    int16_t getLeftJoystickX() const { return leftJoystickX; }
    int16_t getRightJoystickX() const { return rightJoystickX; }

    // 重置编码器位置
    void resetPositions();

private:
    // 编码器状态
    struct EncoderState {
        uint8_t state;
        bool pinA_prev;
        bool pinB_prev;
        int16_t position;
        const char* name;
    };

    EncoderState encoder1; // 左摇杆X轴
    EncoderState encoder2; // 右摇杆X轴

    int16_t leftJoystickX;
    int16_t rightJoystickX;

    // 正交编码器状态机方向检测
    EncoderDirection getEncoderDirection(uint8_t oldState, uint8_t newState);

    // 更新单个编码器
    void updateSingleEncoder(EncoderState& encoder, uint8_t pinA, uint8_t pinB);

    // 更新编码器位置
    void updateEncoderPosition(int16_t& position, EncoderDirection direction, const char* name);
};

#endif // ENCODER_CONTROLLER_H