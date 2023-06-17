///**************************************************************************************
/// \file         controlloop.hpp
/// \brief        Fixed step control loop publisher-subscriber header file.
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
#ifndef CONTROLLOOP_HPP
#define CONTROLLOOP_HPP

//***************************************************************************************
// Include files
//***************************************************************************************
#include <chrono>
#include "microtbx.h"


//***************************************************************************************
// Class definitions
//***************************************************************************************
/// \brief Fixed step control loop subscriber abstract interface class.
class ControlLoopSubscriber
{
public:
  // Destructor.
  virtual ~ControlLoopSubscriber() { }
  // Methods.
  virtual void update(std::chrono::milliseconds t_Delta) = 0;

protected:
  // Flag the class as abstract.
  explicit ControlLoopSubscriber() { }
};

/// \brief Fixed step control loop publisher abstract class.
class ControlLoopPublisher
{
public:
  // Destructor.
  virtual ~ControlLoopPublisher();
  // Methods.
  void attach(ControlLoopSubscriber& t_Subscriber);
  void detach(ControlLoopSubscriber& t_Subscriber);
  void notify(std::chrono::milliseconds t_Delta);

protected:
  // Flag the class as abstract.
  explicit ControlLoopPublisher();

private:
  // Members.
  tTbxList * m_Subscribers{nullptr};  
};


#endif // CONTROLLOOP_HPP
//********************************** end of controlloop.hpp *****************************
