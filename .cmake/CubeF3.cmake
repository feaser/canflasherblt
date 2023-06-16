# Create interface library for the STM32 CubeF3 HAL sources.
add_library(cube_f3_hal INTERFACE)

target_sources(cube_f3_hal INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_adc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_adc_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_can.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_cec.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_comp.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_cortex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_crc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_crc_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_dac.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_dac_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_dma.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_exti.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_flash.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_flash_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_gpio.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_hrtim.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_i2c.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_i2c_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_i2s.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_i2s_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_irda.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_iwdg.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_nand.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_nor.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_opamp.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_opamp_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_pccard.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_pcd.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_pcd_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_pwr.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_pwr_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_rcc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_rcc_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_rtc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_rtc_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_sdadc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_smartcard.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_smartcard_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_smbus.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_spi.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_spi_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_sram.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_tim.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_tim_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_tsc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_uart.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_uart_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_usart.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_usart_ex.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_hal_wwdg.c"
)

target_include_directories(cube_f3_hal INTERFACE 
    "${CMAKE_SOURCE_DIR}/third_party/cmsis/core/CMSIS/Core/Include"
    "${CMAKE_SOURCE_DIR}/third_party/cmsis/device/st/f3/Include"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Inc"
)

target_compile_definitions(cube_f3_hal INTERFACE
    USE_HAL_DRIVER
)

# Create interface library for the STM32 CubeF3 LL sources.
add_library(cube_f3_ll INTERFACE)

target_sources(cube_f3_ll INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_adc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_comp.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_crc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_dac.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_dma.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_exti.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_fmc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_gpio.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_hrtim.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_i2c.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_opamp.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_pwr.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_rcc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_rtc.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_spi.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_tim.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_usart.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_usb.c"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Src/stm32f3xx_ll_utils.c"
)

target_include_directories(cube_f3_ll INTERFACE 
    "${CMAKE_SOURCE_DIR}/third_party/cmsis/core/CMSIS/Core/Include"
    "${CMAKE_SOURCE_DIR}/third_party/cmsis/device/st/f3/Include"
    "${CMAKE_SOURCE_DIR}/third_party/hal/st/f3/Inc"
)

target_compile_definitions(cube_f3_ll INTERFACE
    USE_FULL_LL_DRIVER
)

# Create convenience interface library for both the STM32 CubeF3 HAL and LL sources.
add_library(cube_f3 INTERFACE)

target_link_libraries(cube_f3 INTERFACE
    cube_f3_hal
    cube_f3_ll
)
