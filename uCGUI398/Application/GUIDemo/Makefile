include $(TOPDIR)/config.mk

SRCS	:= $(wildcard *.c) $(wildcard *.C)
OBJS	:= $(SRCS:.c=.o) $(SRCS:.C=.o)

CURDIR	:= $(shell pwd)
FOLDER	:= $(notdir $(CURDIR))
LIB	:= lib$(FOLDER).a

.PHONY: all clean
all: .depend $(LIB)

$(LIB): $(OBJS)
	@$(AR) $(ARFLAGS) $@ $(OBJS)

clean:
	rm -f .depend *.o $(LIB)

#########################################################################
# defines $(obj).depend target
include $(TOPDIR)/rules.mk

sinclude .depend
#########################################################################
