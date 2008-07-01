#ifndef _HLX_DIPBroadcaster_H_
#define _HLX_DIPBroadcaster_H_

// system include files
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include <cstdlib>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "RecoLuminosity/HLXReadOut/CoreUtils/include/ICTypeDefs.hh"
#include "RecoLuminosity/HLXReadOut/HLXCoreLibs/include/LumiStructures.hh"
#include "RecoLuminosity/TCPReceiver/interface/TCPReceiver.h"

#include "RecoLuminosity/HLXReadOut/DIP/include/Dip.h"
#include "RecoLuminosity/HLXReadOut/DIP/include/DipPublicationErrorHandler.h"

using namespace HCAL_HLX;


class ErrHandler:public DipPublicationErrorHandler{
  void handleException(DipPublication* publication, DipException& ex){
    printf("Error because %s", ex.what());
  }
};



class DIPBroadcaster : public edm::EDAnalyzer {

public:
  explicit DIPBroadcaster(const edm::ParameterSet&);
  ~DIPBroadcaster();

private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();
  void resetHistos();

  TCPReceiver HT;      
  unsigned int mProcessCounter;
  unsigned int mErrorCount;

  //DIP data
  DipFloat *mHistogramData;
  DipInt mNumHLXs;
  DipInt mRunNumber;
  DipInt mSectionNumber;
  DipInt mStartOrbit;
  DipInt mNumOrbits;
  DipInt mNumBunches;
  DipInt bCMSLive;
  DipFloat mInstantLumi;
  DipFloat mInstantLumiError;

  // DIP data structures for lumi sections                                                                            
  DipData * mDIPData;
  DipData * mDIPDataLH;

  // Publisher interface                                                                                              
  DipPublication * mDIPPublisher;
  DipPublication * mDIPPublisherLH;

  ErrHandler  mDIPErrorHandler;

  // DIP interface                                                                                                    
  DipFactory * mDIP;

  //connection params
  unsigned int AquireMode;
  unsigned int reconnTime;
  std::string DistribIP;
  int listenPort;

  HCAL_HLX::LUMI_SECTION lumiSection;
};

#endif
