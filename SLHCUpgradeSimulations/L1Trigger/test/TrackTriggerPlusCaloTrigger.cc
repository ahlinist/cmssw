// -*- C++ -*-
//
// Package:    TrackTriggerPlusCaloTrigger
// Class:      TrackTriggerPlusCaloTrigger
// 
/**\class TrackTriggerPlusCaloTrigger TrackTriggerPlusCaloTrigger.cc TrackTriggerPlusCaloTrigger/TrackTriggerPlusCaloTrigger/src/TrackTriggerPlusCaloTrigger.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Andrew William Rose
//         Created:  Fri Mar 13 11:06:51 CET 2009
// $Id$
//
//


// system include files 
#include <memory>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometry.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometryRecord.h"

#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h"

//#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
//#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
//#include "DataFormats/L1Trigger/interface/L1JetParticle.h"
//#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"

//#include "Geometry/Records/interface/CaloGeometryRecord.h"
//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "SLHCUpgradeSimulations/L1Trigger/interface/TriggerTowerGeometry.h"

#include <TROOT.h>
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>

//
// class decleration
//
using namespace std;
using namespace edm;
using namespace cmsUpgrades;

class TrackTriggerPlusCaloTrigger : public edm::EDAnalyzer {
public:
  explicit TrackTriggerPlusCaloTrigger(const edm::ParameterSet&);
  ~TrackTriggerPlusCaloTrigger();


private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  ESHandle<StackedTrackerGeometry> StackedTrackerGeomHandle;
  const StackedTrackerGeometry *theStackedTrackers;
  StackedTrackerGeometry::StackContainerIterator StackedTrackerIterator;

  TH1F *egamma_count, *isoegamma_count, *jet_count, *taujet_count, *LocalStub_count, *GlobalStub_count, *TwoPointTracklet_count, *ThreePointTrackletchain_count;

 };



TrackTriggerPlusCaloTrigger::TrackTriggerPlusCaloTrigger(const edm::ParameterSet& iConfig){}
TrackTriggerPlusCaloTrigger::~TrackTriggerPlusCaloTrigger(){}

//
// member functions
//

// ------------ method called to for each event  ------------
void
TrackTriggerPlusCaloTrigger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

//-------------------------------------------------------------------------
/*	edm::Handle<l1extra::L1EmParticleCollection> l1egamma_;
	iEvent.getByLabel("L1ExtraMaker","EGamma",l1egamma_);
	l1extra::L1EmParticleCollection l1egamma = (*l1egamma_);
	egamma_count	    			->	Fill(	l1egamma.size()	);
//-------------------------------------------------------------------------
	edm::Handle<l1extra::L1EmParticleCollection> l1isoegamma_;
	iEvent.getByLabel("L1ExtraMaker","IsoEGamma",l1isoegamma_);
	l1extra::L1EmParticleCollection l1isoegamma = (*l1isoegamma_);
	isoegamma_count					->	Fill(	l1isoegamma.size()	);
//-------------------------------------------------------------------------
	edm::Handle<l1extra::L1JetParticleCollection> l1tau_;
	iEvent.getByLabel("L1ExtraMaker","Taus",l1tau_);
	l1extra::L1JetParticleCollection l1tau = (*l1tau_);
	jet_count		       			->	Fill(	l1tau.size()	);
//-------------------------------------------------------------------------
	edm::Handle<l1extra::L1JetParticleCollection> l1jets_;
	iEvent.getByLabel("L1ExtraMaker","Jets",l1jets_);
	l1extra::L1JetParticleCollection l1jets = (*l1jets_);
	taujet_count					->	Fill(	l1jets.size()	);*/
//-------------------------------------------------------------------------
	edm::Handle< cmsUpgrades::LocalStub_PixelDigi_Collection > DigiLocalStubHandle;
	iEvent.getByLabel( "LocalStubsFromPixelDigis" , DigiLocalStubHandle);
	cmsUpgrades::LocalStub_PixelDigi_Collection DigiLocalStubs = (*DigiLocalStubHandle);
	LocalStub_count					->	Fill(	DigiLocalStubs.size()	);
	for(unsigned int i = 0; i != DigiLocalStubs.size() ; ++i )
		cout	<< "Local Stub in DetId: "
				<< DigiLocalStubs[i].Id().rawId() << endl;
