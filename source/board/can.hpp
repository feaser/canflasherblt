///**************************************************************************************
/// \file         can.hpp
/// \brief        CAN driver header file.
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
#ifndef CAN_HPP
#define CAN_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include <cstdint>
#include <array>
#include <functional>
#include "microtbx.h"


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief   CAN message class.
/// \details Note that this class uses array subscript operator overloading for easy
///          access to the CAN message data bytes.
/// \example Message initialization using just the constructor:
///            CanMsg myMsg(0x123, TBX_FALSE, 8, { 1, 2, 3, 4, 5, 6, 7, 8 });
///
///          Message initialization using setters:
///            myMsg.setId(0x123);
///            myMsg.setExt(TBX_FALSE);
///            myMsg.setLen(8);
///            for (uint8_t idx = 0; idx < CanMsg::c_DataLenMax; idx++)
///            {
///              myMsg[idx] = idx + 1;
///            }
class CanMsg
{
public:
  // Constants.
  static constexpr size_t c_DataLenMax = 8U;
  static constexpr size_t c_StdIdMax = 0x7FFU;
  static constexpr size_t c_ExtIdMax = 0x1FFFFFFFUL;
  // Type definitions.
  using CanData = std::array<uint8_t, c_DataLenMax>;
  // Constructors and destructor.
  explicit CanMsg(uint32_t t_Id, uint8_t t_Ext, uint8_t t_Len, CanData t_Data)
    : m_Id(t_Id), m_Ext(t_Ext), m_Len(t_Len), m_Data{ t_Data } { }
  explicit CanMsg(uint32_t t_Id, uint8_t t_Ext, uint8_t t_Len) 
    : CanMsg(t_Id, t_Ext, t_Len, { }) { }
  explicit CanMsg() : CanMsg(0, TBX_FALSE, 0, { }) { }
  virtual ~CanMsg() { }
  // Getters and setters.
  uint32_t id() const { return m_Id; }
  uint8_t ext() const { return m_Ext; }
  uint8_t len() const { return m_Len; }
  void setId(uint32_t t_Id) { TBX_ASSERT(t_Id <= c_ExtIdMax); m_Id = t_Id; } 
  void setExt(uint8_t t_Ext) { m_Ext = (t_Ext == TBX_FALSE) ? TBX_FALSE : TBX_TRUE; }
  void setLen(uint8_t t_Len) { TBX_ASSERT(t_Len <= c_DataLenMax); m_Len = t_Len; }
  // Operator overloads.
  uint8_t& operator[](uint8_t t_Idx)
  {
    TBX_ASSERT(t_Idx < c_DataLenMax);
    return m_Data[t_Idx];
  }
  const uint8_t& operator[](uint8_t t_Idx) const
  {
    TBX_ASSERT(t_Idx < c_DataLenMax);
    return m_Data[t_Idx];
  }

private:
  // Members.
  uint32_t m_Id;
  uint8_t m_Ext;
  uint8_t m_Len;
  CanData m_Data;
};


/// \brief   CAN reception acceptance filter class.
/// \details The code and mask values configure the message reception acceptance filter.
///          A mask bit value of 0 means don't care. The code part of the filter
///          determines what bit values to match in the received message identifier.
///          The mode settings sets the identifier type that the filter should apply to:
///          11-bit (STD), 29-bit (EXT) or both CAN identifiers.
///          Example 1: Receive all CAN identifiers
///                     .code = 0x00000000
///                     .mask = 0x00000000
///                     .mode = CanFilter::BOTH
///          Example 2: Receive only CAN identifier 0x124 (11-bit or 29-bit)
///                     .code = 0x00000124
///                     .mask = 0x1fffffff
///                     .mode = CanFilter::BOTH
///          Example 3: Receive only CAN identifier 0x124 (11-bit)
///                     .code = 0x00000124
///                     .mask = 0x1fffffff
///                     .mode = CanFilter::STD
///          Example 4: Receive only CAN identifier 0x124 (29-bit)
///                     .code = 0x00000124
///                     .mask = 0x1fffffff
///                     .mode = CanFilter::Mode::EXT
class CanFilter
{
public:
  // Enumerations.
  enum Mode
  {
    STD, ///< Receive only 11-bit standard CAN identifiers.
    EXT, ///< Receive only 29-bit extended CAN identifiers.
    BOTH ///< Receive both 11-bit and 29-bit CAN identifiers.
  };
  // Constructors and destructor.
  explicit CanFilter(uint32_t t_Code, uint32_t t_Mask, Mode t_Mode)
    : code(t_Code), mask(t_Mask), mode(t_Mode) { }
  explicit CanFilter() : CanFilter(0x00000000UL, 0x00000000UL, BOTH) { }
  virtual ~CanFilter() { }
  // Members.
  uint32_t code;
  uint32_t mask;
  Mode mode;
};


/// \brief   Abstract CAN driver class.
class Can
{
public:
  // Enumerations.
  enum Baudrate
  {
    BR10K  =   10000UL,
    BR20K  =   20000UL,
    BR50K  =   50000UL,
    BR100K =  100000UL,
    BR125K =  125000UL,
    BR250K =  250000UL,
    BR500K =  500000UL,
    BR800K =  800000UL,
    BR1M   = 1000000UL
  };
  // Destructor.
  virtual ~Can() { }
  // Getters and setters.
  virtual void setFilter(CanFilter& t_Filter) = 0;
  // Methods.
  virtual void connect(Baudrate t_Baudrate = BR500K) = 0;
  virtual void disconnect() = 0;
  virtual uint8_t transmit(CanMsg& t_Msg) = 0;
  // Events.
  std::function<void(CanMsg& t_Msg)> onReceived;
  std::function<void()> onBusOff;

protected:
  // Flag the class as abstract.
  explicit Can() { }

private:
  // Flag the class as non-copyable.
  Can(const Can&) = delete;
  const Can& operator=(const Can&) = delete;
};


#endif // CAN_HPP
//********************************** end of can.hpp *************************************
