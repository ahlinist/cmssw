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

#include "DataFormats/Common/interface/View.h"

#include "TTree.h"

using namespace edm;

const int maxP = 500;

class VgAnalyzerKit : public EDProducer {

public:

  explicit VgAnalyzerKit(const ParameterSet&);
  virtual ~VgAnalyzerKit();
  
protected:

  virtual void beginJob();
  virtual void produce( Event &, const EventSetup & );
  virtual void endJob();

  bool saveHistograms_;
  bool saveHLTInfo_;
  bool doGenParticles_;

  InputTag vtxlabel_;
  InputTag gtdigilabel_;
  InputTag tracklabel_;
  InputTag trgResults_;
  InputTag caloTowerlabel_;
  InputTag tcMETlabel_;
  InputTag pfMETlabel_;
  InputTag ebReducedRecHitCollection_;
  InputTag eeReducedRecHitCollection_;

  int verbosity_;

  pat::PatKitHelper helper_;
  TTree *tree_;

  int    run_;
  int    event_;
  int    orbit_;
  int    bx_;
  int    lumis_;
  bool   isData_;
  int    ttbit_[64];
  double pdf_[7];
  double processID_;
  int    HLT_[102];
  int    nHFTowersP_;
  int    nHFTowersN_;
  int    nVtx_;
  double vtx_[50][3];
  int    vtxNTrk_[50];
  int    vtxNDF_[50];
  double vtxD0_[50];
  int    IsVtxGood_;
  int    nTrk_;
  int    nGoodTrk_;
  int    IsTracksGood_;
  // genParticle
  int    nMC_;
  int    mcPID[maxP];
  double mcPt[maxP];
  double mcMass[maxP];
  double mcEta[maxP];
  double mcPhi[maxP];
  int    mcGMomPID[maxP];
  int    mcMomPID[maxP];
  double mcMomPt[maxP];
  double mcMomMass[maxP];
  double mcMomEta[maxP];
  double mcMomPhi[maxP];
  int    mcIndex[maxP];
  int    mcDecayType[maxP];
  // Gen & Reco MET
  double genMET_;
  double genMETx_;
  double genMETy_;
  double genMETPhi_;
  double MET_;
  double METx_;
  double METy_;
  double METPhi_;
  double METsumEt_;
  double uncorrMET_[3];
  double uncorrMETPhi_[3];
  double uncorrMETSumEt_[3];
  // tcMET
  double tcMET_;
  double tcMETx_;
  double tcMETy_;
  double tcMETPhi_;
  double tcMETsumEt_;
  double tcMETmEtSig_;
  double tcMETSig_;
  // pfMET
  double pfMET_;
  double pfMETx_;
  double pfMETy_;
  double pfMETPhi_;
  double pfMETsumEt_;
  double pfMETmEtSig_;
  double pfMETSig_;
  // Electron
  int    nEle_;
  bool   eleID_[maxP][5];
  int    eleClass_[maxP];
  int    eleCharge_[maxP];
  double eleEn_[maxP];
  double eleSCEn_[maxP];
  double eleESEn_[maxP];
  double eleSCEtaWidth_[maxP];
  double eleSCPhiWidth_[maxP];
  double eleVtx_[maxP][3];
  double eleCaloPos_[maxP][3];
  double eleSCPos_[maxP][3];
  double elePt_[maxP];
  double elePz_[maxP];
  double eleEta_[maxP];
  double elePhi_[maxP];
  double eleSCEta_[maxP];
  double eleSCPhi_[maxP];
  double eleSCRawEn_[maxP];
  double eleHoverE_[maxP];
  double eleHoverE1_[maxP];
  double eleHoverE2_[maxP];
  double eleEoverP_[maxP];
  double elePin_[maxP];
  double elePout_[maxP];
  double eleBrem_[maxP];
  double eledEtaAtVtx_[maxP];
  double eledPhiAtVtx_[maxP];
  double eleSigmaEtaEta_[maxP];
  double eleSigmaIEtaIEta_[maxP];
  double eleEMax_[maxP];
  double eleE2nd_[maxP];
  double eleE2x2_[maxP];
  double eleE3x2_[maxP];
  double eleE3x3_[maxP];
  double eleE4x4_[maxP];
  double eleE5x5_[maxP];
  double eleE2x5Right_[maxP];
  double eleE2x5Left_[maxP];
  double eleE2x5Top_[maxP];
  double eleE2x5Bottom_[maxP];
  double eleERight_[maxP];
  double eleELeft_[maxP];
  double eleETop_[maxP];
  double eleEBottom_[maxP];
  int    eleGenIndex_[maxP];
  int    eleGenGMomPID_[maxP];
  int    eleGenMomPID_[maxP];
  double eleGenMomPt_[maxP];
  double eleIsoTrkDR03_[maxP];
  double eleIsoEcalDR03_[maxP];
  double eleIsoHcalDR03_[maxP];
  double eleIsoTrkDR04_[maxP];
  double eleIsoEcalDR04_[maxP];
  double eleIsoHcalDR04_[maxP];
  double eleChi2NDF_[maxP];
  double eleD0_[maxP];
  int    eleNumberOfValidHits_[maxP];
  // Photon
  int    nPho_;
  bool   phoIsPhoton_[maxP];
  double phoE_[maxP];
  double phoEt_[maxP];
  double phoPz_[maxP];
  double phoEta_[maxP];
  double phoPhi_[maxP];
  double phoR9_[maxP];
  double phoTrkIsoSolidDR03_[maxP];
  double phoTrkIsoHollowDR03_[maxP];
  int    phoNTrkSolidDR03_[maxP];
  int    phoNTrkHollowDR03_[maxP];
  double phoEcalIsoDR03_[maxP];
  double phoHcalIsoDR03_[maxP];
  double phoTrkIsoSolidDR04_[maxP];
  double phoTrkIsoHollowDR04_[maxP];
  int    phoNTrkSolidDR04_[maxP];
  int    phoNTrkHollowDR04_[maxP];
  double phoEcalIsoDR04_[maxP];
  double phoHcalIsoDR04_[maxP];
  double phoHoverE_[maxP];
  double phoHoverE1_[maxP];
  double phoHoverE2_[maxP];
  double phoSeedTime_[maxP];
  double phoSigmaEtaEta_[maxP];
  double phoSigmaIEtaIEta_[maxP];
  int    phoPos_[maxP];
  double phoEMax_[maxP];
  double phoE2nd_[maxP];
  double phoE2x2_[maxP];
  double phoE3x2_[maxP];
  double phoE3x3_[maxP];
  double phoE4x4_[maxP];
  double phoE5x5_[maxP];
  double phoE2x5Right_[maxP];
  double phoE2x5Left_[maxP];
  double phoE2x5Top_[maxP];
  double phoE2x5Bottom_[maxP];
  double phoERight_[maxP];
  double phoELeft_[maxP];
  double phoETop_[maxP];
  double phoEBottom_[maxP];
  int    phoGenIndex_[maxP];
  int    phoGenGMomPID[maxP];
  int    phoGenMomPID[maxP];
  double phoGenMomPt[maxP];
  double phoSCE_[maxP];
  double phoSCEt_[maxP];
  double phoSCEta_[maxP];
  double phoSCPhi_[maxP];
  double phoSCEtaWidth_[maxP];
  double phoSCPhiWidth_[maxP];
  int    phoOverlap_[maxP];
  // Muon
  int    nMu_;
  double muEta_[maxP];
  double muPhi_[maxP];
  int    muCharge_[maxP];
  double muPt_[maxP];
  double muPz_[maxP];
  int    muGenIndex_[maxP];
  double muIsoTrk_[maxP];
  double muIsoCalo_[maxP];
  double muIsoEcal_[maxP];
  double muIsoHcal_[maxP];
  double muEmVeto_[maxP];
  double muHadVeto_[maxP];
  int    muType_[maxP];
  bool   muID[maxP][6];
  double muD0_[maxP];
  int    muNumberOfValidHits_[maxP];
  // Jet
  int    nJet_;
  int    jetAlgo_[maxP];
  double jetEn_[maxP];
  double jetPt_[maxP];
  double jetEta_[maxP];
  double jetPhi_[maxP];
  double jetMass_[maxP];
  double jetEt_[maxP];
  double jetmaxEInEmTowers_[maxP];
  double jetmaxEInHadTowers_[maxP];
  double jetenergyFractionHadronic_[maxP];
  double jetemEnergyFraction_[maxP];
  int    jetGenIndex_[maxP];
  int    jetGenJetIndex_[maxP];
  double jetGenJetEn_[maxP];
  double jetGenJetPt_[maxP];
  double jetGenJetEta_[maxP];
  double jetGenJetPhi_[maxP];
  double jetGenJetMass_[maxP];
  int    jetGenPartonID_[maxP];
  int    jetGenPartonMomID_[maxP];
  int    jetpartonFlavour_[maxP];
  double jetRawPt_[maxP];
  double jetRawEn_[maxP];
  double jetCharge_[maxP];
  // b-tagging information
  double jetCombinedSVBJetTags_[maxP];
  double jetCombinedSVMVABJetTags_[maxP];
  double jetConeIsoTauJetTags_[maxP];
  double jetImpactParaMVABJetTags_[maxP];
  double jetJetBProbBJetTags_[maxP];
  double jetJetProbBJetTags_[maxP];
  double jetSimpleSVBJetTags_[maxP];
  double jetSoftElecBJetTags_[maxP];
  double jetSoftMuonBJetTags_[maxP];
  double jetSoftMuonNoIPBJetTags_[maxP];
  double jetTrackCountHiEffBJetTags_[maxP];
  double jetTrackCountHiPurBJetTags_[maxP];
  double jetJetLRval_[maxP];
  double jetJetProb_[maxP];
  // End of b-tagging information
  double jetGenEn_[maxP];
  double jetGenPt_[maxP];
  double jetGenEta_[maxP];
  double jetGenPhi_[maxP];

  // Physics objects handles
  Handle<View<pat::Muon> >                  muonHandle_;
  Handle<View<pat::Electron> >              electronHandle_;
  Handle<View<pat::Tau> >                   tauHandle_;
  Handle<View<pat::Jet> >                   jetHandle_;
  Handle<View<pat::MET> >                   METHandle_;
  Handle<View<pat::Photon> >                photonHandle_;
  Handle<std::vector<reco::RecoChargedCandidate> > trackHandle_;
  Handle<std::vector<reco::GenParticle> >          genParticlesHandle_;

};

#endif
