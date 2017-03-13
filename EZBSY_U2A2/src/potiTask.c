/******************************************************************************/
/** \file       potiTask.c
 *******************************************************************************
 *
 *  \brief      Poll every second the value of the poti and store the value in
 *              the global variable u16LEDSpeed [100,999].
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
 *              vPotiTask
 *  functions  local:
 *              .
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <carme.h>
#include <carme_io2.h>                  /* CARMEIO2 Board Support Package     */

#include <FreeRTOS.h>                   /* All freeRTOS headers               */
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include "potiTask.h"


//----- Macros -----------------------------------------------------------------
#define SIMULATE_LOAD   ( 0 )

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
#if (SIMULATE_LOAD > 0)
static void vWork(void);
#endif

//----- Data -------------------------------------------------------------------
uint16_t u16LEDSpeed = 500;       /*Speed of the led chaser = poti value      */

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    vPotiTask
 ******************************************************************************/
/** \brief        Read every second the value of the potentiometer and store
 *                it in global variable u16LEDSpeed.
 *
 *  \type         global
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
void vPotiTask(void *pvData)   {
    uint16_t potiValue;					/* local copy of ledSpeed        	*/

	for (;;) {
        /* Read poti value */
        CARME_IO2_ADC_Get(CARME_IO2_ADC_PORT0, &potiValue);

        /*  Speed between 100ms and 999ms */
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

#if (SIMULATE_LOAD > 0)
        vWork();
#endif // #if (SIMULATE_LOAD > 0)

        vTaskDelay(100 / portTICK_RATE_MS);     /* Wait for 100ms */
    }
}


/*******************************************************************************
 *  function :    vWork
 ******************************************************************************/
/** \brief        Wait some time using CPU
 *
 *  \type         local
 *
 *  \return       void
 *
 ******************************************************************************/
#if (SIMULATE_LOAD > 0)
static void vWork(void) {

    uint32_t i, j;

    for (i=0; i<2000; i++) {
        for (j=0; j<2000; j++) {
        }
    }
}
#endif // #if (SIMULATE_LOAD > 0)
