# Makefile for Analysis Classes for RootMaker Ntuples
# R. Remington, UF ( inspired by  Mike Schmitt (The University of Florida) )

CC=g++ 

CFLAGS=-Wall -c -g -O -DDROP_CGAL -O2
#LFLAGS=-Wall -lCore -lCint -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lpthread -lm -ldl -rdynamic

ROOTCFLAGS:=$(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS:=$(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS:=$(shell $(ROOTSYS)/bin/root-config --glibs)
CFLAGS +=$(ROOTCFLAGS)
LIBS= $(ROOTLIBS) 

HEADERS=$(wildcard include/*.h) 
INCLUDES=-I$(ROOTSYS)/include -Iinclude/ 
SOURCES=$(wildcard src/Ex*.cpp) 
PACKAGES=$(SOURCES:src/%.cpp=%)
BINARIES=$(PACKAGES:%=bin/%) 

UFAV=obj/UFAV.o
DS=obj/DataSet.o

OBJECTS=$(PACKAGES:%=obj/%.o) $(UFAV) $(DS)


all: $(BINARIES) $(OBJECTS)

clean:
	rm -f $(BINARIES) $(OBJECTS) 

obj/%.o: src/%.cpp include/%.h $(HEADERS) 
	$(CC) $(CFLAGS) $(INCLUDES) $(@:obj/%.o=src/%.cpp) -o $@

obj/%.o: src/%.cpp 
	$(CC) $(CFLAGS) $(INCLUDES) $(@:obj/%.o=src/%.cpp) -o $@

bin/%: obj/%.o $(UFAV) $(DS)
	$(CC) $(LFLAGS) $(LIBS) $(@:bin/%=obj/%.o) $(UFAV) $(DS) -o $@

%: bin/%
	@echo $@ has been successfully built
