#ifndef LCDTASK_H_
#define LCDTASK_H_
/******************************************************************************/
/** \file       taskStatus.h
 *******************************************************************************
 *
 *  \brief      shows task status: stack usage, number of tasks running,
 *  			heap size
 *
 *  \author     ingmacmech
 *
 ******************************************************************************/
/*
 *  function    vStatusTask
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------

//----- Macros -----------------------------------------------------------------
#define Y_HEADERLINE    ( 1 )           /* pixel y-pos for headerline         */
#define Y_POTI          ( 100 )         /* pixel y-pos for counter text       */
#define X_POTI_TEXT     ( 16 )          /* pixel x-pos for counter text       */
#define X_POTI_VALUE    ( 80 )          /* pixel x-pos for cycle value        */

//----- Data types -------------------------------------------------------------


//----- Function prototypes ----------------------------------------------------
extern void vLCDTask(void *pvData);

//----- Data -------------------------------------------------------------------


#endif /* LCDTASK_H_ */
