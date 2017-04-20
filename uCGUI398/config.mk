#
# Copyright (C)  Dinghong Huang(1048272975@qq.com)
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

TEXT_BASE=0x30000000
export TEXT_BASE

CROSS_COMPILE   ?= arm-linux-gnueabi-
export CROSS_COMPILE

DEBUG = n

AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump
export	AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP

AFLAGS := 

ARFLAGS := cr

CFLAGS := -Wall -Wstrict-prototypes -mcpu=arm920t -msoft-float -lc -nostdlib
CFLAGS +=
CFLAGS += -I $(TOPDIR)/GUI/Core -I $(TOPDIR)/GUI/Widget \
	  -I $(TOPDIR)/GUI/WM -I $(TOPDIR)/Config
CFLAGS += -I $(TOPDIR)/GUIDemo
ifeq ($(DEBUG), y)
CFLAGS += -g
else
CFLAGS += -O2
endif

PLATFORM_LIBS := 
# 加入uclibc标准c库以及数学函数库
PLATFORM_LIBS += -L/usr/arm-linux-gnueabi/lib -lm -lc 
# 加入gcc库
PLATFORM_LIBS += -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc
##LDSCRIPT = $(TOPDIR)/start_code/s3c2416.lds
##LDFLAGS := -Bstatic -T $(LDSCRIPT) -Ttext $(TEXT_BASE)

export LDFLAGS AFLAGS CFLAGS ARFLAGS PLATFORM_LIBS

%.o: %.S
	@echo "assembling $<..."
	@$(CC) $(AFLAGS) -c -o $@ $<
%.o: %.s
	@echo "assembling $<..."
	@$(CC) $(AFLAGS) -c -o $@ $<
%.o: %.c
	@echo "compiling $<..."
	@$(CC) $(CFLAGS) -c  -o $@ $<
%.o: %.C
	@echo "compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<















