/******************************************************************************/
/** \file       LCDTask.c
 *******************************************************************************
 *
 *  \brief      manages the LCD
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


#include "LCDTask.h"
#include "potiTask.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------
static const char* pcHello = "Hello freeRTOS";  /* Welcome text               */
static const char* pcPotiText = "Speed = ";     /* Text to display value      */
//----- Function prototypes ----------------------------------------------------

//----- Data -------------------------------------------------------------------

//----- Implementation ---------------------------------------------------------


/*******************************************************************************
 *  function :    LCDTask
 ******************************************************************************/
/** \brief        Thread for the LCDTask
 *
 *  \type         global
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
void  vLCDTask(void *pvData) {

	char    cBuffer[12];
	uint16_t ledSpeed;					/* local copy of ledSpeed        	*/

	/* Initialize the Display and display static text */
	    LCD_Init();
	    LCD_SetFont(&font_8x16B);
	    LCD_DisplayStringCenterLine(Y_HEADERLINE, pcHello);
	    LCD_SetFont(&font_8x13);
	    LCD_DisplayStringXY(X_POTI_TEXT, Y_POTI, pcPotiText);


	for (;;) {

		taskENTER_CRITICAL();	/* disable interrupts */
		ledSpeed = u16LEDSpeed; /* access critical section */
		taskEXIT_CRITICAL();	/* enable interrupts again */

		/* display speed new one */
		sprintf(cBuffer, "%d", (int) ledSpeed);
		LCD_DisplayStringXY(X_POTI_VALUE, Y_POTI, "     ");
		LCD_DisplayStringXY(X_POTI_VALUE, Y_POTI, cBuffer);

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}
