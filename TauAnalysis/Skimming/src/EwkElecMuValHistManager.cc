#include "TauAnalysis/Skimming/interface/EwkElecMuValHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "TauAnalysis/Skimming/interface/ewkTauValAuxFunctions.h"

#include "TMath.h"

#include <iostream>
#include <iomanip>

EwkElecMuValHistManager::EwkElecMuValHistManager(const edm::ParameterSet& cfg) 
  : EwkValHistManagerBase(cfg),
    numWarningsTriggerResults_(0),
    numWarningsHLTpath_(0),
    numWarningsVertex_(0),
    numWarningsBeamSpot_(0),
    numWarningsElectron_(0),
    numWarningsMuon_(0),
    numWarningsCaloMEt_(0),
    numWarningsPFMEt_(0)
{
  triggerResultsSource_ = cfg.getParameter<edm::InputTag>("triggerResultsSource");
  vertexSource_ = cfg.getParameter<edm::InputTag>("vertexSource");
  beamSpotSource_ = cfg.getParameter<edm::InputTag>("beamSpotSource");
  electronSource_ = cfg.getParameter<edm::InputTag>("electronSource");
  muonSource_ = cfg.getParameter<edm::InputTag>("muonSource");
  caloMEtSource_ = cfg.getParameter<edm::InputTag>("caloMEtSource");
  pfMEtSource_ = cfg.getParameter<edm::InputTag>("pfMEtSource");

  hltPaths_ = cfg.getParameter<vstring>("hltPaths");

  electronEtaCut_ = cfg.getParameter<double>("electronEtaCut");
  electronPtCut_ = cfg.getParameter<double>("electronPtCut");
  electronTrackIsoCut_ = cfg.getParameter<double>("electronTrackIsoCut");
  electronEcalIsoCut_ = cfg.getParameter<double>("electronEcalIsoCut");
  std::string electronIsoMode_string = cfg.getParameter<std::string>("electronIsoMode");
  electronIsoMode_ = getIsoMode(electronIsoMode_string, cfgError_);

  muonEtaCut_ = cfg.getParameter<double>("muonEtaCut");
  muonPtCut_ = cfg.getParameter<double>("muonPtCut");
  muonTrackIsoCut_ = cfg.getParameter<double>("muonTrackIsoCut");
  muonEcalIsoCut_ = cfg.getParameter<double>("muonEcalIsoCut");
  std::string muonIsoMode_string = cfg.getParameter<std::string>("muonIsoMode");
  muonIsoMode_ = getIsoMode(muonIsoMode_string, cfgError_);

  visMassCut_ = cfg.getParameter<double>("visMassCut");
}

