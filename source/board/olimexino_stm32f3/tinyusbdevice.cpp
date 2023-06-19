///**************************************************************************************
/// \file         tinyusbdevice.cpp
/// \brief        TinyUSB device source file.
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
#include "microtbx.h"
#include "tinyusbdevice.hpp"
#include "hardwareboard.hpp"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_exti.h"


//***************************************************************************************
// Static data declarations
//***************************************************************************************
/// \brief   Storage for the single TinyUsbDevice instance. 
/// \details Needed for mapping the TinyUSB callback functions to the processCallback()
///          instance method. Note that this only works if there will only be one
///          TinyUsbDevice instance. This is the case for this board, since its
///          microcontroller only exposes one USB device.
TinyUsbDevice* TinyUsbDevice::s_InstancePtr = nullptr;


///**************************************************************************************
/// \brief     TinyUSB device constructor. 
///
///**************************************************************************************
TinyUsbDevice::TinyUsbDevice(HardwareBoard& t_HardwareBoard)
  : UsbDevice(),
    cpp_freertos::Thread("UsbDeviceThread", configMINIMAL_STACK_SIZE + 32, 6),
    m_HardwareBoard(t_HardwareBoard)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct{ };

  // Verify that only one instance of TinyUsbDevice is created.
  TBX_ASSERT(s_InstancePtr == nullptr);
  // Store a pointer to ourselves.
  s_InstancePtr = this;

  // Configure USB GPIO pins. PA11 is USB_DM and PA12 is USB_DP.
  GPIO_InitStruct.Pin = LL_GPIO_PIN_11 | LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_14;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Enable EXTI Line for USB wakeup.
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_18);
  LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_18);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_18);
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_18);

  // Start the thread.
  Start();
}


///**************************************************************************************
/// \brief     TinyUSB device destructor. 
///
///**************************************************************************************
TinyUsbDevice::~TinyUsbDevice()
{
  // Disable EXTI Line for USB wakeup.
  LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_18);
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_18);

  // Disconnect the device from the USB host by setting USB DISC (PC12) high. This turns
  // the P-MOSFET off, which disables the pull-up on the USB_DP line.
  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_12);

  // Reset the instance pointer.
  s_InstancePtr = nullptr;
}


///**************************************************************************************
/// \brief     Submits data for transmission on the USB bulk endpoint.
/// \param     t_Data Byte array with data to transmit.
/// \param     t_Len Number of bytes from the array to transmit.
/// \return    TBX_OK if successful, TBX_ERROR otherwise.
///
///**************************************************************************************
uint8_t TinyUsbDevice::transmit(uint8_t const t_Data[], uint32_t t_Len)
{
  uint8_t result = TBX_ERROR;

  // Verify the parameters.
  TBX_ASSERT((t_Data != nullptr) && (t_Len > 0));

  // Only continue with valid parameters.
  if ((t_Data != nullptr) && (t_Len > 0))
  {
    // Store the data in the transmit FIFO.
    if (tud_vendor_write(t_Data, t_Len) == t_Len)
    {
      // Request transmission start of the data currently stored in the transmit
      // FIFO. No need to check the return value, because worst case the endpoint
      // is already busy with a transfer. That's okay, because the data was already
      // successfully stored in the transmit FIFO, meaning that it will go out
      // eventually, since TinyUSB checks at the end of an endpoint transfer if data
      // is still left in the transmit FIFO. If so, it automatically starts the next
      // endpoint transfer.
      (void)tud_vendor_flush();
      // Update the result.
      result = TBX_OK;
    }
  }
  /* Give the result back to the caller. */
  return result;
}


///**************************************************************************************
/// \brief     Runs the TinyUSB device task.
///
///**************************************************************************************
void TinyUsbDevice::Run()
{
  // Connect the device to the USB host by setting USB DISC (PC12) low. This turns the
  // P-MOSFET on, which enables the pull-up on the USB_DP line.
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_12);

  // Initialize TinyUSB device stack on the configured roothub port. Should be called 
  // after the kernel is started. This is because it enables the USB interrupts and 
  // these use FreeRTOS API calls.
  tud_init(BOARD_TUD_RHPORT);

  // Enter the task body, which should be an infinite loop.
  for (;;)
  {
    // Run the TinyUSB device stack. Note that this puts the task in the waiting state
    // until the arrival of a new event. This assumed that CFG_TUSB_OS is properly
    // set to OPT_OS_FREERTOS.
    tud_task();
  }
}


