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

#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"

using namespace edm;
using namespace std;

const Int_t maxP = 500;

class VgAnalyzerKit : public EDProducer {

public:

  explicit VgAnalyzerKit(const ParameterSet&);
  virtual ~VgAnalyzerKit();

protected:

  virtual void beginJob();
  virtual void produce( Event &, const EventSetup & );
  virtual void endJob();
  Double_t eT(Double_t pt1, Double_t pt2) const;
  Double_t massT(Double_t pt1, Double_t pt2, Double_t wpx, Double_t wpy) const;
  Double_t acop(Double_t phi1, Double_t phi2) const;
  Float_t E2overE9(const DetId id, const EcalRecHitCollection &, Float_t recHitEtThreshold = 10.0 , 
		 Float_t recHitEtThreshold2 = 1.0 , Bool_t avoidIeta85=false, Bool_t KillSecondHit=true);
  Float_t recHitApproxEt( const DetId id, const EcalRecHitCollection &recHits );
  Float_t recHitE( const DetId id, const EcalRecHitCollection &recHits );
  Float_t recHitE( const DetId id, const EcalRecHitCollection & recHits, Int_t di, Int_t dj );
  Float_t getGenCalIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, 
		     const Float_t dRMax=0.4, Bool_t removeMu=true, Bool_t removeNu=false);
  Float_t getGenTrkIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax=0.4);
  Float_t getESRatio(View<pat::Electron>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  std::vector<float> getESProfileFront(View<pat::Electron>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  std::vector<float> getESProfileRear(View<pat::Electron>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  Float_t getESRatio(View<pat::Photon>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  std::vector<float> getESProfileFront(View<pat::Photon>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  std::vector<float> getESProfileRear(View<pat::Photon>::const_iterator photon, const edm::Event&, const edm::EventSetup&);

  Bool_t saveHistograms_;
  Bool_t saveHLTInfo_;
  Bool_t doGenParticles_;
  Bool_t doStoreJets_;
  Bool_t doSkim_;

  InputTag vtxlabel_;
  InputTag gtdigilabel_;
  InputTag tracklabel_;
  InputTag trgResults_;
  InputTag trgEvent_;
  InputTag caloTowerlabel_;
  InputTag tcMETlabel_;
  InputTag pfMETlabel_;
  InputTag ebReducedRecHitCollection_;
  InputTag eeReducedRecHitCollection_;
  InputTag beamSpotCollection_;

  Int_t verbosity_;
  Int_t leadingElePtCut_;
  Int_t leadingMuPtCut_;
  Int_t leadingPhoPtCut_;
  string muonMatch_;

  pat::PatKitHelper helper_;
  TTree *tree_;
  TH1F  *hEvents_; 

  Int_t    run_;
  Int_t    event_;
  Int_t    orbit_;
  Int_t    bx_;
  Int_t    lumis_;
  Bool_t   isData_;
  Int_t    ttbit0_;
  Float_t  pdf_[7];
  Float_t  pthat_;
  Float_t  processID_;
  Int_t    nHLT_;
  Int_t    HLT_[maxP];
  Int_t    HLTIndex_[50];
  Int_t    nHFTowersP_;
  Int_t    nHFTowersN_;
  Int_t    nVtx_;
  Float_t  vtx_[50][3];
  Int_t    vtxNTrk_[50];
  Int_t    vtxNDF_[50];
  Float_t  vtxD0_[50];
  Int_t    IsVtxGood_;
  Int_t    nTrk_;
  Int_t    nGoodTrk_;
  Int_t    IsTracksGood_;
  // genParticle
  Int_t    nMC_;
  Int_t    mcPID[maxP];
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
  Float_t  mcCalIsoDR03[maxP];
  Float_t  mcTrkIsoDR03[maxP];
  Float_t  mcCalIsoDR04[maxP];
  Float_t  mcTrkIsoDR04[maxP];
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
  // Electron
  Int_t  nEle_;
  Int_t    eleTrg_[maxP][13];
  Int_t    eleID_[maxP][12];
  Int_t    eleClass_[maxP];
  Int_t    eleCharge_[maxP];
  Float_t  eleEn_[maxP];
  Float_t  eleSCEn_[maxP];
  Float_t  eleESEn_[maxP];
  Float_t  eleSCEtaWidth_[maxP];
  Float_t  eleSCPhiWidth_[maxP];
  Float_t  eleVtx_[maxP][3];
  Float_t  eleCaloPos_[maxP][3];
  Float_t  eleSCPos_[maxP][3];
  Float_t  elePt_[maxP];
  Float_t  elePz_[maxP];
  Float_t  eleEta_[maxP];
  Float_t  elePhi_[maxP];
  Float_t  eleSCEta_[maxP];
  Float_t  eleSCPhi_[maxP];
  Float_t  eleSCRawEn_[maxP];
  Float_t  eleHoverE_[maxP];
  Float_t  eleEoverP_[maxP];
  Float_t  elePin_[maxP];
  Float_t  elePout_[maxP];
  Float_t  eleBrem_[maxP];
  Float_t  eledEtaAtVtx_[maxP];
  Float_t  eledPhiAtVtx_[maxP];
  Float_t  eleSigmaEtaEta_[maxP];
  Float_t  eleSigmaIEtaIEta_[maxP];
  Float_t  eleE2overE9_[maxP];
  Float_t  eleE3x3_[maxP];
  Float_t  eleSeedTime_[maxP];
  Int_t    eleRecoFlag_[maxP];
  Int_t    eleSeverity_[maxP];
  Int_t    eleGenIndex_[maxP];
  Int_t    eleGenGMomPID_[maxP];
  Int_t    eleGenMomPID_[maxP];
  Float_t  eleGenMomPt_[maxP];
  Float_t  eleIsoTrkDR03_[maxP];
  Float_t  eleIsoEcalDR03_[maxP];
  Float_t  eleIsoHcalDR03_[maxP];
  Float_t  eleIsoTrkDR04_[maxP];
  Float_t  eleIsoEcalDR04_[maxP];
  Float_t  eleIsoHcalDR04_[maxP];
  Float_t  eleChi2NDF_[maxP];
  Float_t  eleD0_[maxP];
  Int_t    eleNumberOfValidHits_[maxP];
  Int_t    eleValidHitInFirstPXB_[maxP];
  Int_t    eleTrkExpectHitsInner_[maxP];
  Float_t  eleDist_[maxP];
  Float_t  eleDcot_[maxP];
  Float_t  eleConvRadius_[maxP];
  Float_t  eleConvPoInt_t_[maxP][3];
  Float_t  eleESRatio_[maxP];
  Float_t  eleESProfileFront_[maxP][63];
  Float_t  eleESProfileRear_[maxP][63];

  // Photon
  Int_t    nPho_;
  Int_t    phoTrg_[maxP][8];
  Bool_t   phoIsPhoton_[maxP];
  Float_t  phoE_[maxP];
  Float_t  phoEt_[maxP];
  Float_t  phoPz_[maxP];
  Float_t  phoEta_[maxP];
  Float_t  phoPhi_[maxP];
  Float_t  phoR9_[maxP];
  Float_t  phoTrkIsoSolidDR03_[maxP];
  Float_t  phoTrkIsoHollowDR03_[maxP];
  Int_t    phoNTrkSolidDR03_[maxP];
  Int_t    phoNTrkHollowDR03_[maxP];
  Float_t  phoEcalIsoDR03_[maxP];
  Float_t  phoHcalIsoDR03_[maxP];
  Float_t  phoTrkIsoSolidDR04_[maxP];
  Float_t  phoTrkIsoHollowDR04_[maxP];
  Int_t    phoNTrkSolidDR04_[maxP];
  Int_t    phoNTrkHollowDR04_[maxP];
  Float_t  phoEcalIsoDR04_[maxP];
  Float_t  phoHcalIsoDR04_[maxP];
  Float_t  phoHoverE_[maxP];
  Float_t  phoSigmaEtaEta_[maxP];
  Float_t  phoSigmaIEtaIEta_[maxP];
  Float_t  phoE2overE9_[maxP];
  Float_t  phoE3x3_[maxP];
  Float_t  phoSeedTime_[maxP];
  Int_t    phoRecoFlag_[maxP];
  Int_t    phoSeverity_[maxP];
  Int_t    phoPos_[maxP];
  Float_t  phoRoundness_[maxP];
  Float_t  phoAngle_[maxP];
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
  Int_t    phoOverlap_[maxP];
  Int_t    phohasPixelSeed_[maxP];
  Int_t    phoIsConv_[maxP];
  Float_t  phoPi0Disc_[maxP];
  Float_t  phoESRatio_[maxP];
  Float_t  phoESProfileFront_[maxP][63];
  Float_t  phoESProfileRear_[maxP][63];

  // Muon
  Int_t    nMu_;
  Int_t    muTrg_[maxP][6];
  Float_t  muEta_[maxP];
  Float_t  muPhi_[maxP];
  Int_t    muCharge_[maxP];
  Float_t  muPt_[maxP];
  Float_t  muPz_[maxP];
  Int_t    muGenIndex_[maxP];
  Float_t  muIsoTrk_[maxP];
  Float_t  muIsoCalo_[maxP];
  Float_t  muIsoEcal_[maxP];
  Float_t  muIsoHcal_[maxP];
  Float_t  muChi2NDF_[maxP];
  Float_t  muEmVeto_[maxP];
  Float_t  muHadVeto_[maxP];
  Int_t    muType_[maxP];
  Bool_t   muID_[maxP][6];
  Float_t  muD0_[maxP];
  Float_t  muDz_[maxP];
  Int_t    muNumberOfValidTrkHits_[maxP];
  Int_t    muNumberOfValidPixelHits_[maxP];
  Int_t    muNumberOfValidMuonHits_[maxP];
  Int_t    muStations_[maxP];
  Int_t    muChambers_[maxP];

  // Jet
  Int_t    nJet_;
  Int_t    jetTrg_[maxP][14];
  Int_t    jetAlgo_[maxP];
  Float_t  jetEn_[maxP];
  Float_t  jetPt_[maxP];
  Float_t  jetEta_[maxP];
  Float_t  jetPhi_[maxP];
  Float_t  jetMass_[maxP];
  Float_t  jetEt_[maxP];
  Float_t  jetmaxEInEmTowers_[maxP];
  Float_t  jetmaxEInHadTowers_[maxP];
  Float_t  jetenergyFractionHadronic_[maxP];
  Float_t  jetemEnergyFraction_[maxP];
  Float_t	 jetfHPD_[maxP];
  Int_t	 jetN60_[maxP] ;
  Int_t	 jetN90_[maxP] ;
  Int_t    jetGenIndex_[maxP];
  Int_t    jetGenJetIndex_[maxP];
  Float_t  jetGenJetEn_[maxP];
  Float_t  jetGenJetPt_[maxP];
  Float_t  jetGenJetEta_[maxP];
  Float_t  jetGenJetPhi_[maxP];
  Float_t  jetGenJetMass_[maxP];
  Int_t    jetGenPartonID_[maxP];
  Int_t    jetGenPartonMomID_[maxP];
  Int_t    jetpartonFlavour_[maxP];
  Float_t  jetRawPt_[maxP];
  Float_t  jetRawEn_[maxP];
  Float_t  jetCharge_[maxP];

  // Zee candidate
  Int_t    nZee_;
  Float_t  ZeeMass_[maxP];
  Float_t  ZeePt_[maxP];
  Float_t  ZeeEta_[maxP];
  Float_t  ZeePhi_[maxP];
  Int_t    ZeeLeg1Index_[maxP];
  Int_t    ZeeLeg2Index_[maxP];

  // Zmumu candidate
  Int_t    nZmumu_;
  Float_t  ZmumuMass_[maxP];
  Float_t  ZmumuPt_[maxP];
  Float_t  ZmumuEta_[maxP];
  Float_t  ZmumuPhi_[maxP];
  Int_t    ZmumuLeg1Index_[maxP];
  Int_t    ZmumuLeg2Index_[maxP];

  // Wenu candidate
  Int_t    nWenu_;
  Float_t  WenuMassTCaloMET_[maxP];
  Float_t  WenuEtCaloMET_[maxP];
  Float_t  WenuACopCaloMET_[maxP];
  Float_t  WenuMassTTcMET_[maxP];
  Float_t  WenuEtTcMET_[maxP];
  Float_t  WenuACopTcMET_[maxP];
  Float_t  WenuMassTPfMET_[maxP];
  Float_t  WenuEtPfMET_[maxP];
  Float_t  WenuACopPfMET_[maxP];
  Int_t    WenuEleIndex_[maxP];

  // Wmunu candidate
  Int_t    nWmunu_;
  Float_t  WmunuMassTCaloMET_[maxP];
  Float_t  WmunuEtCaloMET_[maxP];
  Float_t  WmunuACopCaloMET_[maxP];
  Float_t  WmunuMassTTcMET_[maxP];
  Float_t  WmunuEtTcMET_[maxP];
  Float_t  WmunuACopTcMET_[maxP];
  Float_t  WmunuMassTPfMET_[maxP];
  Float_t  WmunuEtPfMET_[maxP];
  Float_t  WmunuACopPfMET_[maxP];
  Int_t    WmunuMuIndex_[maxP];

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

};

#endif
