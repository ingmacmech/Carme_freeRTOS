/******************************************************************************/
/** \file       EZBSY_U1A1.c
 *******************************************************************************
 *
 *  \brief      EZBSY exercise U1A1, Building a cyclical OS using a table
 *
 *  \author     Roger Weber, wht4
 *
 *  \date       22.08.2011
 *
 *  \remark     Last Modification
 *               \li WBR1, 07.10.2005   New BSP Versions used
 * 			     \li WBR1, 13.12.2005   Code for blinking LED's optimized
 * 			     \li WBR1, 18.09.2006   Comments in doxygen format
 * 			     \li SCE2, 14.06.2007   Hardwarechange from Cobra to CARME-Kit
 * 			     \li WBR1, 13.02.2008   Comments added, blinky modified
 *               \li wht4, 23.08.2011   Adapted because of consistency reason
 *               \li wht4, 24.01.2014   Adapted to CARME-M4
 *               \li WBR1, 09.02.2017   minor optimizations
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              main
 *  functions  local:
 *              vApplication1
 *              vApplication2
 *              vApplication3
 *              vApplication4
 *              vWait
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include <lcd.h>                        /* GUI Library                        */
#include <carme.h>						/* CARME Board Support Package        */
#include <carme_io1.h>                  /* CARMEIO1 Board Support Package     */
#include <carme_io2.h>                  /* CARMEIO2 Board Support Package     */

#include <stdio.h>                      /* Standard Input/Output              */
#include <stdlib.h>                     /* General Utilities                  */
#include <string.h>                     /* String handling                    */

//----- Macros -----------------------------------------------------------------
#define LAST_ENTRY    ( 0 )     /* Last entry for table_entry table[]         */
#define CYCLE_RESET   ( 6 )     /* Number of cycles until cycle-counter reset */

#define Y_HEADERLINE  ( 1 )     /* Pixel y-pos for headerline                 */
#define Y_VALUE       ( 100 )   /* Pixel y-pos for value                      */
#define Y_INSTR1      ( 180 )   /* Pixel y-pos for instruction1 text          */
#define Y_INSTR2      ( 200 )   /* Pixel y-pos for instruction2 text          */
#define X_BORDER      ( 18 )    /* Pixel x-pos for border                     */

#define BUTTON_T0     ( 0x01 )  /* Value returned if Button T0 is pushed      */
#define BUTTON_T1     ( 0x02 )  /* Value returned if Button T1 is pushed      */
#define BUTTON_T2     ( 0x04 )  /* Value returned if Button T2 is pushed      */
#define BUTTON_T3     ( 0x08 )  /* Value returned if Button T3 is pushed      */

//----- Data types -------------------------------------------------------------

/* struct describing a table entry for cyclical OS */
typedef struct   {
    uint32_t u32FirstActivation;    /* Cycle when action is called first time */
    uint32_t u32CyclicActivation;   /* Periodic cycle */
    void     (*pfFunction)(void*);  /* Function pointer to action */
} TableEntry;

//----- Function prototypes ----------------------------------------------------
static void  vApplication1(void *pvDdata);
static void  vApplication2(void *pvDdata);
static void  vApplication3(void *pvDdata);
static void  vApplication4(void *pvDdata);
static void  vWait(void);

//----- Data -------------------------------------------------------------------
static uint8_t  u8ButtonData;	    /* Holds the value of the buttons  */
static int8_t   s8DisplayValue;     /* Holds the displayed value */

/* Table defining cyclical OS */
TableEntry sTableEntry[] = {
    {0, 3, vApplication1}, // erste pos: bei welchem durchgang staren\ zweite Pos: welchem durchgang ausführen\ drite pos: funktion pointer
    {0, 3, vApplication2},
    {0, 1, vApplication3},
    {0, 1, vApplication4},
    {0, 0, LAST_ENTRY}
};

