/******************************************************************************/
/** \file       ledTask.c
 *******************************************************************************
 *
 *  \brief      LED-chaser with the speed of the global variable u16LEDSpeed
 *  			[100,999]ms.
 *
 *  \author     wht4
 *
 *  \date       23.08.2011
 *
 *  \remark     Last Modification
 *               \li wht4, 23.08.2011, Created
 *               \li wht4, 24.01.2014, Adapted to CARME-M4
 *               \li WBR1, 13.02.2017, minor optimizations
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              vLEDTask
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


#include "potiTask.h"

//----- Macros -----------------------------------------------------------------


//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

//----- Data -------------------------------------------------------------------


//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    ledTask
 ******************************************************************************/
/** \brief        Thread for the led-chaser with speed u32LEDSpeed [100,999] ms
 *
 *  \type         global
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
void  vLEDTask(void *pvData) {

    uint8_t u8Led = 1;              	/* LED1 is the first to turn on 	*/
    uint16_t ledSpeed;					/* local copy of ledSpeed        	*/



	for (;;) {
		/* copy global variable u16LEDSpeed, this is an access to a
		 * critical section, so disable interrupts while reading the variable
		 */
		taskENTER_CRITICAL();	/* disable interrupts */
		ledSpeed = u16LEDSpeed; /* access critical section */
		taskEXIT_CRITICAL();	/* enable interrupts again */

        CARME_IO1_LED_Set(u8Led, 0xff);
        u8Led <<= 1;          /* Shift LED left 1 position */
        if (u8Led == 0)   {   /* Turn around, first LED again */
            u8Led = 1;
        }

        /* Time delay depends on speed */
        vTaskDelay(ledSpeed / portTICK_RATE_MS);
    }
}
