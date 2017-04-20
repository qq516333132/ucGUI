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

TOPDIR		:= $(shell pwd)
export TOPDIR

SUBDIRS += $(TOPDIR)/GUI/AntiAlias  $(TOPDIR)/GUI/ConvertColor \
	   $(TOPDIR)/GUI/ConvertMono  $(TOPDIR)/GUI/Core \
	   $(TOPDIR)/GUI/Font  $(TOPDIR)/GUI/LCDDriver \
	   $(TOPDIR)/GUI/MemDev  $(TOPDIR)/GUI/MultiLayer \
	   $(TOPDIR)/GUI/Widget  $(TOPDIR)/GUI/WM
SUBDIRS += $(TOPDIR)/Simulation  $(TOPDIR)/Application/GUIDemo

Project := ucgui

include config.mk

FOLDERS	:= $(notdir $(SUBDIRS))
LIBS	:= $(addsuffix .a, $(addprefix /lib, $(FOLDERS)))
__LIBS	:= $(join $(SUBDIRS), $(LIBS))

ALL = $(Project).bin $(Project).dis $(Project).srec 

.PHONY: all clean depend $(SUBDIRS)

all: $(ALL)
$(Project).hex: $(Project).elf
	@echo "creating hex file $@ from $<..."
	@$(OBJCOPY) ${OBJCFLAGS} -O ihex $< $@

$(Project).srec: $(Project).elf
	@echo "creating srec file $@ from $<..."
	@$(OBJCOPY) ${OBJCFLAGS} -O srec $< $@

$(Project).bin: $(Project).elf
	@echo "creating bin file $@ from $<..."
	@$(OBJCOPY) ${OBJCFLAGS} -R .mmudata -R .no_cache -S -O binary $< $@	

$(Project).dis: $(Project).elf
	@echo "creating dis file $@ from $<..."
	@$(OBJDUMP) -d $< > $@

$(Project).elf: depend $(SUBDIRS) $(LDSCRIPT)
	@echo "Linking..."
	@$(LD) $(LDFLAGS) --start-group $(__LIBS) $(PLATFORM_LIBS) --end-group -Map $(Project).map -o $(Project).elf

depend:
	@echo "Build target $(Project)"
	@for dir in $(SUBDIRS); do $(MAKE) --no-print-directory -C $$dir _depend; done

$(SUBDIRS):
	@$(MAKE) -C $@ all

clean:
	rm -f $(Project).elf $(Project).map $(Project).srec $(Project).bin $(Project).dis
	@for dir in $(SUBDIRS); do $(MAKE) -C $$dir clean; done



