#include <iostream>
#include <pwd.h>
#include <unistd.h>

#include <cerrno>
#include <csignal>
using std::cerr;
#include "main.h"
// internal includes
#include "input.h"
#include "program.h"

RTC_HandleTypeDef hrtc;
UART_HandleTypeDef huart1;
RNG_HandleTypeDef hrng;
SD_HandleTypeDef hsd;

void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	while (1) {}
	/* USER CODE END Error_Handler_Debug */
}

/**
	* @brief RNG Initialization Function
	* @param None
	* @retval None
	*/
static void MX_RNG_Init(void) {
	hrng.Instance = RNG;
	if (HAL_RNG_Init(&hrng) != HAL_OK) {
	  Error_Handler();
	}
}

/**
	* @brief RTC Initialization Function
	* @param None
	* @retval None
	*/
static void MX_RTC_Init(void) {
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
	  Error_Handler();
	}
}
/*
static void MX_SDIO_SD_Init(void) {
	hsd.Instance = SDIO;
	hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd.Init.ClockDiv = 0;
	if (HAL_SD_Init(&hsd) != HAL_OK) {
		__disable_irq();
		while (1) {
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
			HAL_Delay(250);
		}
	}
}
*/
/**
	* @brief USART1 Initialization Function
	* @param None
	* @retval None
	*/
static void MX_USART1_UART_Init(void) {
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
}

/**
	* @brief GPIO Initialization Function
	* @param None
	* @retval None
	*/
static void MX_GPIO_Init(void) {
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* GPIO Ports Clock Enable */
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/*Configure GPIO pin : PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Configure the main internal regulator output voltage */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Initializes the RCC Oscillators */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | 
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // <<< Use PLL!
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2; // APB1 max 50 MHz (here 48MHz)
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; // APB2 max 100 MHz (here 96MHz)
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        Error_Handler();
    }
}

#ifdef  USE_FULL_ASSERT
/**
	* @brief  Reports the name of the source file and the source line number
	*		 where the assert_param error has occurred.
	* @param  file: pointer to the source file name
	* @param  line: assert_param error line source number
	* @retval None
	*/
void assert_failed(uint8_t *file, uint32_t line) {
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

extern "C" { // Required for C++ to link with C library functions
	int _write(int fd, char *ptr, int len) {
		// Ensure the output is directed to stdout (file descriptor 1)
		if (fd == 1) { 
			// Use your specific UART transmit function here
			// Example using STM32 HAL library:
			HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY); 
			return len;
		}
		return -1; // Indicate error for other file descriptors
	}
	// the putchar override is from ST website:
	//https://community.st.com/t5/stm32-mcus/how-to-redirect-the-printf-function-to-a-uart-for-debug-messages/ta-p/49865
	int __io_putchar(int ch) {
		HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
		return ch;
	}
}

int main(int argc, char **argv) {
	bool goodbye = false;
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_RTC_Init();
	MX_USART1_UART_Init();
	MX_RNG_Init();
	//MX_SDIO_SD_Init();

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	std::cout << "STM32 main.cpp Startup\r\n";

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

	// apply default configuration
	Program::ApplyDefault();

	heap heap;
	rpnstack stack;

	while (!goodbye) {
		Program prog(stack, heap);
		string entry;
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
		//HAL_Delay(500);
		std::cout << ">>> ";

		entry = "";
		Input::InputStatus sts = Input(entry).status;
		//std::cout << "input status = " << int(sts) << "\r\n";
		std::cout << "-----------------------------\r\n";
		if (sts == Input::InputStatus::kOk) {
			//std::cout << "-input status OK\r\n";
			if (prog.Parse(entry) == kOk) {
				if (prog.Run() != kGoodbye)
					prog.ShowStack();
				else
					goodbye = true;
			}
		} else if (sts == Input::InputStatus::kCtrlc) {
			//std::cout << "-input status Ctrl+C\r\n";
			goodbye = true;
		} else {
			//std::cout << "-input status problem\r\n";
		}
		//std::cout << "goodbye = " << int(goodbye) << "\r\n";
		//HAL_Delay(5);
	}
	std::cout << "Done!";
}
