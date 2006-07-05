#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


//#include "CalibFormats/SiStripObjects/interface/SiStripStructure.h" // these two will go away
//#include "CalibTracker/Records/interface/SiStripStructureRcd.h"     // these two will go away
#include "CalibTracker/SiStripPedestals/interface/SiStripPedDB.h"

#include "DataFormats/SiStripDetId/interface/SiStripSubStructure.h"
#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h" 
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include "FWCore/Framework/interface/Handle.h"
#include <FWCore/ParameterSet/interface/ParameterSet.h>

// data formats

// std
#include <cstdlib>
#include <string>

using namespace std;

SiStripPedDB::SiStripPedDB(const edm::ParameterSet& iConfig)
{
   conf_ = iConfig;
   pedsPSet_ = conf_.getParameter<edm::ParameterSet>("PedestalsPSet");
   nEvTot_=0;
   apvFactory_=0;
   outPutFileName = conf_.getParameter<string>("OutPutFileName");
   theEventInitNumber_ =  pedsPSet_.getParameter<int>("NumberOfEventsForInit");
   theEventIterNumber_ = pedsPSet_.getParameter<int>("NumberOfEventsForIteration");
   NumCMstripsInGroup_ = pedsPSet_.getParameter<int>("NumCMstripsInGroup");

   //DB related stuff
   
   userEnv_ = "CORAL_AUTH_USER=" + iConfig.getUntrackedParameter<std::string>("userEnv","me");
   passwdEnv_ = "CORAL_AUTH_PASSWORD="+ iConfig.getUntrackedParameter<std::string>("passwdEnv","mypass");
   printdebug_ =iConfig.getUntrackedParameter<bool>("printDebug",false);
     //now do what ever initialization is needed
   
   ::putenv( const_cast<char*>( userEnv_.c_str() ) );
   ::putenv( const_cast<char*>( passwdEnv_.c_str() ) );
   
}


SiStripPedDB::~SiStripPedDB()
{
  if (apvFactory_) {delete apvFactory_;} 
}


void SiStripPedDB::beginJob(const edm::EventSetup& es){
   // retrieve parameters from configuration file
  using namespace edm;
  SelectedDetIds.clear();

  //ApvAnalysisFactory
  apvFactory_ = new ApvAnalysisFactory(pedsPSet_);
  
//getting det id from the fed cabling
  edm::ESHandle<SiStripFedCabling> cabling;
  es.get<SiStripFedCablingRcd>().get( cabling );
  fedCabling_ = const_cast<SiStripFedCabling*>( cabling.product() );

  
  map<uint32_t, int> detIdApv;
  detIdApv.clear();
  //To do so I need to access cabling from FED and so on ... see rob's code
   // Retrieve FED ids from cabling map and iterate through 
  const vector<uint16_t>& fed_ids = fedCabling_->feds(); 
  vector<uint16_t>::const_iterator ifed;
  for ( ifed = fed_ids.begin(); ifed != fed_ids.end(); ifed++ ) {  
    for ( uint16_t channel = 0; channel < 96; channel++ ) {
      const FedChannelConnection& conn = fedCabling_->connection( *ifed, channel );
      uint32_t key_id =  conn.detId();
      int napvs = (conn.nApvPairs())*2;
      if(key_id > 0 && napvs >0) { 
	
	//	cout <<"DetId before the map "<<key_id<< " "<<conn.nApvPairs()<<endl;
	bool newDetId =   apvFactory_->instantiateApvs(key_id,napvs);
	if(newDetId) 
	  SelectedDetIds.push_back(key_id);
      }
    }
    
  }

}


// ------------ method called to produce the data  ------------
void SiStripPedDB::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  nEvTot_++;
  //Increment # of Events
  using namespace edm;
  // retrieve producer name of input StripDigiCollection
    std::string digiProducer = conf_.getParameter<std::string>("DigiProducer");
    // get DigiCollection object from Event
    edm::Handle< edm::DetSetVector<SiStripRawDigi> > digi_collection;
    std::string digiType = "VirginRaw";
    //you have a collection as there are all the digis for the event for every detector
    iEvent.getByLabel(digiProducer, digiType, digi_collection);
    //Pedestals object ...

    SiStripPedestals* SiStripPedestals_ = new SiStripPedestals();
    // loop over all DetIds to be implement
    for(vector<uint32_t>::const_iterator myDet = SelectedDetIds.begin();myDet!=SelectedDetIds.end();myDet++)
      {
	uint32_t detid = *myDet;
	vector< edm::DetSet<SiStripRawDigi> >::const_iterator digis = digi_collection->find( detid );
	if ( digis->data.empty() ) { 
	  edm::LogError("MonitorDigi_tmp") << "[SiStripRawDigiToRaw::createFedBuffers] Zero digis found!"; 
	} 
	//cout <<"Data size "<<digis->data.size()<<endl;
	apvFactory_->update(detid, (*digis));
	//asking for the status
	if((nEvTot_ - theEventInitNumber_)%theEventIterNumber_ == 1)
	  {
	    //Generate Pedestal for det detid
	    std::vector<char> theSiStripVector;  
	    vector<float> tmp_ped;
	    vector<float> tmp_noise;
	    tmp_ped.clear();
	    tmp_noise.clear();
	    apvFactory_->getPedestal(detid, tmp_ped);
	    //apvFactory_->getNoise(detid, tmp_noise);
	    int ibin=0;
	    for (vector<float>::const_iterator iped=tmp_ped.begin(); iped!=tmp_ped.end();iped++) {
	      float ped = *iped;
	      float lTh = 0.5* 12.6;
	      float hTh = 0.5* 12.6;
	      if (hTh < lTh){
		float tmp = hTh;
		hTh = lTh;
		lTh = tmp;
	      }
	      SiStripPedestals_->setData(ped,lTh,hTh,theSiStripVector);
	      ibin++;
	    }
	    
	    SiStripPedestals::Range range(theSiStripVector.begin(),theSiStripVector.end());
	    if ( ! SiStripPedestals_->put(detid,range) )
	      cout<<"[SiStripPedDB::analyze] detid already exists"<<endl;
	    
	    //End now write sistrippedestals data in DB
	    edm::Service<cond::service::PoolDBOutputService> mydbservice;
	    
	    if( mydbservice.isAvailable() ){
	      try{
		size_t callbackToken=mydbservice->callbackToken("SiStripPedestals");
		edm::LogInfo("SiStripPedDB")<<"current time "<<mydbservice->currentTime()<<std::endl;
		mydbservice->newValidityForNewPayload<SiStripPedestals>(SiStripPedestals_,mydbservice->currentTime(), callbackToken);      
	      }catch(const cond::Exception& er){
		edm::LogError("SiStripPedDB")<<er.what()<<std::endl;
	      }catch(const std::exception& er){
		edm::LogError("SiStripPedDB")<<"caught std::exception "<<er.what()<<std::endl;
	      }catch(...){
		edm::LogError("SiStripPedDB")<<"Funny error"<<std::endl;
	      }
  }else{
    edm::LogError("SiStripPedDB")<<"Service is unavailable"<<std::endl;
  }
	  }

      }
}

void SiStripPedDB::endJob(void){}

