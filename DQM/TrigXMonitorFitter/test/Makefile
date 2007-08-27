#
# Makefile for rootsnns.
#

ARCH         := $(shell root-config --arch)

ObjSuf        = o
SrcSuf        = cc
DllSuf        = so
OutPutOpt     = -o

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)


# gcc flags
# '-m32' on line 26  (1of2) allows fitter to run on 64-bit machines
CXX           = g++
LD            = g++
CXXFLAGS      = -O -Wall -Wno-deprecated -m32

##  # Kai flags
##  CXX           = KCC
##  LD            = KCC
##  CXXFLAGS      = -O

# '-m32' (2of2) line 34 allows fitter to run on 64-bit machines by properly linking to 32-bit root libraries
LDFLAGS       = -O  -m32
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
## LIBS          = $(ROOTLIBS) -lMinuit $(SYSLIBS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
LDFLAGS      += $(shell root-config --nonew)

#------------------------------------------------------------------------------

# general objects
# these are compiled regardless of which executable(s) wanted
OBJS               = ResidualStat.o TriggerLineInfo.o 

# Single XMon Fitter
OBJS_STXMF          = ${OBJS} SingleTXMonFitter.o
PROGRAM_STXMF       = SingleTXMonFitter.exe

# All Trigger Lines XMon Fitter
OBJS_ATXMF          = ${OBJS} AllTXMonFitter.o
PROGRAM_ATXMF       = AllTXMonFitter.exe


#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)


# the command 'gmake all' will compile .o files, AllXMonFitter.exe and SingleXMonFitter.exe
all:  $(PROGRAM_STXMF) $(PROGRAM_ATXMF) 

# the command 'gmake single' will compile .o files and SingleXMonFitter.exe
single: $(PROGRAM_STXMF) 

# the command 'gmake multi' will compile .o files and AllXMonFitter.exe
multi:  $(PROGRAM_ATXMF) 


$(PROGRAM_STXMF): $(OBJS_STXMF)
		$(LD) $(LDFLAGS) $(OBJS_STXMF) $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

$(PROGRAM_ATXMF): $(OBJS_ATXMF)
		$(LD) $(LDFLAGS) $(OBJS_ATXMF) $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"


# the command 'gmake clean' removes all files with the following extensions:
clean:
		@rm -f core *~ *.o *.d *.exe

# the command 'gmake distclean' removes all files with the following extensions:
distclean: clean
		@rm -f $(PROGRAM_CB) *_dict.* *.def *.exp \
		   *.root *.ps *.so .def *.o *.d so_locations
		@rm -rf cxx_repository


# tells how to compile .cc files into .o files
.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
