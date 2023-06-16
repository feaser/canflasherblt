/************************************************************************************//**
* \file         FreeRTOSHooks.c
* \brief        Source file that implements the FreeRTOS hook functions.
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

/****************************************************************************************
* Include files
****************************************************************************************/
#include "microtbx.h"                       /* MicroTBX                                */
#include "FreeRTOS.h"                       /* FreeRTOS                                */
#include "task.h"                           /* FreeRTOS tasks                          */
#include "stm32f3xx_ll_tim.h"               /* STM32 LL timer driver                   */
#include "stm32f3xx_ll_rcc.h"               /* STM32 LL RCC driver                     */


/************************************************************************************//**
** \brief     FreeRTOS hook function that gets called when memory allocation failed.
**
****************************************************************************************/
void vApplicationMallocFailedHook(void)
{
  /* Trigger an assertion for debugging purposes. */
  TBX_ASSERT(TBX_ERROR);
} /*** end of vApplicationMallocFailedHook ***/


/************************************************************************************//**
** \brief     FreeRTOS hook function that gets called when a stack overflow was detected.
** \param     xTask Handle of the task that has a stack overflow.
** \param     pcTaskName Name of the task that has a stack overflow.
**
****************************************************************************************/
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName)
{
  TBX_UNUSED_ARG(xTask);
  TBX_UNUSED_ARG(pcTaskName);

  /* Trigger an assertion for debugging purposes. */
  TBX_ASSERT(TBX_ERROR);
} /*** end of vApplicationStackOverflowHook ***/


#if ( configGENERATE_RUN_TIME_STATS == 1 )
/************************************************************************************//**
** \brief     FreeRTOS hook function that gets called to configure the timer used for
**            calculating run-time statistics. 
** \details   It is recommended to make the time base between 10 and 100 times faster
**            than the tick interrupt. The faster the time base the more accurate the
**            statistics will be but also the sooner the timer value will overflow. 
**            Note that run-time statistics for CPU usage only work until the counter
**            overflows. Therefore you really want to use a 32-bit counter. If no timer
**            with a 32-bit free running counter is avaialbe, then configure the timer
**            to generate an interrupt 10 times faster than the tick interrupt and
**            increment a 32-bit volatile counter variable there, which is then returned
**            in uxRunTimeStatsGetTimerCounter(), instead of the free running counter
**            value.
**
****************************************************************************************/
void vRunTimeStatsConfigureTimer(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = { 0 };
  LL_RCC_ClocksTypeDef rccClocks = { 0 };

  /* Initialize the 32-bit free running counter to run 10 times faster than the RTOS tick
   * interrupt.
   */
  LL_RCC_GetSystemClocksFreq(&rccClocks);
  TBX_ASSERT(rccClocks.PCLK1_Frequency != 0);
  TIM_InitStruct.Prescaler = (rccClocks.PCLK1_Frequency / (configTICK_RATE_HZ*10U)) - 1U;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 4294967295UL;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM2);
  LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM2);
  LL_TIM_EnableCounter(TIM2);
} /*** end of vRunTimeStatsConfigureTimer ***/
#endif

/*********************************** end of FreeRTOSHooks.c ****************************/
