/*************************************************************************
 * XDAQ Layer for HCAL HLX                                               *
 * Copyright (C) 2007, Princeton University		                 *
 * All rights reserved.                                                  *
 * Author(s): J. Jones           	   			         *
 *************************************************************************/

#ifndef HLX_READOUT_SUPERVISOR_HH
#define HLX_READOUT_SUPERVISOR_HH

#include "xdaq/Application.h"

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
#include "DebugCoutDistributor.hh"
#include "GIFDistributor.hh"
#include "OracleDistributor.hh"
#include "ROOTDistributor.hh"
#include "TCPDistributor.hh"

//#include "HLXInterface.hh"

using namespace HCAL_HLX;

//namespace HCAL_HLX {

  class HLXReadoutSupervisor: public xdaq::Application, public xdata::ActionListener
  {
    
  public:
    
    XDAQ_INSTANTIATOR();
    HLXReadoutSupervisor(xdaq::ApplicationStub * s) throw (xdaq::exception::Exception);
    ~HLXReadoutSupervisor();
    
    // SOAP INTERFACES
    xoap::MessageReference HLXControl(xoap::MessageReference message) throw (xoap::exception::Exception);
    xoap::MessageReference ChangeState(xoap::MessageReference message) throw (xoap::exception::Exception);
    
    // FSM INTERFACE
    //void ConfigureAction(toolbox::Event::Reference e) throw (xdaq::exception::Exception);
    //void HaltAction(toolbox::Event::Reference e) throw (xdaq::exception::Exception);
    //void EnableAction(toolbox::Event::Reference e) throw (xdaq::exception::Exception);
    //void InitAction(toolbox::Event::Reference e) throw (xdaq::exception::Exception);
    
    // HYPERDAQ INTERFACES
    void Default(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception);
    //void IndividualStatus(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception);
    //void GlobalConfig(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception);
    //void ParameterSet(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception);
    //void RestartCards(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
    //void TransmitterControl(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
    //void RCTCapture(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
    void TransitionFSMConfigure(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
    void TransitionFSMEnable(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
    void TransitionFSMHalt(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
    
    //void ReadRCTData(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
    
    // INFOSPACE INTERFACES (PURE VIRTUAL SO MUST BE IMPLEMENTED)
    void actionPerformed(xdata::Event &e) throw (xoap::exception::Exception);
    
  private:
    
    // Worker thread for readout processing
    static void WorkerThread(void *thisPtr);

    // Associated variables
    pthread_t mThreadId;
    bool mWorkerThreadContinue;

    // Collector instances
    NibbleCollector *mNibbleCollector;

    SectionCollector *mSectionCollectorShort;
    SectionCollector *mSectionCollectorLong;

    DIPDistributor *mDIPDistributor;
    DebugCoutDistributor *mDebugCoutDistributor;
    GIFDistributor *mGIFDistributor;
    OracleDistributor *mOracleDistributor;
    ROOTDistributor *mROOTDistributor;
    TCPDistributor *mTCPDistributor;



    
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
					//HCAL_HLX::HLXInterface::SC_ROUTING_MODE mRoutingMode,
					HCAL_HLX::i32 boardID) throw (xgi::exception::Exception);


    void WebShowLocalDataFiles(xgi::Output *out,
			           std::string *setting,
			   	    std::string *Directory,
			   	    std::string *FileExtension) throw (xgi::exception::Exception);

    
    // Core Classes - HLX interface
    //HCAL_HLX::HLXInterface *mHLX;
    
    // Web page record
    std::string mLastPage;
    
    // State machine control
    void TriggerFSMTransition(const std::string &in) throw (xdaq::exception::Exception);
    toolbox::fsm::FiniteStateMachine fsm_;
    std::string currentState_;
    
    // Infospace Variables
    xdata::String mConfigFile_;

    // Bus information
    xdata::Integer mVMEBus_;
    xdata::Integer mHTRSlot_;
    xdata::Integer mSLBSite_;

  };
  
//}

#endif
