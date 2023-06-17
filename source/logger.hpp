///**************************************************************************************
/// \file         logger.hpp
/// \brief        Event logger header file.
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
#ifndef LOGGER_HPP
#define LOGGER_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include <cstdarg>


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief Abstract event logger class.
class Logger
{
public:
  // Destructor.
  virtual ~Logger() { }
  // Methods.
  void info(const char t_Fmt[], ...)
  { 
    va_list paramList;

    va_start(paramList, t_Fmt);
    log(INFO, t_Fmt, &paramList);
    va_end(paramList);
  }
  void warning(const char t_Fmt[], ...)
  { 
    va_list paramList;

    va_start(paramList, t_Fmt);
    log(WARNING, t_Fmt, &paramList);
    va_end(paramList);
  }
  void error(const char t_Fmt[], ...)
  { 
    va_list paramList;

    va_start(paramList, t_Fmt);
    log(ERROR, t_Fmt, &paramList);
    va_end(paramList);
  }

protected:
  // Enumerations.
  enum Level
  {
    INFO = 0,
    WARNING,
    ERROR
  };
  // Flag the class as abstract.
  explicit Logger() { }

private:
  // Methods
  virtual void log(Level t_Level, const char t_Fmt[], va_list * t_ParamList) = 0;

  // Flag the class as non-copyable.
  Logger(const Logger&) = delete;
  const Logger& operator=(const Logger&) = delete;
};


//***************************************************************************************
// Function prototypes
//***************************************************************************************
// Global getter of the logger's instance. In general global instances are a bad design
// practice. An exception was made for accessing the logger instance, because there is
// always exactly just one in the entire application and this approach makes it
// convenient to access the instance. Otherwise all objects that require logger access
// need to pass around a logger instance reference. 
Logger& logger();


#endif // LOGGER_HPP
//********************************** end of logger.hpp **********************************
