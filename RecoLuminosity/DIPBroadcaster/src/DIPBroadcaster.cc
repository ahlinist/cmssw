/*
    Author:  Jeremy Werner (jwerner@princeton.edu)
    Institution: Princeton University, USA
    Date: June, 2008
*/
#include "RecoLuminosity/DIPBroadcaster/interface/DIPBroadcaster.h"
#include <iostream>
#include <time.h>
#include <sstream>


using std::cout;
using std::endl;

DIPBroadcaster::DIPBroadcaster(int port, std::string ip, int reconnect, int aqMode){
  

  listenPort= port;
  DistribIP= ip;
  reconnTime= reconnect;
  AquireMode= aqMode;

  //std::cout<<"In DIPBroadcaster constructor"<<std::endl;

  try {
    mProcessCounter = 0;
    mDIP = 0;
    mBrowser = 0;
    mDIPPublisher = 0;
    mDIPPublisherLH = 0;
    mDIPData = 0;
    mDIPDataLH = 0;
    mErrorCount = 0;
    mHistogramData = 0;
    mInstantLumi = 0;
    mInstantLumiError = 0;
    mTimestamp = 0;

    time_t start_time = time(NULL);
    struct tm * timeinfo = localtime(&start_time);
    std::ostringstream tmp;
    tmp<<asctime(timeinfo);
    std::string dipFactoryName = "CMS/HF/Lumi/LumiPublisher_";
    dipFactoryName += tmp.str();

    mDIP = Dip::create(dipFactoryName.c_str());
    
    if ( !mDIP ) {
      std::string tempString = "Unable to allocate DIP interface";
      std::cout<<tempString.c_str()<<std::endl;
    }

    // Create the publication data structure                                                                                    
    mDIPData = mDIP->createDipData();
    mDIPDataLH = mDIP->createDipData();
    if ( !mDIPData || !mDIPDataLH) {
      std::string tempString = "Unable to create DIP data";
      std::cout<<tempString.c_str()<<std::endl;      
    }

    // Initialise the data structures
    mHistogramData = new DipFloat[3564];
    for(int i=0; i< 3564; i++){
      mHistogramData[i]= 0;
    }


  } catch (...) {
    std::cout<<"exception in DIPBroadcaster constructor"<<std::endl;
  }

  //std::cout<<"exiting DIPBroadcaster constructor"<<std::endl;

}

DIPBroadcaster::~DIPBroadcaster(){
  HT.Disconnect();

  // Destroy all the interfaces
  mDIP->destroyDipPublication(mDIPPublisher);
  mDIP->destroyDipPublication(mDIPPublisherLH);

  // Shut down DIP
  Dip::shutdown();

  mDIP = 0;
  mBrowser = 0;
  mDIPData = 0;
  mDIPDataLH = 0;
  mDIPPublisher = 0;
  mDIPPublisherLH = 0;

}


