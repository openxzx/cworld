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
CCFLAGS = -W -g
LDFLAGS =
TARGET = ttyusb

INCLUDES = -I.
CSRCS = ttyusb.c

OBJS := $(addsuffix .o, $(basename $(CSRCS)))

all: print ${OBJS}
	@echo "Compile target ${OBJS} done."
	${LD} ${LDFLAF} ${OBJS} -o ${TARGET}
	rm ${OBJS}

print:
	@echo ${DATE}
	@echo ${GIT_VER}

# $@: target files
# $^: all dependence files
# $<: first dependence file
%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) -o $@ -c $<
