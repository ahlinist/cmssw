
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

// data formats
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripEventSummary.h"

#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"

#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h" 
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"

#include "CalibTracker/SiStripPedestals/interface/SiStripOfflinePedNoiseToDb.h"

#include <sstream>

namespace cms{
  SiStripOfflinePedNoiseToDb::SiStripOfflinePedNoiseToDb(const edm::ParameterSet& conf):
    conf_(conf),
    apvFactory_(0),
    nEvTot_(0),  
    appendMode_(conf.getUntrackedParameter<bool>("appendMode",true)){
    edm::ParameterSet p = conf.getParameter<edm::ParameterSet>("PedestalsPSet");
    theEventInitNumber_ = p.getParameter<int>("NumberOfEventsForInit");
    theEventIterNumber_ = p.getParameter<int>("NumberOfEventsForIteration");
    NumCMstripsInGroup_ = p.getParameter<int>("NumCMstripsInGroup");
  }


  SiStripOfflinePedNoiseToDb::~SiStripOfflinePedNoiseToDb()
  {
    if (apvFactory_) {delete apvFactory_;} 
  }
  
  void SiStripOfflinePedNoiseToDb::beginJob(const edm::EventSetup& es){
    // retrieve parameters from configuration file
    SelectedDetIds_.clear();


    //Pedestal and Noise objects ...
    SiStripPedestals_ = new SiStripPedestals();
    SiStripNoises_ = new SiStripNoises();

    //Get SiStripDetCabling
    edm::ESHandle<SiStripDetCabling> SiStripDetCabling_;
    es.get<SiStripDetCablingRcd>().get( SiStripDetCabling_ );

    // get list of active detectors from SiStripDetCabling - this will change and be taken from a SiStripDetControl object
    SiStripDetCabling_->addActiveDetectorsRawIds(SelectedDetIds_);

    //ApvAnalysisFactory
    apvFactory_ = new ApvAnalysisFactory(conf_.getParameter<edm::ParameterSet>("PedestalsPSet"));
  }


  // ------------ method called to produce the data  ------------
  void SiStripOfflinePedNoiseToDb::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
  {
    //Increment # of Events
    nEvTot_++;

    if ( appendMode_ ){
      edm::ESHandle<SiStripPedestals> peds;
      iSetup.get<SiStripPedestalsRcd>().get(peds);

      edm::ESHandle<SiStripNoises> noises;
      iSetup.get<SiStripNoisesRcd>().get(noises);
    }


    // retrieve producer name of input StripDigiCollection
    std::string digiProducer = conf_.getParameter<std::string>("DigiProducer");
    std::string digiType = conf_.getParameter<std::string>("DigiType");

    // get DigiCollection object from Event
    edm::Handle< edm::DetSetVector<SiStripRawDigi> > digi_collection;
    //you have a collection as there are all the digis for the event for every detector
    iEvent.getByLabel(digiProducer, digiType, digi_collection);
    
    // loop over all DetIds to be implement
    for(std::vector<uint32_t>::const_iterator myDet = SelectedDetIds_.begin();myDet!=SelectedDetIds_.end();myDet++)
      {
	uint32_t detid = *myDet;
	std::vector< edm::DetSet<SiStripRawDigi> >::const_iterator digis = digi_collection->find( detid );
	if ( digis->data.empty() ) { 
	  edm::LogError("SiStripOfflinePedNoiseToDb") << "[" << __PRETTY_FUNCTION__ <<"]  Zero digis found for DetId " << detid << std::endl; 
	} 
	//cout <<"Data size "<<digis->data.size()<<endl;
	apvFactory_->update(detid, (*digis));
	//asking for the status
	if((nEvTot_ - theEventInitNumber_)%theEventIterNumber_ == 1)
	  {
	    mSiStripPedestals_.clear();
	    mSiStripNoises_.clear();
	    
	    //Get Pedestal and Noise for detid
	    std::vector<char>  theSiStripPedVector;
	    std::vector<short> theSiStripNoiseVector;
	   
	    std::vector<float> tmp_ped;
	    std::vector<float> tmp_noi;
	    TkApvMask::MaskType tmp_bad;
	    tmp_ped.clear();
	    tmp_noi.clear();
	    apvFactory_->getPedestal(detid, tmp_ped);
	    apvFactory_->getNoise(detid, tmp_noi);
	    apvFactory_->getMask(detid, tmp_bad);
	    
	    if (tmp_ped.size()!=tmp_noi.size()){
	      std::stringstream ss;
	      ss << "[" << __PRETTY_FUNCTION__ <<"]  pedestal and noise container have different size!!" << detid << std::endl; 
	      edm::LogError("SiStripOfflinePedNoiseToDb") << ss.str();
	      throw cms::Exception("") << ss.str(); 
	    }

	    size_t istop=tmp_ped.size();
	    for (size_t ibin=0; ibin<istop; ibin++){
	      float ped = tmp_ped[ibin];
	      float lTh = 2.;
	      float hTh = 5.;
	      float noise = tmp_noi[ibin];
	      bool disable = (tmp_bad[ibin] == 0) ? false : true;

	      //edm::LogInfo("SiStripOfflinePedNoiseToDb") 
	      std::cout << "\n\n Domenico Ped Noise lth hth disable " << ped << " " << noise << " " << lTh << " " << hTh << " " << disable << "\n\n" << std::endl;
	      SiStripPedestals_->setData(ped,lTh,hTh,theSiStripPedVector);
	      SiStripNoises_->setData(noise,disable,theSiStripNoiseVector);
	    }
	    mSiStripPedestals_.push_back(make_pair(detid,theSiStripPedVector));
	    mSiStripNoises_.push_back(make_pair(detid,theSiStripNoiseVector));
	  }	
      }
  }

