
H_FILES = \
	timer.h \
	display.h \
	receiver.h \
	channel.h \
	transmit.h \
	packet.h

CXX_FILES =\
	main.cc \
	timer.cc \
	display.cc \
	receiver.cc \
	channel.cc \
	transmit.cc \
	packet.cc

MODULE = demo

SOURCES = $(CXX_FILES) $(H_FILES)
SRCS = $(CXX_FILES)
OBJS = $(SRCS:.cc=.o)


#LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

#INCLUDES =  -I$(prefix)/include

include ./Makefile.defs
