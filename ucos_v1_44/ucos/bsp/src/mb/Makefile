#############################################################################
#                               uCOS XSDK BSP                               #
#                                                                           #
#        (c) Copyright 2014-2019; Silicon Laboratories Inc.,                #
#               400 W. Cesar Chavez, Austin, TX 78701                       #
#                                                                           #
#  All rights reserved. Protected by international copyright laws.          #
#                                                                           #
# Your use of this software is subject to your acceptance of the terms      #
# of a Silicon Labs Micrium software license, which can be obtained by      #
# contacting info@micrium.com. If you do not agree to the terms of this     #
# license, you may not use this software.                                   #
#                                                                           #
# Please help us continue to provide the Embedded community with the finest #
# software available. Your honesty is greatly appreciated.                  #
#                                                                           #
#   You can find our product's documentation at: doc.micrium.com            #
#                                                                           #
#         For more information visit us at: www.micrium.com                 #
#############################################################################

COMPILER=
ARCHIVER=
CP=cp
COMPILER_FLAGS=
EXTRA_COMPILER_FLAGS=
LIB=libucos.a

CC_FLAGS = $(COMPILER_FLAGS)
ECC_FLAGS = $(EXTRA_COMPILER_FLAGS)

RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I./. -I${INCLUDEDIR}

OUTS = *.o

LIBSOURCES:=ucos_startup.c \
            ucos_tcpip_init.c \
            ucos_stdinout.c \
            ucos_printf.c \
            mb/ucos_impl.c \
            mb/ucos_int_impl.c


INCLUDEFILES:=app_cfg.h \
              ucos_bsp.h \
              ucos_int.h \
              mb/ucos_int_impl.h
              

OBJECTS =	$(addsuffix .o, $(basename ${LIBSOURCES}))

libs: banner ucos_ucos_libs clean

%.o: %.c
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

%.o: %.s
	${COMPILER} $(CC_FLAGS) $(ECC_FLAGS) $(INCLUDES) -o $@ $<

banner:
	echo "Compiling uC/OS"

ucos_ucos_libs: ${OBJECTS}
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OBJECTS}

.PHONY: include
include:
	${CP} ${INCLUDEFILES} ${INCLUDEDIR}

clean:
	rm -rf ${OBJECTS}

