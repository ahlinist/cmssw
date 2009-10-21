#include <iostream>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <DataFormats/EcalDetId/interface/ESDetId.h>

#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"
#include "CondFormats/RunInfo/interface/RunSummary.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"

#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "Geometry/EcalMapping/interface/ESElectronicsMapper.h"

#include "DQM/EcalPreshowerMonitorModule/interface/ESDaqInfoTask.h"

using namespace cms;
using namespace edm;
using namespace std;

ESDaqInfoTask::ESDaqInfoTask(const ParameterSet& ps) {

   dqmStore_ = Service<DQMStore>().operator->();

   prefixME_ = ps.getUntrackedParameter<string>("prefixME", "");

   enableCleanup_ = ps.getUntrackedParameter<bool>("enableCleanup", false);

   mergeRuns_ = ps.getUntrackedParameter<bool>("mergeRuns", false);

   ESFedRangeMin_ = ps.getUntrackedParameter<int>("ESFedRangeMin");
   ESFedRangeMax_ = ps.getUntrackedParameter<int>("ESFedRangeMax");

   meESDaqFraction_ = 0;
   meESDaqActiveMap_ = 0;
   meESDaqError_ = 0;

   for (int i = 0; i < 56; i++) {
      meESDaqActive_[i] = 0;
   }

   if (ps.exists("esMapping")){
      edm::ParameterSet esMap=ps.getParameter<edm::ParameterSet>("esMapping");
      es_mapping_ = new ESElectronicsMapper(esMap);
   }else{
      edm::LogError("ESDaqInfoTask")<<"preshower mapping pointer not initialized. Temporary.";
      es_mapping_=0;
   }



}

ESDaqInfoTask::~ESDaqInfoTask() {
   delete es_mapping_;
}


void ESDaqInfoTask::beginJob(const EventSetup& c){

   char histo[200];

   if ( dqmStore_ ) {

      dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo");

      sprintf(histo, "DAQSummary");
      meESDaqFraction_ = dqmStore_->bookFloat(histo);
      meESDaqFraction_->Fill(0.0);

      sprintf(histo, "DAQSummaryMap");
      meESDaqActiveMap_ = dqmStore_->book2D(histo,histo, 80, 0.5, 80.5, 80, 0.5, 80.5);
      meESDaqActiveMap_->setAxisTitle("Si X", 1);
      meESDaqActiveMap_->setAxisTitle("Si Y", 2);

      dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo/DAQContents");

      for (int i = 0; i < 56; i++) {
	 sprintf(histo, "EcalPreshower_%d", ESFedRangeMin_+i);
	 meESDaqActive_[i] = dqmStore_->bookFloat(histo);
	 meESDaqActive_[i]->Fill(0.0);

	 ESOnFed_[i] = false;
	 for ( int x = 0; x < 80; x++ ) {
	    for ( int y = 0; y < 80; y++ ) {
	       int iz = (x<40)?  1:2;
	       int ip = (y>=40)? 1:2;
	       int ix = (x<40)? x:x-40;
	       int iy = (y<40)?  y:y-40;
	       int ifed = (*es_mapping_).getFED( iz, ip, ix, iy);
	       if(ifed == ESFedRangeMin_+i){
		  ESOnFed_[i] = true;
		  break;
	       }
	    }
	    if(ESOnFed_[i] == true) break;
	 }
      }

      dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo");
      sprintf(histo, "DAQError");
      meESDaqError_ = dqmStore_->book1D(histo, histo, 56, ESFedRangeMin_-0.5, ESFedRangeMax_+0.5);
      meESDaqError_->setAxisTitle("FedID", 1);

   }

}

void ESDaqInfoTask::endJob(void) {

   if ( enableCleanup_ ) this->cleanup();

}

