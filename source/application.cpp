///**************************************************************************************
/// \file         application.cpp
/// \brief        Application source file.
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
#include "application.hpp"
#include "ticks.hpp"


///**************************************************************************************
/// \brief     Application constructor.
/// \param     t_Board Reference to the board instance, which forms the hardware
///            abstraction.
///
///**************************************************************************************
Application::Application(Board& t_Board)
  : cpp_freertos::Thread("AppThread", 100, 6),  m_Board(t_Board)
{
  // Start the thread.
  Start();
}


///**************************************************************************************
/// \brief     Application task function.
///
///**************************************************************************************
void Application::Run()
{
  TickType_t toggleTicks = cpp_freertos::Ticks::MsToTicks(500U);

  for (;;)
  {
    // Wait for the LED toggle time to elapse.
    Delay(toggleTicks);
    // Toggle the LED.
    m_Board.statusLed().toggle();
  }
}

//********************************** end of application.cpp *****************************
