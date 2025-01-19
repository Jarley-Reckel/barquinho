/**
 * @file        : boat_system.h
 * @authors     : Andre Lamego (andrelrc23@ufmg.br), 
 *              : Jarley B. Reckel Jr (jarley@ufmg.br), 
 *              : Mariana Givisiez (marianagivisiez@ufmg.br)
 * @brief       : This file contains the common defines of the application.
 * @version     : 0.1
 * @date        : 2024-12-08
 * 
 * @copyright   : Copyright (c) 2024
 * 
 */

#include "main.h"
#include "tools_and_types.h"

#ifndef __BOAT_SYSTEM_H
#define __BOAT_SYSTEM_H

/**
 * @brief This enum represents the direction of the motor
 * 
 */
typedef enum direction {
    FORWARD = 0b10101100,
    BACKWARD = 0b01010011
} direction;

/**
 * @brief This struct represents the boat system, containing everything needed to control the boat,
 *        like handles, variables, configurations and current states.
 * 
 */
typedef struct boat_system {
    char *name;                         ///< Name of the boat
    Function_t function;                ///< Function of the boat

    I2C_HandleTypeDef *hi2c1;           ///< Handle to the I2C1

    int16_t servo_angle;                ///< Current angle of the servo
    uint32_t servo_channel;             ///< Channel of the servo
    TIM_HandleTypeDef *servo_timer;     ///< Handle to the servo timer

    uint16_t motor_speed;               ///< Current speed of the motor
    direction motor_direction;          ///< Current direction of the motor
    uint32_t motor_channel;             ///< Channel of the motor
    TIM_HandleTypeDef *motor_timer;     ///< Handle to the motor timer

    float x_position;                   ///< Current x position of the boat
    float y_position;                   ///< Current y position of the boat
    float heading;                      ///< Current heading of the boat

    UART_HandleTypeDef *BLE_huart;      ///< Handle to the UART connected to the BLE module
    Baudrate_t BLE_baud;                ///< Baud rate of the BLE module
    Device devices[MAX_DEVICES];        ///< List of devices, default size is 10
    Moving_avg_t rssi_avg;              ///< Moving average of the RSSI values
    int device_count;                   ///< Number of devices
    int rssi_reference;                  ///< Reference RSSI value for 1 meter
} Boat_system_t;

/**
 * @brief Set the current angle of the servo
 * 
 * @param boat_system 
 * @param angle 
 */
void boat_system_set_servo_angle(Boat_system_t *boat_system, int16_t angle);

/**
 * @brief Set the current speed of the motor
 * 
 * @param boat_system 
 * @param speed 
 */
void boat_system_set_motor_speed(Boat_system_t *boat_system, uint16_t speed);

/**
 * @brief Set the current direction of the motor
 * 
 * @param boat_system 
 * @param direction 
 */
void boat_system_set_motor_direction(Boat_system_t *boat_system, direction direction);

/**
 * @brief Set the current x position of the boat
 * 
 * @param boat_system 
 * @param x_position 
 */
void boat_system_set_x_position(Boat_system_t *boat_system, float x_position);

/**
 * @brief Set the current y position of the boat
 * 
 * @param boat_system 
 * @param y_position 
 */
void boat_system_set_y_position(Boat_system_t *boat_system, float y_position);

/**
 * @brief Set the current heading of the boat
 * 
 * @param boat_system 
 * @param heading 
 */
void boat_system_set_heading(Boat_system_t *boat_system, float heading);

/**
 * @brief Set the current number of scanned devices
 * 
 * @param boat_system 
 * @param number_of_devices 
 */
void boat_system_set_number_of_devices(Boat_system_t *boat_system, int number_of_devices);

/**
 * @brief Set the reference RSSI value for 1 meter
 * 
 * @param boat_system 
 * @param rssi_reference 
 */
void boat_system_set_rssi_reference(Boat_system_t *boat_system, int rssi_reference);

/**
 * @brief Get the current angle of the servo
 * 
 * @param boat_system 
 * @return uint16_t 
 */
uint16_t boat_system_get_servo_angle(Boat_system_t *boat_system);

/**
 * @brief Get the current channel of the servo
 * 
 * @param boat_system 
 * @return uint32_t 
 */
uint32_t boat_system_get_servo_channel(Boat_system_t *boat_system);

/**
 * @brief Get the current timer of the servo
 * 
 * @param boat_system 
 * @return TIM_HandleTypeDef* 
 */
TIM_HandleTypeDef *boat_system_get_servo_timer(Boat_system_t *boat_system);

/**
 * @brief Get the current speed of the motor
 * 
 * @param boat_system 
 * @return uint16_t 
 */
uint16_t boat_system_get_motor_speed(Boat_system_t *boat_system);

/**
 * @brief Get the current direction of the motor
 * 
 * @param boat_system 
 * @return direction 
 */
direction boat_system_get_motor_direction(Boat_system_t *boat_system);

/**
 * @brief Get the current channel of the motor
 * 
 * @param boat_system 
 * @return uint32_t 
 */
uint32_t boat_system_get_motor_channel(Boat_system_t *boat_system);

/**
 * @brief Get the current timer of the motor
 * 
 * @param boat_system 
 * @return TIM_HandleTypeDef* 
 */
TIM_HandleTypeDef *boat_system_get_motor_timer(Boat_system_t *boat_system);

/**
 * @brief Get the current x position of the boat
 * 
 * @param boat_system 
 * @return float 
 */
float boat_system_get_x_position(Boat_system_t *boat_system);

/**
 * @brief Get the current y position of the boat
 * 
 * @param boat_system 
 * @return float 
 */
float boat_system_get_y_position(Boat_system_t *boat_system);

/**
 * @brief Get the current heading of the boat
 * 
 * @param boat_system 
 * @return float 
 */
float boat_system_get_heading(Boat_system_t *boat_system);

/**
 * @brief Get the pointer to the list of devices
 * 
 * @param boat_system 
 * @return Device* 
 */
Device *boat_system_get_devices(Boat_system_t *boat_system);

/**
 * @brief Get the current number of scanned devices
 * 
 * @param boat_system 
 * @return int
 */
int boat_system_get_number_of_devices(Boat_system_t *boat_system);

/**
 * @brief Get the reference RSSI value for 1 meter
 * 
 * @param boat_system 
 * @return int 
 */
int boat_system_get_rssi_reference(Boat_system_t *boat_system);



#endif // __BOAT_SYSTEM_H