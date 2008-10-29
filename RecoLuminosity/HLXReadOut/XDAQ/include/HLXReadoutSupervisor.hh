/*************************************************************************
 * XDAQ Layer for HCAL HLX                                               *
 * Copyright (C) 2007, Princeton University		                 *
 * All rights reserved.                                                  *
 * Author(s): J. Jones           	   			         *
 *************************************************************************/

#ifndef HLX_READOUT_SUPERVISOR_HH
#define HLX_READOUT_SUPERVISOR_HH

//#include "xdaq/Application.h"
#include "hcal/Application.hh"
#include "xdaq2rc/RcmsStateNotifier.h"

#include "xoap/domutils.h"
#include "xdaq/WebApplication.h"
#include "toolbox/fsm/FiniteStateMachine.h"

#include "xdaq/NamespaceURI.h"
#include "xoap/MessageReference.h"
#include "xoap/MessageFactory.h"
#include "xoap/SOAPEnvelope.h"
#include "xoap/SOAPBody.h"
#include "xoap/Method.h"

#include "xdata/Integer.h"
#include "xdata/UnsignedShort.h"
#include "xdata/Boolean.h"
#include "xdata/UnsignedLong.h"
#include "xdata/String.h"
#include "xgi/Utils.h"
#include "xgi/Method.h"

#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

#include "ICUtilityToolbox.hh"
#include "ICException.hh"
#include "ICTypeDefs.hh"

// Files used by the HLX readout
#include "NibbleCollector.hh"
#include "SectionCollector.hh"
#include "DIPDistributor.hh"
#include "GIFDistributor.hh"
#include "SectionListDistributor.hh"
#include "OracleDistributor.hh"
#include "TCPDistributor.hh"
#include "FakeHLX.hh"
#include "TestSectionCollector.hh"
#include "TestDistributor.hh"

//#include "HLXInterface.hh"

using namespace HCAL_HLX;

//namespace HCAL_HLX {

class lumiHCALLUMIDAQ : public hcal::Application
{
  
public:
  
  XDAQ_INSTANTIATOR();
  lumiHCALLUMIDAQ(xdaq::ApplicationStub * s) throw (xdaq::exception::Exception);
  ~lumiHCALLUMIDAQ();
  
  // SOAP INTERFACES
  //xoap::MessageReference HLXControl(xoap::MessageReference message) throw (xoap::exception::Exception);
  //xoap::MessageReference ChangeState(xoap::MessageReference message) throw (xoap::exception::Exception);
  
  // FSM INTERFACE
  //void ConfigureAction(toolbox::Event::Reference e) throw (xdaq::exception::Exception);
  //void HaltAction(toolbox::Event::Reference e) throw (xdaq::exception::Exception);
  //void EnableAction(toolbox::Event::Reference e) throw (xdaq::exception::Exception);
  //void InitAction(toolbox::Event::Reference e) throw (xdaq::exception::Exception);
  
  // HYPERDAQ INTERFACES
  //  void Default(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception);
  //void IndividualStatus(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception);
  //void GlobalConfig(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception);
  //void ParameterSet(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception);
  //void RestartCards(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
  //void TransmitterControl(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
  //void RCTCapture(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
  //void TransitionFSMConfigure(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
  //void TransitionFSMEnable(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
  //void TransitionFSMHalt(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
  
  //void ReadRCTData(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
  
  // INFOSPACE INTERFACES (PURE VIRTUAL SO MUST BE IMPLEMENTED)
  //void actionPerformed(xdata::Event &e) throw (xoap::exception::Exception);

protected:

  /// menu method  (default implemenation provides access to the peek and poke calls)
  void webMenu(xgi::Input*, xgi::Output*) throw (hcal::exception::Exception);

  // web configuration page
  void WebConfig(xgi::Input * in, xgi::Output * out) throw (hcal::exception::Exception);

  /// peek method (default implementation shows the contents of the expert monitorable tree)
  //virtual void webPeek(xgi::Input*, xgi::Output*) throw (hcal::exception::Exception);
  /// poke metho
  //virtual void webCfg(xgi::Input*, xgi::Output*) throw (hcal::exception::Exception);
  
  /// actions to perform to go from Ready to Active
  virtual void enable() throw (hcal::exception::Exception);
  /// actions to perform to go from Active to Paused
  virtual void pause() throw (hcal::exception::Exception);
  /// actions to perform to go from Paused to Active
  virtual void resume() throw (hcal::exception::Exception);
  /// actions to perform to go from Paused or Active to Ready
  virtual void disable() throw (hcal::exception::Exception);  
  /// actions to perform in the state COLD.  Automatic transition to INIT after successful completion [Asynchronous]
  virtual void coldInit() throw (hcal::exception::Exception);
  /// actions to perform in the state INIT.  Automatic transition to READY after successful completion [Asynchronous]
  virtual void init() throw (hcal::exception::Exception);
  /// actions to perform to go from Ready or Failed to Uninit
  virtual void reset() throw (hcal::exception::Exception);
  
  /// actions to perform to go from Ready to Active
  //virtual void enableAsync() throw (hcal::exception::Exception);
  /// actions to perform to go from Active to Paused
  //virtual void pauseAsync() throw (hcal::exception::Exception);
  /// actions to perform to go from Paused to Active
  //virtual void resumeAsync() throw (hcal::exception::Exception);
  /// actions to perform to go from Paused or Active to Ready
  //virtual void disableAsync() throw (hcal::exception::Exception);  
  //// actions to perform to go from Ready or Failed to Uninit
  //virtual void resetAsync() throw (hcal::exception::Exception);
  


private:

