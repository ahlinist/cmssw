#include "TauAnalysis/Skimming/interface/EwkElecTauValHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
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

EwkElecTauValHistManager::EwkElecTauValHistManager(const edm::ParameterSet& cfg, DQMStore* dqmStore) 
  : dqmStore_(dqmStore),
    dqmDirectory_(cfg.getParameter<std::string>("dqmDirectory")),
    numEventsAnalyzed_(0),
    numEventsSelected_(0),
    cfgError_(0),
    numWarningsTriggerResults_(0),
    numWarningsHLTpath_(0),
    numWarningsVertex_(0),
    numWarningsBeamSpot_(0),
    numWarningsElectron_(0),
    numWarningsTauJet_(0),
    numWarningsTauDiscrByLeadTrackFinding_(0),
    numWarningsTauDiscrByLeadTrackPtCut_(0),
    numWarningsTauDiscrByTrackIso_(0),
    numWarningsTauDiscrByEcalIso_(0),
    numWarningsTauDiscrAgainstElectrons_(0),
    numWarningsTauDiscrAgainstMuons_(0),
    numWarningsCaloMEt_(0),
    numWarningsPFMEt_(0)
{
  triggerResultsSource_ = cfg.getParameter<edm::InputTag>("triggerResultsSource");
  vertexSource_ = cfg.getParameter<edm::InputTag>("vertexSource");
  beamSpotSource_ = cfg.getParameter<edm::InputTag>("beamSpotSource");
  electronSource_ = cfg.getParameter<edm::InputTag>("electronSource");
  tauJetSource_ = cfg.getParameter<edm::InputTag>("tauJetSource");
  caloMEtSource_ = cfg.getParameter<edm::InputTag>("caloMEtSource");
  pfMEtSource_ = cfg.getParameter<edm::InputTag>("pfMEtSource");

  tauDiscrByLeadTrackFinding_ = cfg.getParameter<edm::InputTag>("tauDiscrByLeadTrackFinding");
  tauDiscrByLeadTrackPtCut_ = cfg.getParameter<edm::InputTag>("tauDiscrByLeadTrackPtCut");
  tauDiscrByTrackIso_ = cfg.getParameter<edm::InputTag>("tauDiscrByTrackIso");
  tauDiscrByEcalIso_ = cfg.getParameter<edm::InputTag>("tauDiscrByEcalIso");
  tauDiscrAgainstElectrons_ = cfg.getParameter<edm::InputTag>("tauDiscrAgainstElectrons");
  tauDiscrAgainstMuons_ = cfg.getParameter<edm::InputTag>("tauDiscrAgainstMuons");

  hltPaths_ = cfg.getParameter<vstring>("hltPaths");

  electronEtaCut_ = cfg.getParameter<double>("electronEtaCut");
  electronPtCut_ = cfg.getParameter<double>("electronPtCut");
  electronTrackIsoCut_ = cfg.getParameter<double>("electronTrackIsoCut");
  electronEcalIsoCut_ = cfg.getParameter<double>("electronEcalIsoCut");
  std::string electronIsoMode_string = cfg.getParameter<std::string>("electronIsoMode");
  electronIsoMode_ = getIsoMode(electronIsoMode_string, cfgError_);

  tauJetEtaCut_ = cfg.getParameter<double>("tauJetEtaCut");
  tauJetPtCut_ = cfg.getParameter<double>("tauJetPtCut");

  visMassCut_ = cfg.getParameter<double>("visMassCut");

  maxNumWarnings_ = cfg.exists("maxNumWarnings") ? cfg.getParameter<int>("maxNumWarnings") : 1;
}

