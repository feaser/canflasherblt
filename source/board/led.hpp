///**************************************************************************************
/// \file         led.hpp
/// \brief        LED driver header file.
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
#ifndef LED_HPP
#define LED_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include "microtbx.h"


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief Abstract LED driver class.
class Led
{
public:
  // Destructor.
  virtual ~Led() { }
  // Methods.
  void on() 
  {
    m_State = TBX_ON; 
    set(m_State);
  }
  void off() 
  {
    m_State = TBX_OFF;
    set(m_State); 
  }
  void toggle()
  { 
    m_State = (m_State == TBX_ON) ?  TBX_OFF : TBX_ON;
    set(m_State); 
  }
  uint8_t state() const
  {
    return m_State;
  }

protected:
  // Flag the class as abstract.
  explicit Led() { }

private:
  // Members.
  uint8_t m_State{TBX_OFF};
  // Getters and setters.
  virtual void set(uint8_t t_State) = 0;

  // Flag the class as non-copyable.
  Led(const Led&) = delete;
  const Led& operator=(const Led&) = delete;
};

#endif // LED_HPP
//********************************** end of led.hpp *************************************
