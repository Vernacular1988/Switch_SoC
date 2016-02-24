#
# Modified style to use Makefile.defs copied from systemc tree 
# levitan@pitt.edu

H_FILES = \
	size.h \
	con.h \
	osc.h \
	a_delay.h \
	a_vco.h \
	pll.h \

#	drain.h \
#	top.h \
#	split.h \

CXX_FILES =\
	main.cpp \
	osc.cpp \
	a_delay.cpp \
	a_vco.cpp \
	pll.cpp \
	con.cpp \

#	split.cpp \
#	top.cpp \

MODULE = main

SOURCES = $(CXX_FILES) $(H_FILES)
SRCS = $(CXX_FILES)
OBJS = $(SRCS:.cpp=.o)

include ../../Makefile.defs




