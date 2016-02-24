

CC     = g++
OPT    = -O3
DEBUG  = -g
OTHER  = -Wall


#no debug
CFLAGS = $(OPT) $(OTHER) -Wno-deprecated

#uncomment this line for debug build
# CFLAGS = $(DEBUG) $(OTHER)


MODULE = run
# sources files separated by spaces
SRCS = WaveRead.cpp WaveWrite.cpp main.cpp

OBJS = $(SRCS:.cpp=.o)

# Set these variables according to your configuration
#SYSTEMC = /path/to/systemc
#SYSTEMCAMS = /path/to/systemc_ams
#TUV_LIB = /path/to/TUV_AMS_Library/output

# Target architecture eg. linux or linux64
TARGET_ARCH = linux64


INCDIR = -I. -I$(SYSTEMC)/include -I$(SYSTEMCAMS)/include -I$(TUV_LIB)/include
LIBDIR = -L$(SYSTEMC)/lib-$(TARGET_ARCH) -L$(SYSTEMCAMS)/lib-$(TARGET_ARCH) -L$(TUV_LIB)/lib

LIBS   = -lpthread -ltuv_ams_library -lsystemc-ams -lsystemc $(EXTRA_LIBS)


EXE    = $(MODULE).exe

.SUFFIXES: .cpp .o .x


$(EXE): $(OBJS) $(SYSTEMC)/lib-$(TARGET_ARCH)/libsystemc.a $(SYSTEMCAMS)/lib-$(TARGET_ARCH)/libsystemc-ams.a
	$(CC) $(CFLAGS) $(LIBDIR) -o $@ $(OBJS) $(LIBS)


all: $(EXE)	
	
.cpp.o:
	$(CC) $(CFLAGS) $(INCDIR) -c $<

clean::
	rm -f $(OBJS) *~ $(EXE) 
