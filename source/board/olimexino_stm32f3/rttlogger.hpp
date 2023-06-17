///**************************************************************************************
/// \file         rttlogger.hpp
/// \brief        Segger RTT based event logger header file.
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
#ifndef RTTLOGGER_HPP
#define RTTLOGGER_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include "logger.hpp"


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief Segger RTT based event logger class.
class RttLogger : public Logger
{
public:
  // Constructors and destructor.
  explicit RttLogger();
  virtual ~RttLogger() { }

private:
  // Methods.
  void log(Level t_Level, const char t_Fmt[], va_list * t_ParamList) override;

  // Flag the class as non-copyable.
  RttLogger(const RttLogger&) = delete;
  const RttLogger& operator=(const RttLogger&) = delete;
};

#endif // RTTLOGGER_HPP
//********************************** end of rttlogger.hpp *******************************
