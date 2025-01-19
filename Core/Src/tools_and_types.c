/**
 * @file        : tools_and_types.h
 * @authors     : Andre Lamego (andrelrc23@ufmg.br), 
 *              : Jarley B. Reckel Jr (jarley@ufmg.br), 
 *              : Mariana Givisiez (marianagivisiez@ufmg.br)
 * @brief       : This file contains the implementation of the common functions and types of the application.
 * @version     : 0.1
 * @date        : 2025-01-15
 * 
 * @copyright   : Copyright (c) 2025
 * 
 */

#include "tools_and_types.h"

UART_HandleTypeDef *serial_huart; // Pointer to the serial port handler

void init_serial(UART_HandleTypeDef *huart) {
    serial_huart = huart;
}

int __io_putchar(int ch) {
    HAL_UART_Transmit(&serial_huart, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

char serial_print(char *_msg) {
    while (*_msg) {
        if (__io_putchar(*_msg) != *_msg) {
            return *_msg;
        }
        _msg++;
    }
    return ' ';
}
