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
#include <lcd.h>
#include <carme_io1.h>
#include <carme_io2.h>

#include <FreeRTOS.h>				/* FreeRTOS								*/
#include <task.h>					/* FreeRTOS tasks						*/
#include <queue.h>					/* FreeRTOS queues						*/
#include <semphr.h>					/* FreeRTOS semaphores					*/
#include <memPoolService.h>			/* Memory pool manager service			*/

/*----- Macros -------------------------------------------------------------*/
#define STACKSIZE_TASK_SWITCH (256)
#define STACKSIZE_TASK_BUTTON (256)
#define STACKSIZE_TASK_LCD (1024)

#define PRIORITY_TASK_SWITCH (1)
#define PRIORITY_TASK_BUTTON (1)
#define PRIORYTY_TASK_LCD (2)

#define Y_HEADERLINE (1)
#define Y_VALUE (100)
#define Y_INSTR1 (180)
#define Y_INSTR2 (200)
#define X_BORDER (18)

#define BUTTON_T0 (0x01)
#define BUTTON_T1 (0x02)
#define BUTTON_T2 (0x03)
#define BUTTON_T3 (0x04)



/*----- Data types ---------------------------------------------------------*/

/*----- Function prototypes ------------------------------------------------*/
static void GetSwitchStatus(void *pvargs);
static void GetButtonStatus(void *pvargs);
static void ShowStatusLCD(void *pvargs);

/*----- Data ---------------------------------------------------------------*/
uint8_t u8SwitchState;
uint8_t u8ButtonState;
static const char *pcHello = "EZBSY U1A4";
static const char *pcButtonsValueText = "Buttons = %d";
static const char *pcSwitchesValueText = "Switches = %d";

/*----- Implementation -----------------------------------------------------*/
/**
 * @brief		main
 * @return		0 if success
 */
int main(void) {

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	CARME_IO1_Init();
	CARME_IO2_Init();
	CARME_Init();
	CARME_IO1_LED_Set(0x01, 0xFF);

	LCD_Init();
	LCD_SetFont(&font_8x16B);
	LCD_DisplayStringCenterLine(Y_HEADERLINE, pcHello);
	LCD_SetFont(&font_8x13);



	//xTaskCreate(GreenLEDtask, "Blinky", 200U, NULL, 4U, NULL);
	//xTaskCreate(Serialtask, "Serial", 1024U, NULL, 4U, NULL);
	xTaskCreate(GetSwitchStatus,
				"Switch",
				STACKSIZE_TASK_SWITCH,
				NULL,
				PRIORITY_TASK_SWITCH,
				NULL);

	xTaskCreate(GetButtonStatus,
				"Button",
				STACKSIZE_TASK_BUTTON,
				NULL,
				PRIORITY_TASK_SWITCH,
				NULL);

	xTaskCreate(ShowStatusLCD,
				"LCD",
				STACKSIZE_TASK_LCD,
				NULL,
				PRIORYTY_TASK_LCD,
				NULL);

	vTaskStartScheduler();
	for (;;) {
	}
	return 0;
}


/**
 * @brief		Read switches.
 * @param[in]	pvargs		Not used
 */
static void GetSwitchStatus(void *pvargs)
{
	for(;;)
	{
		CARME_IO1_SWITCH_Get(&u8SwitchState);
		vTaskDelay(100U / portTICK_RATE_MS);
	}

}

/**
 * @brief		Read buttons.
 * @param[in]	pvargs		Not used
 */
static void GetButtonStatus(void *pvargs)
{

	for(;;)
	{
		CARME_IO1_BUTTON_Get(&u8ButtonState);
		vTaskDelay(100U / portTICK_RATE_MS);
	}
}

/**
 * @brief		Print button and switch status to LCD.
 * @param[in]	pvargs		Not used
 */
static void ShowStatusLCD(void *pvargs)
{
	char cBuffer[64];

	for(;;)
	{
		sprintf(cBuffer, pcButtonsValueText, (int) u8ButtonState);
		LCD_DisplayStringCenterLine(10,cBuffer);

		LCD_DisplayStringCenterLine(10, "              ");
		LCD_DisplayStringCenterLine(10, cBuffer);

		sprintf(cBuffer, pcSwitchesValueText, (int) u8SwitchState);
		LCD_DisplayStringCenterLine(11, cBuffer);

		LCD_DisplayStringCenterLine(11, "              ");
		LCD_DisplayStringCenterLine(11, cBuffer);


		vTaskDelay(100U / portTICK_RATE_MS);
	}
}
