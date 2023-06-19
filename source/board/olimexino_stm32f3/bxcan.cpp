///**************************************************************************************
/// \file         bxcan.cpp
/// \brief        Basic Extended CAN driver source file.
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
#include "bxcan.hpp"
#include "stm32f3xx.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_gpio.h"


//***************************************************************************************
// Static data declarations
//***************************************************************************************
/// \brief   Storage for the single bxCan instance. 
/// \details Needed for mapping the CAN interrupt service routines to the processXxx()
///          instance methods. Note that this only works if there will only be one
///          bxCan instance. This is the case for this board, since its microcontroller
///          only has one CAN controller. 
BxCan* BxCan::s_InstancePtr = nullptr;


///**************************************************************************************
/// \brief     Basic Extended CAN driver constructor. 
///
///**************************************************************************************
BxCan::BxCan()
  : Can(),
    cpp_freertos::Thread("CanThread", configMINIMAL_STACK_SIZE, 8)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct{ };

  // Verify that only one instance of BxCan is created.
  TBX_ASSERT(s_InstancePtr == nullptr);
  // Store a pointer to ourselves.
  s_InstancePtr = this;

  // CAN TX and RX GPIO pin configuration.
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_9;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Enable the CAN related interrupts in the NVIC.
  NVIC_EnableIRQ(CAN_TX_IRQn);
  NVIC_EnableIRQ(CAN_RX0_IRQn);
  NVIC_EnableIRQ(CAN_RX1_IRQn);
  NVIC_EnableIRQ(CAN_SCE_IRQn);  

  // Start the thread.
  Start();
}


///**************************************************************************************
/// \brief     Basic Extended CAN driver destructor.
///
///**************************************************************************************
BxCan::~BxCan()
{
  // Disable the CAN related interrupts in the NVIC.
  NVIC_DisableIRQ(CAN_TX_IRQn);
  NVIC_DisableIRQ(CAN_RX0_IRQn);
  NVIC_DisableIRQ(CAN_RX1_IRQn);
  NVIC_DisableIRQ(CAN_SCE_IRQn);  

  // Reset the instance pointer.
  s_InstancePtr = nullptr;
}


///**************************************************************************************
/// \brief     Sets the message reception acceptance filter. Automatically reconnects,
///            if the driver is in the connected state when calling this method.
/// \param     t_Filter Message reception acceptance filter.
///
///**************************************************************************************
void BxCan::setFilter(CanFilter& t_Filter)
{
  // Store current connection state.
  uint8_t wasConnected = m_Connected;

  // Make sure that we're in the disconnected state, before updating the filter.
  disconnect();
  // Copy and store the filter settings.
  m_Filter = t_Filter;
  // Reconnect if needed.
  if (wasConnected == TBX_TRUE)
  {
    connect(m_Baudrate);
  }
}


