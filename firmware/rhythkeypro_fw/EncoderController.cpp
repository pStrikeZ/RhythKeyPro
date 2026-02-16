#include "EncoderController.h"
#include <XInput.h>

EncoderController::EncoderController() :
    leftJoystickX(0), leftJoystickY(0) {
}

void EncoderController::begin() {
    // 初始化编码器引脚
    pinMode(hardwareConfig.pins.encoder[0], INPUT); // ENC1_A
    pinMode(hardwareConfig.pins.encoder[1], INPUT); // ENC1_B
    pinMode(hardwareConfig.pins.encoder[2], INPUT); // ENC2_A
    pinMode(hardwareConfig.pins.encoder[3], INPUT); // ENC2_B

    // 初始化编码器状态
    encoder1.pinA_prev = digitalRead(hardwareConfig.pins.encoder[0]);
    encoder1.pinB_prev = digitalRead(hardwareConfig.pins.encoder[1]);
    encoder1.state = (encoder1.pinA_prev << 1) | encoder1.pinB_prev;
    encoder1.position = leftJoystickX;
    encoder1.name = "左摇杆X轴";

    encoder2.pinA_prev = digitalRead(hardwareConfig.pins.encoder[2]);
    encoder2.pinB_prev = digitalRead(hardwareConfig.pins.encoder[3]);
    encoder2.state = (encoder2.pinA_prev << 1) | encoder2.pinB_prev;
    encoder2.position = leftJoystickY;
    encoder2.name = "左摇杆Y轴";
}

void EncoderController::update() {
    // 更新两个编码器
    updateSingleEncoder(encoder1, hardwareConfig.pins.encoder[0], hardwareConfig.pins.encoder[1]);
    updateSingleEncoder(encoder2, hardwareConfig.pins.encoder[2], hardwareConfig.pins.encoder[3]);

    // 同步位置变量
    leftJoystickX = encoder1.position;
    leftJoystickY = encoder2.position;

    // 发送到XInput
    XInput.setJoystickX(JOY_LEFT, leftJoystickX);
    XInput.setJoystickY(JOY_LEFT, leftJoystickY);
}

void EncoderController::resetPositions() {
    encoder1.position = 0;
    encoder2.position = 0;
    leftJoystickX = 0;
    leftJoystickY = 0;
}

void EncoderController::updateSingleEncoder(EncoderState& encoder, uint8_t pinA, uint8_t pinB) {
    // 读取当前状态
    bool a_current = digitalRead(pinA);
    bool b_current = digitalRead(pinB);

    // 如果没有变化，直接返回
    if (a_current == encoder.pinA_prev && b_current == encoder.pinB_prev) {
        return;
    }

    // 计算当前状态
    uint8_t current_state = (a_current << 1) | b_current;

    // 检测旋转方向
    EncoderDirection direction = getEncoderDirection(encoder.state, current_state);

    if (direction != DIR_NONE) {
        // 反转方向逻辑：direction > 0 (原顺时针) 现在对应逆时针，反之亦然
        bool clockwise = (direction < 0);
        updateEncoderPosition(encoder.position, clockwise ? DIR_CLOCKWISE : DIR_COUNTER_CLOCKWISE, encoder.name);
    }

    // 更新前一状态
    encoder.pinA_prev = a_current;
    encoder.pinB_prev = b_current;
    encoder.state = current_state;
}

EncoderDirection EncoderController::getEncoderDirection(uint8_t oldState, uint8_t newState) {
    // 正交编码器状态转换表
    static const EncoderDirection transition_table[4][4] = {
        // 0->0, 0->1, 0->2, 0->3
        {DIR_NONE, DIR_CLOCKWISE, DIR_COUNTER_CLOCKWISE, DIR_NONE},
        // 1->0, 1->1, 1->2, 1->3
        {DIR_COUNTER_CLOCKWISE, DIR_NONE, DIR_NONE, DIR_CLOCKWISE},
        // 2->0, 2->1, 2->2, 2->3
        {DIR_CLOCKWISE, DIR_NONE, DIR_NONE, DIR_COUNTER_CLOCKWISE},
        // 3->0, 3->1, 3->2, 3->3
        {DIR_NONE, DIR_COUNTER_CLOCKWISE, DIR_CLOCKWISE, DIR_NONE}
    };

    return transition_table[oldState][newState];
}

void EncoderController::updateEncoderPosition(int16_t& position, EncoderDirection direction, const char* name) {
    if (direction == DIR_CLOCKWISE) {
        // 顺时针旋转：增加位置
        if (position >= hardwareConfig.encoder.joystickMax) {
            position = hardwareConfig.encoder.joystickResetHigh;
        } else {
            position += hardwareConfig.encoder.step;
            if (position > hardwareConfig.encoder.joystickMax) {
                position = hardwareConfig.encoder.joystickMax;
            }
        }
    } else if (direction == DIR_COUNTER_CLOCKWISE) {
        // 逆时针旋转：减少位置
        if (position <= hardwareConfig.encoder.joystickMin) {
            position = hardwareConfig.encoder.joystickResetLow;
        } else {
            position -= hardwareConfig.encoder.step;
            if (position < hardwareConfig.encoder.joystickMin) {
                position = hardwareConfig.encoder.joystickMin;
            }
        }
    }
}