void EwkElecMuValHistManager::bookHistograms()
{
  dqmStore_->setCurrentFolder(dqmDirectory_);

  hNumIdElectrons_ = dqmStore_->book1D("NumIdElectronsMuons" , "Num. id. Muons", 5, -0.5, 4.5);
  hElectronPt_ = dqmStore_->book1D("ElectronPt" , "P_{T}^{e}", 20, 0., 100.);
  hElectronEta_ = dqmStore_->book1D("ElectronEta" , "#eta_{e}", 20, -4.0, +4.0);
  hElectronPhi_ = dqmStore_->book1D("ElectronPhi" , "#phi_{e}", 20, -TMath::Pi(), +TMath::Pi());
  hElectronTrackIsoPt_ = dqmStore_->book1D("ElectronTrackIsoPt" , "Electron Track Iso.", 20, -0.01, 10.);
  hElectronEcalIsoPt_ = dqmStore_->book1D("ElectronEcalIsoPt" , "Electron Ecal Iso.", 20, -0.01, 10.);

  hNumGlobalMuons_ = dqmStore_->book1D("NumGlobalMuons" , "Num. global Muons", 5, -0.5, 4.5);
  hMuonPt_ = dqmStore_->book1D("MuonPt" , "P_{T}^{#mu}", 20, 0., 100.);
  hMuonEta_ = dqmStore_->book1D("MuonEta" , "#eta_{#mu}", 20, -4.0, +4.0);
  hMuonPhi_ = dqmStore_->book1D("MuonPhi" , "#phi_{#mu}", 20, -TMath::Pi(), +TMath::Pi());
  hMuonTrackIsoPt_ = dqmStore_->book1D("MuonTrackIsoPt" , "Muon Track Iso.", 20, -0.01, 10.);
  hMuonEcalIsoPt_ = dqmStore_->book1D("MuonEcalIsoPt" , "Muon Ecal Iso.", 20, -0.01, 10.);
  
  hVisMass_ = dqmStore_->book1D("VisMass", "e + #mu visible Mass", 20, 20., 120.);
  hMtElecCaloMEt_ = dqmStore_->book1D("MtElecCaloMEt", "e + E_{T}^{miss} (Calo) transverse Mass", 20, 20., 120.);
  hMtElecPFMEt_ = dqmStore_->book1D("MtElecPFMEt", "e + E_{T}^{miss} (PF) transverse Mass", 20, 20., 120.);
  hMtMuCaloMEt_ = dqmStore_->book1D("MtMuCaloMEt", "#mu + E_{T}^{miss} (Calo) transverse Mass", 20, 20., 120.);
  hMtMuPFMEt_ = dqmStore_->book1D("MtMuPFMEt", "#mu + E_{T}^{miss} (PF) transverse Mass", 20, 20., 120.);
  hPzetaCaloMEt_ = dqmStore_->book1D("PzetaCaloMEt", "P_{#zeta} - 1.5*P_{#zeta}^{vis} (Calo)", 20, -40., 40.);
  hPzetaPFMEt_ = dqmStore_->book1D("PzetaPFMEt", "P_{#zeta} - 1.5*P_{#zeta}^{vis} (PF)", 20, -40., 40.);
  hElecMuAcoplanarity_ = dqmStore_->book1D("ElecMuAcoplanarity", "#Delta #phi_{e #mu}", 20, -TMath::Pi(), +TMath::Pi());
  hElecMuCharge_ = dqmStore_->book1D("ElecMuCharge" , "Q_{e + #mu}", 11, -5.5, +5.5);

  hVertexChi2_ = dqmStore_->book1D("VertexChi2", "Event Vertex #chi^{2} / n.d.o.f.", 20, 0., 2.0);
  hVertexZ_ = dqmStore_->book1D("VertexZ", "Event Vertex z-Position", 20, -25., +25.);
  hVertexD0_ = dqmStore_->book1D("VertexD0", "Event Vertex d_{0}", 20, -0.0001, 0.05);

  hCaloMEtPt_ = dqmStore_->book1D("CaloMEtPt", "E_{T}^{miss} (Calo)", 20, 0., 100.);
  hCaloMEtPhi_ = dqmStore_->book1D("CaloMEtPhi", "#phi^{miss} (Calo)", 20, -TMath::Pi(), +TMath::Pi());

  hPFMEtPt_ = dqmStore_->book1D("PFMEtPt", "E_{T}^{miss} (PF)", 20, 0., 100.);
  hPFMEtPhi_ = dqmStore_->book1D("PFMEtPhi", "#phi^{miss} (PF)", 20, -TMath::Pi(), +TMath::Pi());

  hCutFlowSummary_ = dqmStore_->book1D("CutFlowSummary", "Cut-flow Summary", 10, 0.5, 10.5);
  hCutFlowSummary_->setBinLabel(kPassedPreselection, "Preselection");
  hCutFlowSummary_->setBinLabel(kPassedTrigger, "HLT");
  hCutFlowSummary_->setBinLabel(kPassedMuonId, "#mu ID");
  hCutFlowSummary_->setBinLabel(kPassedMuonTrackIso, "#mu Trk Iso.");
  hCutFlowSummary_->setBinLabel(kPassedMuonEcalIso, "#mu Ecal Iso.");
  hCutFlowSummary_->setBinLabel(kPassedElectronId, "e ID");
  hCutFlowSummary_->setBinLabel(kPassedElectronTrackIso, "e Trk Iso.");
  hCutFlowSummary_->setBinLabel(kPassedElectronEcalIso, "e Ecal Iso.");
}

