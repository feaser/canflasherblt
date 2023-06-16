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
/// SPDX-License-Identifier: GPL-3.0-or-later
///
/// This program is free software: you can redistribute it and/or modify it under the 
/// terms of the GNU General Public License as published by the Free Software Foundation,
/// either version 3 of the License, or (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful, but WITHOUT ANY
/// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
/// PARTICULAR PURPOSE. See the GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License along with this
/// program. If not, see <https://www.gnu.org/licenses/>.
///
/// \endinternal
///**************************************************************************************

//***************************************************************************************
// Include files
//***************************************************************************************
#include "microtbx.h"
#include "hardwareboard.hpp"
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
}


///**************************************************************************************
/// \brief     Board specific assertion handler.
/// \param     file The filename of the source file where the assertion occurred in.
/// \param     line The line number inside the file where the assertion occurred.
///
///**************************************************************************************
void HardwareBoard::assertHandler(const char * const file, uint32_t line)
{
  /* Disable interrupts. */
  __disable_irq();

  /* Hang the program by entering an infinite loop. The values for file and line can
   * then be inspected with the debugger to locate the source of the run-time assertion.
   */
  for (;;)
  {
    ;
  }
} /*** end of assertHandler ***/


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
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USB);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_CAN);

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
