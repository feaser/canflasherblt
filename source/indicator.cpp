///**************************************************************************************
/// \file         indicator.cpp
/// \brief        Status indicator source file.
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
#include "indicator.hpp"


///**************************************************************************************
/// \brief     Indicator constructor.
/// \param     t_StatusLed Reference to the status LED instance.
///
///**************************************************************************************
Indicator::Indicator(Led& t_StatusLed)
  : m_StatusLed(t_StatusLed)
{
}


///**************************************************************************************
/// \brief     Update method that drives the class. Should be called periodically.
///
///**************************************************************************************
void Indicator::update()
{
  const TickType_t toggleTicks = cpp_freertos::Ticks::MsToTicks(500U);

  // Did the toggle time pass?
  TickType_t deltaTicks = cpp_freertos::Ticks::GetTicks() - m_LastToggleTicks;
  if (deltaTicks >= toggleTicks)
  {
    // Toggle the LED.
    m_StatusLed.toggle();
    // Update the last tick counter for the next interval detection.
    m_LastToggleTicks += toggleTicks;
  }
}

//********************************** end of indicator.cpp *******************************
