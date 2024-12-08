/**
 * @file        : actuators.h
 * @authors     : Andre Lamego (), 
 *              : Jarley B. Reckel Jr (jarley@ufmg.br), 
 *              : Mariana Givisiez ()
 * @brief       : This file contains the declarations of the functions that control the actuators of the boat.
 * @version     : 0.1
 * @date        : 2024-12-08
 * 
 * @copyright   : Copyright (c) 2024
 * 
 */

#ifndef __ACTUATORS__H__
#define __ACTUATORS__H__

#include <stdbool.h>
#include "main.h"
#include "stm32f4xx_hal.h"
#include "boat_system.h"

#define SERVO_MIN_PULSE 30          ///< Minimum pulse of the servo
#define SERVO_MAX_PULSE 158         ///< Maximum pulse of the servo
#define SERVO_INITIAL_PULSE 1500    ///< Initial pulse of the servo
#define SERVO_PERIOD 20000          ///< Period of the servo
#define MOTOR_PERIOD 1250           ///< Period of the motor
#define MOTOR_MAX_SPEED 1250        ///< Maximum speed of the motor
#define MOTOR_MIN_SPEED 0           ///< Minimum speed of the motor

/**
 * @brief Convert an angle to the corresponding pulse
 * 
 * @param angle 
 * @return uint16_t 
 */
uint16_t angle2pulse(uint16_t angle);

/**
 * @brief Set the PWM of a timer
 * 
 * @param timer 
 * @param channel 
 * @param period 
 * @param pulse 
 */
void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse);

/**
 * @brief Send a command to the L293D motor driver
 * 
 * @param boat_system 
 * @param value 
 * @param speed 
 */
void sendCommand(boat_system_t *boat_system, unsigned char value, int speed);

/**
 * @brief Verify if the speed is within the limits
 * 
 * @param speed 
 * @return uint16_t 
 */
uint16_t verifySpeed(uint16_t speed);


/**
 * @brief Set the angle of the servo
 * 
 * @param boat_system 
 * @param timer 
 * @param angle 
 */
void setServoAngle(boat_system_t *boat_system, TIM_HandleTypeDef timer, uint16_t angle);

/**
 * @brief Set the relative angle of the servo
 * 
 * @param boat_system 
 * @param angle 
 * @param clockwise 
 */
void setServoRelativeAngle(boat_system_t *boat_system, uint16_t angle, bool clockwise);

/**
 * @brief Set the speed of the motor
 * 
 * @param boat_system 
 * @param speed 
 */
void setMotorSpeed(boat_system_t *boat_system, uint16_t speed);

/**
 * @brief Set the speed of the motor in the backward direction
 * 
 * @param boat_system 
 * @param speed 
 */
void setMotorBackward(boat_system_t *boat_system, uint16_t speed);


#endif // __ACTUATORS__H__