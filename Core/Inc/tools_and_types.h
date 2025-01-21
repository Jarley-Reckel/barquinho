/**
 * @file        : tools_and_types.h
 * @authors     : Andre Lamego (andrelrc23@ufmg.br), 
 *              : Jarley B. Reckel Jr (jarley@ufmg.br), 
 *              : Mariana Givisiez (marianagivisiez@ufmg.br)
 * @brief       : This file contains the common defines and types of the application.
 * @version     : 0.1
 * @date        : 2025-01-15
 * 
 * @copyright   : Copyright (c) 2025
 * 
 */

#ifndef __DEFINES__H__
#define __DEFINES__H__

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define AVG_WINDOW 10           ///< Window size for the moving average
#define MAX_SLAVES 3            ///< Maximum number of slaves
#define MAX_NAME_LEN 50         ///< Maximum length of the device name
#define MAX_DEVICES 10          ///< Maximum number of devices
#define MAX_MAC_LEN 13          ///< Maximum length of the MAC address
#define B1_X -43.96169                 ///< X coordinate of device B1
#define B1_Y -19.86944                 ///< Y coordinate of device B1
#define B2_X -43.96167               ///< X coordinate of device B2
#define B2_Y -19.86950                  ///< Y coordinate of device B2
#define B3_X -43.96172                 ///< X coordinate of device B3
#define B3_Y -19.86949                  ///< Y coordinate of device B3
#define DESTINY_X -43.96167                  ///< X coordinate of device B3
#define DESTINY_DIRECTION 130                  ///< Angle of the destiny line
#define LIMIT_LINE_FACTOR 10
#define HARD_LIMIT_FACTOR 12
#define B1_RSSI_1M -60          ///< RSSI reference value at 1 meter for device B1
#define B2_RSSI_1M -58          ///< RSSI reference value at 1 meter for device B2
#define B3_RSSI_1M -60          ///< RSSI reference value at 1 meter for device B3

/**
 * @brief Enumeration of the possible functions of the JDY-18 module
 * 
 */
typedef enum {
    SLAVE,                      ///< Slave function
    MASTER,                     ///< Master function
    IBEACON,                    ///< iBeacon function
} Function_t;

/**
 * @brief Define the speed of the change of the servo 
 * 
 */
typedef enum {
    THIN = 1,
    THICK = 10,
} Pass_t;

/**
 * @brief Enumeration of the possible AT commands
 * 
 */
typedef enum {
    AT_COMMAND_SET_PERMISSIONS,     ///< Set the permissions of the device
    AT_COMMAND_RESET,               ///< Reset the device
    AT_COMMAND_DEFINIR_NOME,        ///< Set the name of the device
    AT_COMMAND_DEFINIR_FUNCAO,      ///< Set the function of the device
    AT_COMMAND_DEFINIR_BAUD,        ///< Set the baud rate of the device
    AT_COMMAND_DEFINIR_UUID,        ///< Set the UUID of the device
    AT_COMMAND_DEFINIR_POTENCIA,    ///< Set the power of the device
    AT_COMMAND_SCAN_SLAVES,         ///< Scan for nearby devices
    AT_COMMAND_CONECTAR,            ///< Connect to a device
	AT_COMMAND_IAC,                 ///< Set the IAC
    // Adicionar comandos
} AtCommands_t;

/**
 * @brief Structure to store the information of a device
 * 
 */
typedef struct {
    char mac[13];               ///< MAC address of the device
    int rssi;                   ///< Received Signal Strength Indicator
    int last_scan_id;           ///< Last scan ID where the device was found
    char name[MAX_NAME_LEN];    ///< Name of the device
} Device;

/**
 * @brief Enumeration of the possible baud rates
 * 
 */
typedef enum {
    BAUD_4800,
    BAUD_9600,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200,
} Baudrate_t;

/**
 * @brief Structure to store the moving average
 * 
 */
typedef struct {
    int value[AVG_WINDOW];      ///< Array of values
    int index;                  ///< Index of the array
    int sum;                    ///< Sum of the values
} Moving_avg_t;

// /**
//  * @brief Set the handler for the serial port
//  * 
//  * @param huart 
//  */
// void init_serial(UART_HandleTypeDef *huart);


// /**
//  * @brief Function to print a character to the serial port
//  * 
//  * @param ch 
//  * @return int 
//  */
// int __io_putchar(int ch);

// /**
//  * @brief Function to print a message to the serial port
//  *
//  * @param _msg
//  * @return char
//  */
// char serial_print(char *_msg);



#endif // __DEFINES__H__