void EwkElecMuValHistManager::fillHistograms(const edm::Event& evt, const edm::EventSetup& es)
{
  if ( cfgError_ ) return;

  //-----------------------------------------------------------------------------
  // access event-level information
  //-----------------------------------------------------------------------------

  bool readError = false;

//--- get decision of high-level trigger for the event
  edm::Handle<edm::TriggerResults> hltDecision;
  readEventData(evt, triggerResultsSource_, hltDecision, numWarningsTriggerResults_, maxNumWarnings_, 
		readError, "Failed to access Trigger results");
  if ( readError ) return;
  
  edm::TriggerNames triggerNames;
  triggerNames.init(*hltDecision);
   
  bool isTriggered = false;
  for ( vstring::const_iterator hltPath = hltPaths_.begin();
	hltPath != hltPaths_.end(); ++hltPath ) {
    unsigned int index = triggerNames.triggerIndex(*hltPath);
    if ( index < triggerNames.size() ) {
      if ( hltDecision->accept(index) ) isTriggered = true;
    } else {
      if ( numWarningsHLTpath_ < maxNumWarnings_ || maxNumWarnings_ == -1 ) 
	edm::LogWarning ("EwkElecMuValHistManager") << " Undefined HLT path = " << (*hltPath) << " !!";
      ++numWarningsHLTpath_;
      continue;
    }
  }
  
//--- get reconstructed primary event vertex of the event
//   (take as "the" primary event vertex the first entry in the collection
//    of vertex objects, corresponding to the vertex associated to the highest Pt sum of tracks)
  edm::Handle<reco::VertexCollection> vertexCollection;
  readEventData(evt, vertexSource_, vertexCollection, numWarningsVertex_, maxNumWarnings_,
		readError, "Failed to access Vertex collection");
  if ( readError ) return;

  const reco::Vertex* theEventVertex = ( vertexCollection->size() > 0 ) ? &(vertexCollection->at(0)) : 0;

//--- get beam-spot (expected vertex position) for the event
  edm::Handle<reco::BeamSpot> beamSpot;
  readEventData(evt, beamSpotSource_, beamSpot, numWarningsBeamSpot_, maxNumWarnings_,
		readError, "Failed to access Beam-spot");
  if ( readError ) return;
  
//--- get collections of reconstructed electrons from the event
  edm::Handle<reco::GsfElectronCollection> electrons;
  readEventData(evt, electronSource_, electrons, numWarningsElectron_, maxNumWarnings_,
		readError, "Failed to access Electron collection");
  if ( readError ) return;

  const reco::GsfElectron* theElectron = getTheElectron(*electrons, electronEtaCut_, electronPtCut_);

  double theElectronTrackIsoPt = 1.e+3;
  double theElectronEcalIsoPt = 1.e+3;
  if ( theElectron ) {
    theElectronTrackIsoPt = theElectron->dr03TkSumPt();
    theElectronEcalIsoPt = theElectron->dr03EcalRecHitSumEt();

    if ( electronIsoMode_ == kRelativeIso && theElectron->pt() > 0. ) {
      theElectronTrackIsoPt /= theElectron->pt();
      theElectronEcalIsoPt /= theElectron->pt();
    }
  }

//--- get collections of reconstructed muons from the event
  edm::Handle<reco::MuonCollection> muons;
  readEventData(evt, muonSource_, muons, numWarningsMuon_, maxNumWarnings_,
		readError, "Failed to access Muon collection");
  if ( readError ) return;

  const reco::Muon* theMuon = getTheMuon(*muons, muonEtaCut_, muonPtCut_);

  double theMuonTrackIsoPt = 1.e+3;
  double theMuonEcalIsoPt = 1.e+3;
  if ( theMuon ) {
    theMuonTrackIsoPt = theMuon->isolationR05().sumPt;
    theMuonEcalIsoPt = theMuon->isolationR05().emEt;

    if ( muonIsoMode_ == kRelativeIso && theMuon->pt() > 0. ) {
      theMuonTrackIsoPt /= theMuon->pt();
      theMuonEcalIsoPt /= theMuon->pt();
    }
  }

//--- get missing transverse momentum
//    measured by calorimeters/reconstructed by particle-flow algorithm
  edm::Handle<reco::CaloMETCollection> caloMEtCollection;
  readEventData(evt, caloMEtSource_, caloMEtCollection, numWarningsCaloMEt_, maxNumWarnings_,
		readError, "Failed to access calo. MET collection");
  if ( readError ) return;

  const reco::CaloMET& caloMEt = caloMEtCollection->at(0);
  
  edm::Handle<reco::PFMETCollection> pfMEtCollection;
  readEventData(evt, pfMEtSource_, pfMEtCollection, numWarningsPFMEt_, maxNumWarnings_,
		readError, "Failed to access pf. MET collection");
  if ( readError ) return;

  const reco::PFMET& pfMEt = pfMEtCollection->at(0);

  if ( !(theElectron && theMuon) ) return;

  //-----------------------------------------------------------------------------
  // compute EWK tau analysis specific quantities
  //-----------------------------------------------------------------------------

  double dPhiElecMu = calcDeltaPhi(theElectron->phi(), theMuon->phi());

  double mElecMu = (theElectron->p4() + theMuon->p4()).M();

  double mtElecCaloMEt = calcMt(theElectron->px(), theElectron->px(), caloMEt.px(), caloMEt.py());
  double mtElecPFMEt = calcMt(theElectron->px(), theElectron->px(), pfMEt.px(), pfMEt.py());
  double mtMuCaloMEt = calcMt(theMuon->px(), theMuon->px(), caloMEt.px(), caloMEt.py());
  double mtMuPFMEt = calcMt(theMuon->px(), theMuon->px(), pfMEt.px(), pfMEt.py());

  double pZetaCaloMEt = calcPzeta(theElectron->p4(), theMuon->p4(), caloMEt.px(), caloMEt.py());
  double pZetaPFMEt = calcPzeta(theElectron->p4(), theMuon->p4(), pfMEt.px(), pfMEt.py());

  //-----------------------------------------------------------------------------
  // apply selection criteria; fill histograms
  //-----------------------------------------------------------------------------

//--- fill electron multiplicity histogram
  unsigned numIdElectrons = 0;
  for ( reco::GsfElectronCollection::const_iterator electron = electrons->begin();
	electron != electrons->end(); ++electron ) {
    if ( passesElectronId(*electron) ) {
      ++numIdElectrons;
    }
  }

  hNumIdElectrons_->Fill(numIdElectrons);

//--- fill muon multiplicity histogram
  unsigned numGlobalMuons = 0;
  for ( reco::MuonCollection::const_iterator muon = muons->begin();
	muon != muons->end(); ++muon ) {
    if ( muon->isGlobalMuon() ) {
      ++numGlobalMuons;
    }
  }

  hNumGlobalMuons_->Fill(numGlobalMuons);

  ++numEventsAnalyzed_;

  bool isSelected = false;
  int cutFlowStatus = -1;

  if ( mElecMu > visMassCut_ ) {
    cutFlowStatus = kPassedPreselection;
  }
  if ( cutFlowStatus == kPassedPreselection && (isTriggered || hltPaths_.size() == 0) ) {
    cutFlowStatus = kPassedTrigger;
  }
  if ( cutFlowStatus == kPassedTrigger && theMuon->isGlobalMuon() ) {
    cutFlowStatus = kPassedMuonId;
    hMuonTrackIsoPt_->Fill(theMuonTrackIsoPt);
  }
  if ( cutFlowStatus == kPassedMuonId && theMuonTrackIsoPt < muonTrackIsoCut_ ) {
    cutFlowStatus = kPassedMuonTrackIso;
    hMuonEcalIsoPt_->Fill(theMuonEcalIsoPt);
  }
  if ( cutFlowStatus == kPassedMuonTrackIso && theMuonEcalIsoPt < muonEcalIsoCut_ ) {
    cutFlowStatus = kPassedMuonEcalIso;
  }
  if ( cutFlowStatus == kPassedMuonEcalIso && passesElectronId(*theElectron) ) {
    cutFlowStatus = kPassedElectronId;
    hElectronTrackIsoPt_->Fill(theElectronTrackIsoPt);
  }
  if ( cutFlowStatus == kPassedElectronId && theElectronTrackIsoPt < electronTrackIsoCut_ ) {
    cutFlowStatus = kPassedElectronTrackIso;
    hElectronEcalIsoPt_->Fill(theElectronEcalIsoPt);
  }
  if ( cutFlowStatus == kPassedElectronTrackIso && theElectronEcalIsoPt < electronEcalIsoCut_ ) {
    cutFlowStatus = kPassedElectronEcalIso;
  }

  for ( int iCut = 1; iCut <= cutFlowStatus; ++iCut ) {
    hCutFlowSummary_->Fill(iCut);
  }

  if ( isSelected ) {
    hElectronPt_->Fill(theElectron->pt());
    hElectronEta_->Fill(theElectron->eta());
    hElectronPhi_->Fill(theElectron->phi());

    hMuonPt_->Fill(theMuon->pt());
    hMuonEta_->Fill(theMuon->eta());
    hMuonPhi_->Fill(theMuon->phi());

    hVisMass_->Fill(mElecMu);
    hMtElecCaloMEt_->Fill(mtElecCaloMEt);
    hMtElecPFMEt_->Fill(mtElecPFMEt);
    hMtMuCaloMEt_->Fill(mtMuCaloMEt);
    hMtMuPFMEt_->Fill(mtMuPFMEt);
    hPzetaCaloMEt_->Fill(pZetaCaloMEt);
    hPzetaPFMEt_->Fill(pZetaPFMEt);
    hElecMuAcoplanarity_->Fill(dPhiElecMu);
    hElecMuCharge_->Fill(theElectron->charge() + theMuon->charge());

    if ( theEventVertex ) {
      hVertexChi2_->Fill(theEventVertex->normalizedChi2());
      hVertexZ_->Fill(theEventVertex->z());
      hVertexD0_->Fill(getVertexD0(*theEventVertex, *beamSpot));
    }

    hCaloMEtPt_->Fill(caloMEt.pt());
    hCaloMEtPhi_->Fill(caloMEt.phi());

    hPFMEtPt_->Fill(pfMEt.pt());
    hPFMEtPhi_->Fill(pfMEt.phi());
  }

  if ( isSelected ) ++numEventsSelected_;
}

void EwkElecMuValHistManager::finalizeHistograms()
{
  printFilterStatistics();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(EwkValHistManagerPluginFactory, EwkElecMuValHistManager, "EwkElecMuValHistManager");





