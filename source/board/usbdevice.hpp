///**************************************************************************************
/// \file         usbdevice.hpp
/// \brief        USB device driver header file.
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
#ifndef USBDEVICE_HPP
#define USBDEVICE_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include <functional>


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief   Abstract USB device driver class.
class UsbDevice
{
public:
  // Destructor.
  virtual ~UsbDevice() { }
  // Events.
  std::function<void()> onMounted;
  std::function<void()> onUnmounted;
  std::function<void()> onSuspend;
  std::function<void()> onResume;

protected:
  // Flag the class as abstract.
  explicit UsbDevice() { }

private:
  // Flag the class as non-copyable.
  UsbDevice(const UsbDevice&) = delete;
  const UsbDevice& operator=(const UsbDevice&) = delete;
};

#endif // USBDEVICE_HPP
//********************************** end of usbdevice.hpp *******************************