  void SiStripOfflinePedNoiseToDb::endJob(void){
    edm::LogInfo("SiStripOfflinePedNoiseToDb") << "... now write SiStrip Ped Noise data in DB" << std::endl;

    if (mSiStripPedestals_.size()==0 || mSiStripNoises_.size()==0){
      std::stringstream ss;
      ss << "[" << __PRETTY_FUNCTION__ <<"]  pedestal container size " << mSiStripPedestals_.size() << " noise container size " << mSiStripNoises_.size() << "\nExit without fill DB "<< std::endl; 
      edm::LogError("SiStripOfflinePedNoiseToDb") << ss.str();
      throw cms::Exception("") << ss.str(); 
      return;
    }

	
    for (std::vector< std::pair<uint32_t, std::vector<char> > >::const_iterator iter=mSiStripPedestals_.begin(); iter!=mSiStripPedestals_.end();iter++){
      edm::LogInfo("SiStripOfflinePedNoiseToDb")<<"uploading detid "<< iter->first << " vector size " << iter->second.size() <<std::endl;
      SiStripPedestals::Range range(iter->second.begin(),iter->second.end());
      if ( ! SiStripPedestals_->put(iter->first,range) )
	edm::LogError("SiStripOfflinePedNoiseToDb") <<"[SiStripOfflinePedNoiseToDb::analyze] detid " << iter->first << "already exists"<<std::endl;
    }       

    for (std::vector< std::pair<uint32_t, std::vector<short> > >::const_iterator iter=mSiStripNoises_.begin(); iter!=mSiStripNoises_.end();iter++){
      SiStripNoises::Range range(iter->second.begin(),iter->second.end());
      if ( ! SiStripNoises_->put(iter->first,range) )
	edm::LogError("SiStripNoiseDB") <<"[SiStripNoiseDB::analyze] detid " << iter->first << "already exists"<<std::endl;
    }    

    edm::Service<cond::service::PoolDBOutputService> mydbservice;
    
    if( mydbservice.isAvailable() ){
      try{
	uint32_t callbackToken;
	unsigned long long tillTime;
	if ( appendMode_){
	  tillTime = conf_.getUntrackedParameter<uint32_t>("StartIOV",0); //the -1 allows to start the new IOV from the pedestal run under analysis
	  if (tillTime == 0)
	    tillTime = mydbservice->currentTime();
	}
	else
	  tillTime = mydbservice->endOfTime();

	edm::LogInfo("SiStripOfflinePedNoiseToDb")<<"["<<__PRETTY_FUNCTION__<< "] tillTime = " << tillTime << std::endl;
	
	if (conf_.getUntrackedParameter<bool>("doPedNoiseTansfer",false)){
	  callbackToken=mydbservice->callbackToken("SiStripPedestals");
	  edm::LogInfo("SiStripOfflinePedNoiseToDb")<<"callbackToken SiStripPedestals "<<callbackToken<<std::endl;
	  mydbservice->newValidityForNewPayload<SiStripPedestals>(SiStripPedestals_,tillTime,callbackToken);      
	  callbackToken=mydbservice->callbackToken("SiStripNoises");
	  edm::LogInfo("SiStripOfflinePedNoiseToDb")<<"callbackToken SiStripNoises "<<callbackToken<<std::endl;
	  mydbservice->newValidityForNewPayload<SiStripNoises>(SiStripNoises_,tillTime,callbackToken);      
	}
     }catch(const cond::Exception& er){
	edm::LogError("SiStripOfflinePedNoiseToDb")<<er.what()<<std::endl;
      }catch(const std::exception& er){
	edm::LogError("SiStripOfflinePedNoiseToDb")<<"caught std::exception "<<er.what()<<std::endl;
      }catch(...){
	edm::LogError("SiStripOfflinePedNoiseToDb")<<"Funny error"<<std::endl;
      }
    }else{
      edm::LogError("SiStripOfflinePedNoiseToDb")<<"Service is unavailable"<<std::endl;
    }
  }
}
