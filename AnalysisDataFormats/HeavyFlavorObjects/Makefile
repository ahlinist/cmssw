# ======================================================================
# NOTE: 
# -----
#
# ======================================================================


ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

CXX           = c++
CXXFLAGS      = -g -Wall -fPIC
LD            = c++
LDFLAGS       = -g
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
#GLIBS         = $(ROOTGLIBS)
GLIBS         = $(filter-out -lz, $(ROOTGLIBS))


$(addprefix obj/,%.o) : $(addprefix rootio/,%.cc )
	$(CXX) $(CXXFLAGS) -c $< -o $@


ANA00   = TAna00Event.o TAna00EventDict.o \
          TGenCand.o TGenCandDict.o \
          TAnaTrack.o TAnaTrackDict.o \
          TAnaCand.o TAnaCandDict.o \
          TAnaVertex.o TAnaVertexDict.o


# ================================================================================
all: 
# --
	@$(MAKE) ana00
	@$(MAKE) writeA00Event
	@$(MAKE) readA00Event
	@$(MAKE) runTreeReader
	@$(MAKE) runTrigReader
	@$(MAKE) links

# ================================================================================
ana00: $(addprefix obj/,$(ANA00))
# ----------------------------------
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(ANA00)) -o lib/libAna00.so

rootio/TAna00EventDict.cc: rootio/TAna00Event.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAna00EventDict.cc -c TAna00Event.hh && cd -

rootio/TGenCandDict.cc: rootio/TGenCand.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TGenCandDict.cc -c TGenCand.hh  && cd - 

rootio/TAnaTrackDict.cc: rootio/TAnaTrack.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAnaTrackDict.cc -c TAnaTrack.hh && cd - 

rootio/TAnaVertexDict.cc: rootio/TAnaVertex.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAnaVertexDict.cc -c TAnaVertex.hh && cd - 

rootio/TAnaCandDict.cc: rootio/TAnaCand.hh 
	cd rootio && $(ROOTSYS)/bin/rootcint -f TAnaCandDict.cc -c TAnaCand.hh  && cd - 


# ======================================================================
writeA00Event: test/writeA00Event.cc
# ----------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c writeA00Event.cc -o ../obj/writeA00Event.o && cd - 
	cd test && $(LD) $(LDFLAGS)  -o ../bin/writeA00Event ../obj/writeA00Event.o $(GLIBS) ../lib/libAna00.so && cd - 

# ======================================================================
readA00Event: test/readA00Event.cc
# --------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c readA00Event.cc -o ../obj/readA00Event.o && cd - 
	cd test && $(LD) $(LDFLAGS)  -o ../bin/readA00Event ../obj/readA00Event.o $(GLIBS) ../lib/libAna00.so && cd - 


# ================================================================================
runTreeReader: test/treeReader.cc test/treeReader.cc
# --------------------------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c treeReader.cc -o ../obj/treeReader.o  && cd ..
	cd test && $(ROOTSYS)/bin/rootcint -f treeReaderDict.cc -c treeReader.hh && cd ..
	cd test && $(CXX) $(CXXFLAGS) -o ../obj/runTreeReader.o -c runTreeReader.cc && cd ..
	cd test && $(LD) $(LDFLAGS)  -o ../bin/runTreeReader $(GLIBS) ../lib/libAna00.so ../obj/runTreeReader.o ../obj/treeReader.o && cd ..


# ================================================================================
runTrigReader: test/trigReader.cc test/trigReader.cc
# --------------------------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c trigReader.cc -o ../obj/trigReader.o &&cd - 
	cd test && $(ROOTSYS)/bin/rootcint -f trigReaderDict.cc -c trigReader.hh && cd - 
	cd test && $(CXX) $(CXXFLAGS) -o ../obj/runTrigReader.o -c runTrigReader.cc && cd - 
	cd test && $(LD) $(LDFLAGS)  -o ../bin/runTrigReader $(GLIBS) ../lib/libAna00.so ../obj/runTrigReader.o ../obj/trigReader.o


# ================================================================================
links:
	cd ../../../lib/$(SCRAM_ARCH)/ && rm -f libAna00.so && ln -s ../../AnalysisDataFormats/HeavyFlavorObjects/lib/libAna00.so && cd -

# ================================================================================
clean:
	rm -f obj/*.o rootio/*Dict.* test/*Dict.* lib/libAna00.so

# --------------------------------------------------------------------------------
cleanall:
	@$(MAKE) clean
	rm -f bin/writeA00Event bin/readA00Event
	rm -f bin/runTreeReader bin/runTrigReader


