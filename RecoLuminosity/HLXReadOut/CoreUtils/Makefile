Project=RecoLuminosity/HLXReadOut
Package=CoreUtils

IncludeDirs = \
	$(XDAQ_ROOT)/$(Project)/$(Package)/include

Sources = \
	ASCIIReaderEngine.cc \
	ConfigFileErrorException.cc \
	EventCounter.cc	\
	FileNotFoundException.cc \
	FileNotOpenedException.cc \
	GenericException.cc \
	HardwareAccessException.cc \
	ICExampleException.cc \
	ICException.cc \
	ICUtilityToolbox.cc \
	InvalidArgumentException.cc \
	MemoryAllocationException.cc \
	OracleDBException.cc \
	MyLog.cc \
	TimeStamp.cc

# some XDAQ-required stuff
include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

# Packages to be built

# compiler flags
UserCFlags =
UserCCFlags = 
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =

ExternalObjects = 

DynamicLibrary= $(Package)
StaticLibrary=
Executables= 
Libraries= 
TestExecutables= 
TestLibraries= 

# targets
all : _all 

#standard XDAQ C++ compilation
include $(XDAQ_ROOT)/config/Makefile.rules
