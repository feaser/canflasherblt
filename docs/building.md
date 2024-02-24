# Building from source code

This section contains detailed instructions on how to rebuild the CanFlasherBLT image from its source code. 

## Dependencies

It is assumed that [Git](https://git-scm.com/), [CMake](https://cmake.org/) and the [ARM GCC Embedded toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm) are installed on the system and available on the PATH. Additionally, the GNU tools *make* and *rm* should be installed and available on the path. Windows users can install the [GNU MCU Eclipse Windows Build Tools](https://gnu-mcu-eclipse.github.io/windows-build-tools/) for this. 

Installation instructions for a Debian/Ubuntu based system:

```
sudo apt install git cmake build-essential binutils-arm-none-eabi \
                 gcc-arm-none-eabi libstdc++-arm-none-eabi-newlib \
                 libnewlib-arm-none-eabi
```

## Getting the source code

To get the code, clone the GIT repository to a subdirectory, for example inside your own home directory. Make sure to update the submodules after cloning the GIT repository, because CanFlasherBLT makes use of a few submodules:

```
git clone https://github.com/feaser/canflasherblt.git ~/CanFlasherBLT
cd ~/CanFlasherBLT
git submodule update --init
```

## Building the image

To build the CanFlasherBLT image from its source code, run these commands:

```
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
```

Upon completion, you can find the newly created image file at:

* `~/CanFlasherBLT/build/source/board/olimexino_stm32f3/CanFlasherBLT_Olimexino_STM32F3.srec`

To program it onto the Olimexino STM32F3, refer to the [getting started](gettingstarted.md) section.





