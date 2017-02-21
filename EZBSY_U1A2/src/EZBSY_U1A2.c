/******************************************************************************/
/** \file       EZBSY_U1A2.c
 *******************************************************************************
 *
 *  \brief      EZBSY exercise U1A2, Task creation, starvation
 *
 *  \author     wht4
 *
 *  \date       22.08.2011
 *
 *  \remark     Last Modification
 *               \li wht4, 24.01.2014, Created
 *               \li wht4, 06.01.2015, Migrated to FreeRTOS V8.0.0
 *               \li WBR1, 09.02.2017, minor optimizations
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              main
 *  functions  local:
 *              vAppTask1
 *              vAppTask2
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <carme.h>						/* CARME Module						  */
#include <carme_io1.h>                  /* CARMEIO1 Board Support Package     */

#include <stdio.h>                      /* Standard Input/Output              */
#include <stdlib.h>                     /* General Utilities                  */
#include <string.h>                     /* String handling                    */

#include <FreeRTOS.h>                   /* All freeRTOS headers               */
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include <memPoolService.h>

//----- Macros -----------------------------------------------------------------
#define STACKSIZE_TASK1        ( 256 )
#define STACKSIZE_TASK2        ( 256 )

#define PRIORITY_TASK1         ( 1 )
#define PRIORITY_TASK2         ( 2 )  // TODO: modify priority, low priority number
									  // denotes low priority task

//#define USE_vWAIT				// TODO: set this compiler switch to use CPU-wait
								// instead of vTastDelay

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
static void  vAppTask1(void *pvData);
static void  vAppTask2(void *pvData);

#ifdef USE_vWAIT
static void  vWait(void);
#endif

//----- Data -------------------------------------------------------------------

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    main
 ******************************************************************************/
/** \brief        Initialize GUI, BSP and OS
 *
 *  \type         global
 *
 *  \return       error code
 *
 ******************************************************************************/
int  main(void) {

	TaskHandle_t taskHandle;

    /* Ensure all priority bits are assigned as preemption priority bits. */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    CARME_IO1_Init();               // Initialize the CARMEIO1
    CARME_IO1_LED_Set(0, 0xFF);     // Reset led state

    xTaskCreate(vAppTask1,
                "Task1",
                STACKSIZE_TASK1,
                NULL,
                PRIORITY_TASK1,
                &taskHandle);

    xTaskCreate(vAppTask2,
                "Task2",
                STACKSIZE_TASK2,
                NULL,
                PRIORITY_TASK2,
                &taskHandle);

    vTaskStartScheduler();

    /* code never reached */
	for (;;) {
	}
	return 0;
}

/*******************************************************************************
 *  function :    AppTask1
 ******************************************************************************/
/** \brief        Toggle led 1-4
 *
 *  \type         local
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void vAppTask1(void *pvData) {

    uint8_t u8Led;
    uint8_t u8LedMask = 0x0F;

    while(1) {
        CARME_IO1_LED_Get(&u8Led);
        u8Led = u8Led ^ u8LedMask;
        CARME_IO1_LED_Set(u8Led, u8LedMask);

#ifdef USE_vWAIT
        vWait();
#else
        vTaskDelay(100);
#endif
    }
}

/*******************************************************************************
 *  function :    AppTask2
 ******************************************************************************/
/** \brief        Toggle led 5-8
 *
 *  \type         local
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void  vAppTask2(void *pvData) {

    uint8_t u8Led;
    uint8_t u8LedMask = 0xF0;

    while(1) {
        CARME_IO1_LED_Get(&u8Led);
        u8Led = u8Led ^ u8LedMask;
        CARME_IO1_LED_Set(u8Led, u8LedMask);

#ifdef USE_vWAIT
        vWait();
#else
        vTaskDelay(100);
#endif
    }
}


/*******************************************************************************
 *  function :    wait
 ******************************************************************************/
/** \brief        Wait some time using CPU
 *
 *  \type         local
 *
 *  \return       void
 *
 ******************************************************************************/
#ifdef USE_vWAIT
static void vWait(void) {

    uint32_t i, j;

    for (i=0; i<1000; i++) {
        for (j=0; j<2000; j++) {
        }
    }
}
#endif
