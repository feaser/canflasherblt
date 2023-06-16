///**************************************************************************************
/// \file         application.hpp
/// \brief        Application header file.
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
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include "board.hpp"
#include "thread.hpp"


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief Application class.
class Application: public cpp_freertos::Thread
{
public:
  // Constructors and destructor.
  explicit Application(Board& t_Board);
  virtual ~Application() { }

private:
  // Members.
  Board& m_Board;
  // Methods.
  void Run() override;
  // Event handlers.
  void onUsbDataReceived(uint8_t const t_Data[], uint32_t t_Len);

  // Flag the class as non-copyable.
  Application(const Application&) = delete;
  const Application& operator=(const Application&) = delete; 
};

#endif // APPLICATION_HPP
//********************************** end of application.hpp *****************************
