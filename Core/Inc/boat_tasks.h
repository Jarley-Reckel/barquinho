/**
 * @file        : bast_tasks.h
 * @authors     : Andre Lamego (andrelrc23@ufmg.br), 
 *              : Jarley B. Reckel Jr (jarley@ufmg.br), 
 *              : Mariana Givisiez (marianagivisiez@ufmg.br)
 * @brief       : This file contains the handling of the tasks of the boat system.
 * @version     : 0.1
 * @date        : 2024-12-08
 * 
 * @copyright   : Copyright (c) 2024
 * 
 */

#ifndef __BOAT_TASKS_H
#define __BOAT_TASKS_H

#include "tools_and_types.h"
#include "boat_system.h"
#include "control_system.h"


/**
 * @brief Control the flow of the tasks, getting the location, controlling the motor and the servo.
 * 
 * @param boat_system 
 */
void boat_task_control(Boat_system_t *boat_system);

/**
 * @brief Update the current position of the boat
 * 
 * @param boat_system 
 */
void boat_task_location(Boat_system_t *boat_system);

/**
 * @brief Wait the command to set the speed and direction for the motor
 * 
 * @param boat_system 
 */
void boat_task_motor(Boat_system_t *boat_system);


/**
 * @brief Wait the command to set positions for the servo
 * 
 * @param boat_system 
 */
void boat_task_servo(Boat_system_t *boat_system);

#endif // __BOAT_TASKS_H
