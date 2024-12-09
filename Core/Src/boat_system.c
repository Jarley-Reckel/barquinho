/**
 * @file        : boat_system.c
 * @authors     : Andre Lamego (andrelrc23@ufmg.br), 
 *              : Jarley B. Reckel Jr (jarley@ufmg.br), 
 *              : Mariana Givisiez (marianagivisiez@ufmg.br)
 * @brief       : This file contains the implementation of the boat system.
 * @version     : 0.1
 * @date        : 2024-12-08
 * 
 * @copyright   : Copyright (c) 2024
 * 
 */

#include "boat_system.h"

void boat_system_set_servo_angle(boat_system_t *boat_system, int16_t angle) {
    boat_system->servo_angle = angle;
}

void boat_system_set_motor_speed(boat_system_t *boat_system, uint16_t speed) {
    boat_system->motor_speed = speed;
}

void boat_system_set_motor_direction(boat_system_t *boat_system, direction direction) {
    boat_system->motor_direction = direction;
}

void boat_system_set_x_position(boat_system_t *boat_system, float x_position) {
    boat_system->x_position = x_position;
}

void boat_system_set_y_position(boat_system_t *boat_system, float y_position) {
    boat_system->y_position = y_position;
}

void boat_system_set_heading(boat_system_t *boat_system, float heading) {
    boat_system->heading = heading;
}

uint16_t boat_system_get_servo_angle(boat_system_t *boat_system) {
    return boat_system->servo_angle;
}

uint32_t boat_system_get_servo_channel(boat_system_t *boat_system) {
    return boat_system->servo_channel;
}

TIM_HandleTypeDef *boat_system_get_servo_timer(boat_system_t *boat_system) {
    return boat_system->servo_timer;
}

uint16_t boat_system_get_motor_speed(boat_system_t *boat_system) {
    return boat_system->motor_speed;
}

direction boat_system_get_motor_direction(boat_system_t *boat_system) {
    return boat_system->motor_direction;
}

uint32_t boat_system_get_motor_channel(boat_system_t *boat_system) {
    return boat_system->motor_channel;
}

TIM_HandleTypeDef *boat_system_get_motor_timer(boat_system_t *boat_system) {
    return boat_system->motor_timer;
}

float boat_system_get_x_position(boat_system_t *boat_system) {
    return boat_system->x_position;
}

float boat_system_get_y_position(boat_system_t *boat_system) {
    return boat_system->y_position;
}

float boat_system_get_heading(boat_system_t *boat_system) {
    return boat_system->heading;
}