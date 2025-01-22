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

// UART_HandleTypeDef *serial_huart; // Pointer to the serial port handler

// void init_serial(UART_HandleTypeDef *huart) {
//     serial_huart = huart;
// }

// extern int __io_putchar(int ch) {
//     HAL_UART_Transmit(serial_huart, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
//     return ch;
// }

// char serial_print(char *_msg) {
//     while (*_msg) {
//         if (__io_putchar(*_msg) != *_msg) {
//             return *_msg;
//         }
//         _msg++;
//     }
//     return ' ';
// }

float sub(float a, float b) {
    float s = a - b;
    if(s < -180) {
        s += 360;
    } else if(s > 180) {
        s -= 360;
    }
    return s;
}

float sum(float a, float b) {
    float s = a + b;
    if(s < -180) {
        s += 360;
    } else if(s > 180) {
        s -= 360;
    }
    return s;
}

// Estrutura para armazenar estados do filtro ARMA

// Inicializa o filtro ARMA
void init_arma_filter(ARMAFilter *filter, float *ar_coeffs, float *ma_coeffs, float initial_rssi) {
    for (int i = 0; i < AR_ORDER; i++) {
        filter->ar_coeffs[i] = ar_coeffs[i];
        filter->past_rssi[i] = initial_rssi;
    }
    for (int i = 0; i < MA_ORDER; i++) {
        filter->ma_coeffs[i] = ma_coeffs[i];
        filter->past_errors[i] = 0.0f;
    }
}

// Aplica o filtro exponencial (EWMA)
float apply_ewma(float new_rssi, float prev_filtered_rssi) {
    return ALPHA * new_rssi + (1 - ALPHA) * prev_filtered_rssi;
}

// Aplica o filtro ARMA
float apply_arma(ARMAFilter *filter, float new_rssi) {
    // Calcula a parte AR (autoregressiva)
    float ar_part = 0.0f;
    for (int i = 0; i < AR_ORDER; i++) {
        ar_part += filter->ar_coeffs[i] * filter->past_rssi[i];
    }

    // Calcula a parte MA (média móvel)
    float ma_part = 0.0f;
    for (int i = 0; i < MA_ORDER; i++) {
        ma_part += filter->ma_coeffs[i] * filter->past_errors[i];
    }

    // Calcula o erro atual
    float error = new_rssi - ar_part;

    // Atualiza os buffers
    for (int i = AR_ORDER - 1; i > 0; i--) {
        filter->past_rssi[i] = filter->past_rssi[i - 1];
    }
    filter->past_rssi[0] = new_rssi;

    for (int i = MA_ORDER - 1; i > 0; i--) {
        filter->past_errors[i] = filter->past_errors[i - 1];
    }
    filter->past_errors[0] = error;

    // Retorna o valor filtrado
    return ar_part + ma_part;
}