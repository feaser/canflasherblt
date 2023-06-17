///**************************************************************************************
/// \file         controlloop.cpp
/// \brief        Fixed step control loop publisher-subscriber source file.
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
#include "controlloop.hpp"


///**************************************************************************************
/// \brief     ControlLoopPublisher constructor.
///
///**************************************************************************************
ControlLoopPublisher::ControlLoopPublisher()
{
  // Create the linked list for storing subscribers. Note that a MicroTBX linked list
  // is used, because it builds on memory pools under the hood.
  m_Subscribers = TbxListCreate();
  TBX_ASSERT(m_Subscribers != nullptr);  
}


///**************************************************************************************
/// \brief     ControlLoopPublisher destructor.
///
///**************************************************************************************
ControlLoopPublisher::~ControlLoopPublisher()
{
  // Delete the linked list with subscribers. Note that this class is not responsible for
  // destroying the subscribers themselves.
  if (m_Subscribers != nullptr)
  {
    TbxListDelete(m_Subscribers);
  }  
}


///**************************************************************************************
/// \brief     Attaches a subscriber to receive fixed time step update notifications.
/// \param     t_Subscriber Reference of the subscriber to attached.
///
///**************************************************************************************
void ControlLoopPublisher::attach(ControlLoopSubscriber& t_Subscriber)
{
  uint8_t insertRes;

  // Validate the subscriber list.
  TBX_ASSERT(m_Subscribers != nullptr);

  // Only continue with a valid subscriber list.
  if (m_Subscribers != nullptr)
  {
    // Insert a pointer to the subscriber into the subscriber list.
    insertRes = TbxListInsertItemBack(m_Subscribers, &t_Subscriber);
    // Assert insertion. In case this triggers, increase the heap size.
    TBX_ASSERT(insertRes == TBX_OK);
  }
}


///**************************************************************************************
/// \brief     Detaches a subscriber from receiving fixed time step update notifications.
/// \param     t_Subscriber Reference of the subscriber to detach.
///
///**************************************************************************************
void ControlLoopPublisher::detach(ControlLoopSubscriber& t_Subscriber)
{
  
  // Validate the subscriber list.
  TBX_ASSERT(m_Subscribers != nullptr);

  // Only continue with a valid subscriber list.
  if (m_Subscribers != nullptr)
  {
    // Remove the pointer of the subscriber from the subscriber list.
    TbxListRemoveItem(m_Subscribers, &t_Subscriber);
  }
}


///**************************************************************************************
/// \brief     Notifies all attached subscribers about the elapsed fixed time step.
/// \param     t_Delta Number of milliseconds that passed.
///
///**************************************************************************************
void ControlLoopPublisher::notify(std::chrono::milliseconds t_Delta)
{
  void * subscriber;

  // Iterate over all the attached subscribers.
  subscriber = TbxListGetFirstItem(m_Subscribers);
  while (subscriber != nullptr)
  {
    // Notify the subscriber about the elapsed fixed time step.
    static_cast<ControlLoopSubscriber *>(subscriber)->update(t_Delta);
    // Continue with the next attached subscriber.
    subscriber = TbxListGetNextItem(m_Subscribers, subscriber);
  }
}

//********************************** end of controlloop.cpp *****************************
