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
#include "version.hpp"
#include "logger.hpp"
#include "ticks.hpp"


///**************************************************************************************
/// \brief     Application constructor.
/// \param     t_Board Reference to the board instance, which forms the hardware
///            abstraction.
///
///**************************************************************************************
Application::Application(Board& t_Board)
  : cpp_freertos::Thread("AppThread", configMINIMAL_STACK_SIZE + 48, 4),
    m_Board(t_Board), 
    m_Indicator(t_Board.statusLed()),
    m_Gateway(t_Board.usbDevice(), t_Board.can(), t_Board.boot())
{
  // Set the USB device suspend event handler to the onUsbSuspend() method.
  m_Board.usbDevice().onSuspend = std::bind(&Application::onUsbSuspend, this);
  // Set the USB device resume event handler to the onUsbResume() method.
  m_Board.usbDevice().onResume = std::bind(&Application::onUsbResume, this);
  // Set the gateway connected event handler to the onGatewayConnected() method.
  m_Gateway.onConnected = std::bind(&Application::onGatewayConnected, this);
  // Set the gateway disconnected event handler to the onGatewayDisconnected() method.
  m_Gateway.onDisconnected = std::bind(&Application::onGatewayDisconnected, this);
  // Set the gateway error event handler to the onGatewayError() method.
  m_Gateway.onError = std::bind(&Application::onGatewayError, this);
  // Attach the control loop observers.
  attach(m_Indicator);
  attach(m_Gateway);
  // Transition to the idle state.
  m_Indicator.setState(Indicator::IDLE);
  // Start the gateway.
  m_Gateway.start();
  // Log info.
  logger().info("Application started (v%d.%d.%d).", Version::major, Version::minor, Version::patch);
  // Start the thread.
  Start();
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
  constexpr size_t heapMonitorSteps = 30000UL / stepTimeMillis; // 30 seconds.
  size_t stepsCounter = 0U;

  // Enter the task body, which should be an infinite loop.
  for (;;)
  {
    // Wait until the task's period elapses, while taking into consideration the 
    // execution time of the task itself.
    DelayUntil(deltaTicks);
    // Notify all attached subscribers about the elapsed time step.
    notify(deltaMillis);
    // Run the heap monitor.
    if (++stepsCounter >= heapMonitorSteps)
    {
      stepsCounter = 0;
      logger().info("Heap monitor reports %u of %u bytes available.", TbxHeapGetFree(),
                    TBX_CONF_HEAP_SIZE);
    }
  }
}


///**************************************************************************************
/// \brief     Event handler that gets called when the USB bus is suspended. Within 7 
///            milliseconds the device must draw an average of less that 2.5 mA from the
///            bus.
///
///**************************************************************************************
void Application::onUsbSuspend()
{
  // Stop the gateway.
  m_Gateway.stop();
  // Set indicator to the sleeping state.
  m_Indicator.setState(Indicator::SLEEPING);
  // Log info.
  logger().info("Gateway stopped.");
}


///**************************************************************************************
/// \brief     Event handler that gets called when the USB bus is resumed.
///
///**************************************************************************************
void Application::onUsbResume()
{
  // Set indicator to the idle state,
  m_Indicator.setState(Indicator::IDLE);
  // Start the gateway.
  m_Gateway.start();
  // Log info.
  logger().info("Gateway started.");
}


///**************************************************************************************
/// \brief     Event handler that gets called when the gateway connected to a target on
///            the CAN bus.
///
///**************************************************************************************
void Application::onGatewayConnected()
{
  // Set indicator to the active state,
  m_Indicator.setState(Indicator::ACTIVE);
  // Log info.
  logger().info("Gateway connected.");
}


///**************************************************************************************
/// \brief     Event handler that gets called when the gateway disconnected from a target
///            on the CAN bus.
///
///**************************************************************************************
void Application::onGatewayDisconnected()
{
  // Set indicator to the idle state,
  m_Indicator.setState(Indicator::IDLE);
  // Log info.
  logger().info("Gateway disconnected.");
}


///**************************************************************************************
/// \brief     Event handler that gets called when the gateway detected an error. For
///            example a CAN bus off event.
///
///**************************************************************************************
void Application::onGatewayError()
{
  // Set indicator to the error state,
  m_Indicator.setState(Indicator::ERROR);
}

//********************************** end of application.cpp *****************************