///**************************************************************************************
/// \brief     Configures the CAN controller and synchronizes to the CAN bus.
/// \param     t_Baudrate Desired communication speed.
///
///**************************************************************************************
void BxCan::connect(Baudrate t_Baudrate)
{
  constexpr uint32_t maxLoopCntInitAck = 10000000UL;
  volatile  uint32_t initAckLoopCnt;
  uint16_t           prescaler = 8U;
  uint8_t            tseg1 = 6U;
  uint8_t            tseg2 = 2U;
  uint8_t            sjw;
  uint8_t            bitTimingSettingsFound;

  // Make sure that we're in the disconnected state, before connecting.
  disconnect();
  // Store the baudrate.
  m_Baudrate = t_Baudrate;

  // Enter initialization mode.
  CLEAR_BIT(CAN->MCR, CAN_MCR_SLEEP);
  SET_BIT(CAN->MCR, CAN_MCR_INRQ);
  // Wait for hardware handshake with timeout to not hang the system, in case the CAN
  // controller is malfunctioning. No need to take further action, because in such a
  // case, the CAN controller won't work properly anyways.
  initAckLoopCnt = maxLoopCntInitAck;
  while ((READ_BIT(CAN->MSR, CAN_MSR_INAK) == 0U) && (initAckLoopCnt > 0U))
  {
    initAckLoopCnt--;
  }

  // Attempt to find fitting bit timing settings.
  bitTimingSettingsFound = findBitTimingSettings(prescaler, tseg1, tseg2);
  // Trigger assertion in case no valid bit timing settings could be found. This would
  // indicate a configuration issue that can be fixed by changing the CAN baudrate or
  // the frequency of the clock that drives the CAN peripheral.
  TBX_ASSERT(bitTimingSettingsFound == TBX_OK);
  // Configure the bit timing settings.
  SET_BIT(CAN->BTR, (prescaler - 1U) << CAN_BTR_BRP_Pos);
  SET_BIT(CAN->BTR, (tseg1 - 1U) << CAN_BTR_TS1_Pos);
  SET_BIT(CAN->BTR, (tseg2 - 1U) << CAN_BTR_TS2_Pos);
  // Set SJW as big as possible (4), but no more than tseg2.
  sjw = (tseg2 > 4U) ? 4U : tseg2;
  SET_BIT(CAN->BTR, (sjw - 1U) << CAN_BTR_SJW_Pos);

  // Configure transmit priority by request order. Essentially making the 3 transmit
  // mailboxes behave as a FIFO.
  SET_BIT(CAN->MCR, CAN_MCR_TXFP);
  // Support automatic retransmission in case of arbitration lost.
  CLEAR_BIT(CAN->MCR, CAN_MCR_NART);
  // Make sure time triggered communication mode is disabled.
  CLEAR_BIT(CAN->MCR, CAN_MCR_TTCM);
  // Disable automatic bus off recovery.
  CLEAR_BIT(CAN->MCR, CAN_MCR_ABOM);
  // Discard incoming message in case of reception FIFO overrun.
  SET_BIT(CAN->MCR, CAN_MCR_RFLM);
  // Make sure the transmit request complete bits are reset, by writing a 1 to them. Note
  // that this is a special register. You need to directly write the RQCP bit values.
  WRITE_REG(CAN->TSR, CAN_TSR_RQCP0 | CAN_TSR_RQCP1 | CAN_TSR_RQCP2);

  // Enter reception filter initialization mode.
  SET_BIT(CAN->FMR, CAN_FMR_FINIT);
  // For receiving just 11-bit identifiers, use only FIFO0 with filter 0.
  if (m_Filter.mode == CanFilter::STD)
  {
    // Select identifier mask mode for filter 0.
    CLEAR_BIT(CAN->FM1R, CAN_FM1R_FBM0);
    // Select single 32-bit scaling for filter 0.
    SET_BIT(CAN->FS1R, CAN_FS1R_FSC0);
    // Set the filter's code and mask bits for 11-bit standard identifiers.
    WRITE_REG(CAN->sFilterRegister[0].FR1, m_Filter.code << 21U);
    WRITE_REG(CAN->sFilterRegister[0].FR2, m_Filter.mask << 21U);
    SET_BIT(CAN->sFilterRegister[0].FR2, CAN_F0R2_FB2); // IDE bit
    // Assign filter 0 to FIFO0.
    CLEAR_BIT(CAN->FFA1R, CAN_FFA1R_FFA0);
    // Activate filter 0.
    SET_BIT(CAN->FA1R, CAN_FA1R_FACT0);
  }
  // For receiving just 29-bit identifiers, use only FIFO0 with filter 0.
  else if (m_Filter.mode == CanFilter::EXT)
  {
    // Select identifier mask mode for filter 0.
    CLEAR_BIT(CAN->FM1R, CAN_FM1R_FBM0);
    // Select single 32-bit scaling for filter 0.
    SET_BIT(CAN->FS1R, CAN_FS1R_FSC0);
    // Set the filter's code and mask bits for 29-bit extended identifiers.
    WRITE_REG(CAN->sFilterRegister[0].FR1, m_Filter.code << 3U);
    SET_BIT(CAN->sFilterRegister[0].FR1, CAN_F0R1_FB2); // IDE bit
    WRITE_REG(CAN->sFilterRegister[0].FR2, m_Filter.mask << 3U);
    SET_BIT(CAN->sFilterRegister[0].FR2, CAN_F0R2_FB2); // IDE bit
    // Assign filter 0 to FIFO0.
    CLEAR_BIT(CAN->FFA1R, CAN_FFA1R_FFA0);
    // Activate filter 0.
    SET_BIT(CAN->FA1R, CAN_FA1R_FACT0);
  }
  // For receiving both 11-bit and 29-bit identifers, use FIFO0 with filter 0 for the
  // 11-bit CAN identifiers and FIFO1 with filter 1 for the 29-bit CAN identifiers.
  else
  {
    // Select identifier mask mode for filter 0.
    CLEAR_BIT(CAN->FM1R, CAN_FM1R_FBM0);
    // Select single 32-bit scaling for filter 0.
    SET_BIT(CAN->FS1R, CAN_FS1R_FSC0);
    // Set the filter's code and mask bits for 11-bit standard identifiers.
    WRITE_REG(CAN->sFilterRegister[0].FR1, m_Filter.code << 21U);
    WRITE_REG(CAN->sFilterRegister[0].FR2, m_Filter.mask << 21U);
    SET_BIT(CAN->sFilterRegister[0].FR2, CAN_F0R2_FB2); // IDE bit
    // Assign filter 0 to FIFO0.
    CLEAR_BIT(CAN->FFA1R, CAN_FFA1R_FFA0);
    // Activate filter 0.
    SET_BIT(CAN->FA1R, CAN_FA1R_FACT0);

    // Select identifier mask mode for filter 1.
    CLEAR_BIT(CAN->FM1R, CAN_FM1R_FBM1);
    // Select single 32-bit scaling for filter 1.
    SET_BIT(CAN->FS1R, CAN_FS1R_FSC1);
    // Set the filter's code and mask bits for 29-bit extended identifiers.
    WRITE_REG(CAN->sFilterRegister[1].FR1, m_Filter.code << 3U);
    SET_BIT(CAN->sFilterRegister[1].FR1, CAN_F1R1_FB2); // IDE bit
    WRITE_REG(CAN->sFilterRegister[1].FR2, m_Filter.mask << 3U);
    SET_BIT(CAN->sFilterRegister[1].FR2, CAN_F1R2_FB2); // IDE bit
    // Assign filter 1 to FIFO1.
    SET_BIT(CAN->FFA1R, CAN_FFA1R_FFA1);
    // Activate filter 1.
    SET_BIT(CAN->FA1R, CAN_FA1R_FACT1);
  }
  // Leave reception filter initialization mode.
  CLEAR_BIT(CAN->FMR, CAN_FMR_FINIT);

  // Enable transmit mailbox empty interrupt.
  SET_BIT(CAN->IER, CAN_IER_TMEIE);
  // Enable FIFO message pending interrupt for both FIFOs.
  SET_BIT(CAN->IER, CAN_IER_FMPIE0 | CAN_IER_FMPIE1);
  // Enable bus off error interrupt.
  SET_BIT(CAN->IER, CAN_IER_ERRIE | CAN_IER_BOFIE);

  // Leave initialization mode.
  CLEAR_BIT(CAN->MCR, CAN_MCR_INRQ);
  // Wait for hardware handshake with timeout to not hang the system, in case the CAN
  // controller is malfunctioning. No need to take further action, because in such a
  // case, the CAN controller won't work properly anyways.
  initAckLoopCnt = maxLoopCntInitAck;
  while ((READ_BIT(CAN->MSR, CAN_MSR_INAK) != 0U) && (initAckLoopCnt > 0U))
  {
    initAckLoopCnt--;
  }

  // Update connection state flag.
  m_Connected = TBX_TRUE;
}


