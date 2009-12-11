# ======================================================================
# NOTE: make all
# -----
#
# ======================================================================


ifdef ROOTSYS
  ROOTCINT      = $(ROOTSYS)/bin/rootcint
  ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
  ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
  ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)
else 
  ROOTCINT      = rootcint
  ROOTCFLAGS    = $(shell root-config --cflags)
  ROOTLIBS      = $(shell root-config --libs)
  ROOTGLIBS     = $(shell root-config --glibs)
endif

ifdef SCRAM_ARCH
  CXX         := $(shell scramv1 tool info cxxcompiler | grep CXX= | sed s/CXX=//)
else 
  ifdef CXXCOMPILER
    CXX         := $(CXXCOMPILER)
  else 
    CXX         := c++
  endif
endif

CXXFLAGS      = -g -Wall -fPIC
LD            = $(CXX)
LDFLAGS       = -g
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
CXXFLAGS     += -I../../../AnalysisDataFormats/HeavyFlavorObjects/
LIBS          = $(ROOTLIBS)
#GLIBS         = $(ROOTGLIBS)
GLIBS         = $(filter-out -lz, $(ROOTGLIBS))

CINTFLAGS     = -I../../../AnalysisDataFormats/HeavyFlavorObjects -I../../../AnalysisDataFormats/HeavyFlavorObjects/test

LIBANA        = ../../../AnalysisDataFormats/HeavyFlavorObjects/lib/libAna00.so

$(addprefix obj/,%.o) : %.cc 
	$(CXX) $(CXXFLAGS) -c $< -o $@


READER     = treeReader01.o treeReader01Dict.o \
             myReader01.o myReader01Dict.o

ANACLASSES = ana.o anaDict.o 

# ================================================================================
all: 
# --
	@$(MAKE) reader
	@$(MAKE) runMyReader01

# ================================================================================
reader: $(addprefix obj/,$(READER))
# ---------------------------------
	$(CXX) $(SOFLAGS) -o lib/libReaderClasses.so $(addprefix obj/,$(READER)) 

# ----------------------------------------------------------------------
myReader01Dict.cc: myReader01.hh
	$(ROOTCINT) -f myReader01Dict.cc -c $(CINTFLAGS) myReader01.hh 

# ----------------------------------------------------------------------
treeReader01Dict.cc: treeReader01.hh
	$(ROOTCINT) -f treeReader01Dict.cc -c $(CINTFLAGS) treeReader01.hh 

# ----------------------------------------------------------------------
runMyReader01: obj/runMyReader01.o
	$(LD) $(LDFLAGS)  -o bin/runMyReader01 $(GLIBS) $(LIBANA)  $< $(addprefix obj/,$(READER)) 


# ================================================================================
clean:
	rm -f obj/*.o *Dict.* lib/* bin/*

# --------------------------------------------------------------------------------
cleanall:
	@$(MAKE) clean
	rm -f bin/writeA01Event bin/readA01Event
	rm -f ../../../lib/$(SCRAM_ARCH)/libAna00.so

