/*
    Author:  Jeremy Werner (jwerner@princeton.edu)
    Institution: Princeton University, USA
    Date: May, 2008
*/
#include "RecoLuminosity/DIPBroadcaster/interface/DIPBroadcaster.h"

using std::cout;
using std::endl;

DIPBroadcaster::DIPBroadcaster(const edm::ParameterSet& iConfig){
  
  //std::cout<<"In DIPBroadcaster constructor"<<std::endl;

  listenPort  = iConfig.getUntrackedParameter<unsigned int>("SourcePort", 51001);
  DistribIP   = iConfig.getUntrackedParameter<std::string>("HLXIP","localhost");
  reconnTime  = iConfig.getUntrackedParameter<unsigned int>("ReconnectionTime",5);
  AquireMode  = iConfig.getUntrackedParameter<unsigned int>("AquireMode",0);

  try {
    mProcessCounter = 0;
    mDIP = 0;
    mDIPPublisher = 0;
    mDIPPublisherLH = 0;
    mDIPData = 0;
    mDIPDataLH = 0;
    mErrorCount = 0;
    mHistogramData = 0;
    mInstantLumi = 0;
    mInstantLumiError = 0;

    // Open the DIP interface
    //std::cout<<"Creating DIP"<<std::endl;

    mDIP = Dip::create("CMS/HF/Lumi/LumiPublisher");
    if ( !mDIP ) {
      std::string tempString = "Unable to allocate DIP interface";
      std::cout<<tempString.c_str()<<std::endl;
    }

    char itemName[]={"dip/CMS/HF/LumiData_JW"};
    char itemNameLH[]={"dip/CMS/HF/LumiDataLH_JW"};

    // Create the DIP publication interface
    //cout << "Creating DIP publication " << itemName << endl;
    
    mDIPPublisher = mDIP->createDipPublication(itemName, &mDIPErrorHandler);
    mDIPPublisherLH = mDIP->createDipPublication(itemNameLH, &mDIPErrorHandler);

    if ( !mDIPPublisher || !mDIPPublisherLH ) {
      std::string tempString = "Unable to create DIP publications";
      std::cout<<tempString.c_str()<<std::endl;
    }

    //cout << "Created DIP publication " << itemName << endl;

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
  mDIPData = 0;
  mDIPDataLH = 0;
  mDIPPublisher = 0;
  mDIPPublisherLH = 0;

}


void DIPBroadcaster::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  //std::cout<<"entered analyze"<<std::endl;
  using namespace edm;
  int errorCode;

  do{
    //std::cout<<"grabbing lumi section"<<std::endl;
    errorCode = HT.ReceiveLumiSection(lumiSection);
    //cout << "ReceiveLumiSection: " << errorCode << endl;
    while(errorCode !=1){
      HT.Disconnect();
      cout << "Connecting to TCPDistributor" << endl;
      errorCode = HT.Connect();
      if(errorCode != 1){
	cout << "*** Connection Failed: " << errorCode << " Attempting reconnect in " << reconnTime << " seconds." << endl;
	sleep(reconnTime);
      }
    }    
  }while(errorCode != 1);

  //std::cout<<"... in DIPDistributor::ProcessSection"<<std::endl;
  // Load the DIP header information                                                                                            
  mRunNumber = lumiSection.hdr.runNumber;
  mSectionNumber = lumiSection.hdr.sectionNumber;
  mNumHLXs = lumiSection.hdr.numHLXs;
  mStartOrbit = lumiSection.hdr.startOrbit;
  mNumOrbits = lumiSection.hdr.numOrbits;
  mNumBunches = lumiSection.hdr.numBunches;
  mInstantLumi = lumiSection.lumiSummary.InstantLumi;
  mInstantLumiError = lumiSection.lumiSummary.InstantLumiErr;

  //std::cout<<"bout to insert data"<<std::endl;
  mDIPData->insert(mNumHLXs, "Number of HLXs");
  mDIPData->insert(mRunNumber, "Run number");
  mDIPData->insert(mSectionNumber, "Section number");
  mDIPData->insert(mStartOrbit, "Start orbit");
  mDIPData->insert(mNumOrbits, "Number of orbits");
  mDIPData->insert(mNumBunches, "Number of bunches");
  mDIPData->insert(mInstantLumi, "Inst Lumi");
  mDIPData->insert(mInstantLumiError, "Inst Lumi Error");

  DipTimestamp time;
  mDIPPublisher->send(*mDIPData,time);
  //std::cout<<"published data"<<std::endl;

  //handle the Lumi vs BX Histo
  //std::cout<<"mProcessCounter= "<<mProcessCounter<<std::endl;

  if(mProcessCounter%10 == 0){
    //std::cout<<"mProcessCounter divisible by 10... Lumi Histo Go!"<<std::endl;

    for(int i=0; i< 3564; i++){
      mHistogramData[i]= 0;
    }

    for ( u32 i = 0 ; i != lumiSection.hdr.numBunches ; i++ ) {
      mHistogramData[i] = lumiSection.lumiDetail.LHCLumi[i];
      //handle spurious data in final BXs
      if(i >=3528){ mHistogramData[i]= mHistogramData[3527];}
    }
    
    //for( u32 i= 3500;i<3564; i++){
      //std::cout<<"mHistogramData["<<i<<"]= "<<mHistogramData[i]<<std::endl;
    //}

    mDIPDataLH->insert(mHistogramData, 3564, "Luminosity Histogram");
    // Publish the data
    mDIPPublisherLH->send(*mDIPDataLH,time);
  }//done if counter divisible by 10

  mProcessCounter++;
  //std::cout<<"exiting analyze"<<std::endl;

}  //analyze done 

void DIPBroadcaster::beginJob(const edm::EventSetup&){
 
  //std::cout<<"In DIPBroadcaster::beginJob"<<std::endl;

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
}

void DIPBroadcaster::endJob() {
  //std::cout<<"In DIPBroadcaster::endJob"<<std::endl;
  HT.Disconnect();
  // Destroy all the interfaces
  mDIP->destroyDipPublication(mDIPPublisher);
  mDIP->destroyDipPublication(mDIPPublisherLH);

// Shut down DIP                                                                                                        
Dip::shutdown();

 mDIP = 0;
 mDIPData = 0;
 mDIPDataLH = 0;
 mDIPPublisher = 0;
 mDIPPublisherLH = 0;
 //std::cout<<"exiting DIPBroadcaster::endJob"<<std::endl;
}

DEFINE_FWK_MODULE(DIPBroadcaster);
