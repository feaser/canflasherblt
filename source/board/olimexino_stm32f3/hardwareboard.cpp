///**************************************************************************************
/// \file         hardwareboard.cpp
/// \brief        Hardware specific board support package source file.
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
#include "hardwareboard.hpp"
#include "logger.hpp"
#include "thread.hpp"
#include "critical.hpp"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_utils.h"


///**************************************************************************************
/// \brief     Board support package constructor.
/// \details   Note that this class builds on the concept of polymorphing to create an
///            abstraction layer between the hardware dependent parts and the hardware
///            independent parts. As an example, look at the status LED. It's of type
///            StatusLed, but the statusLed() getter returns a reference to its Led
///            parent type.
///            
///            The board specfic objects are created dynamically on the heap, otherwise
///            their constructors run right before this one. Some of these objects depend
///            on the configuration made by mcuInit(). By instantiating them on the heap,
///            you can better control and make sure that their constructors run after the
///            call to mcuInit(), where applicable.
///
///**************************************************************************************
HardwareBoard::HardwareBoard()
  : Board()
{
  // Register the board specific assertion handler.
  TbxAssertSetHandler(BoardAssertHandler);  
  // Initialize the microcontroller.
  mcuInit();
  // Create the status LED object on the heap.
  m_StatusLed = std::make_unique<StatusLed>();
  // Create the TinyUSB device object on the heap.
  m_TinyUsbDevice = std::make_unique<TinyUsbDevice>();
}


///**************************************************************************************
/// \brief     Performs a software reset of the microcontroller.
///
///**************************************************************************************
void HardwareBoard::reset()
{
  NVIC_SystemReset();
}


///**************************************************************************************
/// \brief     Board specific assertion handler.
/// \param     file The filename of the source file where the assertion occurred in.
/// \param     line The line number inside the file where the assertion occurred.
///
///**************************************************************************************
void HardwareBoard::assertHandler(const char * const file, uint32_t line)
{
  // Disable the interrupts.
  cpp_freertos::CriticalSection::DisableInterrupts();

  // Update the event log.
  logger().error("Assertion at line %d in %s.", line, file);

  /* Hang the program by entering an infinite loop. The values for file and line can
   * then be inspected with the debugger to locate the source of the run-time assertion.
   */
  for (;;)
  {
    ;
  }
}


