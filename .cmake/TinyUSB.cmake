# Create interface library for the TinyUSB sources.
add_library(tinyusb INTERFACE)

target_sources(tinyusb INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/tusb.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/audio/audio_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/bth/bth_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/cdc/cdc_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/cdc/cdc_host.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/cdc/cdc_rndis_host.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/dfu/dfu_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/hid/hid_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/hid/hid_host.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/midi/midi_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/msc/msc_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/msc/msc_host.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/net/ecm_rndis_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/net/ncm_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/usbtmc/usbtmc_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/vendor/vendor_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/vendor/vendor_host.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/video/video_device.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/common/tusb_fifo.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/device/usbd.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/device/usbd_control.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/host/hub.c"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/host/usbh.c"
)

target_include_directories(tinyusb INTERFACE 
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/audio"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/bth"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/cdc"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/dfu"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/hid"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/midi"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/msc"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/net"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/usbtmc"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/vendor"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/class/video"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/common"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/device"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/host"
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/osal"
)

# Create interface library for the TinyUSB STM32F3 port sources.
add_library(tinyusb_stm32f3 INTERFACE)

target_sources(tinyusb_stm32f3 INTERFACE
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c"
)

target_include_directories(tinyusb_stm32f3 INTERFACE 
    "${CMAKE_SOURCE_DIR}/third_party/tinyusb/src/portable/st/stm32_fsdev"
)

target_compile_definitions(tinyusb_stm32f3 INTERFACE
    CFG_TUSB_MCU=OPT_MCU_STM32F3
)
