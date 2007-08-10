include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

Project = lumi/HF
Package = HLXCommissioning
Sources = 

UserSourcePath =
IncludeDirs = \
	$(XDAQ_ROOT)/lumi/HF/CoreUtils/include \
	$(XDAQ_ROOT)/lumi/HF/HLXReadout/include \
	$(XDAQ_ROOT)/lumi/HF/DIP/include \
	$(ORACLE_HOME)/rdbms/demo \
	$(ORACLE_HOME)/rdbms/public \
	$(ORACLE_HOME)/plsql/public \
	$(ORACLE_HOME)/network/public

LibraryDirs = \
	$(XDAQ_ROOT)/lumi/HF/CoreUtils/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
	$(XDAQ_ROOT)/lumi/HF/HLXReadout/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM)/ \
	$(XDAQ_ROOT)/lumi/HF/DIP/lib/ \
	$(ORACLE_HOME)/lib/ \
	$(ORACLE_HOME)/rdbms/lib/ \
	$(ROOTSYS)/lib/

UserCFlags =
UserCCFlags = -D$(PLATFORM)
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =
Libraries = CoreUtils HLXReadout pthread dip dl clntsh occi10 nnz10 n10 \
	Hist MathCore Matrix Core Cint Gpad Graf RIO
DynamicLibrary = 
StaticLibrary =
Executables = \
	hlx_fake.cpp
#	test_debug_cout.cpp \
#	test_distribution.cpp \
#	test_section.cpp \
#	tcp_receiver2.cpp \
#	test_debug_file.cpp \
#	test_nibble.cpp \
#	test_tcp_distributor.cpp \
#	tcp_receiver.cpp \
#	test_DIP_distributor.cpp \
#	test_oracle_distributor.cpp \
#	udp_gen.cpp \
#	test_all_fake.cpp \
#	test_DIP_distributor_fake.cpp \
#	test_oracle_distributor_fake.cpp

include $(XDAQ_ROOT)/config/Makefile.rules
