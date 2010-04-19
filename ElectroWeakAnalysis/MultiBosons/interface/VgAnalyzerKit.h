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

#include "TTree.h"

using namespace edm;

const int maxP = 500;

class VgAnalyzerKit : public EDProducer {

public:

  explicit VgAnalyzerKit(const ParameterSet&);
  virtual ~VgAnalyzerKit();
  
protected:

  virtual void beginJob(const EventSetup&) ;
  virtual void produce( Event &, const EventSetup & );
  virtual void endJob() ;

  bool saveHistograms_;
  bool saveHLTInfo_;
  bool doGenParticles_;

  InputTag trgResults_;
  InputTag ebReducedRecHitCollection_;
  InputTag eeReducedRecHitCollection_;

  int verbosity_;

  pat::PatKitHelper helper_;
  TTree *tree_;

  int    run_;
  int    event_;
  bool   isData;
  double pdf_[7];
  double processID_;
  int    HLT_[102];
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
  // Electron
  int    nEle_;
  bool   eleID[maxP][5];
  int    eleClass[maxP];
  int    eleCharge[maxP];
  double eleEn[maxP];
  double elePt[maxP];
  double elePz[maxP];
  double eleEta[maxP];
  double elePhi[maxP];
  double eleSCEta[maxP];
  double eleSCPhi[maxP];
  double eleSCRawEn[maxP];
  double eleE3x3[maxP];
  double eleHoverE[maxP];
  double eleEoverP[maxP];
  double elePin[maxP];
  double elePout[maxP];
  double eleBrem[maxP];
  double eledEtaAtVtx[maxP];
  double eledPhiAtVtx[maxP];
  double eleSigmaEtaEta[maxP];
  double eleSigmaIEtaIEta[maxP];
  int    eleGenIndex[maxP];
  int    eleGenGMomPID[maxP];
  int    eleGenMomPID[maxP];
  double eleGenMomPt[maxP];
  double eleIsoTrkDR03[maxP];
  double eleIsoEcalDR03[maxP];
  double eleIsoHcalDR03[maxP];
  double eleIsoTrkDR04[maxP];
  double eleIsoEcalDR04[maxP];
  double eleIsoHcalDR04[maxP];
  double eleChi2NDF[maxP];
  double eleD0_[maxP];
  int    eleNumberOfValidHits_[maxP];
  // Photon
  int nPho_;
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
  double phoSigmaEtaEta_[maxP];
  double phoSigmaIEtaIEta_[maxP];
  int    phoPos_[maxP];
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
  Handle<std::vector<pat::Muon> >                  muonHandle_;
  Handle<std::vector<pat::Electron> >              electronHandle_;
  Handle<std::vector<pat::Tau> >                   tauHandle_;
  Handle<std::vector<pat::Jet> >                   jetHandle_;
  Handle<std::vector<pat::MET> >                   METHandle_;
  Handle<std::vector<pat::Photon> >                photonHandle_;
  Handle<std::vector<reco::RecoChargedCandidate> > trackHandle_;
  Handle<std::vector<reco::GenParticle> >          genParticlesHandle_;

};

#endif