///**************************************************************************************
/// \brief     Initializes the microcontroller. Handles setting up the clocks and 
///            interrupts. Note that it does not start the SysTick, because FreeRTOS
///            handles that, when the schedular starts.
/// \details   Notes about interrupt priorities in combination with FreeRTOS and STM32 
///            Cortex-M4:
///
///            - 16 interrupt priority levels: 0..15. Lower number means higher priority.
///            - Only use priority levels 5..15 for interrupts that make use of FreeRTOS
///              API functions, either directly or indirectly. The number 5 comes from
///              configuration macro configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY.
///            - Priority levels 0..4 are available for interrupts that do not call
///              FreeRTOS APIs.
///            - The default interrupt priority level after reset is 0, so highest 
///              priority. Make sure to change this for those interrupts the use FreeRTOS
///              APIs.
///
///            Additional information: https://www.freertos.org/RTOS-Cortex-M3-M4.html
///
///**************************************************************************************
void HardwareBoard::mcuInit()
{
  LL_GPIO_InitTypeDef GPIO_InitStruct{ };

  // SYSCFG and PWR clock enable.
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  // Remap USB interrupts lines from 19, 20 and 42 to 74, 75, 76. This way they no longer
  // overlap with the CAN interrupt lines. With the remap enabled, use the following
  // interrupt handlers for USB interrupts:
  //   - USB_HP_IRQHandler, USB_LP_IRQHandler and USBWakeUp_RMP_IRQHandler.
  // Otherwise use these:
  //   - USB_HP_CAN_TX_IRQHandler, USB_LP_CAN_RX0_IRQHandler and USBWakeUp_IRQHandler.
  LL_SYSCFG_EnableRemapIT_USB();

  // GPIO port and peripheral clocks enable.
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USB);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_CAN);
#if ( configGENERATE_RUN_TIME_STATS == 1 )
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
#endif 

  // Out of reset, the Olimexino-STM32F3 board enables a pull-up on the USB_DP line. If
  // the board already enumerated, then it might stay in that state, even after a reset.
  // It is therefore best to first make sure the USB device disconnects from the USB
  // host. This is done by configuring USB DISC (PC12) as a digital output and setting
  // it logic high. This turns the P-MOSFET off, which disables the pull-up on the USB_DP
  // line.
  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_12);
  GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // Set interrupt group priority. Needs to be NVIC_PRIORITYGROUP_4 for FreeRTOS.
  NVIC_SetPriorityGrouping(0x00000003U);

  // MemoryManagement_IRQn interrupt configuration.
  NVIC_SetPriority(MemoryManagement_IRQn, 0);
  // BusFault_IRQn interrupt configuration.
  NVIC_SetPriority(BusFault_IRQn, 0);
  // UsageFault_IRQn interrupt configuration.
  NVIC_SetPriority(UsageFault_IRQn, 0);
  // SVCall_IRQn interrupt configuration.
  NVIC_SetPriority(SVCall_IRQn, 0);
  // DebugMonitor_IRQn interrupt configuration.
  NVIC_SetPriority(DebugMonitor_IRQn, 0);
  // PendSV_IRQn interrupt configuration.
  NVIC_SetPriority(PendSV_IRQn, 15);
  // SysTick_IRQn interrupt configuration.
  NVIC_SetPriority(SysTick_IRQn, 15);  

  // USB related interrupt configuration.
  NVIC_SetPriority(USB_HP_IRQn, 10);
  NVIC_SetPriority(USB_LP_IRQn, 10);
  NVIC_SetPriority(USBWakeUp_RMP_IRQn, 10);
  
  // CAN related interrupt configuration.
  NVIC_SetPriority(USB_HP_CAN_TX_IRQn, 10);
  NVIC_SetPriority(USB_LP_CAN_RX0_IRQn, 10);
  NVIC_SetPriority(CAN_RX1_IRQn, 10);
  NVIC_SetPriority(CAN_SCE_IRQn, 10);

  // Configure the system clock from reset.
  setupSystemClock();
}


///**************************************************************************************
/// \brief     System Clock Configuration. This code was created by CubeMX and configures
///            the system clock.
/// \details   Configures the PLL to be sourced by the 16 MHz HSE crystal oscillator:
///            - fPLLCLK  = 72 MHz
///            - fSYSCLK  = 72 MHz
///            - fHCLK    = 72 MHz
///            - fAHB     = 72 MHz
///            - fSYSTICK = 72 MHz
///            - fFCLK    = 72 MHz
///            - fAPB1    = 36 MHz
///            - fAPB1TIM = 72 MHz
///            - fAPB2    = 72 MHz
///            - fAPB2TIM = 72 MHz
///            - fHSE     =  8 MHz
///            - fUSB     = 48 MHz (fPLLCLK / 1.5)
///            - fCAN     = 36 MHz (fAPB1)
///
///**************************************************************************************
void HardwareBoard::setupSystemClock()
{
  // Configure the flash latency.
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
    ;
  }

  // Enabled the external high speed oscillator circuit.
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
    ;
  }

  // Configure and enable the PLL.
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
    ;
  }

  // Configure peripheral bus prescalers.
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  // Select the PLL as the system clock.
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
    ;
  }

  // Update the system clock speed setting.
  LL_SetSystemCoreClock(72000000UL);

  // Select peripheral clock sources.
  LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL_DIV_1_5);
}


extern "C"
{
///**************************************************************************************
/// \brief     Board specific assertion handler with C linkage, such that it can be
///            configured with TbxAssertSetHandler().
/// \param     file The filename of the source file where the assertion occurred in.
/// \param     line The line number inside the file where the assertion occurred.
///
///**************************************************************************************
void BoardAssertHandler(const char * const file, uint32_t line)
{
  // Pass the event on to the static method of the board class.
  HardwareBoard::assertHandler(file, line);
}

} // extern "C"

//********************************** end of hardwareboard.cpp ***************************
