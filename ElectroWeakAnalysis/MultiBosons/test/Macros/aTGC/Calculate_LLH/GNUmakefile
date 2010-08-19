#efine

INC :=  -I$(ROOTSYS)/include/
LIB := -L$(ROOTSYS)/lib/ -lCint -lCore -lTree -lm -ldl -lHist -lMatrix  -lNew -lPhysics
OBJECTS := LLH.o CalcLLH.o
OBJECTS_DREW := LLH.o CalcLLH_drew.o

all: $(OBJECTS)
	g++ -Wall  $(INC) $(LIB) $(OBJECTS) -o CalcLLH

LLH.o: LLH.cpp
	g++  $(INC) $(LIB) -c LLH.cpp 

CalcLLH.o: CalcLLH.cpp
	g++  $(INC) $(LIB) -c CalcLLH.cpp

