///**************************************************************************************
/// \file         main.cpp
/// \brief        Program entry point source file.
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
#include <memory>
#include "microtbx.h"
#include "application.hpp"
#include "hardwareboard.hpp"
#include "thread.hpp"


//***************************************************************************************
// Local data declarations
//***************************************************************************************
namespace
{
  // Declare the board instance globally in the anonymous namespace. This way it is only
  // accessible within this source file and the constructor runs before main().
  // Note that its type is the hardware specific version of the board class and not the
  // hardware independent Board interface.
  HardwareBoard board;
}


///**************************************************************************************
/// \brief     This is the entry point for the software application and is called
///            by the reset interrupt vector after the C-startup routines executed.
/// \return    Program exit code.
///
///**************************************************************************************
int main(void)
{
  // Create the application instance on the heap. No need to unnecessarily burden the
  // stack with it. That way the stack can stack small, since it's only used until
  // the RTOS starts, without having to worry about stack overflows in case the 
  // application object expands and requires more RAM data.
  //
  // Note that this polymorphs the hardware specific board instance into the generic
  // hardware independent one. This realizes the hardware abstraction. The application
  // class is completely hardware independent and can be reused on different boards. 
  // Whenever it does need hardware access, it does so by accessing its board member.
  auto app = std::make_unique<Application>(board);

  // Hand over control to the RTOS by starting the scheduler.
  cpp_freertos::Thread::StartScheduler();

  // Program should never get here.
  TBX_ASSERT(TBX_FALSE);

  // Set program exit code.
  return 0;
}

//********************************** end of main.cpp ************************************
