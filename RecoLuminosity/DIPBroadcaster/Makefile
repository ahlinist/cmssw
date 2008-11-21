IncludeDirs := ${DIPBASE}/include \
		$(BUILD_HOME) \
                $(ROOTSYS)/include \
		interface

COMPILEDIR := build
LINKER       := g++
DEPENDFLAGS  := -g -Wall $(patsubst %,-I%,${IncludeDirs}) -fPIC -g
TOUCHHEADERS := ${MYCODEDIR}/*.h

# C

CC     := gcc
CFLAGS  = ${DEPENDFLAGS} -dl

# C++

CXX      := g++
CXXFLAGS  = ${DEPENDFLAGS} -dl

${COMPILEDIR}/%.o : src/%
	${CXX} ${CPPFLAGS} -c $< -o $@ ${CXXFLAGS}

${COMPILEDIR}/%.d : src/%
	mkdir -p ${COMPILEDIR}
	${CXX} ${CXXFLAGS} -M -MT $(patsubst %.d,%.o,$@) -o $@ $<

# C preprocessor (C, C++, FORTRAN)

CPPFLAGS =

# linker

LOADLIBES := ${DIPBASE}/lib/libdip.so \
             ${BUILD_HOME}/RecoLuminosity/HLXReadOut/HLXCoreLibs/lib/linux/x86/libHLXCoreLibs.so \
             ${BUILD_HOME}/RecoLuminosity/HLXReadOut/CoreUtils/lib/linux/x86/libCoreUtils.so \
             ${ROOTSYS}/lib/libGpad.so \
	     ${CMSSW_BASE}/lib/slc4_ia32_gcc345/libRecoLuminosityTCPReceiver.so \
	     ${CMSSW_BASE}/lib/slc4_ia32_gcc345/libLumiDB.so \
	     ${CMSSW_BASE}/lib/slc4_ia32_gcc345/libLumiCalc.so \
	     $(ORACLE_HOME)/lib/libclntsh.so \
	     $(ORACLE_HOME)/lib/libocci.so 

LDFLAGS    =

SOURCES:= DIPBroadcaster.cc RunDIP.cpp
OBJECTS:=$(patsubst %,${COMPILEDIR}/%.o,${SOURCES})
DEPENDS:=$(patsubst %,${COMPILEDIR}/%.d,${SOURCES})
SOURCES:=$(patsubst %,src/% ,${SOURCES})

all: bin/RunDIP

bin/RunDIP: ${OBJECTS} 
	${LINKER} -o $@ $^ ${LOADLIBES} ${LDFLAGS}


include ${DEPENDS}

clean :
	rm -f ${COMPILEDIR}/*.{o,d} bin/RunDIP src/*~ interface/*~










