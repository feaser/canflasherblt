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
#include "logger.hpp"
#include "ticks.hpp"


///**************************************************************************************
/// \brief     Application constructor.
/// \param     t_Board Reference to the board instance, which forms the hardware
///            abstraction.
///
///**************************************************************************************
Application::Application(Board& t_Board)
  : cpp_freertos::Thread("AppThread", configMINIMAL_STACK_SIZE, 6),
    m_Board(t_Board), 
    m_Indicator(t_Board.statusLed())
{
  // Set the USB data received event handler to the onUsbDataReceived() method.
  m_Board.usbDevice().onDataReceived = std::bind(&Application::onUsbDataReceived, 
                                                 this, std::placeholders::_1,
                                                 std::placeholders::_2);
  // Attach the control loop observers.
  attach(m_Indicator);
  // Transition to the idle state.
  m_Indicator.setState(Indicator::IDLE);
  // Start the thread.
  Start();
  // Log info.
  logger().info("Application started.");
}


///**************************************************************************************
/// \brief     Application periodic task function.
///
///**************************************************************************************
void Application::Run()
{
  constexpr size_t stepTimeMillis = 10U;
  constexpr auto deltaMillis = std::chrono::milliseconds{stepTimeMillis};
  const TickType_t deltaTicks = cpp_freertos::Ticks::MsToTicks(stepTimeMillis);

  for (;;)
  {
    // Wait until the task's period to elapses, while taking into consideration the 
    // execution time of the task itself.
    DelayUntil(deltaTicks);
    // Notify all attached subscribers about the elapsed time step.
    notify(deltaMillis);
  }
}


///**************************************************************************************
/// \brief     Event handler that gets called when new data was received on from the USB
///            host.
///
///**************************************************************************************
void Application::onUsbDataReceived(uint8_t const t_Data[], uint32_t t_Len)
{
  // Was this the XCP Connect command with connect parameter 0?
  if ((t_Data[0] == 2U) && (t_Data[1] == 0xFFU) && (t_Data[2] == 0x00U) && (t_Len == 3U))
  {
    // Send the XCP Connect response.
    uint8_t connectRes[] = { 8, 0xFF, 0x10, 0x00, 0x08, 0x08, 0x00, 0x01, 0x01 };
    m_Board.usbDevice().transmit(connectRes, sizeof(connectRes)/sizeof(connectRes[0]));
  }
}

//********************************** end of application.cpp *****************************
