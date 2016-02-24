
H_FILES = \
	driver.h\
	cam.h \
	echo.h \
	constants.h\

CXX_FILES =\
	main.cc \
	driver.cc \
	cam.cc  \
	echo.cc \

MODULE = cam

SOURCES = $(CXX_FILES) $(H_FILES)
SRCS = $(CXX_FILES)
OBJS = $(SRCS:.cc=.o)


#LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

#INCLUDES =  -I$(prefix)/include

include ./Makefile.defs
