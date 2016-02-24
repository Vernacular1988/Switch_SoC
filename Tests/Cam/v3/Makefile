
H_FILES = \
	driver.h\
	cam.h \
	echo.h \
#	cam_array.h \

CXX_FILES =\
	main.cc \
	driver.cc \
	cam.cc  \
	echo.cc \
#	cam_array.cc \

MODULE = cam

SOURCES = $(CXX_FILES) $(H_FILES)
SRCS = $(CXX_FILES)
OBJS = $(SRCS:.cc=.o)


#LDADD = -L$(prefix)/lib-$(TARGET_ARCH) -lsystemc -lm

#INCLUDES =  -I$(prefix)/include

include ./Makefile.defs
