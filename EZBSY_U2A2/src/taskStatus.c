/******************************************************************************/
/** \file       taskStatus.c
 *******************************************************************************
 *
 *  \brief      shows task status: stack usage, number of tasks running,
 *  			heap size
 *
 *  \author     ingmacmech
 *
 *  \date       13.03.2017
 *
 *  \remark     Last Modification
 *              13.03.2017		Created
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              vTaskStatus
 *  functions  local:
 *              .
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <lcd.h>                        /* GUI Library                        */
#include <carme_io1.h>                  /* CARMEIO1 Board Support Package     */

#include <stdio.h>                      /* Standard Input/Output              */
#include <stdlib.h>                     /* General Utilities                  */
#include <string.h>                     /* String handling                    */


#include <FreeRTOS.h>                   /* All freeRTOS headers               */
#include <task.h>
#include <queue.h>
#include <semphr.h>


#include "taskStatus.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------
uint16_t u16StackSize = 600;
uint16_t u16TaskNumber = 500;
uint16_t u16HeapSize = 100;
TaskHandle_t ledTaskHandle;

const char *pcNameLEDTask = "LED Task";
//----- Function prototypes ----------------------------------------------------

//----- Data -------------------------------------------------------------------

//----- Implementation ---------------------------------------------------------


/*******************************************************************************
 *  function :    taskStatus
 ******************************************************************************/
/** \brief        Thread for the taskStatus
 *
 *  \type         global
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
void  vTaskStatus(void *pvData) {
	/* local copies */
	UBaseType_t StackSize;
	UBaseType_t TaskNumber;
	UBaseType_t HeapSize;

	ledTaskHandle = xTaskGetHandle(pcNameLEDTask);
	//TaskHandle_t potiTaskHandle = xTaskGetHandle("Poti Task");
	//TaskHandle_t LCDTaskHandle = xTaskGetHandle("LCD Task");
	//TaskHandle_t statusTaskHandle = xTaskGetHandle("");
	for (;;) {

		StackSize = uxTaskGetStackHighWaterMark( ledTaskHandle );

		taskENTER_CRITICAL();
		u16StackSize = (uint16_t) StackSize;
		taskEXIT_CRITICAL();

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}
