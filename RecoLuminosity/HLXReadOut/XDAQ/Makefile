include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

Project=RecoLuminosity/HLXReadOut
Package=XDAQ

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
	/opt/TriDAS/RecoLuminosity/HLXReadOut/CoreUtils/include \
	/opt/TriDAS/RecoLuminosity/HLXReadOut/HLXCoreLibs/include \
	/opt/TriDAS/RecoLuminosity/LumiDB/include \
	/opt/TriDAS/RecoLuminosity/LumiCalc/interface \
	/opt/DIP/current/include \
	/opt/TriDAS/hcal/hcalBase/include \
	$(XDAQ_ROOT)/daq/extern/log4cplus/$(XDAQ_OS)$(XDAQ_PLATFORM)/include/log4cplus/helpers \
	$(XDAQ_ROOT)/daq/extern/log4cplus/$(XDAQ_OS)$(XDAQ_PLATFORM)/include/log4cplus/spi \
	$(ORACLE_HOME)/rdbms/demo \
        $(ORACLE_HOME)/rdbms/public \
        $(ORACLE_HOME)/plsql/public \
        $(ORACLE_HOME)/network/public \
	$(ROOTSYS)/include

LibraryDirs = \

UserCFlags = 
UserCCFlags =
UserDynamicLinkFlags =  

DependentLibraryDirs = \
			$(XDAQ_ROOT)/daq/extern/xerces/$(XDAQ_OS)$(XDAQ_PLATFORM)/lib/ \
			$(XDAQ_ROOT)/daq/xoap/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
			$(XDAQ_ROOT)/daq/toolbox/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
			$(XDAQ_ROOT)/daq/xcept/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
			$(XDAQ_ROOT)/daq/extern/log4cplus/$(XDAQ_OS)$(XDAQ_PLATFORM)/lib/ \
		        /opt/TriDAS/RecoLuminosity/HLXReadOut/CoreUtils/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
		        /opt/TriDAS/RecoLuminosity/HLXReadOut/HLXCoreLibs/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
		        /opt/TriDAS/RecoLuminosity/LumiDB/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
		        /opt/TriDAS/RecoLuminosity/LumiCalc/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
		        /opt/DIP/current/lib \
			/opt/TriDAS/hcal/hcalBase/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
			$(ORACLE_HOME)/lib/ \
			$(ROOTSYS)/lib/ \
			/opt/TriDAS/hcal/hcalBase/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \


DependentLibraries = toolbox xcept xerces-c log4cplus xoap CoreUtils HLXCoreLibs LumiDB LumiCalc pthread dip dl clntsh occi nnz10 \
	Hist MathCore Matrix Core Cint Gpad Graf hcalBase RIO

UserStaticLinkFlags =
UserExecutableLinkFlags =

# These libraries can be platform specific and
# potentially need conditional processing
#

Libraries = 

#
# Compile the source files and create a shared library
#
DynamicLibrary= HLXReadoutSupervisor

StaticLibrary= 

include $(XDAQ_ROOT)/config/Makefile.rules