void EwkElecTauValHistManager::bookHistograms()
{
  dqmStore_->setCurrentFolder(dqmDirectory_);

  hNumIdElectrons_ = dqmStore_->book1D("NumIdElectronsMuons" , "Num. id. Muons", 5, -0.5, 4.5);
  hElectronPt_ = dqmStore_->book1D("ElectronPt" , "P_{T}^{e}", 20, 0., 100.);
  hElectronEta_ = dqmStore_->book1D("ElectronEta" , "#eta_{e}", 20, -4.0, +4.0);
  hElectronPhi_ = dqmStore_->book1D("ElectronPhi" , "#phi_{e}", 20, -TMath::Pi(), +TMath::Pi());
  hElectronTrackIsoPt_ = dqmStore_->book1D("ElectronTrackIsoPt" , "Electron Track Iso.", 20, -0.01, 10.);
  hElectronEcalIsoPt_ = dqmStore_->book1D("ElectronEcalIsoPt" , "Electron Ecal Iso.", 20, -0.01, 10.);

  hTauJetPt_ = dqmStore_->book1D("TauJetPt" , "P_{T}^{#tau-Jet}", 20, 0., 100.);
  hTauJetEta_ = dqmStore_->book1D("TauJetEta" , "#eta_{#tau-Jet}", 20, -4.0, +4.0);
  hTauJetPhi_ = dqmStore_->book1D("TauJetPhi" , "#phi_{#tau-Jet}", 20, -TMath::Pi(), +TMath::Pi());
  hTauLeadTrackPt_ = dqmStore_->book1D("TauLeadTrackPt" , "P_{T}^{#tau-Jet}", 20, 0., 50.);
  hTauTrackIsoPt_ = dqmStore_->book1D("TauTrackIsoPt" , "Tau Track Iso.", 20, -0.01, 40.);
  hTauEcalIsoPt_ = dqmStore_->book1D("TauEcalIsoPt" , "Tau Ecal Iso.", 10, -0.01, 10.);
  hTauDiscrAgainstElectrons_ = dqmStore_->book1D("TauDiscrAgainstElectrons" , "Tau Discr. against Electrons", 2, -0.5, +1.5);
  hTauDiscrAgainstMuons_ = dqmStore_->book1D("TauDiscrAgainstMuons" , "Tau Discr. against Muons", 2, -0.5, +1.5);
  hTauJetCharge_ = dqmStore_->book1D("TauJetCharge" , "Q_{#tau-Jet}", 11, -5.5, +5.5);
  hTauJetNumSignalTracks_ = dqmStore_->book1D("TauJetNumSignalTracks" , "Num. Tau signal Cone Tracks", 20, -0.5, +19.5);
  hTauJetNumIsoTracks_ = dqmStore_->book1D("TauJetNumIsoTracks" , "Num. Tau isolation Cone Tracks", 20, -0.5, +19.5);
  
  hVisMass_ = dqmStore_->book1D("VisMass", "e + #tau-Jet visible Mass", 20, 20., 120.);
  hMtElecCaloMEt_ = dqmStore_->book1D("MtElecCaloMEt", "e + E_{T}^{miss} (Calo) transverse Mass", 20, 20., 120.);
  hMtElecPFMEt_ = dqmStore_->book1D("MtElecPFMEt", "e + E_{T}^{miss} (PF) transverse Mass", 20, 20., 120.);
  hPzetaCaloMEt_ = dqmStore_->book1D("PzetaCaloMEt", "P_{#zeta} - 1.5*P_{#zeta}^{vis} (Calo)", 20, -40., 40.);
  hPzetaPFMEt_ = dqmStore_->book1D("PzetaPFMEt", "P_{#zeta} - 1.5*P_{#zeta}^{vis} (PF)", 20, -40., 40.);
  hElecTauAcoplanarity_ = dqmStore_->book1D("ElecTauAcoplanarity", "#Delta #phi_{e #tau-Jet}", 20, -TMath::Pi(), +TMath::Pi());
  hElecTauCharge_ = dqmStore_->book1D("ElecTauCharge" , "Q_{e + #tau-Jet}", 11, -5.5, +5.5);

  hVertexChi2_ = dqmStore_->book1D("VertexChi2", "Event Vertex #chi^{2} / n.d.o.f.", 20, 0., 2.0);
  hVertexZ_ = dqmStore_->book1D("VertexZ", "Event Vertex z-Position", 20, -25., +25.);
  hVertexD0_ = dqmStore_->book1D("VertexD0", "Event Vertex d_{0}", 20, -0.0001, 0.05);

  hCaloMEtPt_ = dqmStore_->book1D("CaloMEtPt", "E_{T}^{miss} (Calo)", 20, 0., 100.);
  hCaloMEtPhi_ = dqmStore_->book1D("CaloMEtPhi", "#phi^{miss} (Calo)", 20, -TMath::Pi(), +TMath::Pi());

  hPFMEtPt_ = dqmStore_->book1D("PFMEtPt", "E_{T}^{miss} (PF)", 20, 0., 100.);
  hPFMEtPhi_ = dqmStore_->book1D("PFMEtPhi", "#phi^{miss} (PF)", 20, -TMath::Pi(), +TMath::Pi());

  hCutFlowSummary_ = dqmStore_->book1D("CutFlowSummary", "Cut-flow Summary", 11, 0.5, 11.5);
  hCutFlowSummary_->setBinLabel(kPassedPreselection, "Preselection");
  hCutFlowSummary_->setBinLabel(kPassedTrigger, "HLT");
  hCutFlowSummary_->setBinLabel(kPassedElectronId, "e ID");
  hCutFlowSummary_->setBinLabel(kPassedElectronTrackIso, "e Trk Iso.");
  hCutFlowSummary_->setBinLabel(kPassedElectronEcalIso, "e Ecal Iso.");
  hCutFlowSummary_->setBinLabel(kPassedTauLeadTrack, "#tau lead. Track");
  hCutFlowSummary_->setBinLabel(kPassedTauLeadTrackPt, "#tau lead. Track P_{T}");
  hCutFlowSummary_->setBinLabel(kPassedTauTrackIso, "#tau Track Iso.");
  hCutFlowSummary_->setBinLabel(kPassedTauEcalIso, "#tau Ecal Iso.");
  hCutFlowSummary_->setBinLabel(kPassedTauDiscrAgainstElectrons, "#tau anti-e Discr.");
  hCutFlowSummary_->setBinLabel(kPassedTauDiscrAgainstMuons, "#tau anti-#mu Discr.");
}

