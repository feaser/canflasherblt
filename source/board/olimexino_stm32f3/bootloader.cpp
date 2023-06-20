///**************************************************************************************
/// \file         bootloader.cpp
/// \brief        OpenBLT bootloader interaction source file.
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
#include "bootloader.hpp"
#include "stm32f3xx.h"


///**************************************************************************************
/// \brief     Determine if a bootloader is present on the system.
/// \return    TBX_TRUE if a bootloader is present, TBX_FALSE otherwise.
///
///**************************************************************************************
uint8_t Bootloader::detectLoader()
{
  uint8_t result = TBX_FALSE;

  // If the OpenBLT bootloader is present on this system, then it will have moved the
  // vector base address forward to the begin of this firmware, right before it started
  // this firmware. This means that if the vector base address is not the same as the
  // start of flash memory, then the OpenBLT bootloader is present on this system.
  if (SCB->VTOR != FLASH_BASE)
  {
    // Update the result to indicate that a bootloader is present on this system.
    result = TBX_TRUE;
  }
  // Give the result back to the caller.
  return result;
}


///**************************************************************************************
/// \brief     Activate the bootloader.
///
///**************************************************************************************
void Bootloader::activateLoader()
{
  // Activate the bootloader by performing a software reset.
  NVIC_SystemReset();
}

//********************************** end of bootloader.cpp ******************************
