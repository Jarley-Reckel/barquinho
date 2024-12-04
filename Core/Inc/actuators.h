#ifndef __ACTUATORS__H__
#define __ACTUATORS__H__

#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"

#define DIR_DIRECT 0b10101100
#define DIR_REVERSE 0b01010011

uint16_t angle2pulse(uint16_t angle);
void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse);
void sendCommand(unsigned char value, int speed);
uint16_t verifySpeed(uint16_t speed);

void setServoAngle(TIM_HandleTypeDef timer, uint16_t angle);
void setServoRelativeAngle(uint16_t angle, bool clockwise);

void setMotorSpeed(uint16_t speed);
void setMotorBackward(uint16_t speed);


#endif // __ACTUATORS__H__