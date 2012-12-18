#ifndef VgKit_test_VgAnalyzerKit_h
#define VgKit_test_VgAnalyzerKit_h

#include <memory>
#include <fstream>

#include "ElectroWeakAnalysis/MultiBosons/interface/PatKitHelper.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
// Prescale
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include <TRegexp.h>

using namespace edm;
using namespace std;
using namespace reco;

const Int_t maxP = 600;

class VgAnalyzerKit : public EDProducer {

public:

  explicit VgAnalyzerKit(const ParameterSet&);
  virtual ~VgAnalyzerKit();

protected:

  virtual void beginRun(edm::Run& iRun, edm::EventSetup const& iSetup);
  virtual void beginJob();
  virtual void produce( Event &, const EventSetup & );
  virtual void endJob();
  Double_t eT(Double_t pt1, Double_t pt2) const;
  Double_t massT(Double_t pt1, Double_t pt2, Double_t wpx, Double_t wpy) const;
  Double_t acop(Double_t phi1, Double_t phi2) const;
  Float_t recHitApproxEt( const DetId id, const EcalRecHitCollection &recHits );
  Float_t recHitE( const DetId id, const EcalRecHitCollection &recHits );
  Float_t recHitE( const DetId id, const EcalRecHitCollection & recHits, Int_t di, Int_t dj );
  Float_t getGenIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, 
		     const Float_t dRMax=0.4, Bool_t removeMu=true, Bool_t removeNu=false);
  Float_t getGenCalIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, 
		     const Float_t dRMax=0.4, Bool_t removeMu=true, Bool_t removeNu=false);
  Float_t getGenTrkIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax=0.4);
  Float_t getGenCHIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax=0.4);
  Float_t getGenNHIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax=0.4);
  Float_t getGenPhIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax=0.4);

  Float_t getTrkIso(edm::Handle<reco::TrackCollection> Tracks, math::XYZPoint vtx, Double_t phoEta, Double_t phoPhi, Double_t phoVz, Double_t outerCone, Double_t innerCone, Double_t etaSlice, Double_t dzCut, Double_t d0Cut, Double_t ptCut, Int_t option);

  typedef std::vector< edm::Handle< edm::ValueMap<reco::IsoDeposit> > > IsoDepositMaps;
  typedef std::vector< edm::Handle< edm::ValueMap<double> > > IsoDepositVals;

  Bool_t saveHistograms_;
  Bool_t doGenParticles_;
  Bool_t doStoreJets_;
  Bool_t doJetHLTMatch_;
  Bool_t doStoreSCs_;
  Bool_t doSkim_;

  InputTag vtxlabel_;
  InputTag gtdigilabel_;
  InputTag tracklabel_;
  InputTag trgResults_;
  InputTag trgEvent_;
  InputTag caloTowerlabel_;
  InputTag tcMETlabel_;
  InputTag pfMETlabel_;
  InputTag TypeIpfMETlabel_;
  InputTag TypeIpIIpfMETlabel_;
  InputTag SmearedTypeIpfMETlabel_;
  InputTag PFCandLabel_;
  InputTag puInfoLabel_;
  InputTag rhoLabel_;
  InputTag rho2011Label_;
  InputTag rho2012Label_;
  InputTag sigmaLabel_;
  InputTag ebReducedRecHitCollection_;
  InputTag eeReducedRecHitCollection_;
  InputTag beamSpotCollection_;
  std::vector<edm::InputTag> inputTagIsoDepElectrons_;
  std::vector<edm::InputTag> inputTagIsoDepPhotons_;
  std::vector<edm::InputTag> inputTagIsoValElectronsPFId_;
  std::vector<edm::InputTag> inputTagIsoValPhotonsPFId_;

  Int_t verbosity_;
  Int_t leadingElePtCut_;
  Int_t leadingMuPtCut_;
  Int_t leadingPhoPtCut_;
  std::vector<string> skimedHLTpath_;

  pat::PatKitHelper helper_;
  TTree *tree_;
  TH1F  *hEvents_; 

  Int_t    METFilter_[10];
  Int_t    run_;
  Long64_t event_;
  Int_t    orbit_;
  Int_t    bx_;
  Int_t    lumis_;
  Bool_t   isData_;
  Int_t    ttbit0_;
  Float_t  pdf_[7];
  Float_t  pthat_;
  Float_t  processID_;
  Float_t  TrueNI_;
  Int_t    nBX_;
  Int_t    nPU_[maxP];
  Int_t    BXPU_[maxP];
  Double_t rho_;
  Double_t rho2011_;
  Double_t rho2012_;
  Float_t  sigma_;
  Int_t    nHLT_;
  Int_t    HLT_[maxP];
  Int_t    HLTprescale_[maxP];
  Int_t    HLTIndex_[250];
  Int_t    nVtx_;
  Float_t  vtx_[150][3];
  Int_t    vtxNTrk_[150];
  Float_t  vtxNDF_[150];
  Float_t  vtxD0_[150];
  Int_t    nGoodVtx_;
  Int_t    nTrk_;
  Int_t    nGoodTrk_;
  Int_t    IsTracksGood_;
  // genParticle
  Int_t    nMC_;
  Int_t    mcPID[maxP];
  Float_t  mcVtx[maxP][3];
  Float_t  mcPt[maxP];
  Float_t  mcMass[maxP];
  Float_t  mcEta[maxP];
  Float_t  mcPhi[maxP];
  Int_t    mcGMomPID[maxP];
  Int_t    mcMomPID[maxP];
  Float_t  mcMomPt[maxP];
  Float_t  mcMomMass[maxP];
  Float_t  mcMomEta[maxP];
  Float_t  mcMomPhi[maxP];
  Int_t    mcIndex[maxP];
  Int_t    mcDecayType[maxP];
  Float_t  mcIsoDR03[maxP];
  Float_t  mcCHIsoDR03[maxP];
  Float_t  mcNHIsoDR03[maxP];
  Float_t  mcPhIsoDR03[maxP];
  Float_t  mcIsoDR04[maxP];
  Float_t  mcCHIsoDR04[maxP];
  Float_t  mcNHIsoDR04[maxP];
  Float_t  mcPhIsoDR04[maxP];
  // Gen & Reco MET
  Float_t  genMET_;
  Float_t  genMETx_;
  Float_t  genMETy_;
  Float_t  genMETPhi_;
  Float_t  MET_;
  Float_t  METx_;
  Float_t  METy_;
  Float_t  METPhi_;
  Float_t  METsumEt_;
  Float_t  uncorrMET_[3];
  Float_t  uncorrMETPhi_[3];
  Float_t  uncorrMETSumEt_[3];
  // tcMET
  Float_t  tcMET_;
  Float_t  tcMETx_;
  Float_t  tcMETy_;
  Float_t  tcMETPhi_;
  Float_t  tcMETsumEt_;
  Float_t  tcMETmEtSig_;
  Float_t  tcMETSig_;
  // pfMET
  Float_t  pfMET_;
  Float_t  pfMETx_;
  Float_t  pfMETy_;
  Float_t  pfMETPhi_;
  Float_t  pfMETsumEt_;
  Float_t  pfMETmEtSig_;
  Float_t  pfMETSig_;
  // TypeI pfMET
  Float_t  TypeIpfMET_;
  Float_t  TypeIpfMETx_;
  Float_t  TypeIpfMETy_;
  Float_t  TypeIpfMETPhi_;
  Float_t  TypeIpfMETsumEt_;
  Float_t  TypeIpfMETmEtSig_;
  Float_t  TypeIpfMETSig_;
  // Smeared pfMET
  Float_t  SmearedpfMET_;
  Float_t  SmearedpfMETx_;
  Float_t  SmearedpfMETy_;
  Float_t  SmearedpfMETPhi_;
  Float_t  SmearedpfMETsumEt_;
  Float_t  SmearedpfMETmEtSig_;
  Float_t  SmearedpfMETSig_;
  // Smeared TypeI pfMET
  Float_t  SmearedTypeIpfMET_;
  Float_t  SmearedTypeIpfMETx_;
  Float_t  SmearedTypeIpfMETy_;
  Float_t  SmearedTypeIpfMETPhi_;
  Float_t  SmearedTypeIpfMETsumEt_;
  Float_t  SmearedTypeIpfMETmEtSig_;
  Float_t  SmearedTypeIpfMETSig_;
  // Electron
  Int_t    nEle_;
  Bool_t   eleEcalDriven_[maxP];
  Int_t    eleTrg_[maxP][14];
  Int_t    eleID_[maxP][30];
  Int_t    eleClass_[maxP];
  Int_t    eleCharge_[maxP];
  Float_t  eleVtx_[maxP][3];
  Float_t  eleEn_[maxP];
  Float_t  eleGsfP_[maxP];
  Float_t  elePt_[maxP];
  Float_t  elePz_[maxP];
  Float_t  eleEta_[maxP];
  Float_t  elePhi_[maxP];
  Float_t  eleEcalEn_[maxP];
  Float_t  eleSCEn_[maxP];
  Float_t  eleSCRawEn_[maxP];
  Float_t  eleESEn_[maxP];
  Float_t  eleSCEta_[maxP];
  Float_t  eleSCPhi_[maxP];
  Float_t  eleSCEtaWidth_[maxP];
  Float_t  eleSCPhiWidth_[maxP];
  Float_t  eleHoverE_[maxP];
  Float_t  eleHoverEbc_[maxP];
  Float_t  eleEoverP_[maxP];
  Float_t  eleTrkPErr_[maxP];
  Float_t  elePin_[maxP];
  Float_t  elePout_[maxP];
  Float_t  eleBrem_[maxP];
  Float_t  elenBrem_[maxP];
  Float_t  eledEtaAtVtx_[maxP];
  Float_t  eledPhiAtVtx_[maxP];
  Float_t  eledEtaAtCalo_[maxP];
  Float_t  eledPhiAtCalo_[maxP];
  Float_t  eleSigmaEtaEta_[maxP];
  Float_t  eleSigmaIEtaIEta_[maxP];
  Float_t  eleSigmaIEtaIPhi_[maxP];
  Float_t  eleSigmaIPhiIPhi_[maxP];
  Float_t  eleE2overE9_[maxP];
  Float_t  eleEmax_[maxP];
  Float_t  eleE1x5_[maxP];
  Float_t  eleE3x3_[maxP];
  Float_t  eleE5x5_[maxP];
  Float_t  eleKfChi2_[maxP];
  Float_t  eleKfHits_[maxP];
  Float_t  eleKfHitsAll_[maxP];
  Float_t  eleGsfChi2_[maxP];
  Float_t  eleSeedTime_[maxP];
  Float_t  eleSeedEnergy_[maxP];
  Int_t    eleGenIndex_[maxP];
  Int_t    eleGenGMomPID_[maxP];
  Int_t    eleGenMomPID_[maxP];
  Float_t  eleGenMomPt_[maxP];
  Float_t  eleIsoTrkDR03_[maxP];
  Float_t  eleIsoEcalDR03_[maxP];
  Float_t  eleIsoHcalDR03_[maxP];
  Float_t  eleIsoHcalSolidDR03_[maxP];
  Float_t  eleIsoTrkDR04_[maxP];
  Float_t  eleIsoEcalDR04_[maxP];
  Float_t  eleIsoHcalDR04_[maxP];
  Float_t  eleIsoHcalSolidDR04_[maxP];
  Float_t  eleConvDist_[maxP];
  Float_t  eleConvDcot_[maxP];
  Int_t    eleConvMissinghit_[maxP];
  Bool_t   eleConversionveto_[maxP];
  Float_t  elePVD0_[maxP];
  Float_t  elePVDz_[maxP];
  Float_t  eleGVD0_[maxP];
  Float_t  eleGVDz_[maxP];
  Float_t  eleIP3D_[maxP];
  Float_t  eleIP3DErr_[maxP];
  Bool_t   eleIsPFID_[maxP];
  Double_t elePfChargedHadronDR03_[maxP];
  Double_t elePfNeutralHadronDR03_[maxP];
  Double_t elePfPhotonDR03_[maxP];
  Double_t elePfChargedHadronDR04_[maxP];
  Double_t elePfNeutralHadronDR04_[maxP];
  Double_t elePfPhotonDR04_[maxP];
  // Photon
  Int_t    nPho_;
  Int_t    phoTrg_[maxP][23];
  Bool_t   phoIsPhoton_[maxP];
  Float_t  phoE_[maxP];
  Float_t  phoEt_[maxP];
  Float_t  phoPz_[maxP];
  Float_t  phoEta_[maxP];
  Float_t  phoPhi_[maxP];
  Float_t  phoR9_[maxP];
  Float_t  phoTrkIsoSolidDR03_[maxP];
  Float_t  phoTrkIsoHollowDR03_[maxP];
  Float_t  phoEcalIsoDR03_[maxP];
  Float_t  phoHcalIsoDR03_[maxP];
  Float_t  phoTrkIsoSolidDR04_[maxP];
  Float_t  phoTrkIsoHollowDR04_[maxP];
  Float_t  phoEcalIsoDR04_[maxP];
  Float_t  phoHcalIsoDR04_[maxP];
  Float_t  phoEtVtx_[maxP][150];
  Float_t  phoEtaVtx_[maxP][150];
  Float_t  phoPhiVtx_[maxP][150];
  Float_t  phoHoverE_[maxP];
  Float_t  phoHoverEbc_[maxP];
  Float_t  phoSigmaEtaEta_[maxP];
  Float_t  phoSigmaIEtaIEta_[maxP];
  Float_t  phoSigmaIEtaIPhi_[maxP];
  Float_t  phoSigmaIPhiIPhi_[maxP];
  Float_t  phoE2overE9_[maxP];
  Float_t  phoE1x5_[maxP];
  Float_t  phoE2x5_[maxP];
  Float_t  phoE3x3_[maxP];
  Float_t  phoE5x5_[maxP];
  Float_t  phoSeedTime_[maxP];
  Float_t  phoSeedEnergy_[maxP];
  Int_t    phoPos_[maxP];
  Int_t    phoGenIndex_[maxP];
  Int_t    phoGenGMomPID[maxP];
  Int_t    phoGenMomPID[maxP];
  Float_t  phoGenMomPt[maxP];
  Float_t  phoSCE_[maxP];
  Float_t  phoSCEt_[maxP];
  Float_t  phoSCEta_[maxP];
  Float_t  phoSCPhi_[maxP];
  Float_t  phoSCEtaWidth_[maxP];
  Float_t  phoSCPhiWidth_[maxP];
  Float_t  phoVtx_[maxP][3];
  Int_t    phoOverlap_[maxP];
  Int_t    phohasPixelSeed_[maxP];
  Int_t    phoElectronveto_[maxP];
  Bool_t   phoIsPFID_[maxP];
  Float_t  phoPfChargedHadron_[maxP];
  Float_t  phoPfNeutralHadron_[maxP];
  Float_t  phoPfPhoton_[maxP];

  // Muon
  Int_t    nMu_;
  Bool_t   muIsPFMu_[maxP];
  Bool_t   muIsGlobalMu_[maxP];
  Bool_t   muIsTrackerMu_[maxP];
  Int_t    muTrg_[maxP][11];
  Float_t  muEta_[maxP];
  Float_t  muPhi_[maxP];
  Int_t    muCharge_[maxP];
  Float_t  muPt_[maxP];
  Float_t  muPz_[maxP];
  Int_t    muGenIndex_[maxP];
  Int_t    muGenGMomPID_[maxP];
  Int_t    muGenMomPID_[maxP];
  Float_t  muGenMomPt_[maxP];
  Float_t  muIsoTrk_[maxP];
  Float_t  muIsoCalo_[maxP];
  Float_t  muIsoEcal_[maxP];
  Float_t  muIsoHcal_[maxP];
  Float_t  muChi2NDF_[maxP];
  Int_t    muType_[maxP];
  Bool_t   muID_[maxP][6];
  Float_t  muPVD0_[maxP];
  Float_t  muPVDz_[maxP];
  Float_t  muGVD0_[maxP];
  Float_t  muGVDz_[maxP];
  Float_t  muTrkdPt_[maxP];
  Int_t    muNumberOfValidTrkHits_[maxP];
  Int_t    muNumberOfValidPixelHits_[maxP];
  Int_t    muNumberOfValidMuonHits_[maxP];
  Int_t    muNumberOfTrackerLayers_[maxP];
  Int_t    muNumberOfMatchedStations_[maxP];
  Float_t  muVtx_[maxP][3];
  Float_t  muPfChargedHadron_[maxP];
  Float_t  muPfNeutralHadron_[maxP];
  Float_t  muPfPhoton_[maxP];
  Float_t  muPfSumPUPt_[maxP];

  // Jet
  Int_t    nJet_;
  Int_t    jetTrg_[maxP][7];
  Int_t    jetAlgo_[maxP];
  Float_t  jetEn_[maxP];
  Float_t  jetPt_[maxP];
  Float_t  jetEta_[maxP];
  Float_t  jetPhi_[maxP];
  Float_t  jetMass_[maxP];
  Float_t  jetEt_[maxP];
  Float_t  jetRawPt_[maxP];
  Float_t  jetRawEn_[maxP];
  Float_t  jetCharge_[maxP];
  Int_t    jetpartonFlavour_[maxP];
  Float_t  jetNeutralEmEnergy_[maxP];
  Float_t  jetNeutralEmEnergyFraction_[maxP];
  Float_t  jetNeutralHadronEnergy_[maxP];
  Float_t  jetNeutralHadronEnergyFraction_[maxP];
  Int_t    jetNConstituents_[maxP];
  Float_t  jetChargedEmEnergy_[maxP];
  Float_t  jetChargedEmEnergyFraction_[maxP];
  Float_t  jetChargedHadronEnergy_[maxP];
  Float_t  jetChargedHadronEnergyFraction_[maxP];
  Int_t    jetChargedHadronMultiplicity_[maxP];
  Float_t  jetChargedMuEnergy_[maxP];
  Float_t  jetChargedMuEnergyFraction_[maxP];
  Double_t jetJVAlpha_[maxP];
  Double_t jetJVBeta_[maxP];
  Float_t  jetmaxEInEmTowers_[maxP];
  Float_t  jetmaxEInHadTowers_[maxP];
  Float_t  jetenergyFractionHadronic_[maxP];
  Float_t  jetemEnergyFraction_[maxP];
  Float_t  jetfHPD_[maxP];
  Int_t	   jetN60_[maxP] ;
  Int_t	   jetN90_[maxP] ;
  Int_t    jetGenJetIndex_[maxP];
  Float_t  jetGenJetEn_[maxP];
  Float_t  jetGenJetPt_[maxP];
  Float_t  jetGenJetEta_[maxP];
  Float_t  jetGenJetPhi_[maxP];
  Float_t  jetGenJetMass_[maxP];
  Int_t    jetGenPartonID_[maxP];
  Int_t    jetGenPartonMomID_[maxP];

  // SC information
  Int_t    nEBSC_;
  Float_t  scEBEn_[maxP];
  Float_t  scEBEt_[maxP];
  Float_t  scEBEta_[maxP];
  Float_t  scEBPhi_[maxP];

  Int_t    nEESC_;
  Float_t  scEEEn_[maxP];
  Float_t  scEEEt_[maxP];
  Float_t  scEEEta_[maxP];
  Float_t  scEEPhi_[maxP];

  // Physics objects handles
  Handle<View<pat::Muon> >                  muonHandle_;
  Handle<View<pat::Electron> >              electronHandle_;
  Handle<View<pat::Tau> >                   tauHandle_;
  Handle<View<pat::Jet> >                   jetHandle_;
  Handle<View<pat::MET> >                   METHandle_;
  Handle<View<pat::Photon> >                photonHandle_;
  Handle<std::vector<reco::RecoChargedCandidate> > trackHandle_;
  Handle<std::vector<reco::GenParticle> >          genParticlesHandle_;
  Handle<reco::CandidateView>               zmumuHandle_;

  HLTConfigProvider hltConfig_;
};

#endif
