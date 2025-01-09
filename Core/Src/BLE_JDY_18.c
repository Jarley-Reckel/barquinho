#include "BLE_JDY_18.h"
UART_HandleTypeDef *huart;

// Prefixos comandos AT
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
    // Add mais prefixos
};

// Função para configurar o JDY-18 com configurações iniciais
void BLE_setup(UART_HandleTypeDef *huartInt, char *nome, Funcao_t funcao, Baudrate_t baud) {
	huart = huartInt;

	//APP permission Settings, todos habilitados
    BLE_send_command(AT_COMMAND_SET_PERMISSIONS, "11111");

	// Define o nome do dispositivo
    BLE_send_command(AT_COMMAND_DEFINIR_NOME, nome);

    // Define a função do dispositivo
    char funcao_str[2];
    itoa(funcao, funcao_str, 10);
    BLE_send_command(AT_COMMAND_DEFINIR_FUNCAO, funcao_str);

    // Define a taxa de transmissão (baud rate)
    char baud_str[2];
    itoa(baud, baud_str, 10);
    BLE_send_command(AT_COMMAND_DEFINIR_BAUD, baud_str);

    BLE_send_command(AT_COMMAND_IAC, "");

    BLE_send_command(AT_COMMAND_RESET, "");

    serial_print("hello world");
}

// escaneia por dispositivos BLE próximos e armazena suas informações em uma lista
void BLE_connect_Master_to_Slave_MAC (char *mac) {
	BLE_send_command(AT_COMMAND_CONECTAR, mac);
}


// Envia comando AT para o módulo JDY-18
void BLE_send_command(AtCommands_t command, char *parameter) {
    char *command_prefix = (char *)at_commands[command];

    size_t command_length = strlen(command_prefix) + strlen(parameter) + 4; // 4 para "\r\n"

    char complete_command[command_length];

    // Comando AT completo
    snprintf(complete_command, command_length, "%s%s\r\n", command_prefix, parameter);

    // Envia o comando AT para a UART
    HAL_UART_Transmit(&huart, (uint8_t *)complete_command, strlen(complete_command), HAL_MAX_DELAY);
}


void init_media_movel(MediaMovel_t *media) {
    for (int i = 0; i < MEDIA_MOVEL_JANELA; i++) {
        media->valores[i] = 0;
    }
    media->indice = 0;
    media->soma = 0;
}

int update_media_movel(MediaMovel_t *media, int new_value) {
    media->soma -= media->valores[media->indice];
    media->soma += new_value;
    media->valores[media->indice] = new_value;
    media->indice = (media->indice + 1) % MEDIA_MOVEL_JANELA;
    return media->soma / MEDIA_MOVEL_JANELA;
}

// Função para verificar se um MAC já está na lista de dispositivos
int find_device_index(Device *devices, int device_count, const char *mac)
{
    for (int i = 0; i < device_count; i++)
    {
        if (strcmp(devices[i].mac, mac) == 0)
        {
            return i; // Retorna o índice do dispositivo encontrado
        }
    }
    return -1; // MAC não encontrado
}

void parse_devices(const char *input, Device *devices, int *device_count, int scan_id)
{
    const char *start = input;
    const char *current;

    while ((current = strstr(start, "+DEV:")) != NULL)
    {
        if (*device_count >= MAX_DEVICES)
            break;

        // Avançar para o início dos dados do dispositivo
        current = strchr(current, '=');
        if (!current)
            break;
        current++;

        // Parsear MAC
        char mac[13];
        char *mac_end = strchr(current, ',');
        if (!mac_end || (mac_end - current) > 12)
        {
            start = current + 1;
            continue;
        }
        strncpy(mac, current, mac_end - current);
        mac[mac_end - current] = '\0';

        // Verificar se o MAC é válido
        if (!is_valid_mac(mac))
        {
            start = mac_end + 1;
            continue;
        }

        // Parsear RSSI
        current = mac_end + 1;
        char *rssi_end = strchr(current, ',');
        if (!rssi_end)
        {
            start = current + 1;
            continue; // Ignorar dispositivo mal formatado
        }
        int rssi = atoi(current);

        // Parsear Nome
        current = rssi_end + 1;
        char *name_end = strchr(current, '\n');
        if (!name_end)
            name_end = current + strlen(current); // Última linha pode não ter \n
        char name[MAX_NAME_LEN];
        strncpy(name, current, name_end - current);
        name[name_end - current] = '\0';

        // Verificar duplicidade de MAC
        int index = find_device_index(devices, *device_count, mac);
        if (index != -1)
        {
            // MAC já existe
            if (devices[index].scan_id == scan_id)
            {
                // Mesmo scan: calcular média do RSSI
                devices[index].rssi = (devices[index].rssi + rssi) / 2;
            }
            else
            {
                // Novo scan: atualizar RSSI diretamente
                devices[index].rssi = rssi;
                devices[index].scan_id = scan_id;
            }
        }
        else
        {
            // Novo dispositivo
            devices[*device_count].scan_id = scan_id;
            strcpy(devices[*device_count].mac, mac);
            devices[*device_count].rssi = rssi;
            strcpy(devices[*device_count].name, name);
            (*device_count)++;
        }

        start = name_end;
    }
}

// Função para verificar se um MAC é válido (12 caracteres hexadecimais)
int is_valid_mac(const char *mac)
{
    if (strlen(mac) != 12)
        return 0;
    for (int i = 0; i < 12; i++)
    {
        if (!isxdigit(mac[i]))
            return 0;
    }
    return 1;
}

// Function to calculate the distance from RSSI
double rssi_to_distance(int rssi, int A)
{
    return pow(10, (A - rssi) / (10 * 2)); // A = potencia à 1m
}

// Function to find a device by name and calculate its distance
double get_device_distance(Device *devices, int device_count, const char *device_name, int A) 
{
    for (int i = 0; i < device_count; i++)
    {
        if (strcmp(devices[i].name, device_name) == 0)
        {
            // Device found, calculate and return its distance
            return rssi_to_distance(devices[i].rssi, A);
        }
    }
    return -1; // Return -1 if device is not found
}
