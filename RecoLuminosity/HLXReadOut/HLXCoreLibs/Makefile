# Packages to be built
ProjectGroup=RecoLuminosity
Project=$(ProjectGroup)/HLXReadOut
Package=HLXCoreLibs

IncludeDirs = \
	$(XDAQ_ROOT)/$(ProjectGroup)/LumiDB/include \
	$(XDAQ_ROOT)/$(ProjectGroup)/LumiCalc/include \
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
        TCPDistributor.cc \
	ROOTDistributor.cc \
	ROOTDistributor_Dict.cc

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
Libraries= CoreUtils dip dl LumiDB LumiCalc
TestExecutables= 
TestLibraries= 

# targets
all : _all 

#standard XDAQ C++ compilation
include $(XDAQ_ROOT)/config/Makefile.rules
