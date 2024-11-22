#include "actuators.h"

static void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse) {
    HAL_TIM_PWM_Stop(&timer, channel);
    TIM_OC_InitTypeDef sConfigOC;
    timer.Init.Period = period;
    HAL_TIM_PWM_Init(&timer);
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = pulse;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&timer, &sConfigOC, channel);
    HAL_TIM_PWM_Start(&timer, channel);
}

static bool initServo(TIM_HandleTypeDef timer, uint32_t channel) {
    extern htim_servo;
    HAL_TIM_PWM_Stop(&timer, channel);
    TIM_OC_InitTypeDef sConfigOC;
    timer.Init.Period = SERVO_INITIAL_PULSE;
    HAL_TIM_PWM_Init(&timer);
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = SERVO_INITIAL_PULSE;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&timer, &sConfigOC, channel);
    HAL_TIM_PWM_Start(&timer, channel);
}

static bool initMotor(TIM_HandleTypeDef timer, uint32_t channel) {

}

static void setServoAngle(uint16_t angle) {
    if(angle < 0) {
        angle = 0;
    } else if(angle > 180) {
        angle = 180;
    }

    uint16_t pulse = SERVO_MIN_PULSE + (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180);
    
}

static void setServoRelativeAngle(uint16_t angle, bool clockwise) {

}

static void setMotorSpeed(uint16_t speed) {

}

static void setMotorAcceleration(uint16_t acceleration) {

}

static void setMotorDeceleration(uint16_t deceleration) {

}