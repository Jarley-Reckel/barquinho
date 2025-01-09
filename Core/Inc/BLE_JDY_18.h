#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "main.h"


#define MEDIA_MOVEL_JANELA 10
#define MAX_SLAVES 3
#define MAX_NAME_LEN 50
#define MAX_DEVICES 10

typedef enum {
    SLAVE,
    MASTER,
    IBEACON,
} Funcao_t;

// JDY-18 comandos AT
typedef enum {
    AT_COMMAND_SET_PERMISSIONS,
    AT_COMMAND_RESET,
    AT_COMMAND_DEFINIR_NOME,
    AT_COMMAND_DEFINIR_FUNCAO,
    AT_COMMAND_DEFINIR_BAUD,
    AT_COMMAND_DEFINIR_UUID,
    AT_COMMAND_DEFINIR_POTENCIA,
    AT_COMMAND_SCAN_SLAVES,
    AT_COMMAND_CONECTAR,
	AT_COMMAND_IAC,
    // Adicionar comandos
} AtCommands_t;

typedef struct
{
    char mac[13];
    int rssi;
    int scan_id; // Último scan no qual o dispositivo foi encontrado
    char name[MAX_NAME_LEN];
} Device;

typedef enum {
    BAUD_4800,
    BAUD_9600,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200,
} Baudrate_t;

typedef struct {
    int valores[MEDIA_MOVEL_JANELA];
    int indice;
    int soma;
} MediaMovel_t;

void BLE_setup(UART_HandleTypeDef *huartInt, char *nome, Funcao_t funcao, Baudrate_t baud);
void BLE_connect_Master_to_Slave_MAC (char *mac);
void BLE_send_command(AtCommands_t command, char *parameter);


void init_media_movel(MediaMovel_t *media);
int update_media_movel(MediaMovel_t *media, int new_value);

int find_device_index(Device *devices, int device_count, const char *mac);
void parse_devices(const char *input, Device *devices, int *device_count, int scan_id);
int is_valid_mac(const char *mac);
double rssi_to_distance(int rssi, int A);
double get_device_distance(Device *devices, int device_count, const char *device_name, int A);
