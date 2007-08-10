IncludeDirs = \
	$(XDAQ_ROOT)/lumi/HF/HLXReadout/include \
	$(XDAQ_ROOT)/lumi/HF/CoreUtils/include \
	$(XDAQ_ROOT)/lumi/HF/DIP/include \
	$(ORACLE_HOME)/rdbms/demo \
	$(ORACLE_HOME)/plsql/public \
	$(ORACLE_HOME)/rdbms/public \
	$(ORACLE_HOME)/network/public \
	$(ROOTSYS)/include

Sources = \
	DebugCoutDistributor.cc \
	NibbleCollector.cc \
	DebugFileDistributor.cc \
	OracleDistributor.cc \
	TestDistributor.cc \
	DIPDistributor.cc \
	GIFDistributor.cc \
        SectionCollector.cc \
	TestSectionCollector.cc \
        TCPDistributor.cc

# some XDAQ-required stuff
include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

# Packages to be built
Project=lumi/HF
Package=HLXReadout

# compiler flags
UserCFlags =
UserCCFlags = -D$(PLATFORM)
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =

ExternalObjects = 

DynamicLibrary= HLXReadout
StaticLibrary=
Executables= 
Libraries= CoreUtils dip dl
TestExecutables= 
TestLibraries= 

# targets
all : _all 

#standard XDAQ C++ compilation
include $(XDAQ_ROOT)/config/Makefile.rules
