///**************************************************************************************
/// \file         bxcan.cpp
/// \brief        Basic Extended CAN driver source file.
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
#include "bxcan.hpp"


///**************************************************************************************
/// \brief     Basic Extended CAN driver constructor. 
///
///**************************************************************************************
BxCan::BxCan()
  : Can()
{
  // TODO ##Vg Implement BxCan().
}


///**************************************************************************************
/// \brief     Basic Extended CAN driver destructor.
///
///**************************************************************************************
BxCan::~BxCan()
{
  // TODO ##Vg Implement ~BxCan().
}


///**************************************************************************************
/// \brief     Sets the message reception acceptance filter. Automaticaly reconnected,
///            if the driver is in the connected state when calling this method.
/// \param     t_Filter Message reception acceptance filter.
///
///**************************************************************************************
void BxCan::setFilter(CanFilter& t_Filter)
{
  // TODO ##Vg Implement setFilter().
}


///**************************************************************************************
/// \brief     Configures the CAN controller and synchronizes to the CAN bus.
/// \param     t_Baudrate Desired communication speed.
///
///**************************************************************************************
void BxCan::connect(Baudrate t_Baudrate)
{
  // TODO ##Vg Implement connect().
}


///**************************************************************************************
/// \brief     Disconnects from the CAN bus.
///
///**************************************************************************************
void BxCan::disconnect()
{
  // TODO ##Vg Implement disconnect().
}


///**************************************************************************************
/// \brief     Submits a message for transmission on the CAN bus.
/// \param     t_Msg The message to transmit.
/// \return    TBX_OK if successful, TBX_ERROR otherwise.
///
///**************************************************************************************
uint8_t BxCan::transmit(CanMsg& t_Msg)
{
  uint8_t result = TBX_ERROR;

  // TODO ##Vg Implement transmit().

  // Give the result back to the caller.
  return result;
}

//********************************** end of bxcan.cpp ***********************************
