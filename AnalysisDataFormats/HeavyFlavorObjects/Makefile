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
             TAna00Event.o TAna00EventDict.o \
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
	@$(MAKE) writeA01Event
	@$(MAKE) readA01Event
	@$(MAKE) writeA00Event
	@$(MAKE) readA00Event
	@$(MAKE) runTreeReader
	@$(MAKE) runMyReader
	@$(MAKE) links

# ================================================================================
ana00: $(addprefix obj/,$(ANA00))
# ----------------------------------
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(ANA00)) -o lib/libAna00.so

rootio/TAna01EventDict.cc: rootio/TAna01Event.hh 
	cd rootio && $(ROOTCINT) -f TAna01EventDict.cc -c TAna01Event.hh && cd -

rootio/TAna00EventDict.cc: rootio/TAna00Event.hh 
	cd rootio && $(ROOTCINT) -f TAna00EventDict.cc -c TAna00Event.hh && cd -

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
runHttReader: test/httReader.hh test/httReader.cc
# --------------------------------------------------
	cd test && $(CXX) $(CXXFLAGS) -c httReader.cc -o ../obj/httReader.o  && cd ..
	cd test && $(ROOTCINT) -f httReaderDict.cc -c httReader.hh && cd ..
	cd test && $(CXX) $(CXXFLAGS) -o ../obj/runHttReader.o -c runHttReader.cc && cd ..
	cd test && $(LD) $(LDFLAGS)  -o ../bin/runHttReader $(GLIBS) ../lib/libAna00.so ../obj/runHttReader.o ../obj/httReader.o ../obj/treeReader.o && cd ..


# ================================================================================
links:
	mkdir -p ../../../lib/$(SCRAM_ARCH)
	cd ../../../lib/$(SCRAM_ARCH)/ && rm -f libAna00.so && ln -s ../../src/AnalysisDataFormats/HeavyFlavorObjects/lib/libAna00.so && cd -
	cd ../../../lib/$(SCRAM_ARCH)/ && rm -f libUtil.so && ln -s ../../src/AnalysisDataFormats/HeavyFlavorObjects/lib/libUtil.so && cd -

# ================================================================================
clean:
	rm -f obj/*.o rootio/*Dict.* test/*Dict.* lib/libAna00.so

# --------------------------------------------------------------------------------
cleanall:
	@$(MAKE) clean
	rm -f bin/writeA00Event bin/readA00Event
	rm -f bin/runTreeReader bin/runTrigReader
	rm -f ../../../lib/$(SCRAM_ARCH)/libAna00.so

