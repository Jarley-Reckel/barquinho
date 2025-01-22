/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @authors        : Andre Lamego (andrelrc23@ufmg.br), 
  *                 : Jarley B. Reckel Jr (jarley@ufmg.br), 
  *                 : Mariana Givisiez (marianagivisiez@ufmg.br)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "tools_and_types.h"
#include "actuators.h"
#include "I2Cdev.h"
#include "HMC5883L_dev.h"
#include "boat_system.h"
#include "location_service.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct 
{
    uint32_t x;
    uint32_t y;
} location_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CMD_SCAN "AT+INQ\r\n"
#define RX_BUFFER_SIZE 512
#define MAX_DEVICES 10

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t rx_buffer[RX_BUFFER_SIZE] = {0}; // Buffer for received data
uint8_t received_data;                  // Temporary variable for incoming data
volatile uint8_t rx_index = 0;          // Index for the receive buffer
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void BLE_scan(Device *devices, int *device_count, int scan_id, char *msg); 
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void scan(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    float ar_coeffs[AR_ORDER] = {0.7, -0.3, 0.2}; // Coeficientes AR
    float ma_coeffs[MA_ORDER] = {0.5, 0.3, 0.1};       // Coeficientes MA


  ARMAFilter filter1;
  ARMAFilter filter2;
  ARMAFilter filter3;
  init_arma_filter(&filter1, ar_coeffs, ma_coeffs, RSSI_B1_START);
  init_arma_filter(&filter2, ar_coeffs, ma_coeffs, RSSI_B2_START);
  init_arma_filter(&filter3, ar_coeffs, ma_coeffs, RSSI_B3_START);

  Boat_system_t bs;
  bs.name = "Barco_vermelho";
  bs.function = MASTER;
  bs.hi2c1 = &hi2c1;
  bs.servo_angle = 0;
  bs.servo_channel = TIM_CHANNEL_1;
  bs.servo_timer = &htim4;
  bs.motor_speed = 0;
  bs.motor_direction = FORWARD;
  bs.motor_channel = TIM_CHANNEL_2;
  bs.motor_timer = &htim3;
  bs.x_position = 0;
  bs.y_position = 0;
  bs.heading = 0;
  bs.BLE_huart = &huart3;
  bs.BLE_baud = BAUD_9600;
  bs.device_count = 0;
  // strcpy(bs.devices[0].name,"PSE2022_B1");
  // strcpy(bs.devices[0].mac, " ");
  // bs.devices[0].rssi = RSSI_B1_START;
  // init_arma_filter(&bs.devices[0].filter, ar_coeffs, ma_coeffs, RSSI_B1_START);
  // strcpy(bs.devices[1].name,"PSE2022_B2");
  // strcpy(bs.devices[1].mac, " ");
  // bs.devices[1].rssi = RSSI_B2_START;
  // init_arma_filter(&bs.devices[1].filter, ar_coeffs, ma_coeffs, RSSI_B2_START);
  // strcpy(bs.devices[2].name,"PSE2022_B3");
  // strcpy(bs.devices[2].mac, " ");
  // bs.devices[2].rssi = RSSI_B3_START;
  // init_arma_filter(&bs.devices[2].filter, ar_coeffs, ma_coeffs, RSSI_B3_START);
  bs.rssi_reference = -61;
  char msg[RX_BUFFER_SIZE];
  I2Cdev_init(&hi2c1);
  HMC5883L_initialize();
  BLE_setup(bs.BLE_huart, bs.name, bs.function, bs.BLE_baud);
  // sprintf(msg, "Teste magnetometro: %d\n", (int)HMC5883L_testConnection());
  serial_print(msg);
  // HMC5883L_testConnection();
  HAL_GPIO_WritePin(GPIOA, L293D_EN_Pin, GPIO_PIN_RESET);

  double B1_distance;
  double B2_distance;
  double B3_distance;
  int B1_rssi;
  int B2_rssi;
  int B3_rssi;
  int scan_id = 1;

  // init_moving_average(&bs.rssi_avg);
  int media;
    // sprintf(msg, "Inicializando Barco Vermelho!!\n");
    serial_print(msg);
    bs.motor_speed = MOTOR_MAX_SPEED;
    for (int angle = 0; angle <= 180; angle += 10) {
      setServoAngle(&bs, angle);
      HAL_Delay(100);
    }
    for (int angle = 180; angle >= 0; angle -= 10) {
      setServoAngle(&bs, angle);
      HAL_Delay(100);
    }
    setServoAngle(&bs, 90);

    // sprintf(msg, "Escaneando dispositivos!!\n");
    serial_print(msg);
    // while (bs.device_count < 3) {
    HAL_UART_Receive_IT(&huart3, &received_data, 1);
    BLE_scan(bs.devices, &bs.device_count, scan_id, msg);
    HAL_Delay(100);
    // }
    for (int i = 0; i < bs.device_count; ++i) {
      // sprintf(msg, "\nDevice %d : %s", i, bs.devices[i].name);
      serial_print(msg);
    }
    

    int16_t aux_y =  COORD_Y;
    int16_t limit = abs(B1_X - B2_X) / 2;
    int16_t limit_line = abs(B1_X - B2_X) / LIMIT_LINE_FACTOR;
    int16_t limit_hard = abs(B1_X - B2_X) / HARD_LIMIT_FACTOR;
    int16_t distance_x = 0;
    int16_t distance_y = 0;
    double final_angle = atan2(B3_Y - B2_Y, B3_X - B2_X) * (180.0 / PI);
    sendCommand(&bs, FORWARD, MOTOR_MAX_SPEED);


    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
      BLE_scan(bs.devices, &bs.device_count, scan_id, msg);
      // sprintf(msg, "\nBuffer: %s\n", rx_buffer);
      serial_print(msg);
      for(int i = 0; i < bs.device_count; i++) {
        // sprintf(msg, "\nDevice %d : %s||||", i, bs.devices[i].name);
        serial_print(msg);
      }

      B1_rssi = get_device_rssi(bs.devices, bs.device_count,"PSE2022_B1");
      B2_rssi = get_device_rssi(bs.devices, bs.device_count,"PSE2022_B2");
      B3_rssi = get_device_rssi(bs.devices, bs.device_count,"PSE2022_B3");
      B1_rssi = apply_arma(&filter1, B1_rssi);
      B2_rssi = apply_arma(&filter2, B2_rssi);
      B3_rssi = apply_arma(&filter3, B3_rssi);
      // bs.devices[0].rssi = B1_rssi;
      // bs.devices[1].rssi = B2_rssi;
      // bs.devices[2].rssi = B3_rssi;  
      B1_distance = rssi_to_distance(B1_rssi, RSSI_Pt1, RSSI_n1);
      B2_distance = rssi_to_distance(B2_rssi, RSSI_Pt2, RSSI_n2);
      B3_distance = rssi_to_distance(B3_rssi, RSSI_Pt3, RSSI_n3);
      // B1_distance = get_device_distance(bs.devices, bs.device_count, "PSE2022_B1", B1_RSSI_1M);
      // B2_distance = get_device_distance(bs.devices, bs.device_count, "PSE2022_B2", B2_RSSI_1M);
      // B3_distance = get_device_distance(bs.devices, bs.device_count, "PSE2022_B3", B3_RSSI_1M);
      // sprintf(msg, "\nB1 distance : %.2f \t B1 RSSI: %d\nB2 distance : %.2f \t B2 RSSI: %d\nB3 distance : %.2f \t B3 RSSI: %d\n", B1_distance, B1_rssi, B2_distance, B2_rssi, B3_distance, B3_rssi);
      serial_print(msg);
      // media = update_moving_average(&bs.rssi_avg, B1_rssi);
      // // sprintf(msg, "\nMedia %d \n", media);
      // serial_print(msg);


      // if (distance_y < limit_line) {
      //   if(B2_distance < limit_hard) {
      //     if (bs.motor_speed > MOTOR_MIN_SPEED) {
      //       bs.motor_speed -= DESACELERATION;
      //       sendCommand(&bs, FORWARD, bs.motor_speed);
      //       HAL_Delay(10);
      //       continue;
      //     }
      //   }
      //   update_servor_angle(&bs, B3_Y, B3_X, final_angle);
      //   HAL_Delay(10);
      //   continue;
      // }

      update_boat_position(&bs, B1_distance, B2_distance, B3_distance);
      HMC588L_getDegree(&bs);
      // sprintf(msg, "\nX: %.2f \t Y: %.2f Ang: %.2f Servor: %.2f\n", bs.x_position, bs.y_position, bs.heading, bs.servo_angle);
      serial_print(msg);
      // distance_x = abs(B1_X - bs.x_position);
      distance_y = abs(B1_Y - bs.y_position);

      // update_servor_angle(&bs, B2_Y, B2_X);

      update_servor_angle(&bs, aux_y, B3_X);
      // setServoAngle(&bs, bs.servo_angle);
      // HAL_Delay(10);
      if (distance_y < limit) {
        aux_y = aux_y + (aux_y - B2_Y) / 2;
        limit /= 2;
      }
      if (limit < limit_hard) {
        limit = -1;
        aux_y = B2_Y;
      }

      // setServoAngle(&bs, DESTINY_DIRECTION);
      // HAL_Delay(10);
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 64-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1250;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 64-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1250;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, L293D_LATCH_Pin|L293D_SER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(L293D_EN_GPIO_Port, L293D_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(L293D_CLK_GPIO_Port, L293D_CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : L293D_LATCH_Pin L293D_EN_Pin L293D_SER_Pin */
  GPIO_InitStruct.Pin = L293D_LATCH_Pin|L293D_EN_Pin|L293D_SER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : L293D_CLK_Pin */
  GPIO_InitStruct.Pin = L293D_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(L293D_CLK_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void BLE_scan(Device *devices, int *device_count, int scan_id, char *msg)
{
    scan(); 
    // Print received data via UART2 for debugging
    if (rx_index > 0)
    {
        snprintf(msg, RX_BUFFER_SIZE, "Dispositivo encontrado: MAC: %s\n", (char *)rx_buffer);
        serial_print(msg); // Envia a mensagem via UART// Reset index
        parse_devices((char *)rx_buffer, devices, device_count, scan_id);
        scan_id++;
        memset(rx_buffer, 0, RX_BUFFER_SIZE); // Clear the buffer
        rx_index = 0;
    }
}

int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

char serial_print(char *_msg) {
    // while (*_msg) {
    //     if (__io_putchar(*_msg) != *_msg) {
    //         return *_msg;
    //     }
    //     _msg++;
    // }
    return ' ';
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) { // Handle UART3 data reception
        if (rx_index < RX_BUFFER_SIZE - 1) {
            rx_buffer[rx_index++] = received_data; // Append to buffer
        }
        HAL_UART_Receive_IT(&huart3, &received_data, 1); // Continue receiving
    }
}

void scan() 
{
	char command[] = "AT+INQ\r\n";
    HAL_UART_Transmit(&huart3, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
    HAL_Delay(300); // Ensure sufficient time for command execution
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
/* User can add his own implementation to report the HAL error return state */
__disable_irq();
while (1)
{
}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
/* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
