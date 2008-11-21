#ifndef _HLX_DIPBroadcaster_H_
#define _HLX_DIPBroadcaster_H_

// system include files
#include <iostream>
#include <string>
#include <memory>
#include <iomanip>
#include <cstdlib>

// user include files
#include "RecoLuminosity/HLXReadOut/CoreUtils/include/ICTypeDefs.hh"
#include "RecoLuminosity/TCPReceiver/interface/LumiStructures.hh"
#include "RecoLuminosity/TCPReceiver/interface/TCPReceiver.h"

#include "RecoLuminosity/HLXReadOut/DIP/include/Dip.h"
#include "RecoLuminosity/HLXReadOut/DIP/include/DipBrowser.h"
#include "RecoLuminosity/HLXReadOut/DIP/include/DipPublicationErrorHandler.h"

using namespace HCAL_HLX;


class ErrHandler:public DipPublicationErrorHandler{
  void handleException(DipPublication* publication, DipException& ex){
    printf("Error because %s", ex.what());
  }
};



class DIPBroadcaster {

public:
  explicit DIPBroadcaster(int port, std::string ip, int reconnect, int aqMode);
  ~DIPBroadcaster();


   void beginJob(std::string pubName, std::string lhPubName);
   void analyze();
   void endJob();
   void resetHistos();
   bool doesPublicationAlreadyExist(std::string pubName); //true if already exists, flase otherwise

private:
  TCPReceiver HT;      
  unsigned int mProcessCounter;
  unsigned int mErrorCount;

  //DIP data
  DipFloat *mHistogramData;
  DipInt mNumHLXs;
  int mTimestamp;
  int mTimestamp_micros;
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
  DipFactory *mDIP;

  DipBrowser *mBrowser;


  //connection params
  unsigned int AquireMode;
  unsigned int reconnTime;
  std::string DistribIP;
  int listenPort;

  int mCounter;

  HCAL_HLX::LUMI_SECTION lumiSection;
};

#endif
