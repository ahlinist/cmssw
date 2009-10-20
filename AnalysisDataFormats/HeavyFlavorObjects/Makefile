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
LIBS          = $(ROOTLIBS)
#GLIBS         = $(ROOTGLIBS)
GLIBS         = $(filter-out -lz, $(ROOTGLIBS))


$(addprefix obj/,%.o) : $(addprefix rootio/,%.cc )
	$(CXX) $(CXXFLAGS) -c $< -o $@


ANA00   =    TAna01Event.o TAna01EventDict.o \
             TGenCand.o TGenCandDict.o \
             TAnaTrack.o TAnaTrackDict.o \
             TAnaMuon.o TAnaMuonDict.o \
             TTrgObj.o TTrgObjDict.o \
             TAnaCand.o TAnaCandDict.o \
             TAnaVertex.o TAnaVertexDict.o \
             TAnaJet.o TAnaJetDict.o 

ANACLASSES = ana.o anaDict.o 

UTIL       = PidTable.o PidTableDict.o \
             PidData.o PidDataDict.o 


# ================================================================================
all: 
# --
	@$(MAKE) ana00
	@$(MAKE) util
	@$(MAKE) writeA01Event
	@$(MAKE) readA01Event
	@$(MAKE) runTreeReader01
	@$(MAKE) runMyReader01
	@$(MAKE) links

# ================================================================================
ana00: $(addprefix obj/,$(ANA00))
# ----------------------------------
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(ANA00)) -o lib/libAna00.so

rootio/TAna01EventDict.cc: rootio/TAna01Event.hh 
	cd rootio && $(ROOTCINT) -f TAna01EventDict.cc -c TAna01Event.hh && cd -

rootio/TGenCandDict.cc: rootio/TGenCand.hh 
	cd rootio && $(ROOTCINT) -f TGenCandDict.cc -c TGenCand.hh  && cd - 

rootio/TAnaTrackDict.cc: rootio/TAnaTrack.hh 
	cd rootio && $(ROOTCINT) -f TAnaTrackDict.cc -c TAnaTrack.hh && cd - 

rootio/TAnaMuonDict.cc: rootio/TAnaMuon.hh 
	cd rootio && $(ROOTCINT) -f TAnaMuonDict.cc -c TAnaMuon.hh && cd - 

rootio/TTrgObjDict.cc: rootio/TTrgObj.hh 
	cd rootio && $(ROOTCINT) -f TTrgObjDict.cc -c TTrgObj.hh && cd - 

rootio/TAnaVertexDict.cc: rootio/TAnaVertex.hh 
	cd rootio && $(ROOTCINT) -f TAnaVertexDict.cc -c TAnaVertex.hh && cd - 

rootio/TAnaCandDict.cc: rootio/TAnaCand.hh 
	cd rootio && $(ROOTCINT) -f TAnaCandDict.cc -c TAnaCand.hh  && cd - 

rootio/TAnaJetDict.cc: rootio/TAnaJet.hh 
	cd rootio && $(ROOTCINT) -f TAnaJetDict.cc -c TAnaJet.hh && cd - 

rootio/TGenMuonDict.cc: rootio/TGenMuon.hh 
	cd rootio && $(ROOTCINT) -f TGenMuonDict.cc -c TGenMuon.hh && cd - 


# ================================================================================
util: $(addprefix obj/,$(UTIL))
# -----------------------------
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(UTIL)) -o lib/libUtil.so

rootio/PidTableDict.cc: rootio/PidTable.hh
	cd rootio && $(ROOTCINT) -f PidTableDict.cc -c PidTable.hh 

rootio/PidDataDict.cc: rootio/PidData.hh 
	cd rootio && $(ROOTCINT) -f PidDataDict.cc -c PidData.hh 


# ======================================================================
writeA01Event: test/writeA01Event.cc
# ----------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c writeA01Event.cc -o ../obj/writeA01Event.o && cd - 
	cd test && $(LD) $(LDFLAGS)  -o ../bin/writeA01Event ../obj/writeA01Event.o $(GLIBS) ../lib/libAna00.so && cd - 

# ======================================================================
readA01Event: test/readA01Event.cc
# --------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c readA01Event.cc -o ../obj/readA01Event.o && cd - 
	cd test && $(LD) $(LDFLAGS)  -o ../bin/readA01Event ../obj/readA01Event.o $(GLIBS) ../lib/libAna00.so && cd - 


