/**
 * @file        : location_service.c
 * @authors     : Andre Lamego (andrelrc23@ufmg.br), 
 *              : Jarley B. Reckel Jr (jarley@ufmg.br), 
 *              : Mariana Givisiez (marianagivisiez@ufmg.br)
 * @brief       : This file contains the implementation of the functions for the location service.
 * @version     : 0.1
 * @date        : 2024-12-08
 * 
 * @copyright   : Copyright (c) 2024
 * 
 */

#include "location_service.h"
#include <ctype.h>
UART_HandleTypeDef *huart;

const char *at_commands[] = {
    "AT+PERM",
    "AT+RESET",
    "AT+NAME=",
    "AT+ROLE=",
    "AT+BAUD=",
    "AT+UUID=",
    "AT+POWER=",
    "AT+INQ",
    "AT+CONN",
    "AT+IAC"
};

void BLE_setup(UART_HandleTypeDef *huartInt, char *name, Function_t device_function, Baudrate_t baud) {
	huart = huartInt;

    BLE_send_command(AT_COMMAND_SET_PERMISSIONS, "11111");
    BLE_send_command(AT_COMMAND_DEFINIR_NOME, name);

    char function_str[2];
    itoa(device_function, function_str, 10);
    BLE_send_command(AT_COMMAND_DEFINIR_FUNCAO, function_str);

    char baud_str[2];
    itoa(baud, baud_str, 10);
    BLE_send_command(AT_COMMAND_DEFINIR_BAUD, baud_str);
    BLE_send_command(AT_COMMAND_IAC, "");
    BLE_send_command(AT_COMMAND_RESET, "");

    serial_print("hello world");
}

void BLE_connect_Master_to_Slave_MAC (char *mac) {
	BLE_send_command(AT_COMMAND_CONECTAR, mac);
}


void BLE_send_command(AtCommands_t command, char *parameter) {
    char *command_prefix = (char *)at_commands[command];

    size_t command_length = strlen(command_prefix) + strlen(parameter) + 4; // 4 para "\r\n"

    char complete_command[command_length];

    // Format the complete AT command
    snprintf(complete_command, command_length, "%s%s\r\n", command_prefix, parameter);

    // Send the command via UART
    HAL_UART_Transmit(huart, (uint8_t *)complete_command, strlen(complete_command), HAL_MAX_DELAY);
}


void init_moving_average(Moving_avg_t *average) {
    for (int i = 0; i < AVG_WINDOW; ++i) {
        average->value[i] = 0;
    }
    average->index = 0;
    average->sum = 0;
}

int update_moving_average(Moving_avg_t *average, int new_value) {
    average->sum -= average->value[average->index];
    average->sum += new_value;
    average->value[average->index] = new_value;
    average->index = (average->index + 1) % AVG_WINDOW;
    return average->sum / AVG_WINDOW;
}

int find_device_index(Device *devices, int device_count, const char *mac) {
    for (int i = 0; i < device_count; ++i) {
        if (strcmp(devices[i].mac, mac) == 0) {
            return i;
        }
    }
    return -1;
}

int calculate_average_rssi(int rssi_sum, int count) {
    return count > 0 ? rssi_sum / count : rssi_sum;
}

void parse_devices(const char *input, Device *devices, int *device_count, int scan_id) {
    const char *start = input;
    const char *current;

    while ((current = strstr(start, "+DEV:")) != NULL) {
        if (*device_count >= MAX_DEVICES) {
            break;
        }

        // Go to the beginning of the device data
        current = strchr(current, '=');
        if (!current) {
            break;
        }
        ++current;

        // Parse MAC
        char mac[13];
        char *mac_end = strchr(current, ',');
        if (!mac_end || (mac_end - current) > 12) {
            start = current + 1;
            continue;
        }
        strncpy(mac, current, mac_end - current);
        mac[mac_end - current] = '\0';

        if (!is_valid_mac(mac)) {
            start = mac_end + 1;
            continue;
        }

        // Parse RSSI
        current = mac_end + 1;
        char *rssi_end = strchr(current, ',');
        if (!rssi_end) {
            start = current + 1;
            continue;
        }
        int rssi = atoi(current);

        // Parse Name
        current = rssi_end + 1;
        char *name_end = strchr(current, '+');
        if (!name_end) {
            name_end = current + strlen(current); // Last line can't have '+'
        }
        char name[MAX_NAME_LEN];
        strncpy(name, current, name_end - current);
        name[name_end - current] = '\0';

        // Check duplicate MAC
        int index = find_device_index(devices, *device_count, mac);
        if (index != -1) {
            if (devices[index].last_scan_id == scan_id) {
                // Same scan: calculate average RSSI
                devices[index].rssi = (devices[index].rssi + rssi) / 2;
            } else {
                // New scan: update RSSI
                devices[index].rssi = rssi;
                devices[index].last_scan_id = scan_id;
            }
        } else {
            // New device
            devices[*device_count].last_scan_id = scan_id;
            strcpy(devices[*device_count].mac, mac);
            devices[*device_count].rssi = rssi;
            strcpy(devices[*device_count].name, name);
            (*device_count)++;
        }
        start = name_end; // Move to the next device
    }
}

int is_valid_mac(const char *mac) {
    if (strlen(mac) != 12) {
        return 0;
    }
    for (int i = 0; i < 12; ++i) {
        if (!isxdigit(mac[i])) {
            return 0;
        }
    }
    return 1;
}

double rssi_to_distance(int rssi, int rssi_1m) {
    return pow(10, (double)(rssi_1m - rssi) / (10 * 2.5) + 0.3);
}

double get_device_distance(Device *devices, int device_count, const char *device_name, int rssi_1m) {
    for (int i = 0; i < device_count; ++i) {
        if (strcmp(devices[i].name, device_name) == 0) {
            return rssi_to_distance(devices[i].rssi, rssi_1m);
        }
    }
    return -1;
}

int get_device_rssi(Device *devices, int device_count, const char *device_name) {
    for (int i = 0; i < device_count; ++i) {
        if (strcmp(devices[i].name, device_name) == 0) {
            return devices[i].rssi;
        }
    }
    return -1;
}

void update_boat_position(Boat_system_t *bs, double B1_distance, double B2_distance, double B3_distance) {
    double A = 2 * (B2_X - B1_X);
    double B = 2 * (B2_Y - B1_Y);
    double C = B1_distance * B1_distance - B2_distance * B2_distance - B1_X * B1_X + B2_X * B2_X - B1_Y * B1_Y + B2_Y * B2_Y;

    double D = 2 * (B3_X - B2_X);
    double E = 2 * (B3_Y - B2_Y);
    double F = B2_distance * B2_distance - B3_distance * B3_distance - B2_X * B2_X + B3_X * B3_X - B2_Y * B2_Y + B3_Y * B3_Y;

    bs->x_position = (C * E - F * B) / (A * E - B * D);
    bs->y_position = (C * D - A * F) / (B * D - A * E);
}
