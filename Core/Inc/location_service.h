/**
 * @file        : location_service.h
 * @authors     : Andre Lamego (andrelrc23@ufmg.br), 
 *              : Jarley B. Reckel Jr (jarley@ufmg.br), 
 *              : Mariana Givisiez (marianagivisiez@ufmg.br)
 * @brief       : This file contains the common defines of the application. The 
 *              : functions in this file are used to configure the JDY-18 module 
 *              : and to scan for nearby devices, with this informations we can 
 *              : calculate the distance between the devices.
 * @version     : 0.1
 * @date        : 2025-01-15
 * 
 * @copyright   : Copyright (c) 2025
 * 
 */

#ifndef __LOCATION_SERVICE__H__
#define __LOCATION_SERVICE__H__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "main.h"


#define AVG_WINDOW 10           ///< Window size for the moving average
#define MAX_SLAVES 3            ///< Maximum number of slaves
#define MAX_NAME_LEN 50         ///< Maximum length of the device name
#define MAX_DEVICES 10          ///< Maximum number of devices
#define MAX_MAC_LEN 13          ///< Maximum length of the MAC address

/**
 * @brief Enumeration of the possible functions of the JDY-18 module
 * 
 */
typedef enum {
    SLAVE,                      ///< Slave function
    MASTER,                     ///< Master function
    IBEACON,                    ///< iBeacon function
} Funcao_t;

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

/**
 * @brief Function to set up the JDY-18 module
 * 
 * @param huartInt          UART handler
 * @param name              Name of the device
 * @param device_function   Function of the device
 * @param baud              Baud rate of the device
 */
void BLE_setup(UART_HandleTypeDef *huartInt, char *name, Funcao_t device_function, Baudrate_t baud);

/**
 * @brief Function to scan for nearby devices and store their information in a list
 * 
 * @param mac   MAC address of the device
 */
void BLE_connect_Master_to_Slave_MAC (char *mac);

/**
 * @brief Function to send an AT command to the JDY-18 module
 * 
 * @param command AT command for the JDY-18 module
 * @param parameter For the respective AT command
 */
void BLE_send_command(AtCommands_t command, char *parameter);

/**
 * @brief Function to initialize the moving average
 * 
 * @param average Pointer to the moving average
 */
void init_moving_average(Moving_avg_t *average);

/**
 * @brief Function to update the moving average
 * 
 * @param average       Pointer to the moving average
 * @param new_value     New value to be added to the moving average
 * @return              Current moving average
 */
int update_moving_average(Moving_avg_t *average, int new_value);

/**
 * @brief Function to check if a device is already in the list
 * 
 * @param devices       List of devices
 * @param device_count  Number of devices in the list
 * @param mac           MAC address of the device to be found
 * @return              Index of the device in the list or -1 if not found
 */
int find_device_index(Device *devices, int device_count, const char *mac);

/**
 * @brief Function to calculate the average RSSI
 * 
 * @param rssi_sum      Sum of the RSSI values
 * @param count         Number of RSSI values
 * @return              Average RSSI
 */
int calculate_average_rssi(int rssi_sum, int count);

/**
 * @brief Parse the devices found in the scan and store them in a list of devices. 
 * 
 * @param input         Input string
 * @param devices       List of devices
 * @param device_count  Number of devices in the list
 * @param scan_id       ID of the scan
 */
void parse_devices(const char *input, Device *devices, int *device_count, int scan_id);

/**
 * @brief Function to verify if a MAC address is valid, i.e., if it has 12 hexadecimal characters
 * 
 * @param mac       MAC address to be verified
 * @return          1 if the MAC address is valid, 0 otherwise
 */
int is_valid_mac(const char *mac);

/**
 * @brief Function to calculate the distance from RSSI
 * 
 * @param rssi      RSSI value to be converted
 * @param rssi_1m   RSSI reference value at 1 meter
 * @return          Distance
 */
double rssi_to_distance(int rssi, int A);

/**
 * @brief Function to find a device by name and calculate its distance
 * 
 * @param devices       List of devices
 * @param device_count  Number of devices in the list
 * @param device_name   Name of the device to be found
 * @param rssi_1m       RSSI reference value at 1 meter 
 * @return              Distance of the device, -1 if the device is not found
 */
double get_device_distance(Device *devices, int device_count, const char *device_name, int rssi_1m);

/**
 * @brief Function to find a device by name and calculate its RSSI
 * 
 * @param devices       List of devices
 * @param device_count  Number of devices in the list
 * @param device_name   Name of the device to be found
 * @return              RSSI of the device, -1 if the device is not found
 */
int get_device_rssi(Device *devices, int device_count, const char *device_name);




#endif // __LOCATION_SERVICE__H__