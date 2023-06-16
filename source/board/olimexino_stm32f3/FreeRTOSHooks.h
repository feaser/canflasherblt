/************************************************************************************//**
* \file         FreeRTOSHooks.h
* \brief        Header file that implements the FreeRTOS hook functions.
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2023 by Feaser     www.feaser.com     All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
*
* SPDX-License-Identifier: MIT
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* \endinternal
****************************************************************************************/
#ifndef FREERTOSHOOKS_H
#define FREERTOSHOOKS_H

#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************************
* Include files
****************************************************************************************/
#include "stm32f3xx.h"


/****************************************************************************************
* Macro definitions
****************************************************************************************/
#if ( configGENERATE_RUN_TIME_STATS == 1 )
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()      vRunTimeStatsConfigureTimer()
#define portGET_RUN_TIME_COUNTER_VALUE()	            uxRunTimeStatsGetTimerCounter()
#endif


/****************************************************************************************
* Function prototypes
****************************************************************************************/
#if ( configGENERATE_RUN_TIME_STATS == 1 )
void vRunTimeStatsConfigureTimer(void);
static inline uint32_t uxRunTimeStatsGetTimerCounter(void)
{
  return (uint32_t)(READ_REG(TIM2->CNT));
}
#endif


#ifdef __cplusplus
}
#endif

#endif /* FREERTOSHOOKS_H */
/*********************************** end of FreeRTOSHooks.h ****************************/
