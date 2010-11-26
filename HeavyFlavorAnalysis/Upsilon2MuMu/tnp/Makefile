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

CXXFLAGS      = -g -O3 -Wall -fPIC -pipe
LD            = $(CXX)
LDFLAGS       = -g
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(filter-out -lz, $(ROOTGLIBS))
RELLIBPATH    = ../../../../AnalysisDataFormats/HeavyFlavorObjects/lib

#########
# FILES #
#########

READER_HEADERS = tnpReader.hh treeReaderTNP.hh tnpReader2.hh
READER = treeReaderTNP.o tnpReader.o tnpReader2.o
READERDICT = ${READER:.o=Dict.o}

ANA_HEADERS = anaTNP.hh
ANACLASSES = anaTNP.o
ANACLASSES_DICT = ${ANACLASSES:.o=Dict.o}

#################
# PATTERN RULES #
#################

obj/%.o : %.cc $(READER_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o : %.cc $(ANA_HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%Dict.cc : %.hh %LinkDef.h
	$(ROOTCINT) -f $@ -c $^

%Dict.cc : %.hh
	$(ROOTCINT) -f $@ -c $<

###########
# TARGETS #
###########

all: 
	@$(MAKE) bin/runTNPReaders
	@$(MAKE) lib/libAna.so 

lib/libAnaClasses.so: $(addprefix obj/,$(READER) $(READERDICT)) lib/libAna00.so
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(READER) $(READERDICT)) -o lib/libAnaClasses.so $(GLIBS) lib/libAna00.so

lib/libAna.so: $(addprefix obj/,$(ANACLASSES) $(ANACLASSES_DICT)) lib/libAna00.so
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(ANACLASSES) $(ANACLASSES_DICT)) -o lib/libAna.so $(GLIBS) lib/libAna00.so  

lib/libAna00.so:
	cd lib && ln -s $(RELLIBPATH)/libAna00.so libAna00.so && cd -
	cd lib && ln -s $(RELLIBPATH)/libUtil.so libUtil.so && cd -

bin/runTNPReaders: lib/libAnaClasses.so obj/runTNPReaders.o lib/libAna00.so
	$(LD) $(LDFLAGS) -o bin/runTNPReaders $(GLIBS) obj/runTNPReaders.o lib/libAnaClasses.so lib/libAna00.so lib/libUtil.so

clean:
	rm -f obj/*.o *Dict.* lib/libAna00.so lib/libUtil.so

cleanall: clean
	rm -f bin/runTNPReaders
	rm -f lib/lib*.so
	rm -f ../../../lib/$(SCRAM_ARCH)/libAna.so
	rm -f ../../../lib/$(SCRAM_ARCH)/libAna00.so
	rm -f ../../../lib/$(SCRAM_ARCH)/libUtil.so
	rm -f ../../../lib/$(SCRAM_ARCH)/libAnaClasses.so
