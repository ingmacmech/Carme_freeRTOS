/**
 *****************************************************************************
 * @defgroup	Project
 * @brief		Project related code
 * @{
 *
 * @file		main.c
 * @version		1.0
 * @date		2013-11-22
 * @author		rct1
 *
 * @brief		main.c FreeRTOS template
 *
 *****************************************************************************
 * @copyright
 * @{
 * Copyright &copy; 2013, Bern University of Applied Sciences.
 * All rights reserved.
 *
 * ##### GNU GENERAL PUBLIC LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 * @}
 *****************************************************************************
 */

/*----- Header-Files -------------------------------------------------------*/
#include <stdio.h>					/* Standard input and output			*/
#include <stm32f4xx.h>				/* Processor STM32F407IG				*/
#include <carme.h>					/* CARME Module							*/
#include <uart.h>					/* CARME BSP UART port					*/

#include "../FreeRTOS/FreeRTOS.h"	/* FreeRTOS								*/
#include "../FreeRTOS/task.h"		/* FreeRTOS tasks						*/
#include "../FreeRTOS/queue.h"		/* FreeRTOS queues						*/
#include "../FreeRTOS/semphr.h"		/* FreeRTOS semaphores					*/
#include "../FreeRTOS/memPoolService.h"	/* Memory pool manager service		*/

/*----- Macros -------------------------------------------------------------*/

/*----- Data types ---------------------------------------------------------*/

/*----- Function prototypes ------------------------------------------------*/
static void GreenLEDtask(void *pvargs);
static void Serialtask(void *pvargs);

/*----- Data ---------------------------------------------------------------*/

/*----- Implementation -----------------------------------------------------*/
/**
 * @brief		main
 * @return		0 if success
 */
int main(void) {

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	xTaskCreate(GreenLEDtask, "Blinky", 200U, NULL, 4U, NULL);
	xTaskCreate(Serialtask, "Serial", 1024U, NULL, 4U, NULL);

	vTaskStartScheduler();
	for (;;) {
	}
	return 0;
}

/**
 * @brief		Blink the green LED on the CARME Module every second.
 * @param[in]	pvargs		Not used
 */
static void GreenLEDtask(void *pvargs) {

	portTickType xLastWakeTime = xTaskGetTickCount();

	for (;;) {

		CARME_LED_Green_Set();
		vTaskDelayUntil(&xLastWakeTime, 50U / portTICK_RATE_MS);
		CARME_LED_Green_Reset();
		vTaskDelayUntil(&xLastWakeTime, 950U / portTICK_RATE_MS);
	}
}

/**
 * @brief		Print welcome string to the standard output.
 * @param[in]	pvargs		Not used
 */
static void Serialtask(void *pvargs) {

	USART_InitTypeDef USART_InitStruct;
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = 115200U;
	CARME_UART_Init(CARME_UART0, &USART_InitStruct);
	vTaskDelay(5U / portTICK_RATE_MS);
	printf("\033c");		/* Reset to initial state	*/
	printf("\033[2J");		/* Clear screen				*/
	printf("\033[?25l");	/* Cursor off				*/
	vTaskDelay(5 / portTICK_RATE_MS);

	printf("Welcome to CARME-M4 FreeRTOS\r\n");
	vTaskDelay(2000U / portTICK_RATE_MS);

	for (;;) {
		vTaskDelay(1000U);
	}
}

/**
 * @}
 */
