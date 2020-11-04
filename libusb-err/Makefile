#
# Author: Zhenxing Xu <xzxlnmail.163.com>
#
# This is free and unencumbered software released into the public domain.
# For details see the UNLICENSE file at the root of the source tree.
#

DATE := $(shell date +%Y-%m-%d)
GIT_VER := $(shell git rev-parse HEAD | cut -c 1-6)

CC = gcc
LD = gcc
CCFLAGS = -W -llibusb
TARGET = ft2232hl

INCLUDES = -I. -I/usr/include/libusb-1.0
CSRCS = test_loragw_sx1302_spi.c loragw_sx1302_spi.c ftdi.c fast.c support.c mpsse.c

OBJS := $(addsuffix .o,$(basename $(CSRCS)))

all: print ${OBJS}
	@echo ${OBJS}
	$(CC) -o $(TARGET) $(OBJS)
	@echo "Compile Done"
	rm ${OBJS}

print:
	@echo ${DATE}
	@echo ${GIT_VER}

# $@: target files
# $^: all dependence files
# $<: first dependence file
%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<