void DIPBroadcaster::analyze(){
  //std::cout<<"entered analyze"<<std::endl;

  try {

  int errorCode;

  do{
    //std::cout<<"grabbing lumi section"<<std::endl;

    //std::cout<<"Is connected= "<<HT.IsConnected()<<std::endl;

    errorCode = HT.ReceiveLumiSection(lumiSection);
    //cout << "ReceiveLumiSection: " << errorCode << endl;
    while(errorCode !=1){
      HT.Disconnect();
      cout << "Connecting to TCPDistributor" << endl;
      errorCode = HT.Connect();
      if(errorCode != 1){
	cout << "*** Connection Failed: " << errorCode << " Attempting reconnect in " << reconnTime << " seconds." << endl;
	sleep(1);
      }
    }    
  }while(errorCode != 1);

  //std::cout<<"... in DIPDistributor::ProcessSection"<<std::endl;

  // Load the DIP header information                                                                                            
  mCounter++;
  mRunNumber = lumiSection.hdr.runNumber;
  mSectionNumber = lumiSection.hdr.sectionNumber;
  mNumHLXs = lumiSection.hdr.numHLXs;
  mTimestamp = lumiSection.hdr.timestamp;
  mTimestamp_micros = lumiSection.hdr.timestamp_micros;
  mStartOrbit = lumiSection.hdr.startOrbit;
  mNumOrbits = lumiSection.hdr.numOrbits;
  mNumBunches = lumiSection.hdr.numBunches;
  mInstantLumi = lumiSection.lumiSummary.InstantLumi;
  //TRandom* random = new TRandom(); 
  //random->SetSeed(mCounter);
  //mInstantLumi = random->PoissonD(100.0);
  mInstantLumiError = lumiSection.lumiSummary.InstantLumiErr;
  //mInstantLumiError = random->PoissonD(10.0);

  mDIPData->insert(mNumHLXs, "Number of HLXs");
  mDIPData->insert(mRunNumber, "Run number");
  mDIPData->insert(mSectionNumber, "Section number");
  mDIPData->insert(mStartOrbit, "Start orbit");
  mDIPData->insert(mNumOrbits, "Number of orbits");
  mDIPData->insert(mNumBunches, "Number of bunches");
  mDIPData->insert(mInstantLumi, "Inst Lumi");
  mDIPData->insert(mInstantLumiError, "Inst Lumi Error");

  DipTimestamp time;

  long long time_64 =  mTimestamp;
  time_64 = time_64*1000 + mTimestamp_micros/1000; 
  time = time_64; 
  mDIPPublisher->send(*mDIPData,time);

  //this next line controls the frequency of the lumi histo
  if(mProcessCounter%1 == 0){
    for(int i=0; i< 3564; i++){
      mHistogramData[i]= 0;
    }

    for ( u32 i = 0 ; i != lumiSection.hdr.numBunches ; i++ ) {
      mHistogramData[i] = lumiSection.lumiDetail.LHCLumi[i];
      //mHistogramData[i] = random->PoissonD(10.0);
      //handle spurious data in final BXs
      //if(i >=3528){ mHistogramData[i]= mHistogramData[3527];}
    }
    
    mDIPDataLH->insert(mHistogramData, 3564, "Luminosity Histogram");
    // Publish the data
    mDIPPublisherLH->send(*mDIPDataLH,time);
  }

  mProcessCounter++;
  //std::cout<<"exiting analyze"<<std::endl;


  }catch(...){std::cout<<"Exception thrown in DIPBroadcaster::analyze"<<std::endl;}

  //sleep(1);
}  //analyze done 

void DIPBroadcaster::beginJob(std::string pubName, std::string lhPubName){
 
  try{

    //std::cout<<"In DIPBroadcaster::beginJob"<<std::endl;
    
    mDIPPublisher = mDIP->createDipPublication(pubName.c_str(), &mDIPErrorHandler);
    mDIPPublisherLH = mDIP->createDipPublication(lhPubName.c_str(), &mDIPErrorHandler);

    if ( !mDIPPublisher || !mDIPPublisherLH ) {
      std::string tempString = "Unable to create DIP publications";
      std::cout<<tempString.c_str()<<std::endl;
    }

    int errorCode;

    HT.SetIP(DistribIP);

    errorCode = HT.SetPort(listenPort);
    cout << "SetPort: " << errorCode << endl;
    errorCode = HT.SetMode(AquireMode);
    cout << "AquireMode: " << errorCode << endl;
  
    do{
      // cout << "Connecting to TCPDistributor" << endl;
      errorCode = HT.Connect();
      if(errorCode != 1){
	cout << "Error code= "<<errorCode<<" Attempting to reconnect in " << reconnTime << " seconds." << endl;
	sleep(reconnTime);
      }
    }while(errorCode != 1);

    //std::cout<<"exiting beginjob"<<std::endl;

  }catch(...){std::cout<<"Exception thrown in DIPBroadcaster::beginJob"<<std::endl;}
}

bool DIPBroadcaster::doesPublicationAlreadyExist(std::string pubName){

  bool exists= true;  
  
  try{
    mBrowser = mDIP->createDipBrowser();
    const char** pubNames;
    unsigned int numMatchingPubs= 0;
    pubNames=  mBrowser->getPublications(pubName.c_str(), numMatchingPubs);
    if(numMatchingPubs == 0){exists= false;}
  }catch(...){std::cout<<"Exception thrown in DIPBroadcaster::doesPublicationAlreadyExist"<<std::endl;} 
  return exists;
}

void DIPBroadcaster::endJob() {
  try{
    //std::cout<<"In DIPBroadcaster::endJob"<<std::endl;
    HT.Disconnect();
    // Destroy all the interfaces
    mDIP->destroyDipPublication(mDIPPublisher);
    mDIP->destroyDipPublication(mDIPPublisherLH);

    // Shut down DIP                                                                                                        
    Dip::shutdown();

    mDIP = 0;
    mBrowser = 0;
    mDIPData = 0;
    mDIPDataLH = 0;
    mDIPPublisher = 0;
    mDIPPublisherLH = 0;
    //std::cout<<"exiting DIPBroadcaster::endJob"<<std::endl;
  }catch(...){std::cout<<"Exception thrown in DIPBroadcaster::endJob"<<std::endl;} 
}


