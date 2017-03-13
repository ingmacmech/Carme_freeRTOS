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


	for (;;) {

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}
