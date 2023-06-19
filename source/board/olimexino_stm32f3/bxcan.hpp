///**************************************************************************************
/// \file         bxcan.hpp
/// \brief        Basic Extended CAN driver header file.
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
#ifndef BXCAN_HPP
#define BXCAN_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include "can.hpp"
#include "thread.hpp"
#include "microtbx.h"


//***************************************************************************************
// Function prototypes
//***************************************************************************************
extern "C" void USB_HP_CAN_TX_IRQHandler(void);
extern "C" void USB_LP_CAN_RX0_IRQHandler(void);
extern "C" void CAN_RX1_IRQHandler(void);
extern "C" void CAN_SCE_IRQHandler(void);


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief Basic Extended CAN driver class.
class BxCan : public Can, public cpp_freertos::Thread
{
public:
  // Constructors and destructor.
  explicit BxCan();
  virtual ~BxCan();
  // Getters and setters.
  void setFilter(CanFilter& t_Filter) override;
  // Methods.
  void connect(Baudrate t_Baudrate) override;
  void disconnect() override;
  uint8_t transmit(CanMsg& t_Msg) override;

private:
  // Members.
  static BxCan* s_InstancePtr;
  uint8_t m_Connected{TBX_FALSE};
  Baudrate m_Baudrate{BR500K};
  CanFilter m_Filter{0UL, 0UL, CanFilter::BOTH};
  // Methods.
  void Run() override;
  void processInterrupt();
  uint8_t findBitTimingSettings(uint16_t& t_Prescaler, uint8_t& t_Tseg1, 
                                uint8_t& t_Tseg2);
  // Friends.
  friend void USB_HP_CAN_TX_IRQHandler(void);
  friend void USB_LP_CAN_RX0_IRQHandler(void);
  friend void CAN_RX1_IRQHandler(void);
  friend void CAN_SCE_IRQHandler(void);

  // Flag the class as non-copyable.
  BxCan(const BxCan&) = delete;
  const BxCan& operator=(const BxCan&) = delete;
};

#endif // BXCAN_HPP
//********************************** end of bxcan.hpp ***********************************