  void StartReadout(uint32_t runNumber) throw (xdaq::exception::Exception);
  void StopReadout() throw (xdaq::exception::Exception);

  void StartFakeHLX() throw (xdaq::exception::Exception);
  void StopFakeHLX() throw (xdaq::exception::Exception);
 
  // Configuration variables
  uint16_t m_interfacePort;
  string m_interfaceAddress;
  uint16_t m_numOrbits;
  uint16_t m_numBunches;
  uint32_t m_shortSectionLength;
  uint32_t m_longSectionLength;
  uint32_t m_numHLXs;
  uint16_t m_tcpShortPort;
  uint16_t m_tcpLongPort;
  string m_oracleDBUserName;
  string m_oracleDBPassword;
  string m_oracleDBLocation;
  string m_oracleDBSchema;

  bool m_debugData;

  bool m_fakeHLXMode;

  bool m_useShortTCP;
  bool m_useLongTCP;
  bool m_useOracle;
  bool m_useGIF;
  bool m_useDIP;
  bool m_useSectionList;

  std::string m_hostName;

  // Status
  std::string mStatusString;
  
  // Associated variables
  pthread_t mThreadId;
  bool mWorkerThreadContinue;
  
  // Collector instances
  NibbleCollector *mNibbleCollector;
  SectionCollector *mSectionCollectorShort;
  SectionCollector *mSectionCollectorLong;

  DIPDistributor *mDIPDistributor;
  GIFDistributor *mGIFDistributor;
  OracleDistributor *mOracleDistributor;
  SectionListDistributor *mShortSectionListDistributor;
  SectionListDistributor *mLongSectionListDistributor;
  TCPDistributor *mShortSectionTCPDistributor;
  TCPDistributor *mLongSectionTCPDistributor;
  FakeHLX *mFakeHLX;

  // Test mode classes
  TestDistributor *mShortTestDistributor;
  TestDistributor *mLongTestDistributor;
  TestSectionCollector *mTestSectionCollector;

  // Board Initialisation
  //void BoardInit() throw (xdaq::exception::Exception);
  //void UpdateLoggerLevels();
  
  // Board Update / Set Parameters
  //void UpdateBoardInfo() throw (xdaq::exception::Exception);
  //void ResetClockSystem(HCAL_HLX::HLXInterface *theBoard) throw (xdaq::exception::Exception);
  void SetHLXParameter(const std::string &setting, const std::string &value, const HCAL_HLX::u32 boardID, const bool bValidBoardID) throw (xdaq::exception::Exception);
  void ConfigFileInit(const std::string &FileName) throw (xdaq::exception::Exception);
  
  void WebShowFSMControls(xgi::Output * out) throw (xgi::exception::Exception);
  void WebConstructPageHeader(xgi::Output * out, const std::string &title) throw (xgi::exception::Exception);
  void WebShowGenericLinks(xgi::Output *out) throw (xgi::exception::Exception);
  void WebShowHighlightOption(xgi::Output *out,
			      std::string *title,
			      std::string *setting,
			      HCAL_HLX::u32 numOptions,
			      HCAL_HLX::u32 currentOption,
			      std::string *optionValues,
			      std::string *optionDisplays,
			      HCAL_HLX::i32 boardID) throw (xgi::exception::Exception);
  
  void WebShowHighlightOption(xgi::Output *out,
			      std::string *title,
			      std::string *setting,
			      HCAL_HLX::u32 numOptions,
			      const bool  *currentOption,
			      std::string *optionValues,
			      std::string *optionDisplays,
			      HCAL_HLX::i32 boardID) throw (xgi::exception::Exception);
  
  
  void WebShowTextOption(xgi::Output *out,
			 std::string *title,
			 std::string *setting,
			 std::string *defaultValue,
			 HCAL_HLX::i32 boardID) throw (xgi::exception::Exception);
  
  void WebShowHexOption(xgi::Output *out,
			std::string *title,
			std::string *setting,
			std::string *defaultValue,
			HCAL_HLX::i32 boardID) throw (xgi::exception::Exception);
  
  void WebShowFourChannelHexOption(xgi::Output *out,
				   std::string *title,
				   std::string *setting,
				   std::string *defaultValue,
				   HCAL_HLX::i32 boardID) throw (xgi::exception::Exception);
  
  void WebShowModeLengthOption(xgi::Output *out,
			       std::string *title,
			       std::string *setting,
			       HCAL_HLX::u32 numTerms,
			       std::string *currentMode,
			       std::string *ModeOptions,
			       HCAL_HLX::u32 numOptions,
			       std::string *currentLength,
			       HCAL_HLX::i32 boardID) throw (xgi::exception::Exception);
  
  void WebShowPhysicalDataOption(xgi::Output *out,
				 HCAL_HLX::u16 *mFixedPatternData,
				 HCAL_HLX::i32 boardID) throw (xgi::exception::Exception);
  
  void WebShowLocalDataFiles(xgi::Output *out,
			     std::string *setting,
			     std::string *Directory,
			     std::string *FileExtension) throw (xgi::exception::Exception);
  
  // Web page record
  std::string mLastPage;
  
  // State machine control
  //void TriggerFSMTransition(const std::string &in) throw (xdaq::exception::Exception);
  //toolbox::fsm::FiniteStateMachine fsm_;
  //std::string currentState_;
  
  // Infospace Variables
  xdata::String mConfigFile_;
  
};
  
//}

#endif
