///**************************************************************************************
/// \file         indicator.cpp
/// \brief        Status indicator source file.
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
#include "indicator.hpp"


///**************************************************************************************
/// \brief     Indicator constructor.
/// \param     t_StatusLed Reference to the status LED instance.
///
///**************************************************************************************
Indicator::Indicator(Led& t_StatusLed)
  : ControlLoopSubscriber(), m_StatusLed(t_StatusLed)
{
}


///**************************************************************************************
/// \brief     Setter for the indicator state.
/// \param     t_Value New state value.
///
///**************************************************************************************
void Indicator::setState(State t_Value)
{
  // Perform state transition, if the state actually changed.
  if (m_State != t_Value)
  {
    // Perform new state entry actions.
    switch (t_Value)
    {
      case SLEEPING:
      {
        // Turn the status LED off.
        m_StatusLed.off();
      }
      break;

      case IDLE:
      case ACTIVE:
      {
        // Turn the status LED off.
        m_StatusLed.off();
        // Reset the play index.
        m_PlayIdx = 0;
        // Initialize the last toggle time.
        m_LastToggleMillis = m_CurrentMillis;
      }
      break;

      case ERROR:
      default:
      {
        // Turn the status LED on.
        m_StatusLed.on();
      }
      break;
    }
    // Update the state.
    m_State = t_Value;
  }

}

///**************************************************************************************
/// \brief     Update method that drives the class. Should be called periodically.
/// \param     t_Delta Number of milliseconds that passed.
///
///**************************************************************************************
void Indicator::update(std::chrono::milliseconds t_Delta)
{
  // Update the current time. 
  m_CurrentMillis += t_Delta;

  // Only need to run a play in the IDLE and ACTIVE states.
  if ( (m_State == IDLE) || (m_State == ACTIVE) )
  {
    // Did one play step pass?
    if ((m_CurrentMillis - m_LastToggleMillis) >= c_PlayStepMillis)
    {
      // Update the last toggle time for the next interval detection.
      m_LastToggleMillis += c_PlayStepMillis;
      // Run the state specific indicator control, where applicable.
      switch (m_State)
      {
        case IDLE:
        {
          // Play the current index.
          if (c_PlayIdle[m_PlayIdx] == 0)
          {
            m_StatusLed.off();
          }
          else
          {
            m_StatusLed.on();
          }
          // Update the indexer.
          m_PlayIdx = (m_PlayIdx < (c_PlayIdle.size() - 1) ? m_PlayIdx + 1 : 0);
        }
        break;
      
        case ACTIVE:
        {
          // Play the current index.
          if (c_PlayActive[m_PlayIdx] == 0)
          {
            m_StatusLed.off();
          }
          else
          {
            m_StatusLed.on();
          }
          // Update the indexer.
          m_PlayIdx = (m_PlayIdx < (c_PlayActive.size() - 1) ? m_PlayIdx + 1 : 0);
        }
        break;

        default:
        {
          // Nothing to do in the other states.
        }
        break;
      }

    }
  }
}

//********************************** end of indicator.cpp *******************************
