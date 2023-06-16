///**************************************************************************************
/// \file         tinyusbdevice.hpp
/// \brief        TinyUSB device header file.
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
#ifndef TINYUSBDEVICE_HPP
#define TINYUSBDEVICE_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include "usbdevice.hpp"
#include "thread.hpp"
#include "tusb.h"


//***************************************************************************************
// Forward declarations
//***************************************************************************************
class HardwareBoard;


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief TinyUSB device class.
class TinyUsbDevice : public UsbDevice, public cpp_freertos::Thread
{
public:
  // Constructors and destructor.
  explicit TinyUsbDevice();
  virtual ~TinyUsbDevice();
  // Methods.
  uint8_t transmit(uint8_t const t_Data[], uint32_t t_Len) override;

private:
  // Enumerations.
  enum CallbackId
  {
    RXNEWDATA,
    MOUNTED,
    UNMOUNTED,
    SUSPEND,
    RESUME
  };
  // Members.
  static TinyUsbDevice* s_InstancePtr;
  std::array<uint8_t, CFG_TUD_VENDOR_RX_BUFSIZE> m_RxBuf;
  // Methods.
  void Run() override;
  void processCallback(CallbackId t_CallbackId);
  // Friends.
  friend void tud_vendor_rx_cb(uint8_t itf);
  friend void tud_mount_cb(void);
  friend void tud_umount_cb(void);
  friend void tud_suspend_cb(bool remote_wakeup_en);
  friend void tud_resume_cb(void);

  // Flag the class as non-copyable.
  TinyUsbDevice(const TinyUsbDevice&) = delete;
  const TinyUsbDevice& operator=(const TinyUsbDevice&) = delete;
};

#endif // TINYUSBDEVICE_HPP
//********************************** end of tinyusbdevice.hpp ***************************
