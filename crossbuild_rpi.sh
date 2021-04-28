#!/bin/bash

if [ ! -d rpibuild ]; then
	mkdir rpibuild
fi

cd rpibuild

# toolchains/rpi_rootfs needs to contain a raspberry root filesystem, copy or mount sshfs of existing pi (toolchains/mount_rpi_rootfs.sh)
export RASPBERRY_VERSION=3

cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchains/clang-rpi.cmake