void EwkElecTauValHistManager::fillHistograms(const edm::Event& evt, const edm::EventSetup& es)
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
	edm::LogWarning ("EwkElecTauValHistManager") << " Undefined HLT path = " << (*hltPath) << " !!";
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

//--- get collections of reconstructed tau-jets from the event
  edm::Handle<reco::PFTauCollection> tauJets;
  readEventData(evt, tauJetSource_, tauJets, numWarningsTauJet_, maxNumWarnings_,
		readError, "Failed to access Tau-jet collection");
  if ( readError ) return;

//--- get collections of tau-jet discriminators for those tau-jets
  edm::Handle<reco::PFTauDiscriminator> tauDiscrByLeadTrackFinding;
  readEventData(evt, tauDiscrByLeadTrackFinding_, tauDiscrByLeadTrackFinding, numWarningsTauDiscrByLeadTrackFinding_, maxNumWarnings_,
		readError, "Failed to access collection of pf. Tau discriminators by leading Track finding");
  edm::Handle<reco::PFTauDiscriminator> tauDiscrByLeadTrackPtCut;
  readEventData(evt, tauDiscrByLeadTrackPtCut_, tauDiscrByLeadTrackPtCut, numWarningsTauDiscrByLeadTrackPtCut_, maxNumWarnings_,
		readError, "Failed to access collection of pf. Tau discriminators by leading Track Pt cut");
  edm::Handle<reco::PFTauDiscriminator> tauDiscrByTrackIso;
  readEventData(evt, tauDiscrByTrackIso_, tauDiscrByTrackIso, numWarningsTauDiscrByTrackIso_, maxNumWarnings_,
		readError, "Failed to access collection of pf. Tau discriminators by Track isolation");
  edm::Handle<reco::PFTauDiscriminator> tauDiscrByEcalIso;
  readEventData(evt, tauDiscrByTrackIso_, tauDiscrByEcalIso, numWarningsTauDiscrByEcalIso_, maxNumWarnings_,
		readError, "Failed to access collection of pf. Tau discriminators by ECAL isolation");
  edm::Handle<reco::PFTauDiscriminator> tauDiscrAgainstElectrons;
  readEventData(evt, tauDiscrAgainstElectrons_, tauDiscrAgainstElectrons, numWarningsTauDiscrAgainstElectrons_, maxNumWarnings_,
		readError, "Failed to access collection of pf. Tau discriminators against Electrons");
  edm::Handle<reco::PFTauDiscriminator> tauDiscrAgainstMuons;
  readEventData(evt, tauDiscrAgainstMuons_, tauDiscrAgainstMuons, numWarningsTauDiscrAgainstMuons_, maxNumWarnings_,
		readError, "Failed to access collection of pf. Tau discriminators against Muons");
  if ( readError ) return;

  int theTauJetIndex = -1;
  const reco::PFTau* theTauJet = getTheTauJet(*tauJets, tauJetEtaCut_, tauJetPtCut_, theTauJetIndex);

  double theTauDiscrByLeadTrackFinding = -1.;
  double theTauDiscrByLeadTrackPtCut = -1.;
  double theTauDiscrByTrackIso = -1.;
  double theTauDiscrByEcalIso = -1.;
  double theTauDiscrAgainstElectrons = -1.;
  double theTauDiscrAgainstMuons = -1.;
  if ( theTauJetIndex != -1 ) {
    reco::PFTauRef theTauJetRef(tauJets, theTauJetIndex);
    theTauDiscrByLeadTrackFinding = (*tauDiscrByLeadTrackFinding)[theTauJetRef];
    theTauDiscrByLeadTrackPtCut = (*tauDiscrByLeadTrackPtCut)[theTauJetRef];
    theTauDiscrByTrackIso = (*tauDiscrByTrackIso)[theTauJetRef];
    theTauDiscrByEcalIso = (*tauDiscrByEcalIso)[theTauJetRef];
    theTauDiscrAgainstElectrons = (*tauDiscrAgainstElectrons)[theTauJetRef];
    theTauDiscrAgainstMuons = (*tauDiscrAgainstMuons)[theTauJetRef];
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

  if ( !(theElectron && theTauJet && theTauJetIndex != -1) ) return;

  //-----------------------------------------------------------------------------
  // compute EWK tau analysis specific quantities
  //-----------------------------------------------------------------------------

  double dPhiElecTau = calcDeltaPhi(theElectron->phi(), theTauJet->phi());

  double mElecTau = (theElectron->p4() + theTauJet->p4()).M();

  double mtElecCaloMEt = calcMt(theElectron->px(), theElectron->px(), caloMEt.px(), caloMEt.py());
  double mtElecPFMEt = calcMt(theElectron->px(), theElectron->px(), pfMEt.px(), pfMEt.py());

  double pZetaCaloMEt = calcPzeta(theElectron->p4(), theTauJet->p4(), caloMEt.px(), caloMEt.py());
  double pZetaPFMEt = calcPzeta(theElectron->p4(), theTauJet->p4(), pfMEt.px(), pfMEt.py());

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

  ++numEventsAnalyzed_;

  bool isSelected = false;
  int cutFlowStatus = -1;

  if ( mElecTau > visMassCut_ ) {
    cutFlowStatus = kPassedPreselection;
  }
  if ( cutFlowStatus == kPassedPreselection && (isTriggered || hltPaths_.size() == 0) ) {
    cutFlowStatus = kPassedTrigger;
  }
  if ( cutFlowStatus == kPassedTrigger && passesElectronId(*theElectron) ) {
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
  if ( cutFlowStatus == kPassedElectronEcalIso && theTauDiscrByLeadTrackFinding > 0.5 ) {
    cutFlowStatus = kPassedTauLeadTrack;
    if ( theTauJet->leadTrack().isAvailable() ) hTauLeadTrackPt_->Fill(theTauJet->leadTrack()->pt());
  }
  if ( cutFlowStatus == kPassedTauLeadTrack && theTauDiscrByLeadTrackPtCut > 0.5 ) {
    cutFlowStatus = kPassedTauLeadTrackPt;
    hTauTrackIsoPt_->Fill(theTauJet->isolationPFChargedHadrCandsPtSum());
  }
  if ( cutFlowStatus == kPassedTauLeadTrackPt && theTauDiscrByTrackIso > 0.5 ) {
    cutFlowStatus = kPassedTauTrackIso;
    hTauEcalIsoPt_->Fill(theTauJet->isolationPFGammaCandsEtSum());
  }
  if ( cutFlowStatus == kPassedTauTrackIso && theTauDiscrByEcalIso > 0.5 ) {
    cutFlowStatus = kPassedTauEcalIso;
    hTauDiscrAgainstElectrons_->Fill(theTauDiscrAgainstElectrons);
  }
  if ( cutFlowStatus == kPassedTauEcalIso && theTauDiscrAgainstElectrons > 0.5 ) {
    cutFlowStatus = kPassedTauDiscrAgainstElectrons;
    hTauDiscrAgainstMuons_->Fill(theTauDiscrAgainstMuons);
  }
  if ( cutFlowStatus == kPassedTauDiscrAgainstElectrons && theTauDiscrAgainstMuons > 0.5 ) {
    cutFlowStatus = kPassedTauDiscrAgainstMuons;
    isSelected = true;
  }

  for ( int iCut = 1; iCut <= cutFlowStatus; ++iCut ) {
    hCutFlowSummary_->Fill(iCut);
  }

  if ( isSelected ) {
    hElectronPt_->Fill(theElectron->pt());
    hElectronEta_->Fill(theElectron->eta());
    hElectronPhi_->Fill(theElectron->phi());

    hTauJetPt_->Fill(theTauJet->pt());
    hTauJetEta_->Fill(theTauJet->eta());
    hTauJetPhi_->Fill(theTauJet->phi());

    hTauJetCharge_->Fill(theTauJet->charge());
    if ( theTauJet->signalTracks().isAvailable()    ) hTauJetNumSignalTracks_->Fill(theTauJet->signalTracks().size());
    if ( theTauJet->isolationTracks().isAvailable() ) hTauJetNumIsoTracks_->Fill(theTauJet->isolationTracks().size());
  
    hVisMass_->Fill(mElecTau);
    hMtElecCaloMEt_->Fill(mtElecCaloMEt);
    hMtElecPFMEt_->Fill(mtElecPFMEt);
    hPzetaCaloMEt_->Fill(pZetaCaloMEt);
    hPzetaPFMEt_->Fill(pZetaPFMEt);
    hElecTauAcoplanarity_->Fill(dPhiElecTau);
    hElecTauCharge_->Fill(theElectron->charge() + theTauJet->charge());

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

void EwkElecTauValHistManager::finalizeHistograms()
{
  edm::LogInfo ("EwkElecTauValHistManager") 
    << "Filter-Statistics Summary:" << std::endl
    << " Events analyzed = " << numEventsAnalyzed_ << std::endl
    << " Events selected = " << numEventsSelected_;
  if ( numEventsAnalyzed_ > 0 ) {
    double eff = numEventsSelected_/(double)numEventsAnalyzed_;
    edm::LogInfo ("") 
      << "Overall efficiency = " << std::setprecision(4) << eff*100. 
      << " +/- " << std::setprecision(4) << TMath::Sqrt(eff*(1 - eff)/numEventsAnalyzed_)*100. << ")%";
  }
}

