///**************************************************************************************
/// \file         can.cpp
/// \brief        CAN driver source file.
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
#include "can.hpp"


///**************************************************************************************
/// \brief     CAN message constructor.
/// \param     t_Id CAN message identifier.
/// \param     t_Ext TBX_TRUE for a 29-bit extended CAN identifier, TBX_FALSE for 11-bit
///            standard CAN identifier.
/// \param     t_Len Data length in the range 0..c_DataLenMax.
/// \param     t_Data CAN message data bytes.
///
///**************************************************************************************
CanMsg::CanMsg(uint32_t t_Id, uint8_t t_Ext, uint8_t t_Len, CanData t_Data)
 : m_Id(t_Id), m_Ext(t_Ext), m_Len(t_Len), m_Data{ t_Data }
{
}


///**************************************************************************************
/// \brief     Setter for the CAN message identifier.
/// \param     t_Id CAN message identifier.
///
///**************************************************************************************
void CanMsg::setId(uint32_t t_Id)
{
  // Validate the parameter.
  TBX_ASSERT(t_Id <= c_ExtIdMax);

  // Only continue with a valid parameter.
  if (t_Id <= c_ExtIdMax)
  {
    m_Id = t_Id;
  }
}


///**************************************************************************************
/// \brief     Setter for the CAN message identifier type.
/// \param     t_Ext TBX_TRUE for a 29-bit extended CAN identifier, TBX_FALSE for 11-bit
///            standard CAN identifier.
///
///**************************************************************************************
void CanMsg::setExt(uint8_t t_Ext)
{
  m_Ext = (t_Ext == TBX_FALSE) ? TBX_FALSE : TBX_TRUE;
}


///**************************************************************************************
/// \brief     Setter for the CAN message data length.
/// \param     t_Len Data length in the range 0..c_DataLenMax.
///
///**************************************************************************************
void CanMsg::setLen(uint8_t t_Len)
{
  // Validate the parameter.
  TBX_ASSERT(t_Len <= c_DataLenMax);

  // Only continue with a valid parameter.
  if (t_Len <= c_DataLenMax)
  {
    m_Len = t_Len;
  }
}


///**************************************************************************************
/// \brief     Array subscript operator overload for data byte array assignment of
///            non-const objects.
/// \param     t_Idx Index into the data byte array in the range 0..(c_DataLenMax-1).
/// \return    Reference to the data byte array element.
///
///**************************************************************************************
uint8_t& CanMsg::operator[](uint8_t t_Idx)
{
  TBX_ASSERT(t_Idx < c_DataLenMax);
  return m_Data[t_Idx];
}


///**************************************************************************************
/// \brief     Array subscript operator overload for data byte array reading of const
///            objects.
/// \param     t_Idx Index into the data byte array in the range 0..(c_DataLenMax-1).
/// \return    Reference to the data byte array element.
///
///**************************************************************************************
const uint8_t& CanMsg::operator[](uint8_t t_Idx) const //reading of const objects.
{
  TBX_ASSERT(t_Idx < c_DataLenMax);
  return m_Data[t_Idx];
}


///**************************************************************************************
/// \brief     CAN filter constructor.
/// \param     t_Code The code part of the filter determines what bit values to match in
///            the received message identifier.
/// \param     t_Mask A mask bit value of 0 means don't care.
/// \param     t_Mode The mode settings sets the identifier type that the filter should
///            apply to
///
///**************************************************************************************
CanFilter::CanFilter(uint32_t t_Code, uint32_t t_Mask, Mode t_Mode)
  : code(t_Code), mask(t_Mask), mode(t_Mode)
{
}

//********************************** end of can.cpp *************************************
