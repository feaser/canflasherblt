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
  explicit CanMsg(uint32_t t_Id, uint8_t t_Ext, uint8_t t_Len, CanData t_Data);
  explicit CanMsg(uint32_t t_Id, uint8_t t_Ext, uint8_t t_Len) 
    : CanMsg(t_Id, t_Ext, t_Len, { }) { }
  explicit CanMsg() : CanMsg(0, TBX_FALSE, 0, { }) { }
  virtual ~CanMsg() { }
  // Getters and setters.
  uint32_t id() const { return m_Id; }
  uint8_t ext() const { return m_Ext; }
  uint8_t len() const { return m_Len; }
  void setId(uint32_t t_Id);
  void setExt(uint8_t t_Ext);
  void setLen(uint8_t t_Len);
  // Operator overloads.
  uint8_t& operator[](uint8_t t_Idx);
  const uint8_t& operator[](uint8_t t_Idx) const;

private:
  // Members.
  uint32_t m_Id;
  uint8_t m_Ext;
  uint8_t m_Len;
  CanData m_Data;
};

#endif // CAN_HPP
//********************************** end of can.hpp *************************************