# ================================================================================
runTreeReader01: test/treeReader01.cc test/treeReader01.cc
# --------------------------------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c treeReader01.cc -o ../obj/treeReader01.o  && cd ..
	cd test && $(ROOTSYS)/bin/rootcint -f treeReader01Dict.cc -c treeReader01.hh && cd ..
	cd test && $(CXX) $(CXXFLAGS) -o ../obj/runTreeReader01.o -c runTreeReader01.cc && cd ..
	cd test && $(LD) $(LDFLAGS)  -o ../bin/runTreeReader01 $(GLIBS) ../lib/libAna00.so ../obj/runTreeReader01.o ../obj/treeReader01.o && cd ..


# ================================================================================
runMyReader01: test/myReader01.hh test/myReader01.cc
# --------------------------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c myReader01.cc -o ../obj/myReader01.o  && cd ..
	cd test && $(ROOTSYS)/bin/rootcint -f myReader01Dict.cc -c myReader01.hh && cd ..
	cd test && $(CXX) $(CXXFLAGS) -o ../obj/runMyReader01.o -c runMyReader01.cc && cd ..
	cd test && $(LD) $(LDFLAGS)  -o ../bin/runMyReader01 $(GLIBS) ../lib/libAna00.so ../obj/runMyReader01.o ../obj/myReader01.o ../obj/treeReader01.o && cd ..



# ================================================================================
runTreeReader: test/treeReader.cc test/treeReader.cc
# --------------------------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c treeReader.cc -o ../obj/treeReader.o  && cd ..
	cd test && $(ROOTCINT) -f treeReaderDict.cc -c treeReader.hh && cd ..
	cd test && $(CXX) $(CXXFLAGS) -o ../obj/runTreeReader.o -c runTreeReader.cc && cd ..
	cd test && $(LD) $(LDFLAGS)  -o ../bin/runTreeReader $(GLIBS) ../lib/libAna00.so ../obj/runTreeReader.o ../obj/treeReader.o && cd ..


# ================================================================================
runMyReader: test/myReader.hh test/myReader.cc
# --------------------------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c myReader.cc -o ../obj/myReader.o  && cd ..
	cd test && $(ROOTCINT) -f myReaderDict.cc -c myReader.hh && cd ..
	cd test && $(CXX) $(CXXFLAGS) -o ../obj/runMyReader.o -c runMyReader.cc && cd ..
	cd test && $(LD) $(LDFLAGS)  -o ../bin/runMyReader $(GLIBS) ../lib/libAna00.so ../obj/runMyReader.o ../obj/myReader.o ../obj/treeReader.o && cd ..


# ======================================================================
ana: test/ana.cc
	cd test && $(CXX) $(CXXFLAGS) -c ana.cc -o ../obj/ana.o  && cd ..
	cd test && $(ROOTCINT)  -f anaDict.cc -c ana.hh && cd ..
	cd test && $(CXX) $(CXXFLAGS) -c anaDict.cc -o ../obj/anaDict.o  && cd ..
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(ANACLASSES)) -o lib/libAnaClasses.so



# ================================================================================
links:
	mkdir -p ../../../lib/$(SCRAM_ARCH)
	cd ../../../lib/$(SCRAM_ARCH)/ && rm -f libAna00.so && ln -s ../../src/AnalysisDataFormats/HeavyFlavorObjects/lib/libAna00.so && cd -
	cd ../../../lib/$(SCRAM_ARCH)/ && rm -f libUtil.so && ln -s ../../src/AnalysisDataFormats/HeavyFlavorObjects/lib/libUtil.so && cd -

copy:
	cd ../../../lib/$(SCRAM_ARCH)/ && rm -f libAna00.so && cp ../../src/AnalysisDataFormats/HeavyFlavorObjects/lib/libAna00.so . && cd -
	cd ../../../lib/$(SCRAM_ARCH)/ && rm -f libUtil.so && cp ../../src/AnalysisDataFormats/HeavyFlavorObjects/lib/libUtil.so . && cd -

# ================================================================================
clean:
	rm -f obj/*.o rootio/*Dict.* test/*Dict.* lib/libAna00.so

# --------------------------------------------------------------------------------
cleanall:
	@$(MAKE) clean
	rm -f bin/writeA01Event bin/readA01Event
	rm -f ../../../lib/$(SCRAM_ARCH)/libAna00.so

