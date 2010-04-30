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

CXXFLAGS      = -g -Os -Wall -fPIC -pipe
LD            = $(CXX)
LDFLAGS       = -g
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
#GLIBS         = $(ROOTGLIBS)
GLIBS         = $(filter-out -lz, $(ROOTGLIBS))

# ======================================================================
# -- Default rules
$(addprefix obj/,%.o) : $(addprefix rootio/,%.cc )
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(addprefix obj/,%.o) : $(addprefix test/,%.cc )
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(addprefix obj/,%.o) : $(addprefix tnp/,%.cc ) 
	$(CXX) $(CXXFLAGS) -c $< -I. -o $@
# ======================================================================


# ======================================================================
ANA00 = TAna01Event.o TGenCand.o TAnaTrack.o TAnaMuon.o TTrgObj.o TAnaCand.o TAnaVertex.o TAnaJet.o
ANA00_DICT = ${ANA00:.o=Dict.o}
ANA00_DICT_SRCS = ${ANA00_DICT:.o=.cc}

ANACLASSES = anaTNP2.o
ANACLASSES_DICT = ${ANACLASSES:.o=Dict.o}
ANACLASSES_DICT_SRCS = ${ANACLASSES_DICT:.o=.cc}

UTIL = PidTable.o PidData.o
UTIL_W_LINKDEF = functions.o util.o hpl.o
UTIL_DICT = ${UTIL:.o=Dict.o}
UTIL_W_LINKDEF_DICT = ${UTIL_W_LINKDEF:.o=Dict.o}
UTIL_DICT_SRCS = ${UTIL_DICT:.o=.cc}
UTIL_W_LINKDEF_DICT_SRCS = ${UTIL_W_LINKDEF_DICT:.o=.cc}


# ================================================================================
all: 
# --
	@$(MAKE) ana00
	@$(MAKE) util
	@$(MAKE) anaclasses
	@$(MAKE) links

examples: 
	@$(MAKE) writeA01Event
	@$(MAKE) readA01Event
	@$(MAKE) runTreeReader01
	@$(MAKE) runMyReader01

# ================================================================================
ana00: $(addprefix rootio/,$(ANA00_DICT_SRCS)) $(addprefix obj/,$(ANA00) $(ANA00_DICT))
# ----------------------------------
	$(CXX) $(SOFLAGS) $(GLIBS) $(addprefix obj/,$(ANA00) $(ANA00_DICT)) -o lib/libAna00.so

# ================================================================================
anaclasses: $(addprefix tnp/,$(ANACLASSES_DICT_SRCS)) $(addprefix obj/,$(ANACLASSES) $(ANACLASSES_DICT)) util
# ----------------------------------
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(ANACLASSES) $(ANACLASSES_DICT)) -o lib/libAnaClasses.so $(GLIBS) lib/libUtil.so

# ================================================================================
util: $(addprefix rootio/,$(UTIL_DICT_SRCS) $(UTIL_W_LINKDEF_DICT_SRCS)) $(addprefix obj/,$(UTIL) $(UTIL_DICT) $(UTIL_W_LINKDEF))
# -----------------------------
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(UTIL) $(UTIL_DICT) $(UTIL_W_LINKDEF)) $(GLIBS) -o lib/libUtil.so


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
	rm -f obj/*.o rootio/*Dict.* test/*Dict.* tnp/*Dict.* lib/libAna00.so

# --------------------------------------------------------------------------------
cleanall:
	@$(MAKE) clean
	rm -f bin/writeA01Event bin/readA01Event
	rm -f lib/lib*.so
	rm -f ../../../lib/$(SCRAM_ARCH)/libAna00.so
	rm -f ../../../lib/$(SCRAM_ARCH)/libUtil.so
	rm -f ../../../lib/$(SCRAM_ARCH)/libAnaClasses.so


.SECONDEXPANSION:
$(addprefix rootio/,$(ANA00_DICT_SRCS) $(UTIL_DICT_SRCS)): $$(patsubst %Dict.cc,%.hh,$$@)
	cd rootio && $(ROOTCINT) -f $(notdir $@) -c $(notdir $(patsubst %Dict.cc,%.hh,$@)) && cd -

$(addprefix rootio/,$(UTIL_W_LINKDEF_DICT_SRCS)): $$(patsubst %Dict.cc,%.hh,$$@)
	cd rootio && $(ROOTCINT) -f $(notdir $@) -c $(notdir $(patsubst %Dict.cc,%.hh,$@)) $(notdir $(patsubst %Dict.cc,%LinkDef.h,$@)) && cd -

$(addprefix tnp/,$(ANACLASSES_DICT_SRCS)): $$(patsubst %Dict.cc,%.hh,$$@)
	$(ROOTCINT) -f $@ -c $(patsubst %Dict.cc,%.hh,$@)
#	cd tnp && $(ROOTCINT) -f $(notdir $@) -c $(notdir $(patsubst %Dict.cc,%.hh,$@)) && cd -
