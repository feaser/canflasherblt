///**************************************************************************************
/// \file         hardwareboard.hpp
/// \brief        Hardware specific board support package header file.
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
#ifndef HARDWAREBOARD_HPP
#define HARDWAREBOARD_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include <memory>
#include "board.hpp"


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief   Board support package class that represents the hardware abstraction layer.
///          It implements the getters of the abstract class that it derives from.
/// \details Note that the getters actually return a reference to the hardware specific
///          object, thereby realizing the mapping between hardware independent parts and
///          hardware dependent parts. For example: m_StatusLed is of type StatusLed but
///          the statusLed() getter returns the reference of the generic hardware
///          abstracted type Led.
class HardwareBoard : public Board
{
public:
  // Constructors and destructor.
  HardwareBoard();
  virtual ~HardwareBoard() { }
  // Methods.
  static void assertHandler(const char * const file, uint32_t line);  

private:
  // Methods.
  void mcuInit();
  void setupSystemClock();

  // Flag the class as non-copyable.
  HardwareBoard(const HardwareBoard&) = delete;
  const HardwareBoard& operator=(const HardwareBoard&) = delete;
};

#endif // HARDWAREBOARD_HPP
//********************************** end of hardwareboard.hpp ***************************
