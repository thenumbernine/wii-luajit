#!/bin/sh
make HOST_CC="gcc-mp-4.6 -m32" \
    CROSS="$DEVKITPRO/devkitPPC/bin/powerpc-eabi-" \
    TARGET_CFLAGS="-DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -g -O2 -Wall " \
    TARGET=ppcspe \
    TARGET_SYS=Wii
    
#also set the following inside src/Makefile
#BUILDMODE= static
#XCFLAGS+= -DLUAJIT_USE_SYSMALLOC
