///**************************************************************************************
/// \file         gateway.cpp
/// \brief        Gateway for XCP USB-CAN source file.
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
#include "gateway.hpp"
#include "logger.hpp"


///**************************************************************************************
/// \brief     Gateway constructor.
/// \param     t_UsbDevice Reference to the USB device instance.
/// \param     t_Can Reference to the CAN driver instance.
/// \param     t_Boot Reference to the Bootloader interaction instance.
/// \param     t_OwnNodeId Own node identifier for firmware updates. Upon reception of
///            an XCP Connect command via USB with the command parameter (CM) set to
///            this node identifier value, the XCP packet is not pushed through the
///            gateway. Instead this system's own bootloader is activated.
/// \param     t_CanBaudrate Desired CAN communication baudrate.
/// \param     t_CanExtIds TBX_TRUE if the specified CAN identifiers are 29-bit extended,
///            TBX_FALSE for 11-bit standard CAN identifiers.
/// \param     t_CanIdToTarget The CAN identifier to use when sending XCP packets to the
///            microcontroller target via the CAN bus.
/// \param     t_CanIdFromTarget The CAN identifier for receiving XCP packets from the
///            microcontroller target via the CAN bus.
///
///**************************************************************************************
Gateway::Gateway(UsbDevice& t_UsbDevice, Can& t_Can, Boot& t_Boot, uint8_t t_OwnNodeId,
                 Can::Baudrate t_CanBaudrate, uint8_t t_CanExtIds, 
                uint32_t t_CanIdToTarget, uint32_t t_CanIdFromTarget)
  : ControlLoopSubscriber(),
    m_UsbDevice(t_UsbDevice), m_Can(t_Can), m_Boot(t_Boot),
    m_OwnNodeId(t_OwnNodeId), m_CanBaudrate(t_CanBaudrate), m_CanExtIds(t_CanExtIds),
    m_CanIdToTarget(t_CanIdToTarget), m_CanIdFromTarget(t_CanIdFromTarget)
{
  // Set the USB data received event handler to the onUsbDataReceived() method.
  m_UsbDevice.onDataReceived = std::bind(&Gateway::onUsbDataReceived, 
                                         this, std::placeholders::_1,
                                         std::placeholders::_2);
  // Set the CAN message received event handler to the onCanReceived() method.
  m_Can.onReceived = std::bind(&Gateway::onCanReceived, this, std::placeholders::_1);
}


///**************************************************************************************
/// \brief     Starts the gateway. 
///
///**************************************************************************************
void Gateway::start()
{
  // Configure the CAN reception acceptance filter to just receive XCP packets from
  // the target.
  CanFilter canFilter(m_CanIdFromTarget, 0x1FFFFFFFUL, CanFilter::STD);
  if (m_CanExtIds == TBX_TRUE)
  {
    canFilter.mode = CanFilter::EXT;
  }
  m_Can.setFilter(canFilter);
  // Connect to the CAN bus.
  m_Can.connect(m_CanBaudrate);
  // Log info.
  logger().info("Gateway started.");
}


///**************************************************************************************
/// \brief     Stops the gateway. 
///
///**************************************************************************************
void Gateway::stop()
{
  // Disconnect from the CAN bus.
  m_Can.disconnect();
  // Log info.
  logger().info("Gateway stopped.");
}


///**************************************************************************************
/// \brief     Update method that drives the class. Should be called periodically.
/// \param     t_Delta Number of milliseconds that passed.
///
///**************************************************************************************
void Gateway::update(std::chrono::milliseconds t_Delta)
{
  // TODO ##Vg Implement update().
}


///**************************************************************************************
/// \brief     Event handler that gets called when new data was received on from the USB
///            host.
///
///**************************************************************************************
void Gateway::onUsbDataReceived(uint8_t const t_Data[], uint32_t t_Len)
{
  // TODO ##Vg Implement onUsbDataReceived().
}


///**************************************************************************************
/// \brief     Event handler that gets called when a new CAN message was received.
/// \param     t_Msg The newly received CAN message.
///
///**************************************************************************************
void Gateway::onCanReceived(CanMsg& t_Msg)
{
  // TODO ##Vg Implement onCanReceived().
}


///**************************************************************************************
/// \brief     Event handler that gets called when a CAN bus off error event was
///            detected.
///
///**************************************************************************************
void Gateway::onCanBusOff()
{
  // Log warning
  logger().warning("CAN bus off detected.");
  // Trigger the event handler, if assigned.
  if (onError)
  {
    onError();
  }
}

//********************************** end of gateway.cpp *********************************
