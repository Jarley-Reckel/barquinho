#ifndef __ACTUATORS__H__
#define __ACTUATORS__H__

#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "boat_system.h"

#define SERVO_MIN_PULSE 30
#define SERVO_MAX_PULSE 158
#define SERVO_INITIAL_PULSE 1500
#define SERVO_PERIOD 20000
#define MOTOR_PERIOD 1250
#define MOTOR_MAX_SPEED 1250
#define MOTOR_MIN_SPEED 0

uint16_t angle2pulse(uint16_t angle);
void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse);
void sendCommand(boat_system_t *boat_system, unsigned char value, int speed);
uint16_t verifySpeed(uint16_t speed);

void setServoAngle(boat_system_t *boat_system, TIM_HandleTypeDef timer, uint16_t angle);
void setServoRelativeAngle(boat_system_t *boat_system, uint16_t angle, bool clockwise);

void setMotorSpeed(boat_system_t *boat_system, uint16_t speed);
void setMotorBackward(boat_system_t *boat_system, uint16_t speed);


#endif // __ACTUATORS__H__