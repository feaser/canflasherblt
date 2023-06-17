///**************************************************************************************
/// \file         rttlogger.cpp
/// \brief        Segger RTT based event logger source file.
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
#include <array>
#include "microtbx.h"
#include "SEGGER_RTT.h"
#include "rttlogger.hpp"
#include "ticks.hpp"


///**************************************************************************************
/// \brief     Global getter for the logger.
/// \details   This function uses polymorphism to realize the actual hardware abstration
///            layer for the logger. The application can call logger() to access the
///            hardware dependent logger functionality. With other words, this function
///            is the one that glues the hardware independent logger interface, to the
///            hardware dependent logger implementation.
///
///            Note that this function uses lazy initialization. The first time the
///            function is called, the logger object is created and remains valid for the
///            lifetime of the program. The benefit of this is that the object only gets
///            jreated when actually used. Just keep in mind the side-effect that its
///            constructor only runs when used for the first time.
///
///**************************************************************************************
Logger& logger()
{
  static RttLogger rttLogger;
  return rttLogger;
}


///**************************************************************************************
/// \brief     Event logger constructor. 
///
///**************************************************************************************
RttLogger::RttLogger()
  : Logger()
{
  // Configure UP-buffer 0 to not block when data written to it does not fit. Discard the
  // data that does not fit. Make sure to never use SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL
  // here, because then the system hangs if the buffer is full. This can happen when no
  // RTT terminal is attached through the debugger, to read out the data.
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
}


///**************************************************************************************
/// \brief     Logs the event with the specified severity level. Basically a printf()
///            like function with an additional severity level parameter.
/// \param     t_Level Severity level of the log event.
/// \param     t_Fmt Format string.
/// \param     t_ParamList Pointer to the list of arguments for the format string.
///
///**************************************************************************************
void RttLogger::log(Logger::Level t_Level, const char t_Fmt[], va_list * t_ParamList)
{
  static constexpr std::array<char[8], 3> colorLevel = 
  {
    RTT_CTRL_TEXT_GREEN, 
    RTT_CTRL_TEXT_YELLOW, 
    RTT_CTRL_TEXT_RED
  };

  // Verify the parameters.
  TBX_ASSERT((t_Level<=Logger::ERROR) && (t_Fmt != nullptr) && (t_ParamList != nullptr));

  // Only continue with valid parameters.
  if ((t_Level <= Logger::ERROR) && (t_Fmt != nullptr) && (t_ParamList != nullptr))
  {
    // Get the system time in seconds.
    TickType_t currentTicks = cpp_freertos::Ticks::GetTicks();
    TickType_t currentMillis = cpp_freertos::Ticks::TicksToMs(currentTicks);
    uint32_t sysTimeSec = currentMillis / 1000UL;
    // Convert to hours, minutes and seconds.
    int hours = (sysTimeSec / 3600) % 24;
    int minutes = (sysTimeSec / 60) % 60;
    int seconds = sysTimeSec % 60;
    // Print the timestamp.
    SEGGER_RTT_printf(0, "[%02d:%02d:%02d] ", hours, minutes, seconds);
    // Print the formatted string in a color based on the severity level.
    SEGGER_RTT_WriteString(0, colorLevel[t_Level]);
    SEGGER_RTT_vprintf(0, t_Fmt, t_ParamList);
    // Add trailing new line.
    SEGGER_RTT_WriteString(0, RTT_CTRL_RESET "\n");
  }
}


//********************************** end of rttlogger.cpp *******************************
