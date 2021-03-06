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
#include <carme_io1.h>
#include <carme_io2.h>
#include <FreeRTOS.h>				/* FreeRTOS								*/
#include <task.h>					/* FreeRTOS tasks						*/
#include <queue.h>					/* FreeRTOS queues						*/
#include <semphr.h>					/* FreeRTOS semaphores					*/
#include <memPoolService.h>			/* Memory pool manager service			*/

/*----- Macros -------------------------------------------------------------*/
#define STACKSIZE_TASK_READPOTI 	(256)
#define STACKSIZE_TASK_SETLEDSPEED 	(256)

#define PRIORITY_TASK_READPOTI		(2)
#define PRIORITY_TASK_SETLEDSPEED 	(1)
/*----- Data types ---------------------------------------------------------*/

/*----- Function prototypes ------------------------------------------------*/


static void ReadPoti(void *pvargs);
static void SetLEDSpeed(void *pvargs);

/*----- Data ---------------------------------------------------------------*/
uint16_t u16LEDSpeed = 0;
/*----- Implementation -----------------------------------------------------*/
/**
 * @brief		main
 * @return		0 if success
 */
int main(void) {

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	CARME_IO1_Init();
	CARME_IO1_LED_Set(0x81, 0xFF);
	CARME_IO2_Init();


	xTaskCreate(ReadPoti, "Poti", STACKSIZE_TASK_READPOTI, NULL,
				PRIORITY_TASK_READPOTI, NULL);
	xTaskCreate(SetLEDSpeed, "LEDSpeed", STACKSIZE_TASK_SETLEDSPEED, NULL,
			PRIORITY_TASK_SETLEDSPEED, NULL);

	vTaskStartScheduler();
	for (;;) {
	}
	return 0;
}




/**
 * @brief		Read Potentiometer Value and store it to u16LEDSpeed.
 * @param[in]	pvargs		Not used
 */
static void ReadPoti(void *pvargs)
{
	uint16_t potiValue;					/* local copy of ledSpeed        	*/

		for (;;) {

	        /* Read poti value */
	        CARME_IO2_ADC_Get(CARME_IO2_ADC_PORT0, &potiValue);

	        /*  value between 100 and 999 */
	        if (potiValue < 100)   {
	        	potiValue = 100;
	        }
	        if (potiValue > 999)   {
	        	potiValue = 999;
	        }
			/* copy ledSpeed to global variable u16LEDSpeed, this is an access to a
			 * critical section, so disable interrupts while writing to the variable
			 */
			taskENTER_CRITICAL();	/* disable interrupts */
			u16LEDSpeed = potiValue; /* access critical section */
			taskEXIT_CRITICAL();	/* enable interrupts again */

	        vTaskDelay(100 / portTICK_RATE_MS);     /* Wait for 100ms */

			}
}
/**
 * @brief		Read Potentiometer Value and store it to u16LEDSpeed.
 * @param[in]	pvargs		Not used
 */
static void SetLEDSpeed(void *pvargs)
{
	uint8_t u8LED1to6;
	uint8_t u8Led;
	uint16_t ledSpeed;

	while(1)
	{
		taskENTER_CRITICAL();	/* disable interrupts */
		ledSpeed = u16LEDSpeed; /* access critical section */
		taskEXIT_CRITICAL();	/* enable interrupts again */

		CARME_IO1_LED_Get(&u8Led);
		u8LED1to6 = u8Led & 0x3F;
		u8LED1to6 <<= 1;
		if(u8LED1to6 == 0x40)
		{
			u8LED1to6 = 1;
		}
		CARME_IO1_LED_Set(u8LED1to6, 0x3F);
		vTaskDelay(ledSpeed);
	}

}
