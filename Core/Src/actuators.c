#include "actuators.h"

#define SERVO_MIN_PULSE 30
#define SERVO_MAX_PULSE 158
#define SERVO_INITIAL_PULSE 1500
#define SERVO_PERIOD 20000
#define MOTOR_PERIOD 1250
#define MOTOR_MAX_SPEED 1250
#define MOTOR_MIN_SPEED 0
#define MOTOR_TIMER TIM_CHANNEL_2
#define SERVO_TIMER TIM_CHANNEL_1

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
uint16_t SERVO_ANGLE = 0;

uint16_t angle2pulse(uint16_t angle) {
    return SERVO_MIN_PULSE + (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180.0);
}

void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse) {
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

uint16_t verifySpeed(uint16_t speed) {
    if (speed >= MOTOR_MAX_SPEED) {
        return MOTOR_MAX_SPEED;
    }
    if (speed <= MOTOR_MIN_SPEED) {
        return MOTOR_MIN_SPEED;
    }
    return speed;
}

void setServoAngle(TIM_HandleTypeDef timer, uint16_t angle) {
    if(angle < 0) {
        angle = 0;
    } else if(angle > 180) {
        angle = 180;
    }
    uint16_t pulse = angle2pulse(angle);
    setPWM(htim4, TIM_CHANNEL_1, SERVO_PERIOD, pulse);
    SERVO_ANGLE = angle;
}

void setServoRelativeAngle(uint16_t angle, bool clockwise) {    
    if(clockwise) {
        angle = SERVO_ANGLE + angle;
    } else {
        angle = SERVO_ANGLE - angle;
    }
    uint16_t pulse = angle2pulse(angle);

    if(pulse < SERVO_MIN_PULSE) {
        pulse = SERVO_MIN_PULSE;
    } else if(pulse > SERVO_MAX_PULSE) {
        pulse = SERVO_MAX_PULSE;
    }
    setPWM(htim4, SERVO_TIMER, SERVO_PERIOD, pulse);
    SERVO_ANGLE = angle;
}

void setMotorSpeed(uint16_t speed) {
    speed = verifySpeed(speed);
    sendCommand(DIR_DIRECT, speed);
}

void setMotorBackward(uint16_t speed) {
    speed = verifySpeed(speed);
    sendCommand(DIR_REVERSE, speed);
}

void sendCommand(unsigned char value, int speed) {
    int verify;
    setPWM(htim3, TIM_CHANNEL_2, 1250, speed);
    HAL_GPIO_WritePin(GPIOA, L293D_LATCH_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(L293D_CLK_GPIO_Port, L293D_CLK_Pin,   GPIO_PIN_RESET);

    for(unsigned char i=0x0; i<0x08; i++) {
        HAL_GPIO_WritePin(L293D_CLK_GPIO_Port, L293D_CLK_Pin, GPIO_PIN_RESET);

        if(value & (1<<i)) {
            verify = GPIO_PIN_SET;
        } else {
            verify = GPIO_PIN_RESET;
        }

        HAL_GPIO_WritePin(GPIOA, L293D_SER_Pin, verify);
        HAL_GPIO_WritePin(L293D_CLK_GPIO_Port, L293D_CLK_Pin, GPIO_PIN_SET);

    }
    HAL_GPIO_WritePin(L293D_CLK_GPIO_Port, L293D_CLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, L293D_LATCH_Pin, GPIO_PIN_SET);
}