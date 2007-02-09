/* This is HiggsTo4LeptonsProducer
 *
 * author: Dominique Fortin - UC Riverside
 */

#include <AnalysisAlgos/HiggsTo4Leptons/src/HiggsTo4LeptonsProducer.h>
#include <AnalysisAlgos/HiggsTo4Leptons/src/HiggsTo4LeptonsMCBuilder.h>
#include <AnalysisDataFormats/HiggsTo4Leptons/interface/HiggsTo4LeptonsMCObjects.h>
#include <AnalysisDataFormats/HiggsTo4Leptons/interface/HiggsTo4LeptonsMCObjectsCollection.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <SimDataFormats/HepMCProduct/interface/HepMCProduct.h>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"

#include <iostream>
#include <memory>

/*
 * Constructor
 */
HiggsTo4LeptonsProducer::HiggsTo4LeptonsProducer( const edm::ParameterSet& ps ) : iev( 0 ) {

  isData       = ps.getUntrackedParameter<bool>("RunningOnData");
  isVtxSmeared = ps.getUntrackedParameter<bool>("isVtxSmeared");
  debug        = ps.getUntrackedParameter<bool>("Debug");

  mcBuilder_ = new HiggsTo4LeptonsMCBuilder( ps );
  
  produces<HiggsTo4LeptonsMCObjectsCollection>();
}


/*
 * Destructor
 */
HiggsTo4LeptonsProducer::~HiggsTo4LeptonsProducer() {

  std::cout << "deleting builder_ after processing " << iev << " events" << std::endl;

  delete mcBuilder_;
}


/*
 * Producer Main
 */
void HiggsTo4LeptonsProducer::produce( edm::Event& e, const edm::EventSetup& setup ) {

  ievt++;

  // If running on MC access generated particles
  edm::Handle<edm::HepMCProduct> MCEvtHandler;

  if (isVtxSmeared) {
    e.getByLabel("VtxSmeared", MCEvtHandler);
  } else { 
    e.getByLabel("source", MCEvtHandler);
  }
  const HepMC::GenEvent* evt = MCEvtHandler->GetEvent() ; 

  /*  // Get reconstructed objects

  // get calo towers collection
  Handle<CaloTowerCollection> calotowers; 
  iEvent.getByType(calotowers);
  
  // get calo jet collection
  Handle<CaloJetCollection> jets;
  iEvent.getByLabel(m_jetsSrc, jets);
  
  // get gen jet collection
  Handle<GenJetCollection> jetsgen;
  iEvent.getByLabel(m_jetsgenSrc, jetsgen);

  // get track collection
  Handle<TrackCollection> tracks;
  iEvent.getByLabel(m_tracksSrc, tracks);

  // Taus
  Handle<IsolatedTauTagInfoCollection> tauTagInfoHandle;
  iEvent.getByLabel(m_tautaginfo, tauTagInfoHandle);
  const IsolatedTauTagInfoCollection & tauTagInfo = *(tauTagInfoHandle.product());

  // Island basic cluster collection
  Handle<BasicClusterCollection> BarrelBasicClusters;
  Handle<BasicClusterCollection> EndcapBasicClusters;
  iEvent.getByLabel(m_ecalBClSrc, "islandBarrelBasicClusters", BarrelBasicClusters);
  iEvent.getByLabel(m_ecalBClSrc, "islandEndcapBasicClusters", EndcapBasicClusters);

  // Electrons
  Handle<ElectronCollection> electrons;
  iEvent.getByLabel(m_electronsSrc, electrons);

  // Muons
  Handle<MuonCollection> muons;
  iEvent.getByLabel(m_muonsSrc, muons);
  */


  // Create empty collection for generated MC objects
  std::auto_ptr<HiggsTo4LeptonsMCObjectsCollection> mc( new HiggsTo4LeptonsMCObjectsCollection );

  
  // Fill collections:

  // 1) MC truth
  mcBuilder_->build(evt, *mc);


  // 2) Reconstructed objects



  // Put collections in event
  e.put( mc );

}

//define this as a plug-in
DEFINE_FWK_MODULE(HiggsTo4LeptonsProducer);
