///**************************************************************************************
/// \file         gateway.hpp
/// \brief        Gateway for XCP USB-CAN header file.
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
#ifndef GATEWAY_HPP
#define GATEWAY_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include <cstdint>
#include <functional>
#include <chrono>
#include "controlloop.hpp"
#include "usbdevice.hpp"
#include "can.hpp"
#include "boot.hpp"
#include "microtbx.h"


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief Gateway for XCP USB-CAN class.
class Gateway : public ControlLoopSubscriber
{
public:
  // Constructors and destructor.
  explicit Gateway(UsbDevice& t_UsbDevice, Can& t_Can, Boot& t_Boot, 
                   uint8_t t_OwnNodeId, Can::Baudrate t_CanBaudrate, uint8_t t_CanExtIds, 
                   uint32_t t_CanIdToTarget, uint32_t t_CanIdFromTarget);
  explicit Gateway(UsbDevice& t_UsbDevice, Can& t_Can, Boot& t_Boot)
    : Gateway(t_UsbDevice, t_Can, t_Boot, 255U, Can::BR500K, 
              TBX_FALSE, 0x667UL, 0x7E1UL) { }
  virtual ~Gateway() { }
  // Methods.
  void start();
  void stop();
  void update(std::chrono::milliseconds t_Delta) override;
  // Events.
  std::function<void()> onConnected;
  std::function<void()> onDisconnected;
  std::function<void()> onError;

private:
  // Constants.
  static constexpr std::chrono::milliseconds c_IdleTimeoutMillis{12000};
  // Members.
  UsbDevice& m_UsbDevice;
  Can& m_Can;
  Boot& m_Boot;
  uint8_t m_OwnNodeId;
  Can::Baudrate m_CanBaudrate;
  uint8_t m_CanExtIds;
  uint32_t m_CanIdToTarget;
  uint32_t m_CanIdFromTarget;
  uint8_t m_Started{TBX_FALSE};
  uint8_t m_Connected{TBX_FALSE};
  std::chrono::milliseconds m_LastPacketMillis{0};
  std::chrono::milliseconds m_CurrentMillis{0};
  // Methods.
  void onUsbDataReceived(uint8_t const t_Data[], uint32_t t_Len);
  void onCanReceived(CanMsg& t_Msg);
  void onCanBusOff();

  // Flag the class as non-copyable.
  Gateway(const Gateway&) = delete;
  const Gateway& operator=(const Gateway&) = delete; 
};

#endif // GATEWAY_HPP
//********************************** end of gateway.hpp *********************************
