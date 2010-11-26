#ifndef VgKit_test_VgAnalyzerKit_h
#define VgKit_test_VgAnalyzerKit_h

#include <memory>
#include <fstream>

#include "ElectroWeakAnalysis/MultiBosons/Int_terface/PatKitHelper.h"

#include "FWCore/Framework/Int_terface/Frameworkfwd.h"
#include "FWCore/Framework/Int_terface/EDProducer.h"
#include "FWCore/Framework/Int_terface/Event.h"
#include "FWCore/Framework/Int_terface/MakerMacros.h"
#include "FWCore/ParameterSet/Int_terface/ParameterSet.h"
#include "FWCore/ServiceRegistry/Int_terface/Service.h"
#include "CommonTools/UtilAlgos/Int_terface/TFileService.h"
#include "CommonTools/Utils/Int_terface/TFileDirectory.h"
#include "DataFormats/Candidate/Int_terface/Candidate.h"
#include "DataFormats/Common/Int_terface/View.h"
#include "DataFormats/DetId/Int_terface/DetId.h"
#include "DataFormats/EcalRecHit/Int_terface/EcalRecHitCollections.h"

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
  double eT(double pt1, double pt2) const;
  double massT(double pt1, double pt2, double wpx, double wpy) const;
  double acop(double phi1, double phi2) const;
  float E2overE9(const DetId id, const EcalRecHitCollection &, float recHitEtThreshold = 10.0 ,
		 float recHitEtThreshold2 = 1.0 , bool avoidIeta85=false, bool KillSecondHit=true);
  float recHitApproxEt( const DetId id, const EcalRecHitCollection &recHits );
  float recHitE( const DetId id, const EcalRecHitCollection &recHits );
  float recHitE( const DetId id, const EcalRecHitCollection & recHits, Int_t di, Int_t dj );
  float getGenCalIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho,
		     const Float_t dRMax=0.4, bool removeMu=true, bool removeNu=false);
  float getGenTrkIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax=0.4);

  bool saveHistograms_;
  bool saveHLTInfo_;
  bool doGenParticles_;
  bool doStoreJets_;

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

  Int_t  run_;
  Int_t  event_;
  Int_t  orbit_;
  Int_t  bx_;
  Int_t  lumis_;
  bool   isData_;
  Int_t  ttbit0_;
  float  pdf_[7];
  float  pthat_;
  float  processID_;
  Int_t  nHLT_;
  Int_t  HLT_[maxP];
  Int_t  HLTIndex_[100];
  Int_t  nHFTowersP_;
  Int_t  nHFTowersN_;
  Int_t  nVtx_;
  float  vtx_[50][3];
  Int_t  vtxNTrk_[50];
  Int_t  vtxNDF_[50];
  float  vtxD0_[50];
  Int_t  IsVtxGood_;
  Int_t  nTrk_;
  Int_t  nGoodTrk_;
  Int_t  IsTracksGood_;
  // genParticle
  Int_t  nMC_;
  Int_t  mcPID[maxP];
  float  mcPt[maxP];
  float  mcMass[maxP];
  float  mcEta[maxP];
  float  mcPhi[maxP];
  Int_t  mcGMomPID[maxP];
  Int_t  mcMomPID[maxP];
  float  mcMomPt[maxP];
  float  mcMomMass[maxP];
  float  mcMomEta[maxP];
  float  mcMomPhi[maxP];
  Int_t  mcIndex[maxP];
  Int_t  mcDecayType[maxP];
  float  mcCalIsoDR03[maxP];
  float  mcTrkIsoDR03[maxP];
  float  mcCalIsoDR04[maxP];
  float  mcTrkIsoDR04[maxP];
  // Gen & Reco MET
  float  genMET_;
  float  genMETx_;
  float  genMETy_;
  float  genMETPhi_;
  float  MET_;
  float  METx_;
  float  METy_;
  float  METPhi_;
  float  METsumEt_;
  float  uncorrMET_[3];
  float  uncorrMETPhi_[3];
  float  uncorrMETSumEt_[3];
  // tcMET
  float  tcMET_;
  float  tcMETx_;
  float  tcMETy_;
  float  tcMETPhi_;
  float  tcMETsumEt_;
  float  tcMETmEtSig_;
  float  tcMETSig_;
  // pfMET
  float  pfMET_;
  float  pfMETx_;
  float  pfMETy_;
  float  pfMETPhi_;
  float  pfMETsumEt_;
  float  pfMETmEtSig_;
  float  pfMETSig_;
  // Electron
  Int_t  nEle_;
  Int_t  eleTrg_[maxP][50];
  Int_t  eleID_[maxP][12];
  Int_t  eleClass_[maxP];
  Int_t  eleCharge_[maxP];
  float  eleEn_[maxP];
  float  eleSCEn_[maxP];
  float  eleESEn_[maxP];
  float  eleSCEtaWidth_[maxP];
  float  eleSCPhiWidth_[maxP];
  float  eleVtx_[maxP][3];
  float  eleCaloPos_[maxP][3];
  float  eleSCPos_[maxP][3];
  float  elePt_[maxP];
  float  elePz_[maxP];
  float  eleEta_[maxP];
  float  elePhi_[maxP];
  float  eleSCEta_[maxP];
  float  eleSCPhi_[maxP];
  float  eleSCRawEn_[maxP];
  float  eleHoverE_[maxP];
  float  eleEoverP_[maxP];
  float  elePin_[maxP];
  float  elePout_[maxP];
  float  eleBrem_[maxP];
  float  eledEtaAtVtx_[maxP];
  float  eledPhiAtVtx_[maxP];
  float  eleSigmaEtaEta_[maxP];
  float  eleSigmaIEtaIEta_[maxP];
  float  eleE2overE9_[maxP];
  float  eleE3x3_[maxP];
  float  eleSeedTime_[maxP];
  Int_t  eleRecoFlag_[maxP];
  Int_t  eleSeverity_[maxP];
  Int_t  eleGenIndex_[maxP];
  Int_t  eleGenGMomPID_[maxP];
  Int_t  eleGenMomPID_[maxP];
  float  eleGenMomPt_[maxP];
  float  eleIsoTrkDR03_[maxP];
  float  eleIsoEcalDR03_[maxP];
  float  eleIsoHcalDR03_[maxP];
  float  eleIsoTrkDR04_[maxP];
  float  eleIsoEcalDR04_[maxP];
  float  eleIsoHcalDR04_[maxP];
  float  eleChi2NDF_[maxP];
  float  eleD0_[maxP];
  Int_t  eleNumberOfValidHits_[maxP];
  Int_t  eleValidHitInFirstPXB_[maxP];
  Int_t  eleTrkExpectHitsInner_[maxP];
  float  eleDist_[maxP];
  float  eleDcot_[maxP];
  float  eleConvRadius_[maxP];
  float  eleConvPoInt_t_[maxP][3];

  // Photon
  Int_t  nPho_;
  Int_t  phoTrg_[maxP][50];
  bool   phoIsPhoton_[maxP];
  float  phoE_[maxP];
  float  phoEt_[maxP];
  float  phoPz_[maxP];
  float  phoEta_[maxP];
  float  phoPhi_[maxP];
  float  phoR9_[maxP];
  float  phoTrkIsoSolidDR03_[maxP];
  float  phoTrkIsoHollowDR03_[maxP];
  Int_t  phoNTrkSolidDR03_[maxP];
  Int_t  phoNTrkHollowDR03_[maxP];
  float  phoEcalIsoDR03_[maxP];
  float  phoHcalIsoDR03_[maxP];
  float  phoTrkIsoSolidDR04_[maxP];
  float  phoTrkIsoHollowDR04_[maxP];
  Int_t  phoNTrkSolidDR04_[maxP];
  Int_t  phoNTrkHollowDR04_[maxP];
  float  phoEcalIsoDR04_[maxP];
  float  phoHcalIsoDR04_[maxP];
  float  phoHoverE_[maxP];
  float  phoSigmaEtaEta_[maxP];
  float  phoSigmaIEtaIEta_[maxP];
  float  phoE2overE9_[maxP];
  float  phoE3x3_[maxP];
  float  phoSeedTime_[maxP];
  Int_t  phoRecoFlag_[maxP];
  Int_t  phoSeverity_[maxP];
  Int_t  phoPos_[maxP];
  float  phoRoundness_[maxP];
  float  phoAngle_[maxP];
  Int_t  phoGenIndex_[maxP];
  Int_t  phoGenGMomPID[maxP];
  Int_t  phoGenMomPID[maxP];
  float  phoGenMomPt[maxP];
  float  phoSCE_[maxP];
  float  phoSCEt_[maxP];
  float  phoSCEta_[maxP];
  float  phoSCPhi_[maxP];
  float  phoSCEtaWidth_[maxP];
  float  phoSCPhiWidth_[maxP];
  Int_t  phoOverlap_[maxP];
  Int_t  phohasPixelSeed_[maxP];
  Int_t  phoIsConv_[maxP];
  float  phoPi0Disc_[maxP];

  // Muon
  Int_t  nMu_;
  Int_t  muTrg_[maxP][50];
  float  muEta_[maxP];
  float  muPhi_[maxP];
  Int_t  muCharge_[maxP];
  float  muPt_[maxP];
  float  muPz_[maxP];
  Int_t  muGenIndex_[maxP];
  float  muIsoTrk_[maxP];
  float  muIsoCalo_[maxP];
  float  muIsoEcal_[maxP];
  float  muIsoHcal_[maxP];
  float  muChi2NDF_[maxP];
  float  muEmVeto_[maxP];
  float  muHadVeto_[maxP];
  Int_t  muType_[maxP];
  bool   muID_[maxP][6];
  float  muD0_[maxP];
  float  muDz_[maxP];
  Int_t  muNumberOfValidTrkHits_[maxP];
  Int_t  muNumberOfValidPixelHits_[maxP];
  Int_t  muNumberOfValidMuonHits_[maxP];
  Int_t  muStations_[maxP];
  Int_t  muChambers_[maxP];

  // Jet
  Int_t  nJet_;
  Int_t  jetTrg_[maxP][50];
  Int_t  jetAlgo_[maxP];
  float  jetEn_[maxP];
  float  jetPt_[maxP];
  float  jetEta_[maxP];
  float  jetPhi_[maxP];
  float  jetMass_[maxP];
  float  jetEt_[maxP];
  float  jetmaxEInEmTowers_[maxP];
  float  jetmaxEInHadTowers_[maxP];
  float  jetenergyFractionHadronic_[maxP];
  float  jetemEnergyFraction_[maxP];
  float	 jetfHPD_[maxP];
  Int_t	 jetN60_[maxP] ;
  Int_t	 jetN90_[maxP] ;
  Int_t  jetGenIndex_[maxP];
  Int_t  jetGenJetIndex_[maxP];
  float  jetGenJetEn_[maxP];
  float  jetGenJetPt_[maxP];
  float  jetGenJetEta_[maxP];
  float  jetGenJetPhi_[maxP];
  float  jetGenJetMass_[maxP];
  Int_t  jetGenPartonID_[maxP];
  Int_t  jetGenPartonMomID_[maxP];
  Int_t  jetpartonFlavour_[maxP];
  float  jetRawPt_[maxP];
  float  jetRawEn_[maxP];
  float  jetCharge_[maxP];

  // Zee candidate
  Int_t  nZee_;
  float  ZeeMass_[maxP];
  float  ZeePt_[maxP];
  float  ZeeEta_[maxP];
  float  ZeePhi_[maxP];
  Int_t  ZeeLeg1Index_[maxP];
  Int_t  ZeeLeg2Index_[maxP];

  // Zmumu candidate
  Int_t  nZmumu_;
  float  ZmumuMass_[maxP];
  float  ZmumuPt_[maxP];
  float  ZmumuEta_[maxP];
  float  ZmumuPhi_[maxP];
  Int_t  ZmumuLeg1Index_[maxP];
  Int_t  ZmumuLeg2Index_[maxP];

  // Wenu candidate
  Int_t  nWenu_;
  float  WenuMassTCaloMET_[maxP];
  float  WenuEtCaloMET_[maxP];
  float  WenuACopCaloMET_[maxP];
  float  WenuMassTTcMET_[maxP];
  float  WenuEtTcMET_[maxP];
  float  WenuACopTcMET_[maxP];
  float  WenuMassTPfMET_[maxP];
  float  WenuEtPfMET_[maxP];
  float  WenuACopPfMET_[maxP];
  Int_t  WenuEleIndex_[maxP];

  // Wmunu candidate
  Int_t  nWmunu_;
  float  WmunuMassTCaloMET_[maxP];
  float  WmunuEtCaloMET_[maxP];
  float  WmunuACopCaloMET_[maxP];
  float  WmunuMassTTcMET_[maxP];
  float  WmunuEtTcMET_[maxP];
  float  WmunuACopTcMET_[maxP];
  float  WmunuMassTPfMET_[maxP];
  float  WmunuEtPfMET_[maxP];
  float  WmunuACopPfMET_[maxP];
  Int_t  WmunuMuIndex_[maxP];

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
