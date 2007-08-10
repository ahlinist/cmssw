include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

Project=lumi/HF
Package=HLXReadoutXDAQ

Sources = \
	HLXReadoutSupervisor.cc \
	HLX_WebFunctions.cc

IncludeDirs = \
        $(XDAQ_ROOT)/daq/extern/xerces/$(XDAQ_OS)$(XDAQ_PLATFORM)/include \
        $(XDAQ_ROOT)/daq/extern/log4cplus/$(XDAQ_OS)$(XDAQ_PLATFORM)/include \
        $(XDAQ_ROOT)/daq/extern/cgicc/$(XDAQ_OS)$(XDAQ_PLATFORM)/include \
        $(XDAQ_ROOT)/daq/toolbox/include \
        $(XDAQ_ROOT)/daq/toolbox/include/$(XDAQ_OS) \
        $(XDAQ_ROOT)/daq/toolbox/include/solaris \
        $(XDAQ_ROOT)/daq/extern/mimetic/$(XDAQ_OS)$(XDAQ_PLATFORM)/include \
        $(XDAQ_ROOT)/daq/xoap/include \
        $(XDAQ_ROOT)/daq/xdata/include \
        $(XDAQ_ROOT)/daq/pt/include \
        $(XDAQ_ROOT)/daq/xcept/include \
        $(XDAQ_ROOT)/daq/xgi/include \
        $(XDAQ_ROOT)/daq/ \
        $(XDAQ_ROOT)/daq/extern/i2o/include/ \
        $(XDAQ_ROOT)/daq/extern/i2o/include/i2o \
        $(XDAQ_ROOT)/daq/extern/i2o/include/i2o/shared \
	${XDAQ_ROOT}/daq/config/include \
        $(XDAQ_ROOT)/daq/xdaq/include \
        $(XDAQ_ROOT)/daq/xdaq/include/solaris \
        $(XDAQ_ROOT)/daq/xdaq/include/linux \
	$(XDAQ_ROOT)/lumi/HF/CoreUtils/include \
	$(XDAQ_ROOT)/lumi/HF/HLXReadout/include \
	$(XDAQ_ROOT)/daq/extern/log4cplus/$(XDAQ_OS)$(XDAQ_PLATFORM)/include/log4cplus/helpers \
	$(XDAQ_ROOT)/daq/extern/log4cplus/$(XDAQ_OS)$(XDAQ_PLATFORM)/include/log4cplus/spi \
	$(XDAQ_ROOT)/lumi/HF/DIP/include \
	$(ORACLE_HOME)/rdbms/demo \
        $(ORACLE_HOME)/rdbms/public \
        $(ORACLE_HOME)/plsql/public \
        $(ORACLE_HOME)/network/public \
	$(ROOTSYS)/include

LibraryDirs = \

UserCFlags = 
UserCCFlags = -D$(PLATFORM)
UserDynamicLinkFlags =  

DependentLibraryDirs = \
			$(XDAQ_ROOT)/daq/extern/xerces/$(XDAQ_OS)$(XDAQ_PLATFORM)/lib/ \
			$(XDAQ_ROOT)/daq/xoap/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
			$(XDAQ_ROOT)/daq/toolbox/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
			$(XDAQ_ROOT)/daq/xcept/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
			$(XDAQ_ROOT)/daq/extern/log4cplus/$(XDAQ_OS)$(XDAQ_PLATFORM)/lib/ \
		        $(XDAQ_ROOT)/lumi/HF/CoreUtils/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
	        	$(XDAQ_ROOT)/lumi/HF/HLXReadout/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
		        $(XDAQ_ROOT)/lumi/HF/DIP/lib/ \
			$(ORACLE_HOME)/lib/ \
			$(ORACLE_HOME)/rdbms/lib/ \
			$(ROOTSYS)/lib/


DependentLibraries = toolbox xcept xerces-c log4cplus xoap CoreUtils HLXReadout pthread dip dl clntsh occi10 nnz10 n10 \
	Hist MathCore Matrix Core Cint Gpad Graf RIO
UserStaticLinkFlags =
UserExecutableLinkFlags =

# These libraries can be platform specific and
# potentially need conditional processing
#

Libraries = 

#
# Compile the source files and create a shared library
#
DynamicLibrary= HLXReadoutXDAQ

StaticLibrary= 

include $(XDAQ_ROOT)/config/Makefile.rules
