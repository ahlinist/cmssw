# Packages to be built
ProjectGroup=RecoLuminosity
Project=$(ProjectGroup)/HLXReadOut
Package=HLXCoreLibs

IncludeDirs = \
	$(BUILD_HOME)/$(ProjectGroup)/LumiDB/include \
	$(BUILD_HOME)/$(ProjectGroup)/LumiCalc/interface \
	$(BUILD_HOME)/$(Project)/$(Package)/include \
	$(BUILD_HOME)/$(Project)/CoreUtils/include \
	$(DIPBASE)/include \
	/opt/oracle/current/sdk/include \
	$(ROOTSYS)/include \

Sources = \
	AbstractDistributor.cc \
	DebugCoutDistributor.cc \
	NibbleCollector.cc \
	DebugFileDistributor.cc \
	OracleDistributor.cc \
	TestDistributor.cc \
	DIPDistributor.cc \
	GIFDistributor.cc \
        WedgeGIFDistributor.cc \
        SectionCollector.cc \
	TestSectionCollector.cc \
        TCPDistributor.cc \
	SectionListDistributor.cc \
	ROOTMutex.cc \
	FakeHLX.cc 
	#ROOTDistributor.cc \
	#ROOTDistributor_Dict.cc \
        #ROOTMutex.cc \

# some XDAQ-required stuff
include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

# compiler flags
UserCFlags =
UserCCFlags = -D_REENTRANT
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =

ExternalObjects = 

DynamicLibrary= HLXCoreLibs
StaticLibrary=
Executables= 
Libraries= # CoreUtils dip dl LumiDB LumiCalc
TestExecutables= 
TestLibraries= 

# targets
all : _all 

# standard XDAQ C++ compilation
include $(XDAQ_ROOT)/config/Makefile.rules
