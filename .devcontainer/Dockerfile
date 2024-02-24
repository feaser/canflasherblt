# Container base
FROM mcr.microsoft.com/devcontainers/base:ubuntu-22.04

# Configuration
ARG TOOLS_PATH=/opt/gcc-arm-none-eabi

# Update package repositories
RUN apt-get update

# Install package dependencies
RUN apt-get install build-essential wget git cmake openocd libncurses5 usbutils -y

# Download and install ARM GNU Embedded toolchain
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.gz
RUN mkdir ${TOOLS_PATH}
RUN tar xf gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.gz --strip-components=1 -C ${TOOLS_PATH}
RUN rm gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.gz

# Add ARM GNU Embedded toolchain binaries directory to the path
ENV PATH="$PATH:${TOOLS_PATH}/bin"
