#ifndef __ACTUATORS_H
#define __ACTUATORS_H

#include <stdbool.h>

#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

#define SERVO_MIN_PULSE 700
#define SERVO_MAX_PULSE 2300
#define SERVO_INITIAL_PULSE 1500
#define SERVO_PERIOD 20000

#define MOTOR_TIMER TIM_CHANNEL_3
#define SERVO_TIMER TIM_CHANNEL_1

uint16_t angle2pulse(uint16_t angle);

void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse);

bool initServo();

void setServoAngle(uint16_t angle);

void setServoRelativeAngle(uint16_t angle, bool clockwise);

bool initMotor();

void setMotorSpeed(uint16_t speed);

void setMotorAcceleration(uint16_t acceleration);

void setMotorDeceleration(uint16_t deceleration);

#endif // __ACTUATORS_H