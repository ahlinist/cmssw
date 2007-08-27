# Packages to be built
Project=RecoLuminosity/HLXReadOut
Package=HLXCoreLibs

IncludeDirs = \
	$(XDAQ_ROOT)/$(Project)/LumiDBLib/include \
	$(XDAQ_ROOT)/$(Project)/$(Package)/include \
	$(XDAQ_ROOT)/$(Project)/CoreUtils/include \
	$(XDAQ_ROOT)/$(Project)/DIP/include \
	$(ORACLE_ROOT)/rdbms/demo \
	$(ORACLE_ROOT)/plsql/public \
	$(ORACLE_ROOT)/rdbms/public \
	$(ORACLE_ROOT)/network/public \
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

# compiler flags
UserCFlags =
UserCCFlags = 
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =

ExternalObjects = 

DynamicLibrary= HLXCoreLibs
StaticLibrary=
Executables= 
Libraries= CoreUtils dip dl LumiDBLib
TestExecutables= 
TestLibraries= 

# targets
all : _all 

#standard XDAQ C++ compilation
include $(XDAQ_ROOT)/config/Makefile.rules
