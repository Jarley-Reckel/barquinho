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

#define MOTOR_TIMER TIM_CHANNEL_
#define SERVO_TIMER TIM_CHANNEL_1

TIM_HandleTypeDef htim_motor;
TIM_HandleTypeDef htim_servo;



static void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse);

static bool initServo(TIM_HandleTypeDef timer, uint32_t channel);

static bool initMotor(TIM_HandleTypeDef timer, uint32_t channel);

static void setServoAngle(uint16_t angle);

static void setServoRelativeAngle(uint16_t angle, bool clockwise);

static void setMotorSpeed(uint16_t speed);

static void setMotorAcceleration(uint16_t acceleration);

static void setMotorDeceleration(uint16_t deceleration);

#endif // __ACTUATORS_H