/* Text to display */
static const char* pcHello = "EZBSY U1A1: Hello cyclical OS";/* Welcome text              */
static const char* pcValueText = "Value = %d";   /* Text to display value     */
static const char* pcInstructionLine1 = "Button 0 increment value";
static const char* pcInstructionLine2 = "Button 1 decrement value";

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

    static uint32_t u32Cycle = 0;         /* Counts number of cycles */
    uint8_t         u8Task;

    /* Initialize board: CPU, LED's, and GUI */
    CARME_IO1_Init();
    CARME_IO1_LED_Set(0x81, 0xFF); // Turn on LED 1 and 8
    CARME_IO2_Init();
    LCD_Init();
    LCD_SetFont(&font_8x16B);
    LCD_DisplayStringCenterLine(Y_HEADERLINE, pcHello);
    LCD_SetFont(&font_8x13);
    LCD_DisplayStringXY(X_BORDER, Y_INSTR1, pcInstructionLine1);
    LCD_DisplayStringXY(X_BORDER, Y_INSTR2, pcInstructionLine2);

    /* Endless loop */
    while (1) {
        u8Task = 0;    /* Start with first table entry */

        /* Process table and call task if necessary */
        /* Still task in table?                     */
        while (sTableEntry[u8Task].pfFunction != LAST_ENTRY) {
        	/* Modulo 0 yields undefined result! Prevent this */
        	if(sTableEntry[u8Task].u32CyclicActivation != 0) {
				/* Call  task? */
				if (((u32Cycle % sTableEntry[u8Task].u32CyclicActivation) -
						(sTableEntry[u8Task].u32FirstActivation)) == 0)   {

					(*sTableEntry[u8Task].pfFunction)((void*)0);
				}
        	}
        	u8Task++;  /* Prepare next task */
        }

        /* Prepare next cycle and check if counter has to be reset */
        u32Cycle++;
        if (u32Cycle >= CYCLE_RESET) {
            u32Cycle = 0;
        }

        /* Wait some time before processing next loop */
        vWait();
    }
    return(0);		// Code never reached because of endless loop
}

/*******************************************************************************
 *  function :    Application1
 ******************************************************************************/
/** \brief        Doing something with LED's L1 to L6
 *
 *  \type         local
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void vApplication1(void *pvData) {

    uint8_t     u8LED1to6;
    uint8_t     u8Led;

    /* Get LED state, shift left, check overflow and show again */
    CARME_IO1_LED_Get(&u8Led);
    u8LED1to6 = u8Led & 0x3F;                /* Mask lower 6 bits */
    u8LED1to6 <<= 1;                         /* Shift LED left one position */
    if (u8LED1to6 == 0x40)   {
        u8LED1to6 = 1;                       /* LED 7 on?, set LED1 */
    }
    CARME_IO1_LED_Set(u8LED1to6, 0x3F);
}

/*******************************************************************************
 *  function :    Application2
 ******************************************************************************/
/** \brief        Doing something with LED's L7 and L8
 *
 *  \type         local
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void  vApplication2 (void *pvData) {

    uint8_t     u8Led;

    /* Get LED state, toggle LED7 and LED8 and show again */
    CARME_IO1_LED_Get(&u8Led);
    u8Led = (u8Led ^ 0xC0);
    CARME_IO1_LED_Set(u8Led, 0xC0);
}

/*******************************************************************************
 *  function :    Application3
 ******************************************************************************/
/** \brief        Get the Buttons value
 *
 *  \type         local
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void  vApplication3 (void *pvData) {

    /* Just poll the state of the four Buttons T0-T3 */
    CARME_IO1_BUTTON_Get(&u8ButtonData);
}

/*******************************************************************************
 *  function :    Application4
 ******************************************************************************/
/** \brief        Set the u8ButtonData value and display it on the screen
 *
 *  \type         local
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void  vApplication4 (void *pvData) {

    char cBuffer[64];

    /* Increment s8DisplayValue if T0 is pressed or decrement if T1 is pressed */
    switch (u8ButtonData) {
    case BUTTON_T0:  /* Button T0 is pushed */
        s8DisplayValue++;
        break;
    case BUTTON_T1:  /* Button T1 is pushed */
        s8DisplayValue--;
        break;
    case BUTTON_T2:  /* Button T2 is pushed */
        break;
    case BUTTON_T3:  /* Button T3 is pushed */
        break;
    default:
        break;
    }
    /* Wrap around s8DisplayValue */
    if (s8DisplayValue > 9) {
        s8DisplayValue = 0;
    }
    if (s8DisplayValue < 0) {
        s8DisplayValue = 9;
    }
    /* Display s8DisplayValue */
    sprintf(cBuffer, pcValueText, (int) s8DisplayValue);
    LCD_DisplayStringXY(X_BORDER, Y_VALUE, cBuffer);

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
static void vWait(void) {

    uint32_t i, j;

    for (i=0; i<1000; i++) {
        for (j=0; j<2000; j++) {
        }
    }
}
