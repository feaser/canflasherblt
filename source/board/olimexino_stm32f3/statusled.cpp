///**************************************************************************************
/// \file         statusled.cpp
/// \brief        Status LED source file.
/// \internal
///--------------------------------------------------------------------------------------
///                          C O P Y R I G H T
///--------------------------------------------------------------------------------------
///   Copyright (c) 2023 by Feaser     www.feaser.com     All rights reserved
///
///--------------------------------------------------------------------------------------
///                            L I C E N S E
///--------------------------------------------------------------------------------------
///
/// SPDX-License-Identifier: MIT
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///
/// \endinternal
///**************************************************************************************

//***************************************************************************************
// Include files
//***************************************************************************************
#include "statusled.hpp"
#include "stm32f3xx_ll_gpio.h"


///**************************************************************************************
/// \brief     Status LED constructor. 
///
///**************************************************************************************
StatusLed::StatusLed()
  : Led()
{
  LL_GPIO_InitTypeDef GPIO_InitStruct{ };

  // Configure GPIO pin PA5 as a digital output and turn the LED off.
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


///**************************************************************************************
/// \brief     Status LED destructor.
///
///**************************************************************************************
StatusLed::~StatusLed()
{
  // Make sure the LED is off.
  off();
}


///**************************************************************************************
/// \brief     Sets the state of the LED.
/// \param     t_State TBX_ON if the LED should be turned on, TBX_OFF for off.
///
///**************************************************************************************
void StatusLed::set(uint8_t t_State)
{
  // Should the LED be turned on?
  if (t_State == TBX_ON)
  {
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
  }
  // Turn the LED off.
  else
  {
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
  }
}

//********************************** end of statusled.cpp *******************************
