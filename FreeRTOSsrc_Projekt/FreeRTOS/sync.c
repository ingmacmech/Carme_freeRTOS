/*****************************************************************************
 * \brief		FatFs synchronize functions.\n
 *				Add synchronize functions to the FatFs. This will add the
 *				reentrant feature to the module.
 *
 * \file		sync.c
 * \version		1.0
 * \date		2013-10-08
 * \author		rct1
 *
 * Copyright (c) 2013, Tobias Rueetschi. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Berner Fachhochschule BFH nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/*
 * functions global:
 *				ff_cre_syncobj
 *				ff_del_syncobj
 *				ff_req_grant
 *				ff_rel_grant
 * functions local:
 *				.
 *
 ****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*----- Header-Files -------------------------------------------------------*/
#include <FreeRTOS.h>				/* FreeRTOS main include				*/
#include <semphr.h>					/* FreeRTOS semaphore functionality		*/

#define _FS_REENTRANT	1			/**< Define as in ffconf.h !!!			*/

#if _FS_REENTRANT

//----- Macros ---------------------------------------------------------------
#define	_SYNC_t			void *

//----- Data types -----------------------------------------------------------
typedef unsigned char	BYTE;

//----- Function prototypes --------------------------------------------------

//----- Data -----------------------------------------------------------------

//----- Implementation -------------------------------------------------------
/**
 *****************************************************************************
 * \brief		Create a sync object. In freeRTOS we use a mutex for this.
 *
 * \param[in]	vol not used.
 * \param[in]	A pointer to the mutex.
 *
 * \return		0 if mutex has been created, else !0.
 *****************************************************************************
 */
int ff_cre_syncobj(BYTE vol, _SYNC_t* mutex) {

	vSemaphoreCreateBinary(*mutex);

	return (*mutex != NULL);
}

/**
 *****************************************************************************
 * \brief		Delete the sync object.
 *
 * \param[in]	The mutex
 *
 * \return		0 if mutex has been deleted.
 *****************************************************************************
 */
int ff_del_syncobj(_SYNC_t mutex) {

	vSemaphoreDelete(mutex);

	return (!0);
}

/**
 *****************************************************************************
 * \brief		Request the access to the mutex.
 *
 * \param[in]	The mutex
 *
 * \return		0 if access is granted, else !0.
 *****************************************************************************
 */
int ff_req_grant(_SYNC_t mutex) {

	return (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE);
}

/**
 *****************************************************************************
 * \brief		Release the access.
 *
 * \param[in]	The mutex
 *****************************************************************************
 */
void ff_rel_grant(_SYNC_t mutex) {

	xSemaphoreGive(mutex);
}
#endif /* _FS_REENTRANT */
