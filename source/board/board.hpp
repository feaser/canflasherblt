///**************************************************************************************
/// \file         board.hpp
/// \brief        Board support package header file.
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
#ifndef BOARD_HPP
#define BOARD_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include "led.hpp"
#include "usbdevice.hpp"


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief   Abstract board support package class that represents the hardware
///          abstraction layer. It defines a hardware independent interface for getters
///          of hardware specific objects.
/// \details The idea is that you create a derived class that implements the getters and,
///          more importantly, returns the hardware specific version of these objects.
class Board
{
public:
  // Destructor.
  virtual ~Board() { }
  // Getters and setters.
  virtual Led& statusLed() = 0;
  virtual UsbDevice& usbDevice() = 0;
  // Methods.
  virtual void reset() = 0;

protected:
  // Flag the class as abstract.
  explicit Board() { }

private:
  // Flag the class as non-copyable.
  Board(const Board&) = delete;
  const Board& operator=(const Board&) = delete;
};

#endif // BOARD_HPP
//********************************** end of board.hpp ***********************************
