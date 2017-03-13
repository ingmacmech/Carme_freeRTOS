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
#include <lcd.h>                        /* GUI Library                       */
#include <carme_io1.h>                  /* CARMEIO1 Board Support Package     */
#include <carme_io2.h>                  /* CARMEIO2 Board Support Package     */

#include <FreeRTOS.h>				/* FreeRTOS								*/
#include <task.h>					/* FreeRTOS tasks						*/
#include <queue.h>					/* FreeRTOS queues						*/
#include <semphr.h>					/* FreeRTOS semaphores					*/
#include <timers.h>
#include <memPoolService.h>			/* Memory pool manager service			*/

#include "potiTask.h"
#include "ledTask.h"
#include "taskStatus.h"

/*----- Macros -------------------------------------------------------------*/
#define PRIORITY_LEDTASK    ( 2 )      /* Priority of LED Task                */
#define PRIORITY_POTITASK   ( 2 )      /* Priority of Poti Task               */

#define STACKSIZE_LEDTASK   ( 256 )    /* Stacksize in words of LED Task      */
#define STACKSIZE_POTITASK  ( 64 )    /* Stacksize in words of Poti Task     */

/*----- Data types ---------------------------------------------------------*/

/*----- Function prototypes ------------------------------------------------*/
static void vCreateTasks(void);

/*----- Data ---------------------------------------------------------------*/

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

	vCreateTasks();

	vTaskStartScheduler();
	for (;;) {
	}
	return 0;
}

/*******************************************************************************
 *  function :    vCreateTasks
 ******************************************************************************/
/** \brief        Create all application task
 *
 *  \type         local
 *
 *  \return       error code
 *
 ******************************************************************************/
static void vCreateTasks(void)  {
    /* Create LED Task */
    xTaskCreate(vLEDTask,
                "LED Task",
                STACKSIZE_LEDTASK,
                NULL,
                PRIORITY_LEDTASK,
                NULL);
    /* Create Poti Task */
    xTaskCreate(vPotiTask,
                "Poti Task",
                STACKSIZE_POTITASK,
                NULL,
                PRIORITY_POTITASK,
                NULL);
}
