ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)
#######################################
# -- DEFINE ARCH to something sensible!
#######################################

#
CXX           = g++
CXXFLAGS      = -g -Wall -fPIC
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared

CXXFLAGS      += $(ROOTCFLAGS)
LIBS           = $(ROOTLIBS)

NGLIBS         = $(ROOTGLIBS)
NGLIBS        += -lMinuit
GLIBS          = $(filter-out -lNew, $(NGLIBS))

runres: runres.o
	$(CXX) $(CXXFLAGS) -c $<
	$(LD) $(LDFLAGS) -o runres runres.o $(GLIBS)

runres.o:  runres.C
	$(CXX) $(CXXFLAGS) -c runres.C

