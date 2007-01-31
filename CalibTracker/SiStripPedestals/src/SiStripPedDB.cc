#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


#include "CalibTracker/SiStripPedestals/interface/SiStripPedDB.h"

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

SiStripPedDB::SiStripPedDB(const edm::ParameterSet& iConfig)
{
   conf_ = iConfig;
   pedsPSet_ = conf_.getParameter<edm::ParameterSet>("PedestalsPSet");
   nEvTot_=0;
   apvFactory_=0;
   theEventInitNumber_ =  pedsPSet_.getParameter<int>("NumberOfEventsForInit");
   theEventIterNumber_ = pedsPSet_.getParameter<int>("NumberOfEventsForIteration");
   NumCMstripsInGroup_ = pedsPSet_.getParameter<int>("NumCMstripsInGroup");

}


SiStripPedDB::~SiStripPedDB()
{
  if (apvFactory_) {delete apvFactory_;} 
}


void SiStripPedDB::beginJob(const edm::EventSetup& es){
   // retrieve parameters from configuration file
  using namespace edm;
  SelectedDetIds.clear();

  //Pedestals object ...
  SiStripPedestals_ = new SiStripPedestals();

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
	    mSiStripPedestals.clear();
	    //Generate Pedestal for det detid
	    std::vector<char> theSiStripVector;  
	    vector<float> tmp_ped;
	    tmp_ped.clear();
	    apvFactory_->getPedestal(detid, tmp_ped);
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
	    mSiStripPedestals.push_back(make_pair(detid,theSiStripVector));
	  }	
      }
}

void SiStripPedDB::endJob(void){
  edm::LogInfo("SiStripPedDB") << "... now write sistrippedestals data in DB" << std::endl;
	
  for (std::vector< std::pair<uint32_t, std::vector<char> > >::const_iterator iter=mSiStripPedestals.begin(); iter!=mSiStripPedestals.end();iter++)
    {
      edm::LogInfo("SiStripPedDB")<<"uploading detid "<< iter->first << " vector size " << iter->second.size() <<std::endl;
      SiStripPedestals::Range range(iter->second.begin(),iter->second.end());
      if ( ! SiStripPedestals_->put(iter->first,range) )
	edm::LogError("SiStripPedDB") <<"[SiStripPedDB::analyze] detid " << iter->first << "already exists"<<endl;
    }       

  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  
  if( mydbservice.isAvailable() ){
    try{
      mydbservice->createNewIOV<SiStripPedestals>(SiStripPedestals_,mydbservice->endOfTime(), "SiStripPedestalsRcd");      
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

