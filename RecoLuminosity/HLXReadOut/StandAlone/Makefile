include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

ProjectGroup = RecoLuminosity
Project = $(ProjectGroup)/HLXReadOut
Package = StandAlone
Sources = 

UserSourcePath =
IncludeDirs = \
	$(XDAQ_ROOT)/$(ProjectGroup)/LumiDB/include \
	$(XDAQ_ROOT)/$(Project)/CoreUtils/include \
	$(XDAQ_ROOT)/$(Project)/HLXCoreLibs/include \
	$(XDAQ_ROOT)/$(Project)/DIP/include \
	$(ORACLE_HOME)/rdbms/demo \
	$(ORACLE_HOME)/rdbms/public \
	$(ORACLE_HOME)/plsql/public \
	$(ORACLE_HOME)/network/public \
	$(ROOTSYS)/include

LibraryDirs = \
	$(XDAQ_ROOT)/$(ProjectGroup)/LumiDB/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(XDAQ_ROOT)/$(Project)/CoreUtils/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(XDAQ_ROOT)/$(Project)/HLXCoreLibs/lib/$(XDAQ_OS)/$(XDAQ_PLATFORM) \
	$(XDAQ_ROOT)/$(Project)/DIP/lib \
	$(ORACLE_HOME)/lib \
	$(ORACLE_HOME)/rdbms/lib \
	$(ROOTSYS)/lib

UserCFlags =
UserCCFlags = 
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =
Libraries = LumiDB CoreUtils HLXCoreLibs pthread dip dl clntsh occi10 \
	nnz10 n10 Tree Net \
	Hist MathCore Matrix Core Cint Gpad Graf RIO
DynamicLibrary = 
StaticLibrary =
Executables = \
	hlx_fake.cpp \
	test_distribution.cpp \
	test_oracle_distributor.cpp \
	test_oracle_distributor_fake.cpp \
	test_DIP_distributor.cpp \
	tcp_server.cpp \
        tcp_client.cpp \
	test_tcp_distributor.cpp \
	test_ROOT_distributor.cpp \
	global_run.cpp
#	test_debug_cout.cpp \
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