//-------------------------------------------------------------------------
	edm::Handle< cmsUpgrades::GlobalStub_PixelDigi_Collection > DigiGlobalStubHandle;
	iEvent.getByLabel( "GlobalStubsFromPixelDigis" , DigiGlobalStubHandle);
	cmsUpgrades::GlobalStub_PixelDigi_Collection DigiGlobalStubs = (*DigiGlobalStubHandle);
	GlobalStub_count				->	Fill(	DigiGlobalStubs.size()	);
	for(unsigned int i = 0; i != DigiGlobalStubs.size() ; ++i )
		cout	<< "Global Stub at position: "
				<< DigiGlobalStubs[i].position() << endl;
//-------------------------------------------------------------------------
	edm::Handle< cmsUpgrades::Tracklet_PixelDigi_Collection > TwoPointTrackletHandle;
	iEvent.getByLabel( "TrackletsFromPixelDigis" , "ShortTracklets" , TwoPointTrackletHandle);
	cmsUpgrades::Tracklet_PixelDigi_Collection TwoPointTracklets = (*TwoPointTrackletHandle);
	TwoPointTracklet_count			->	Fill(	TwoPointTracklets.size()	);
	for(unsigned int i = 0; i != TwoPointTracklets.size() ; ++i )
		cout	<< "Two Point Tracklets with stubs at: "
				<< TwoPointTracklets[i].stub(0)->position() << " , "
				<< TwoPointTracklets[i].stub(1)->position() << endl;
//-------------------------------------------------------------------------
	edm::Handle< cmsUpgrades::Tracklet_PixelDigi_Collection > ThreePointTrackletHandle;
	iEvent.getByLabel( "TrackletChainsFromPixelDigis" , "TrackletChains" , ThreePointTrackletHandle);
	cmsUpgrades::Tracklet_PixelDigi_Collection ThreePointTracklets = (*ThreePointTrackletHandle);
	ThreePointTrackletchain_count	->	Fill(	ThreePointTracklets.size()	);
	for(unsigned int i = 0; i != ThreePointTracklets.size() ; ++i )
		cout	<< "Three Point Tracklets with stubs at: "
				<< ThreePointTracklets[i].stub(0)->position() << " , "
				<< ThreePointTracklets[i].stub(1)->position() << " , "
				<< ThreePointTracklets[i].stub(2)->position() << endl;
//-------------------------------------------------------------------------

}


// ------------ method called once each job just before starting event loop  ------------
void 
TrackTriggerPlusCaloTrigger::beginJob(const edm::EventSetup& iSetup)
{
 
  iSetup.get<StackedTrackerGeometryRecord>().get(StackedTrackerGeomHandle);
  theStackedTrackers = StackedTrackerGeomHandle.product();
    
  edm::Service<TFileService> fs;
  
  
  egamma_count	    				= fs->make<TH1F>( "egamma_count"			, "egamma count"					, 10 , 0 , 10 );
  isoegamma_count					= fs->make<TH1F>( "isoegamma_count"			, "isoegamma count"					, 10 , 0 , 10 );
  jet_count		       				= fs->make<TH1F>( "jet_count"				, "jet count"						, 10 , 0 , 10 );
  taujet_count						= fs->make<TH1F>( "taujet_count"			, "taujet count"					, 10 , 0 , 10 );

  LocalStub_count					= fs->make<TH1F>( "localstub_count"			, "localstub count"					, 100 , 0 , 100 );
  GlobalStub_count					= fs->make<TH1F>( "globalstub_count"		, "globalstub count"				, 100 , 0 , 100 );
  TwoPointTracklet_count			= fs->make<TH1F>( "tracklet_count"			, "two point tracklet count"		, 100 , 0 , 100 );
  ThreePointTrackletchain_count		= fs->make<TH1F>( "trackletchain_count"		, "three point tracklet count"		, 100 , 0 , 100 );
     
}




// ------------ method called once each job just after ending the event loop  ------------
void 
TrackTriggerPlusCaloTrigger::endJob() {
  

}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackTriggerPlusCaloTrigger);
