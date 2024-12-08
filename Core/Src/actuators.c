/**
 * @file        : actuators.c
 * @authors     : Andre Lamego (), 
 *              : Jarley B. Reckel Jr (jarley@ufmg.br), 
 *              : Mariana Givisiez ()
 * @brief       : This file contains the implementation of the functions that control the actuators of the boat.
 * @version     : 0.1
 * @date        : 2024-12-08
 * 
 * @copyright   : Copyright (c) 2024
 * 
 */

#include "actuators.h"

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

void setServoAngle(boat_system_t *boat_system, TIM_HandleTypeDef timer, uint16_t angle) {
    if(angle < 0) {
        angle = 0;
    } else if(angle > 180) {
        angle = 180;
    }
    uint16_t pulse = angle2pulse(angle);

    setPWM( *boat_system_get_servo_timer(boat_system),
            boat_system_get_servo_channel(boat_system),
            SERVO_PERIOD, 
            pulse);

    boat_system_set_servo_angle(boat_system, angle);
}

void setServoRelativeAngle(boat_system_t *boat_system, uint16_t angle, bool clockwise) {  
    uint16_t current_angle = boat_system_get_servo_angle(boat_system);  
    if(clockwise) {
        angle = current_angle + angle;
    } else {
        angle = current_angle - angle;
    }
    uint16_t pulse = angle2pulse(angle);

    if(pulse < SERVO_MIN_PULSE) {
        pulse = SERVO_MIN_PULSE;
    } else if(pulse > SERVO_MAX_PULSE) {
        pulse = SERVO_MAX_PULSE;
    }

    setPWM( *boat_system_get_servo_timer(boat_system),
            boat_system_get_servo_channel(boat_system),
            SERVO_PERIOD, 
            pulse);

    boat_system_set_servo_angle(boat_system, angle);
}

void setMotorSpeed(boat_system_t *boat_system, uint16_t speed) {
    speed = verifySpeed(speed);
    sendCommand(boat_system, FORWARD, speed);
    boat_system_set_motor_speed(boat_system, speed);
    boat_system_set_motor_direction(boat_system, FORWARD);
}

void setMotorBackward(boat_system_t *boat_system, uint16_t speed) {
    speed = verifySpeed(speed);
    sendCommand(boat_system, BACKWARD, speed);
    boat_system_set_motor_speed(boat_system, speed);
    boat_system_set_motor_direction(boat_system, BACKWARD);
}

void sendCommand(boat_system_t *boat_system, unsigned char value, int speed) {
    int verify;
    setPWM( *boat_system_get_motor_timer(boat_system), 
            boat_system_get_motor_channel(boat_system), 
            MOTOR_PERIOD, 
            speed);
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