///**************************************************************************************
/// \brief     Disconnects from the CAN bus.
///
///**************************************************************************************
void BxCan::disconnect()
{
  // Only continue if actually connected.
  if (m_Connected == TBX_TRUE)
  {
    // Update connection state flag.
    m_Connected = TBX_FALSE;

    // TODO ##Vg Implement disconnect().

  }
}


///**************************************************************************************
/// \brief     Submits a message for transmission on the CAN bus.
/// \param     t_Msg The message to transmit.
/// \return    TBX_OK if successful, TBX_ERROR otherwise.
///
///**************************************************************************************
uint8_t BxCan::transmit(CanMsg& t_Msg)
{
  uint8_t result = TBX_ERROR;

  // TODO ##Vg Implement transmit().

  // Give the result back to the caller.
  return result;
}


///**************************************************************************************
/// \brief     CAN communication event task function.
///
///**************************************************************************************
void BxCan::Run()
{
  for (;;)
  {
    // TODO ##Vg Implement event loop.
    Delay(50U);
  }
}


///**************************************************************************************
/// \brief     CAN communication interrupt service routine.
///
///**************************************************************************************
void BxCan::processInterrupt()
{
  // TODO ##Vg Implement processInterrupt()
}


///**************************************************************************************
/// \brief     Helper function to find appropriate bit timing settings to the requested
///            baudrate configuration and taking into account the clock frequency that
///            drives the CAN controller.
/// \param     t_Prescaler Prescaler value for the baudrate generator [1..1024].
/// \param     t_Tseg1 Time segment 1 bit value [1..16].
/// \param     t_Tseg2 Time segment 2 bit value [1..8].
/// \return    TBX_OK if fitting bit timing settings could be found, TBX_ERROR otherwise.
///
///**************************************************************************************
uint8_t BxCan::findBitTimingSettings(uint16_t& t_Prescaler, uint8_t& t_Tseg1,
                                  uint8_t& t_Tseg2)
{
  uint8_t result = TBX_ERROR;
  // Bit time in time quanta (TQ) = 1 (for SYNC) + tseg1 + tseg2. Given the possible
  // range for tseg1 and tseg2 this means:
  // - bitTimeMaxTQ = 1 + 16 + 8 = 25 TQ
  // - bitTimeMinTQ = 1 + 1 + 1 = 3 TQ
  // Just keep in mind the tseg ranges that the hardware supports:
  // - tseg1 = 1..16
  // - tseg2 = 1..8
  //
  // The following table lists the different configuration options for each supported
  // total number of TQ in a bit, such that tseg1 and tseg2 result in a sample point
  // (SP) that is as close to 75% as possible, as this is the preferred sample point for
  // protocols such as CANopen and DeviceNet. Made static to lower ROM and stack load.
  static const struct
  {
    uint8_t tq;
    uint8_t tseg1;
    uint8_t tseg2;
  } bitTimingTbl[] =
  {
    { 23U, 16U,  6U },  // SP = 100 - (6 / 23) * 100 = 74%
    { 22U, 16U,  5U },  // SP = 100 - (5 / 22) * 100 = 77%
    { 21U, 15U,  5U },  // SP = 100 - (5 / 21) * 100 = 76%
    { 20U, 14U,  5U },  // SP = 100 - (5 / 20) * 100 = 75%
    { 19U, 13U,  5U },  // SP = 100 - (5 / 19) * 100 = 74%
    { 18U, 13U,  4U },  // SP = 100 - (4 / 18) * 100 = 78%
    { 17U, 12U,  4U },  // SP = 100 - (4 / 17) * 100 = 76%
    { 16U, 11U,  4U },  // SP = 100 - (4 / 16) * 100 = 75%
    { 15U, 10U,  4U },  // SP = 100 - (4 / 15) * 100 = 72%
    { 14U, 10U,  3U },  // SP = 100 - (3 / 14) * 100 = 79%
    { 13U,  9U,  3U },  // SP = 100 - (3 / 13) * 100 = 77%
    { 12U,  8U,  3U },  // SP = 100 - (3 / 12) * 100 = 75%
    { 11U,  7U,  3U },  // SP = 100 - (3 / 11) * 100 = 73%
    { 10U,  7U,  2U },  // SP = 100 - (2 / 10) * 100 = 80%
    {  9U,  6U,  2U },  // SP = 100 - (2 /  9) * 100 = 78%
    {  8U,  5U,  2U },  // SP = 100 - (2 /  8) * 100 = 75%
    {  7U,  4U,  2U },  // SP = 100 - (2 /  7) * 100 = 71%
    {  6U,  4U,  1U },  // SP = 100 - (1 /  6) * 100 = 83%
    {  5U,  3U,  1U },  // SP = 100 - (1 /  5) * 100 = 80%
    {  4U,  3U,  1U },  // SP = 100 - (1 /  4) * 100 = 75%
    {  3U,  2U,  1U }   // SP = 100 - (1 /  3) * 100 = 67%
  };
  constexpr size_t     bitTimeTblSize = sizeof(bitTimingTbl)/sizeof(bitTimingTbl[0]);
  constexpr uint16_t   maxPrescaler = 1024U;
  constexpr uint16_t   minPrescaler = 1U;
  const uint32_t       baudrate = static_cast<uint32_t>(m_Baudrate);
  size_t               tblIdx;
  uint16_t             prescaler;
  LL_RCC_ClocksTypeDef rccClocks{ };

  // Determine the speed of the clock that drives the CAN peripheral.
  LL_RCC_GetSystemClocksFreq(&rccClocks);
  TBX_ASSERT(rccClocks.PCLK1_Frequency != 0);
  uint32_t canClockFreqHz = rccClocks.PCLK1_Frequency;

  // Search for a fitting prescaler by looping through all its possible values.
  for (prescaler = minPrescaler; prescaler <= maxPrescaler; prescaler++)
  {
    // Determine if this prescaler, combined with a TQ per bit results in an exact match
    // for the requested baudrate, while taking into account that a larger amount of TQ
    // per bit, allows for a better sample point configuration.
    for (tblIdx = 0U; tblIdx < bitTimeTblSize; tblIdx++)
    {
      // Exact baudrate match using this prescaler and TQ per bit configuration?
      if ((canClockFreqHz / prescaler / bitTimingTbl[tblIdx].tq) == baudrate)
      {
        // Found a match. Store the bit timing settings, update the result and break
        // this loop, since we are done.
        t_Prescaler = prescaler;
        t_Tseg1 = bitTimingTbl[tblIdx].tseg1;
        t_Tseg2 = bitTimingTbl[tblIdx].tseg2;
        result = TBX_OK;
        break;
      }
    }
    // Stop looping if a match was already found.
    if (result == TBX_OK)
    {
      break;
    }
  }
  // Give the result back to the caller.
  return result;
}


