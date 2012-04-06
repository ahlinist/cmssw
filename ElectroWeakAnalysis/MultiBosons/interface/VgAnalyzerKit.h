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

const Int_t maxP = 500;

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
  Float_t E2overE9(const DetId id, const EcalRecHitCollection &, Float_t recHitEtThreshold = 10.0 , 
		 Float_t recHitEtThreshold2 = 1.0 , Bool_t avoidIeta85=false, Bool_t KillSecondHit=true);
  Float_t recHitApproxEt( const DetId id, const EcalRecHitCollection &recHits );
  Float_t recHitE( const DetId id, const EcalRecHitCollection &recHits );
  Float_t recHitE( const DetId id, const EcalRecHitCollection & recHits, Int_t di, Int_t dj );
  Float_t getGenIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, 
		     const Float_t dRMax=0.4, Bool_t removeMu=true, Bool_t removeNu=false);
  Float_t getGenCalIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, 
		     const Float_t dRMax=0.4, Bool_t removeMu=true, Bool_t removeNu=false);
  Float_t getGenTrkIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax=0.4);
  Float_t getESRatio(View<pat::Electron>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  std::vector<float> getESProfileFront(View<pat::Electron>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  std::vector<float> getESProfileRear(View<pat::Electron>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  Float_t getESRatio(View<pat::Photon>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  std::vector<float> getESProfileFront(View<pat::Photon>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  std::vector<float> getESProfileRear(View<pat::Photon>::const_iterator photon, const edm::Event&, const edm::EventSetup&);
  Float_t getTrkIso(edm::Handle<reco::TrackCollection> Tracks, math::XYZPoint vtx, Double_t phoEta, Double_t phoPhi, Double_t phoVz, Double_t outerCone, Double_t innerCone, Double_t etaSlice, Double_t dzCut, Double_t d0Cut, Double_t ptCut, Int_t option);

  Bool_t saveHistograms_;
  Bool_t doGenParticles_;
  Bool_t doStoreJets_;
  Bool_t doJetHLTMatch_;
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
  InputTag SmearedpfMETlabel_;
  InputTag SmearedTypeIpfMETlabel_;
  InputTag PFCandLabel_;
  InputTag puInfoLabel_;
  InputTag rhoLabel_;
  InputTag sigmaLabel_;
  InputTag rhoNeutralLabel_;
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
  Float_t  rho_;
  Float_t  sigma_;
  Float_t  rhoNeutral_;
  Int_t    nHLT_;
  Int_t    HLT_[maxP];
  Int_t    HLTprescale_[maxP];
  Int_t    HLTIndex_[250];
  Int_t    nHFTowersP_;
  Int_t    nHFTowersN_;
  Int_t    nVtx_;
  Float_t  vtx_[150][3];
  Int_t    vtxNTrk_[150];
  Float_t  vtxNDF_[150];
  Float_t  vtxD0_[150];
  Int_t    nGoodVtx_;
  Int_t    IsVtxGood_;
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
  Float_t  mcCalIsoDR03[maxP];
  Float_t  mcTrkIsoDR03[maxP];
  Float_t  mcIsoDR04[maxP];
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
  // TypeI pfMET
  Float_t  TypeIpfMET_;
  Float_t  TypeIpfMETx_;
  Float_t  TypeIpfMETy_;
  Float_t  TypeIpfMETPhi_;
  Float_t  TypeIpfMETsumEt_;
  Float_t  TypeIpfMETmEtSig_;
  Float_t  TypeIpfMETSig_;
  // TypeI + TypeII pfMET
  Float_t  TypeIpIIpfMET_;
  Float_t  TypeIpIIpfMETx_;
  Float_t  TypeIpIIpfMETy_;
  Float_t  TypeIpIIpfMETPhi_;
  Float_t  TypeIpIIpfMETsumEt_;
  Float_t  TypeIpIIpfMETmEtSig_;
  Float_t  TypeIpIIpfMETSig_;
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
  // pfCharged and pf Neutral Sum Pt
  Int_t    npfCharged_;
  Float_t  pfChargedSumPt_;
  Int_t    npfChargedHadron_;
  Float_t  pfChargedHadronSumPt_;
  Int_t    npfLepton_;
  Float_t  pfLeptonSumPt_;
  Int_t    npfNeutral_;
  Float_t  pfNeutralSumPt_;
  Int_t    npfNeutralHadron_;
  Float_t  pfNeutralHadronSumPt_;
  Int_t    npfPhoton_;
  Float_t  pfPhotonSumPt_;
  // Electron
  Int_t    nEle_;
  Int_t    eleTrg_[maxP][31];
  Int_t    eleID_[maxP][30];
  Float_t  eleIDLH_[maxP];
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
  Float_t  elenBrem_[maxP];
  Float_t  eledEtaAtVtx_[maxP];
  Float_t  eledPhiAtVtx_[maxP];
  Float_t  eleSigmaEtaEta_[maxP];
  Float_t  eleSigmaIEtaIEta_[maxP];
  Float_t  eleSigmaIEtaIPhi_[maxP];
  Float_t  eleSigmaIPhiIPhi_[maxP];
  Float_t  eleE2overE9_[maxP];
  Float_t  eleE3x3_[maxP];
  Float_t  eleSeedTime_[maxP];
  Float_t  eleSeedEnergy_[maxP];
  Int_t    eleRecoFlag_[maxP];
  Int_t    eleSeverity_[maxP];
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
  Float_t  eleChi2NDF_[maxP];
  Float_t  eleD0_[maxP];
  Int_t    eleNumberOfValidHits_[maxP];
  Int_t    eleValidHitInFirstPXB_[maxP];
  Int_t    eleTrkExpectHitsInner_[maxP];
  Float_t  eleConvDist_[maxP];
  Float_t  eleConvDcot_[maxP];
  Float_t  eleConvRadius_[maxP];
  Int_t    eleConvFlag_[maxP];
  Int_t    eleConvMissinghit_[maxP];
  Float_t  eleConvPoint_[maxP][3];
  Float_t  eleConversionveto_[maxP];
  Float_t  eleESRatio_[maxP];
  Float_t  eleESProfileFront_[maxP][123];
  Float_t  eleESProfileRear_[maxP][123];
  Float_t  elePV2D_[maxP];
  Float_t  elePV3D_[maxP];
  Float_t  eleBS2D_[maxP];
  Float_t  eleBS3D_[maxP];
  Float_t  elePVD0_[maxP];
  Float_t  elePVDz_[maxP];
  // Photon
  Int_t    nPho_;
  Int_t    phoTrg_[maxP][14];
  Bool_t   phoIsPhoton_[maxP];
  Bool_t   phoElectronveto_[maxP];
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
  Float_t  phoHcalIsoSolidDR03_[maxP];
  Float_t  phoTrkIsoSolidDR04_[maxP];
  Float_t  phoTrkIsoHollowDR04_[maxP];
  Int_t    phoNTrkSolidDR04_[maxP];
  Int_t    phoNTrkHollowDR04_[maxP];
  Float_t  phoEcalIsoDR04_[maxP];
  Float_t  phoHcalIsoDR04_[maxP];
  Float_t  phoHcalIsoSolidDR04_[maxP];
  Float_t  phoEtVtx_[maxP][150];
  Float_t  phoEtaVtx_[maxP][150];
  Float_t  phoPhiVtx_[maxP][150];
  Float_t  phoTrkIsoSolidDR03Vtx_[maxP][150];
  Float_t  phoTrkIsoHollowDR03Vtx_[maxP][150];
  Float_t  phoTrkIsoSolidDR04Vtx_[maxP][150];
  Float_t  phoTrkIsoHollowDR04Vtx_[maxP][150];
  Float_t  phoHoverE_[maxP];
  Float_t  phoSigmaEtaEta_[maxP];
  Float_t  phoSigmaIEtaIEta_[maxP];
  Float_t  phoSigmaIEtaIPhi_[maxP];
  Float_t  phoSigmaIPhiIPhi_[maxP];
  Float_t  phoE2overE9_[maxP];
  Float_t  phoE3x3_[maxP];
  Float_t  phoE5x5_[maxP];
  Float_t  phoSeedTime_[maxP];
  Float_t  phoSeedEnergy_[maxP];
  Int_t    phoRecoFlag_[maxP];
  Int_t    phoSeverity_[maxP];
  Int_t    phoPos_[maxP];
  Int_t    phoGenIndex_[maxP];
  Int_t    phoGenGMomPID[maxP];
  Int_t    phoGenMomPID[maxP];
  Float_t  phoGenMomPt[maxP];
  Float_t  phoSCE_[maxP];
  Float_t  phoESE_[maxP];
  Float_t  phoSCEt_[maxP];
  Float_t  phoSCEta_[maxP];
  Float_t  phoSCPhi_[maxP];
  Float_t  phoSCEtaWidth_[maxP];
  Float_t  phoSCPhiWidth_[maxP];
  Float_t  phoVtx_[maxP][3];
  Float_t  phoVtxD0_[maxP];
  Int_t    phoOverlap_[maxP];
  Int_t    phohasPixelSeed_[maxP];
  Int_t    phoIsConv_[maxP];
  Float_t  phoESRatio_[maxP];
  Float_t  phoESProfileFront_[maxP][123];
  Float_t  phoESProfileRear_[maxP][123];
  Int_t    phoNTracks_[maxP];
  Float_t  phoConvPairInvariantMass_[maxP];
  Float_t  phoConvPairCotThetaSeparation_[maxP];
  Float_t  phoConvPairMomentumEta_[maxP];
  Float_t  phoConvPairMomentumPhi_[maxP];
  Float_t  phoConvPairMomentumX_[maxP];
  Float_t  phoConvPairMomentumY_[maxP];
  Float_t  phoConvPairMomentumZ_[maxP];
  Float_t  phoConvDistOfMinimumApproach_[maxP];
  Float_t  phoConvDPhiTracksAtVtx_[maxP];
  Float_t  phoConvDPhiTracksAtEcal_[maxP];
  Float_t  phoConvDEtaTracksAtEcal_[maxP];
  Float_t  phoConvVtxValid_[maxP];
  Float_t  phoConvVtxEta_[maxP];
  Float_t  phoConvVtxPhi_[maxP];
  Float_t  phoConvVtxR_[maxP];
  Float_t  phoConvVtxX_[maxP];
  Float_t  phoConvVtxY_[maxP];
  Float_t  phoConvVtxZ_[maxP];
  Float_t  phoConvVtxChi2_[maxP];
  Float_t  phoConvVtxNdof_[maxP];
  Float_t  phoConvChi2Prob_[maxP];
  Float_t  phoConvEoverP_[maxP];
  Int_t    phoNxtal_[maxP];
  Float_t  phoXtalTime_[maxP][200];
  Float_t  phoXtalEnergy_[maxP][200];
  Int_t    phoXtalZ_[maxP][200];
  Int_t    phoXtalX_[maxP][200];
  Int_t    phoXtalY_[maxP][200];
  Int_t    phoXtalEta_[maxP][200];
  Int_t    phoXtalPhi_[maxP][200];
  Float_t  pho5x5Time_[maxP][25];
  Float_t  pho5x5Energy_[maxP][25];
  Int_t    pho5x5Z_[maxP][25];
  Int_t    pho5x5X_[maxP][25];
  Int_t    pho5x5Y_[maxP][25];
  Int_t    pho5x5Eta_[maxP][25];
  Int_t    pho5x5Phi_[maxP][25];

  // Muon
  Int_t    nMu_;
  Int_t    muTrg_[maxP][16];
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
  Float_t  muEmVeto_[maxP];
  Float_t  muHadVeto_[maxP];
  Int_t    muType_[maxP];
  Bool_t   muID_[maxP][6];
  Float_t  muD0_[maxP];
  Float_t  muDz_[maxP];
  Float_t  muPVD0_[maxP];
  Float_t  muPVDz_[maxP];
  Float_t  muValidFraction_[maxP];
  Float_t  muTrkdPt_[maxP];
  Int_t    muNumberOfHits_[maxP];
  Int_t    muNumberOfValidHits_[maxP];
  Int_t    muNumberOfInactiveHits_[maxP];
  Int_t    muNumberOfValidTrkHits_[maxP];
  Int_t    muNumberOfValidPixelHits_[maxP];
  Int_t    muNumberOfValidMuonHits_[maxP];
  Int_t    muStations_[maxP];
  Int_t    muChambers_[maxP];
  Float_t  muPV2D_[maxP];
  Float_t  muPV3D_[maxP];
  Float_t  muBS2D_[maxP];
  Float_t  muBS3D_[maxP];
  Float_t  muVtx_[maxP][3];

  // Jet
  Int_t    nJet_;
  Int_t    jetTrg_[maxP][23];
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

  HLTConfigProvider hltConfig_;
};

#endif
