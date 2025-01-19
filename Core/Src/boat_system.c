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

void boat_system_set_servo_angle(Boat_system_t *boat_system, int16_t angle) {
    boat_system->servo_angle = angle;
}

void boat_system_set_motor_speed(Boat_system_t *boat_system, uint16_t speed) {
    boat_system->motor_speed = speed;
}

void boat_system_set_motor_direction(Boat_system_t *boat_system, direction direction) {
    boat_system->motor_direction = direction;
}

void boat_system_set_x_position(Boat_system_t *boat_system, float x_position) {
    boat_system->x_position = x_position;
}

void boat_system_set_y_position(Boat_system_t *boat_system, float y_position) {
    boat_system->y_position = y_position;
}

void boat_system_set_heading(Boat_system_t *boat_system, float heading) {
    boat_system->heading = heading;
}

void boat_system_set_number_of_devices(Boat_system_t *boat_system, int number_of_devices) {
    boat_system->device_count = number_of_devices;
}

void boat_system_set_rssi_reference(Boat_system_t *boat_system, int rssi_reference) {
    boat_system->rssi_reference = rssi_reference;
}

uint16_t boat_system_get_servo_angle(Boat_system_t *boat_system) {
    return boat_system->servo_angle;
}

uint32_t boat_system_get_servo_channel(Boat_system_t *boat_system) {
    return boat_system->servo_channel;
}

TIM_HandleTypeDef *boat_system_get_servo_timer(Boat_system_t *boat_system) {
    return boat_system->servo_timer;
}

uint16_t boat_system_get_motor_speed(Boat_system_t *boat_system) {
    return boat_system->motor_speed;
}

direction boat_system_get_motor_direction(Boat_system_t *boat_system) {
    return boat_system->motor_direction;
}

uint32_t boat_system_get_motor_channel(Boat_system_t *boat_system) {
    return boat_system->motor_channel;
}

TIM_HandleTypeDef *boat_system_get_motor_timer(Boat_system_t *boat_system) {
    return boat_system->motor_timer;
}

float boat_system_get_x_position(Boat_system_t *boat_system) {
    return boat_system->x_position;
}

float boat_system_get_y_position(Boat_system_t *boat_system) {
    return boat_system->y_position;
}

float boat_system_get_heading(Boat_system_t *boat_system) {
    return boat_system->heading;
}

Device *boat_system_get_devices(Boat_system_t *boat_system) {
    return boat_system->devices;
}

int boat_system_get_number_of_devices(Boat_system_t *boat_system) {
    return boat_system->device_count;
}

int boat_system_get_rssi_reference(Boat_system_t *boat_system) {
    return boat_system->rssi_reference;
}