extern "C"
{
///**************************************************************************************
/// \brief     Interrupt service routine of the CAN transmitter.
///
///**************************************************************************************
void USB_HP_CAN_TX_IRQHandler(void)
{
  // Only continue if an instance of BxCan was actually created.
  if (BxCan::s_InstancePtr != nullptr)
  {
    // Pass the event on for further handling in the generic interrupt handler.
    BxCan::s_InstancePtr->processInterrupt();
  }
}


///**************************************************************************************
/// \brief     Interrupt service routine of the CAN reception FIFO0.
///
///**************************************************************************************
void USB_LP_CAN_RX0_IRQHandler(void)
{
  // Only continue if an instance of BxCan was actually created.
  if (BxCan::s_InstancePtr != nullptr)
  {
    // Pass the event on for further handling in the generic interrupt handler.
    BxCan::s_InstancePtr->processInterrupt();
  }
}


///**************************************************************************************
/// \brief     Interrupt service routine of the CAN reception FIFO1.
///
///**************************************************************************************
void CAN_RX1_IRQHandler(void)
{
  // Only continue if an instance of BxCan was actually created.
  if (BxCan::s_InstancePtr != nullptr)
  {
    // Pass the event on for further handling in the generic interrupt handler.
    BxCan::s_InstancePtr->processInterrupt();
  }
}


///**************************************************************************************
/// \brief     Interrupt service routine of the CAN error and status monitor.
///
///**************************************************************************************
void CAN_SCE_IRQHandler(void)
{
  // Only continue if an instance of BxCan was actually created.
  if (BxCan::s_InstancePtr != nullptr)
  {
    // Pass the event on for further handling in the generic interrupt handler.
    BxCan::s_InstancePtr->processInterrupt();
  }
}
} // extern "C"


//********************************** end of bxcan.cpp ***********************************
