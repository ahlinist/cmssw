include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

ProjectGroup = RecoLuminosity
Project = $(ProjectGroup)/HLXReadOut
Package = StandAlone
Sources = 

UserSourcePath =
IncludeDirs = \
	$(BUILD_HOME)/$(ProjectGroup)/LumiDB/include \
	$(BUILD_HOME)/$(ProjectGroup)/LumiCalc/interface \
	$(BUILD_HOME)/$(Project)/CoreUtils/include \
	$(BUILD_HOME)/$(Project)/HLXCoreLibs/include \
	$(DIPBASE)/include \
	/opt/oracle/current/sdk/include \
	$(ROOTSYS)/include

LibraryDirs = \
	$(BUILD_HOME)/$(ProjectGroup)/LumiDB/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(BUILD_HOME)/$(ProjectGroup)/LumiCalc/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(BUILD_HOME)/$(Project)/CoreUtils/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(BUILD_HOME)/$(Project)/HLXCoreLibs/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(DIPBASE)/lib \
	/opt/oracle/current \
	$(ROOTSYS)/lib

UserCFlags =
UserCCFlags = -D_REENTRANT
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =
Libraries = LumiCalc LumiDB CoreUtils HLXCoreLibs pthread dip dl occi clntsh \
	nnz10 \
        Hist MathCore Matrix Core Cint Gpad Graf RIO

DynamicLibrary = 
StaticLibrary =
Executables = \
	hlx_fake.cpp \
	hlx_fake_2.cpp \
	global_run_fake.cpp \
	test_distribution.cpp \
	tcp_client_short.cpp \
	tcp_client_long.cpp \
	stupid_hack.cpp
#	global_run.cpp \
#	test_distribution.cpp \
#	test_DIP_distributor.cpp \
#	test_oracle_distributor.cpp

include $(XDAQ_ROOT)/config/Makefile.rules
