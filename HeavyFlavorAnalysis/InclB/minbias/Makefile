ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

CXX           = c++
CXXFLAGS      = -O2 -Wall -fPIC -Wno-deprecated
CXXFLAGS      = -g -Wall -fPIC -Wno-deprecated
LD            = c++
LDFLAGS       = -O2
LDFLAGS       = -g
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
MLIBS         = $(ROOTGLIBS) 
MLIBS        += -lMinuit
GLIBS         = $(filter-out -lz, $(MLIBS))


$(addprefix obj/,%.o) : %.cc 
	$(CXX) $(CXXFLAGS) -c $< -o $@

ANA00   = TAna00Event.o TAna00EventDict.o \
          TGenCand.o TGenCandDict.o \
          TAnaTrack.o TAnaTrackDict.o \
          TAnaCand.o TAnaCandDict.o \
          TAnaVertex.o TAnaVertexDict.o \
          TAnaJet.o TAnaJetDict.o

# ================================================================================
analysis: analysis.cc treeAnalysis.cc
# --
	$(CXX) $(CXXFLAGS) -I../ -c treeAnalysis.cc -o treeAnalysis.o
	$(ROOTSYS)/bin/rootcint -f treeAnalysisDict.cc -c treeAnalysis.hh LinkDef.h
	$(CXX) $(CXXFLAGS) -o analysis.o -c analysis.cc
	$(LD) $(LDFLAGS)  -o analysis $(GLIBS) /shome/caminada/cmssw/CMSSW_3_5_6/lib/slc5_ia32_gcc434/libAna00.so analysis.o treeAnalysis.o


# ======================================================================
clean:
	rm -f *.o *Dict.* *~
	rm -f analysis

cleanall:
	@$(MAKE) clean
	rm -f *.o *Dict.* *~
	rm -f analysis

