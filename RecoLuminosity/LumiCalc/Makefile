# Packages to be built
Project=RecoLuminosity
Package=LumiCalc

IncludeDirs = \
	$(XDAQ_ROOT)/$(Project)/$(Package)/interface \
	$(XDAQ_ROOT)/$(Project)/HLXReadOut/CoreUtils/include \
	$(XDAQ_ROOT)/$(Project)/HLXReadOut/HLXCoreLibs/include

Sources = \
	LumiCalc.cc

# some XDAQ-required stuff
include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

LibraryDirs =

# compiler flags
UserCFlags =
UserCCFlags = 
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =

ExternalObjects = 

DynamicLibrary= LumiCalc
StaticLibrary=
Executables= 
Libraries= 
TestExecutables= 
TestLibraries= 

# targets
all : _all 

#standard XDAQ C++ compilation
include $(XDAQ_ROOT)/config/Makefile.rules
