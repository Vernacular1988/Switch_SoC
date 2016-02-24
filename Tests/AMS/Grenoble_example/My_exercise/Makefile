#
# Modified style to use Makefile.defs copied from systemc tree 
# levitan@pitt.edu

H_FILES = \
	comb_filter.h \
	prefi.h \
	sadc.h \

CXX_FILES =\
	dec_path_tb.cpp \

#	comb_filter.cpp \


MODULE = dec_path_tb

SOURCES = $(CXX_FILES) $(H_FILES)
SRCS = $(CXX_FILES)
OBJS = $(SRCS:.cpp=.o)

include ../Makefile.defs

# remove # 
#SRCS = sin_src.cpp #\
#SRCS	dec_path_tb.cpp \
#	prefi.cpp \
#	comb_filter.cpp \
#	sadc.cpp
#
#
#SRCS_EXE = sin_src_tb.cpp  #\
#SRCS_EXE = dec_path_tb.cpp



