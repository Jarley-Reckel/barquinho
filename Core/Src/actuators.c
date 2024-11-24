#include "actuators.h"

extern TIM_HandleTypeDef htim_servo;
extern TIM_HandleTypeDef htim_motor; 

uint16_t angle2pulse(uint16_t angle) {
    return SERVO_MIN_PULSE + (angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180);
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

bool initServo() {
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    htim_servo.Instance = SERVO_TIMER;
    htim_servo.Init.Prescaler = 84 - 1;
    htim_servo.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim_servo.Init.Period = SERVO_PERIOD - 1;
    htim_servo.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim_servo.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim_servo) != HAL_OK) {
        return false;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim_servo, &sClockSourceConfig) != HAL_OK) {
        return false;
    }
    if (HAL_TIM_PWM_Start(&htim_servo, SERVO_TIMER) != HAL_OK) {
        return false;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim_servo, &sMasterConfig) != HAL_OK) {
        return false;
    }
    setPWM(htim_servo, SERVO_TIMER, SERVO_PERIOD, SERVO_INITIAL_PULSE);
    if (HAL_TIM_PWM_ConfigChannel(&htim_servo, &sConfigOC, SERVO_TIMER) != HAL_OK) {
        return false;
    }
    return true;
}

void setServoAngle(uint16_t angle) {
    if(angle < 0) {
        angle = 0;
    } else if(angle > 180) {
        angle = 180;
    }

    uint16_t pulse = angle2pulse(angle);
    setPWM(htim_servo, SERVO_TIMER, SERVO_PERIOD, pulse);
}

void setServoRelativeAngle(uint16_t angle, bool clockwise) {
    uint16_t pulse = htim_servo.Instance->CCR1;
    uint16_t delta_pulse = angle2pulse(angle);
    
    if(clockwise) {
        pulse += delta_pulse;
    } else {
        pulse -= delta_pulse;
    }

    if(pulse < SERVO_MIN_PULSE) {
        pulse = SERVO_MIN_PULSE;
    } else if(pulse > SERVO_MAX_PULSE) {
        pulse = SERVO_MAX_PULSE;
    }

    setPWM(htim_servo, SERVO_TIMER, SERVO_PERIOD, pulse);

}

bool initMotor() {
    // htim_motor.Instance = timer;
    // htim_motor.Init.Prescaler = 84 - 1; // Não sei se vai ser assim ainda
    // htim_motor.Init.CounterMode = TIM_COUNTERMODE_UP;
    // htim_motor.Init.Period = 1000 - 1; // Não sei se vai ser assim ainda
    // htim_motor.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    // htim_motor.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    // if (HAL_TIM_PWM_Init(&htim_motor) != HAL_OK) {
    //     return false;
    // }
    // setPWM(htim_motor, channel, 1000, 0); // Não sei se vai ser assim ainda
    return true;
}

void setMotorSpeed(uint16_t speed) {

}

void setMotorAcceleration(uint16_t acceleration) {

}

void setMotorDeceleration(uint16_t deceleration) {

}