void ESDaqInfoTask::beginLuminosityBlock(const edm::LuminosityBlock& lumiBlock, const  edm::EventSetup& iSetup){

   this->reset();

   for ( int x = 0; x < 80; x++ ) {
      for ( int y = 0; y < 80; y++ ) {
	 int iz = (x<40)?  1:2;
	 int ip = (y>=40)? 1:2;
	 int ix = (x<40)? x:x-40;
	 int iy = (y<40)?  y:y-40;
	 int ifed = (*es_mapping_).getFED( iz, ip, ix, iy);
	 if( ifed > 0 ) meESDaqActiveMap_->setBinContent( x+1, y+1, 0.0 );
	 else meESDaqActiveMap_->setBinContent( x+1, y+1, -1.0 );
      }
   }

   for (int i = 0; i < 56; i++) {
      if ( meESDaqError_ ) meESDaqError_->setBinContent(i, 0.0);
   }

   edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("RunInfoRcd"));

   if( iSetup.find( recordKey ) ) {

      edm::ESHandle<RunInfo> sumFED;
      iSetup.get<RunInfoRcd>().get(sumFED);    

      std::vector<int> FedsInIds= sumFED->m_fed_in;   

      float ESFedCount = 0.;

      for( unsigned int fedItr=0; fedItr<FedsInIds.size(); ++fedItr ) {

	 int fedID=FedsInIds[fedItr];

	 if ( fedID >= ESFedRangeMin_ && fedID <= ESFedRangeMax_ ) {

	    if( ESOnFed_[fedID - ESFedRangeMin_] ) ESFedCount++;

	    if ( meESDaqActive_[fedID-ESFedRangeMin_] ) meESDaqActive_[fedID-ESFedRangeMin_]->Fill(1.0);

	    if( meESDaqActiveMap_ ) {

	       for( int x = 0; x < 80; x++ ) {
		  for( int y = 0; y < 80; y++ ) {
		     int iz = (x<40)?  1:2;
		     int ip = (y>=40)? 1:2;
		     int ix = (x<40) ?  x:x-40;
		     int iy = (x<40) ?  y:y-40;
		     int ifed = es_mapping_->getFED(iz, ip, ix, iy);
		     if( fedID==ifed ) meESDaqActiveMap_->setBinContent( x+1, y+1, 1.0 );
		  }
	       }

	    }

	    if( meESDaqFraction_ ) meESDaqFraction_->Fill( ESFedCount/40. );

	    if( meESDaqError_ ){
	       for( int i = 0; i < 56; i++){
		  if( ESOnFed_[fedID-ESFedRangeMin_] ) meESDaqError_->setBinContent(i+1, 1.0);
		  else meESDaqError_->setBinContent(i+1, 2.0);
	       }
	    }

	 }

      }

 } else {

      LogWarning("ESDaqInfoTask") << "Cannot find any RunInfoRcd" << endl;

   }

}

void ESDaqInfoTask::endLuminosityBlock(const edm::LuminosityBlock&  lumiBlock, const  edm::EventSetup& iSetup) {

}

void ESDaqInfoTask::reset(void) {

   if ( meESDaqFraction_ ) meESDaqFraction_->Reset();

   for (int i = 0; i < 56; i++) {
      if ( meESDaqActive_[i] ) meESDaqActive_[i]->Reset();
   }

   if ( meESDaqActiveMap_ ) meESDaqActiveMap_->Reset();

   if ( meESDaqError_ ) meESDaqError_->Reset();

}


void ESDaqInfoTask::cleanup(void){

   if ( dqmStore_ ) {

      dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo");

      if ( meESDaqFraction_ ) dqmStore_->removeElement( meESDaqFraction_->getName() );

      if ( meESDaqActiveMap_ ) dqmStore_->removeElement( meESDaqActiveMap_->getName() );

      if ( meESDaqError_ ) dqmStore_->removeElement( meESDaqError_->getName() );

      dqmStore_->setCurrentFolder(prefixME_ + "/EventInfo/DAQContents");

      for (int i = 0; i < 56; i++) {
	 if ( meESDaqActive_[i] ) dqmStore_->removeElement( meESDaqActive_[i]->getName() );
      }

   }

}

void ESDaqInfoTask::analyze(const Event& e, const EventSetup& c){ 

}

DEFINE_FWK_MODULE(ESDaqInfoTask);