///**************************************************************************************
/// \brief     Processes the triggered callback.
/// \param     t_CallbackId Identifier of the callback that was triggered and requires
///            possible further processing.
///
///**************************************************************************************
void TinyUsbDevice::processCallback(CallbackId t_CallbackId)
{
  // Filter on the ID of the callback.
  switch (t_CallbackId)
  {
    case RXNEWDATA:
    {
      // Retrieve the newly received data from the USB endpoint.
      uint32_t rxCount = tud_vendor_read(m_RxBuf.data(), m_RxBuf.size());
      // Only trigger the event handler if it was assigned and the data size is not zero.
      if ((onDataReceived) && (rxCount > 0))
      {
        onDataReceived(m_RxBuf.data(), rxCount);
      }
    }
    break;

    case SUSPEND:
    {
      // Trigger the event handler, if assigned.
      if (onSuspend)
      {
        onSuspend();
      }
      // Perform actual suspend.
      m_HardwareBoard.suspend();
    }
    break;

    case RESUME:
    {
      // Trigger the event handler, if assigned.
      if (onResume)
      {
        onResume();
      }
    }
    break;

    default:
    {
      // Not a valid callback ID.
      TBX_ASSERT(TBX_FALSE);
    }
    break;
  }
}


extern "C"
{
//***************************************************************************************
//           T I N Y U S B   C A L L B A C K   F U N C T I O N S
//***************************************************************************************
///**************************************************************************************
/// \brief     TinyUSB device callback function that gets called upon reception of new
///            data on the bulk endpoint.
/// \param     itf The USB interface number that triggered the event.
///
///**************************************************************************************
void tud_vendor_rx_cb(uint8_t itf)
{
  // Current implementation only supports one USB interface (number 0).
  if (itf == 0)
  {
    // Only continue if an instance of TinyUsbDevice was actually created.
    if (TinyUsbDevice::s_InstancePtr != nullptr)
    {
      // Call the instance's method for processing the callback.
      TinyUsbDevice::s_InstancePtr->processCallback(TinyUsbDevice::RXNEWDATA);
    }
  }
}


///**************************************************************************************
/// \brief     TinyUSB device callback function that gets called when the USB bus is
///            suspended. Within 7 milliseconds the device must draw an average of less
///            that 2.5 mA from the bus.
/// \param     remote_wakeup_en True if the host allows us to perform a remote wakeup.
///
///**************************************************************************************
void tud_suspend_cb(bool remote_wakeup_en)
{
  TBX_UNUSED_ARG(remote_wakeup_en);

  // Only continue if an instance of TinyUsbDevice was actually created.
  if (TinyUsbDevice::s_InstancePtr != nullptr)
  {
    // Call the instance's method for processing the callback.
    TinyUsbDevice::s_InstancePtr->processCallback(TinyUsbDevice::SUSPEND);
  }
}


///**************************************************************************************
/// \brief     TinyUSB device callback function that gets called when the USB bus is
///            resumed.
///
///**************************************************************************************
void tud_resume_cb(void)
{
  // Only continue if an instance of TinyUsbDevice was actually created.
  if (TinyUsbDevice::s_InstancePtr != nullptr)
  {
    // Call the instance's method for processing the callback.
    TinyUsbDevice::s_InstancePtr->processCallback(TinyUsbDevice::RESUME);
  }
}


//***************************************************************************************
//           I N T E R R U P T   S E R V I C E   R O U T I N E S
//***************************************************************************************
///**************************************************************************************
/// \brief     This function handles USB high priority interrupt remap.
///
///**************************************************************************************
void USB_HP_IRQHandler(void)
{
  // Pass the event on to the TinyUSB device stack on the configured roothub port.
  tud_int_handler(BOARD_TUD_RHPORT);
}


///**************************************************************************************
/// \brief     This function handles USB low priority interrupt remap.
///
///**************************************************************************************
void USB_LP_IRQHandler(void)
{
  // Pass the event on to the TinyUSB device stack on the configured roothub port.
  tud_int_handler(BOARD_TUD_RHPORT);
}


///**************************************************************************************
/// \brief     This function handles USB wake-up interrupt through EXTI line 18.
///
///**************************************************************************************
void USBWakeUp_RMP_IRQHandler(void)
{
  // Clear the EXTI Line for USB wakeup interrupt flag.
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_18);

  // Only continue if an instance of TinyUsbDevice was actually created.
  if (TinyUsbDevice::s_InstancePtr != nullptr)
  {
    // Make sure perform resume action right here in the interrupt and not in 
    // tud_resume_cb(), because at this point the system clocks are not yet properly
    // configured. This means that peripherals and timers can not yet be fully accessed.
    // Callback tud_resume_cb() is called at task level, but the RTOS scheduler does
    // not yet run.
    TinyUsbDevice::s_InstancePtr->m_HardwareBoard.resume();
  }

  // Pass the event on to the TinyUSB device stack on the configured roothub port.
  tud_int_handler(BOARD_TUD_RHPORT);
}
} // extern "C"


//********************************** end of tinyusbdevice.cpp ***************************
