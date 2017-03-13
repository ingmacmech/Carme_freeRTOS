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

#define Y_TASK 		    ( 120 )         /* pixel y-pos for number of tasks    */
#define X_TASK_TEXT     ( 16 )			/* pixel x-pos for text of tasks      */
#define X_TASK_NUMBER	( 180 )			/* pixel x-pos for value 			  */

#define Y_STACK 		( 140 )			/* pixel y-pos for stack size		  */
#define X_STACK_TEXT    ( 16  )			/* pixel x-pos for stack text		  */
#define X_STACK_NUMBER  ( 130 )			/* pixel x-pos for stack value		  */

#define Y_HEAP			( 160 )			/* pixel y-pos for heap size		  */
#define X_HEAP_TEXT		( 16 )			/* pixel x-pos for heap text		  */
#define X_HEAP_NUMBER   ( 200 )			/* pixel x-pos for heap value		  */

//----- Data types -------------------------------------------------------------


//----- Function prototypes ----------------------------------------------------
extern void vLCDTask(void *pvData);

//----- Data -------------------------------------------------------------------


#endif /* LCDTASK_H_ */
