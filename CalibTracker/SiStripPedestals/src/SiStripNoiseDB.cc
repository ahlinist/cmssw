#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


#include "CalibTracker/SiStripPedestals/interface/SiStripNoiseDB.h"

#include "DataFormats/SiStripDetId/interface/SiStripSubStructure.h"
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

SiStripNoiseDB::SiStripNoiseDB(const edm::ParameterSet& iConfig)
{
   conf_ = iConfig;
   pedsPSet_ = conf_.getParameter<edm::ParameterSet>("PedestalsPSet");
   nEvTot_=0;
   apvFactory_=0;
   theEventInitNumber_ =  pedsPSet_.getParameter<int>("NumberOfEventsForInit");
   theEventIterNumber_ = pedsPSet_.getParameter<int>("NumberOfEventsForIteration");
   NumCMstripsInGroup_ = pedsPSet_.getParameter<int>("NumCMstripsInGroup");

}


SiStripNoiseDB::~SiStripNoiseDB()
{
  if (apvFactory_) {delete apvFactory_;} 
}


void SiStripNoiseDB::beginJob(const edm::EventSetup& es){
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
void SiStripNoiseDB::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
    //Noises object ...

    SiStripNoises_ = new SiStripNoises();
    
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
	    mSiStripNoises.clear();
	    //Generate Pedestal for det detid
	    std::vector<short> theSiStripVector;  
	    vector<float> tmp_noise;
	    tmp_noise.clear();
	    apvFactory_->getNoise(detid, tmp_noise);
	    TkApvMask::MaskType temp;
	    apvFactory_->getMask(detid, temp);
	    int ibin=0;
	    for (vector<float>::const_iterator iped=tmp_noise.begin(); iped!=tmp_noise.end();iped++) {
	      float noise = *iped;
	      bool disable = true;
	      if(temp[ibin] == 0) disable = false;

	      SiStripNoises_->setData(noise,disable,theSiStripVector);
	      ibin++;
	    } 
	    mSiStripNoises.push_back(make_pair(detid,theSiStripVector));
	  }
      }
}

void SiStripNoiseDB::endJob(void){

  edm::LogInfo("SiStripNoiseDB") << "... now write sistripnoise data in DB" << std::endl;

  for (std::vector< std::pair<uint32_t, std::vector<short> > >::const_iterator iter=mSiStripNoises.begin(); iter!=mSiStripNoises.end();iter++)
    {
      SiStripNoises::Range range(iter->second.begin(),iter->second.end());
      if ( ! SiStripNoises_->put(iter->first,range) )
	edm::LogError("SiStripNoiseDB") <<"[SiStripNoiseDB::analyze] detid " << iter->first << "already exists"<<endl;
    }    
	
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  
  if( mydbservice.isAvailable() ){
    try{
      edm::LogInfo("SiStripNoiseDB")<<"current time "<<mydbservice->currentTime()<<std::endl;
      mydbservice->createNewIOV<SiStripNoises>(SiStripNoises_,mydbservice->endOfTime(), "SiStripNoisesRcd");      
    }catch(const cond::Exception& er){
      edm::LogError("SiStripNoiseDB")<<er.what()<<std::endl;
    }catch(const std::exception& er){
      edm::LogError("SiStripNoiseDB")<<"caught std::exception "<<er.what()<<std::endl;
    }catch(...){
      edm::LogError("SiStripNoiseDB")<<"Funny error"<<std::endl;
    }
  }else{
    edm::LogError("SiStripNoiseDB")<<"Service is unavailable"<<std::endl;
  }
}



