
/*
 *  See header file for a description of this class.
 *
 *  $Date: 2010/06/29 15:51:16 $
 *  $Revision: 1.2 $
 *  \author L. Uplegger F. Yumiceva - Fermilab
 */

#include "Calibration/TkAlCaRecoProducers/interface/AlcaBeamSpotHarvester.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/FileBlock.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "CondFormats/BeamSpotObjects/interface/BeamSpotObjects.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
//#include "CondCore/Utilities/bin/cmscond_export_iov.cpp"
//#include "CondCore/Utilities/interface/Utilities.h"

#include <iostream> 

using namespace edm;
using namespace reco;
//using namespace std;

//--------------------------------------------------------------------------------------------------
AlcaBeamSpotHarvester::AlcaBeamSpotHarvester(const edm::ParameterSet& iConfig) :
  theAlcaBeamSpotManager_(iConfig)
{  
  beamSpotOutputBase_ = iConfig.getParameter<ParameterSet>("AlcaBeamSpotHarvesterParameters").getUntrackedParameter<std::string>("BeamSpotOutputBase");
}

//--------------------------------------------------------------------------------------------------
AlcaBeamSpotHarvester::~AlcaBeamSpotHarvester(){}

//--------------------------------------------------------------------------------------------------
void AlcaBeamSpotHarvester::beginJob() {}

//--------------------------------------------------------------------------------------------------
void AlcaBeamSpotHarvester::endJob() {}  

//--------------------------------------------------------------------------------------------------
void AlcaBeamSpotHarvester::analyze(const edm::Event&, const edm::EventSetup&) {}

//--------------------------------------------------------------------------------------------------
void AlcaBeamSpotHarvester::beginRun(const edm::Run&, const edm::EventSetup&) {}

//--------------------------------------------------------------------------------------------------
void AlcaBeamSpotHarvester::endRun(const edm::Run& iRun, const edm::EventSetup&){
  theAlcaBeamSpotManager_.createWeightedPayloads();
  std::map<edm::LuminosityBlockNumber_t,reco::BeamSpot> beamSpotMap = theAlcaBeamSpotManager_.getPayloads();
  Service<cond::service::PoolDBOutputService> poolDbService;
//  cond::ExportIOVUtilities utilities;
  if(poolDbService.isAvailable() ) {
    for(AlcaBeamSpotManager::bsMap_iterator it=beamSpotMap.begin(); it!=beamSpotMap.end();it++){
      BeamSpotObjects *aBeamSpot = new BeamSpotObjects();
      aBeamSpot->SetType(it->second.type());
      aBeamSpot->SetPosition(it->second.x0(),it->second.y0(),it->second.z0());
      aBeamSpot->SetSigmaZ(it->second.sigmaZ());
      aBeamSpot->Setdxdz(it->second.dxdz());
      aBeamSpot->Setdydz(it->second.dydz());
      aBeamSpot->SetBeamWidthX(it->second.BeamWidthX());
      aBeamSpot->SetBeamWidthY(it->second.BeamWidthY());
      aBeamSpot->SetEmittanceX(it->second.emittanceX());
      aBeamSpot->SetEmittanceY(it->second.emittanceY());
      aBeamSpot->SetBetaStar(it->second.betaStar() );
	
      for (int i=0; i<7; ++i) {
	for (int j=0; j<7; ++j) {
	  aBeamSpot->SetCovariance(i,j,it->second.covariance(i,j));
	}
      }

      cond::Time_t thisIOV;

      // run based      
      if (beamSpotOutputBase_ == "runbased" ) {
	thisIOV = (cond::Time_t) iRun.id().run();
      }
      // lumi based
      else if (beamSpotOutputBase_ == "lumibased" ) {
	edm::LuminosityBlockID lu(iRun.id().run(),it->first);
	thisIOV = (cond::Time_t)(lu.value()); 
      }

      if (poolDbService->isNewTagRequest( "BeamSpotObjectsRcd" ) ) {
          edm::LogInfo("AlcaBeamSpotSpotHarvester")
              << "new tag requested" << std::endl;
          //poolDbService->createNewIOV<BeamSpotObjects>(aBeamSpot, poolDbService->beginOfTime(),poolDbService->endOfTime(),"BeamSpotObjectsRcd");
	  
	  //poolDbService->createNewIOV<BeamSpotObjects>(aBeamSpot, poolDbService->currentTime(), poolDbService->endOfTime(),"BeamSpotObjectsRcd");
	  poolDbService->writeOne<BeamSpotObjects>(aBeamSpot, thisIOV,"BeamSpotObjectsRcd");
      } 
      else {
        edm::LogInfo("AlcaBeamSpotSpotHarvester")
            << "no new tag requested, appending IOV" << std::endl;
        //poolDbService->appendSinceTime<BeamSpotObjects>(aBeamSpot, poolDbService->currentTime(),"BeamSpotObjectsRcd");
	poolDbService->writeOne<BeamSpotObjects>(aBeamSpot, thisIOV,"BeamSpotObjectsRcd");
      }
/*
      int         argc = 15;
      const char* argv[] = {"endRun"
                           ,"-d","sqlite_file:combined.db"
                           ,"-s","sqlite_file:testbs2.db"
                           ,"-l","sqlite_file:log.db"
			   ,"-i","TestLSBasedBS"
			   ,"-t","TestLSBasedBS"
			   ,"-b","1"
			   ,"-e","10"
			   };
      
      edm::LogInfo("AlcaBeamSpotSpotHarvester")
        << "Running utilities!" 
	<< utilities.run(argc,(char**)argv);
      edm::LogInfo("AlcaBeamSpotSpotHarvester")
        << "Run utilities!" 
	<< std::endl;
*/
    }
  }
}

//--------------------------------------------------------------------------------------------------
void AlcaBeamSpotHarvester::beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&) {}

//--------------------------------------------------------------------------------------------------
void AlcaBeamSpotHarvester::endLuminosityBlock(const edm::LuminosityBlock& iLumi, const edm::EventSetup&) {
  theAlcaBeamSpotManager_.readLumi(iLumi);
}


DEFINE_FWK_MODULE(AlcaBeamSpotHarvester);
