#include "ElectroWeakAnalysis/MultiBosons/interface/VgAnalyzerKit.h"

#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HepMCCandidate/interface/PdfInfo.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "PhysicsTools/CandUtils/interface/AddFourMomenta.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

#include "DataFormats/EgammaCandidates/interface/PhotonPi0DiscriminatorAssociation.h"

#include <iostream>

using namespace std;
using namespace pat;
using namespace edm;
using namespace reco;

VgAnalyzerKit::VgAnalyzerKit(const edm::ParameterSet& ps) : verbosity_(0), helper_(ps) {
  // cout << "VgAnalyzerKit: entering ctor ..." << endl;

  saveHistograms_ = ps.getUntrackedParameter<bool>("saveHistograms", false);
  saveHLTInfo_    = ps.getUntrackedParameter<bool>("saveHLTInfo", true);
  trgResults_     = ps.getParameter<InputTag>("triggerResults");
  doGenParticles_ = ps.getParameter<bool>("doGenParticles");
  gtdigilabel_    = ps.getParameter<InputTag>("GTDigiLabel");
  vtxlabel_       = ps.getParameter<InputTag>("VtxLabel");
  caloTowerlabel_ = ps.getParameter<InputTag>("CaloTowerLabel");
  tracklabel_     = ps.getParameter<InputTag>("TrackLabel");
  tcMETlabel_     = ps.getParameter<InputTag>("tcMETLabel");
  pfMETlabel_     = ps.getParameter<InputTag>("pfMETLabel");

  leadingElePtCut_ = ps.getParameter<double>("LeadingElePtCut");
  leadingMuPtCut_  = ps.getParameter<double>("LeadingMuPtCut");
  leadingPhoPtCut_ = ps.getParameter<double>("LeadingPhoPtCut");

  ebReducedRecHitCollection_ = ps.getParameter<InputTag>("ebReducedRecHitCollection");
  eeReducedRecHitCollection_ = ps.getParameter<InputTag>("eeReducedRecHitCollection");
  beamSpotCollection_        = ps.getParameter<InputTag>("BeamSpotCollection");

  if (saveHistograms_) helper_.bookHistos(this);

  // cout << "VgAnalyzerKit: making output tree" << endl;

  Service<TFileService> fs;
  hEvents_ = fs->make<TH1F>("hEvents", "total processed and skimmed events", 2, 0, 2);
  tree_ = fs->make<TTree>("EventTree", "Event data");

  tree_->Branch("run", &run_, "run/I");
  tree_->Branch("event", &event_, "event/I");
  tree_->Branch("orbit", &orbit_, "orbit/I");
  tree_->Branch("bx", &bx_, "bx/I");
  tree_->Branch("lumis", &lumis_, "lumis/I");
  tree_->Branch("isData", &isData_, "isData/O");
  tree_->Branch("ttbit0", &ttbit0_, "ttbit0/I");
  tree_->Branch("ttbit34", &ttbit34_, "ttbit34/I");
  tree_->Branch("ttbit40", &ttbit40_, "ttbit40/I");
  tree_->Branch("ttbit41", &ttbit41_, "ttbit41/I");
  tree_->Branch("ttbitBH", &ttbitBH_, "ttbitBH/I");
  tree_->Branch("nHLT", &nHLT_, "nHLT/I");
  tree_->Branch("HLT", HLT_, "HLT[nHLT]/I");
  tree_->Branch("nHFTowersP", &nHFTowersP_, "nHFTowersP/I");
  tree_->Branch("nHFTowersN", &nHFTowersN_, "nHFTowersN/I");
  tree_->Branch("nVtx", &nVtx_, "nVtx/I");
  tree_->Branch("vtx", vtx_, "vtx[nVtx][3]/F");
  tree_->Branch("vtxNTrk", vtxNTrk_, "vtxNTrk[nVtx]/I");
  tree_->Branch("vtxNDF", vtxNDF_, "vtxNDF[nVtx]/I");
  tree_->Branch("vtxD0", vtxD0_, "vtxD0[nVtx]/F");
  tree_->Branch("IsVtxGood", &IsVtxGood_, "IsVtxGood/I");
  tree_->Branch("nTrk", &nTrk_, "nTrk/I");
  tree_->Branch("nGoodTrk", &nGoodTrk_, "nGoodTrk/I");
  tree_->Branch("IsTracksGood", &IsTracksGood_, "IsTracksGood/I");
  if (doGenParticles_) {
    tree_->Branch("pdf", pdf_, "pdf[7]/F");
    tree_->Branch("pthat", &pthat_, "pthat/F");
    tree_->Branch("processID", &processID_, "processID/F");
    // genParticle
    tree_->Branch("nMC", &nMC_, "nMC/I");
    tree_->Branch("mcPID", mcPID, "mcPID[nMC]/I");
    tree_->Branch("mcPt", mcPt, "mcPt[nMC]/F");
    tree_->Branch("mcMass", mcMass, "mcMass[nMC]/F");
    tree_->Branch("mcEta", mcEta, "mcEta[nMC]/F");
    tree_->Branch("mcPhi", mcPhi, "mcPhi[nMC]/F");
    tree_->Branch("mcGMomPID", mcGMomPID, "mcGMomPID[nMC]/I");
    tree_->Branch("mcMomPID", mcMomPID, "mcMomPID[nMC]/I");
    tree_->Branch("mcMomPt", mcMomPt, "mcMomPt[nMC]/F");
    tree_->Branch("mcMomMass", mcMomMass, "mcMomMass[nMC]/F");
    tree_->Branch("mcMomEta", mcMomEta, "mcMomEta[nMC]/F");
    tree_->Branch("mcMomPhi", mcMomPhi, "mcMomPhi[nMC]/F");
    tree_->Branch("mcIndex", mcIndex, "mcIndex[nMC]/I");
    tree_->Branch("mcDecayType", mcDecayType, "mcDecayType[nMC]/I"); //-999:non W or Z, 1:hardronic, 2:e, 3:mu, 4:tau
    // Gen MET
    tree_->Branch("genMET", &genMET_, "genMET/F");
    tree_->Branch("genMETx", &genMETx_, "genMETx/F");
    tree_->Branch("genMETy", &genMETy_, "genMETy/F");
    tree_->Branch("genMETPhi", &genMETPhi_, "genMETPhi/F");
  }
  // Calo MET
  tree_->Branch("MET", &MET_, "MET/F");
  tree_->Branch("METx", &METx_, "METx/F");
  tree_->Branch("METy", &METy_, "METy/F");
  tree_->Branch("METPhi", &METPhi_, "METPhi/F");
  tree_->Branch("METsumEt", &METsumEt_, "METsumEt/F");
  tree_->Branch("uncorrMET", uncorrMET_, "uncorrMET[3]/F"); // [0]: uncorrALL, [1]: uncorrJES, [2]: uncorrMUON
  tree_->Branch("uncorrMETPhi", uncorrMETPhi_, "uncorrMETPhi[3]/F");
  tree_->Branch("uncorrMETSumEt", uncorrMETSumEt_, "uncorrMETSumEt[3]/F");
  // tcMET
  tree_->Branch("tcMET", &tcMET_, "tcMET/F");
  tree_->Branch("tcMETx", &tcMETx_, "tcMETx/F");
  tree_->Branch("tcMETy", &tcMETy_, "tcMETy/F");
  tree_->Branch("tcMETPhi", &tcMETPhi_, "tcMETPhi/F");
  tree_->Branch("tcMETsumEt", &tcMETsumEt_, "tcMETsumEt/F");
  tree_->Branch("tcMETmEtSig", &tcMETmEtSig_, "tcMETmEtSig/F");
  tree_->Branch("tcMETSig", &tcMETSig_, "tcMETSig/F");
  // pfMET
  tree_->Branch("pfMET", &pfMET_, "pfMET/F");
  tree_->Branch("pfMETx", &pfMETx_, "pfMETx/F");
  tree_->Branch("pfMETy", &pfMETy_, "pfMETy/F");
  tree_->Branch("pfMETPhi", &pfMETPhi_, "pfMETPhi/F");
  tree_->Branch("pfMETsumEt", &pfMETsumEt_, "pfMETsumEt/F");
  tree_->Branch("pfMETmEtSig", &pfMETmEtSig_, "pfMETmEtSig/F");
  tree_->Branch("pfMETSig", &pfMETSig_, "pfMETSig/F");
  // Electron
  tree_->Branch("nEle", &nEle_, "nEle/I");
  // ELECTRON ID
  tree_->Branch("eleID", eleID_, "eleID[nEle][12]/I");
  tree_->Branch("eleClass", eleClass_, "eleClass[nEle]/I");
  tree_->Branch("eleCharge", eleCharge_, "eleCharge[nEle]/I");
  tree_->Branch("eleEn", eleEn_, "eleEn[nEle]/F");
  tree_->Branch("eleSCRawEn", eleSCRawEn_, "eleSCRawEn[nEle]/F");
  tree_->Branch("eleESEn", eleESEn_, "eleESEn[nEle]/F");
  tree_->Branch("eleSCEn", eleSCEn_, "eleSCEn[nEle]/F");
  tree_->Branch("elePt", elePt_, "elePt[nEle]/F");
  tree_->Branch("elePz", elePz_, "elePz[nEle]/F");
  tree_->Branch("eleEta", eleEta_, "eleEta[nEle]/F");
  tree_->Branch("elePhi", elePhi_, "elePhi[nEle]/F");
  tree_->Branch("eleSCEta", eleSCEta_, "eleSCEta[nEle]/F");
  tree_->Branch("eleSCPhi", eleSCPhi_, "eleSCPhi[nEle]/F");
  tree_->Branch("eleSCEtaWidth", eleSCEtaWidth_, "eleSCEtaWidth[nEle]/F");
  tree_->Branch("eleSCPhiWidth", eleSCPhiWidth_, "eleSCPhiWidth[nEle]/F");
  tree_->Branch("eleVtx", eleVtx_, "eleVtx[nEle][3]/F");
  tree_->Branch("eleCaloPos", eleCaloPos_ ,"eleCaloPos[nEle][3]/F");
  tree_->Branch("eleSCPos", eleSCPos_, "eleSCPos[nEle][3]/F");
  tree_->Branch("eleHoverE", eleHoverE_, "eleHoverE[nEle]/F");
  tree_->Branch("eleEoverP", eleEoverP_, "eleEoverP[nEle]/F");
  tree_->Branch("elePin", elePin_, "elePin[nEle]/F");
  tree_->Branch("elePout", elePout_, "elePout[nEle]/F");
  tree_->Branch("eleBrem", eleBrem_, "eleBrem[nEle]/F");
  tree_->Branch("eledEtaAtVtx", eledEtaAtVtx_, "eledEtaAtVtx[nEle]/F");
  tree_->Branch("eledPhiAtVtx", eledPhiAtVtx_, "eledPhiAtVtx[nEle]/F");
  tree_->Branch("eleSigmaEtaEta", eleSigmaEtaEta_, "eleSigmaEtaEta[nEle]/F");
  tree_->Branch("eleSigmaIEtaIEta", eleSigmaIEtaIEta_, "eleSigmaIEtaIEta[nEle]/F");
  tree_->Branch("eleEMax", eleEMax_, "eleEMax[nEle]/F");
  tree_->Branch("eleE3x3", eleE3x3_, "eleE3x3[nEle]/F");
  tree_->Branch("eleERight", eleERight_, "eleERight[nEle]/F");
  tree_->Branch("eleELeft", eleELeft_, "eleELeft[nEle]/F");
  tree_->Branch("eleETop", eleETop_, "eleETop[nEle]/F");
  tree_->Branch("eleEBottom", eleEBottom_, "eleEBottom[nEle]/F");
  if (doGenParticles_) {
    tree_->Branch("eleGenIndex", eleGenIndex_, "eleGenIndex[nEle]/I");
    tree_->Branch("eleGenGMomPID", eleGenGMomPID_, "eleGenGMomPID[nEle]/I");
    tree_->Branch("eleGenMomPID", eleGenMomPID_, "eleGenMomPID[nEle]/I");
    tree_->Branch("eleGenMomPt", eleGenMomPt_, "eleGenMomPt[nEle]/F");
  }
  tree_->Branch("eleIsoTrkDR03", eleIsoTrkDR03_, "eleIsoTrkDR03[nEle]/F");
  tree_->Branch("eleIsoEcalDR03", eleIsoEcalDR03_, "eleIsoEcalDR03[nEle]/F");
  tree_->Branch("eleIsoHcalDR03", eleIsoHcalDR03_, "eleIsoHcalDR03[nEle]/F");
  tree_->Branch("eleIsoTrkDR04", eleIsoTrkDR04_, "eleIsoTrkDR04[nEle]/F");
  tree_->Branch("eleIsoEcalDR04", eleIsoEcalDR04_, "eleIsoEcalDR04[nEle]/F");
  tree_->Branch("eleIsoHcalDR04", eleIsoHcalDR04_, "eleIsoHcalDR04[nEle]/F");
  tree_->Branch("eleChi2NDF", eleChi2NDF_, "eleChi2NDF[nEle]/F");
  tree_->Branch("eleD0", eleD0_, "eleD0[nEle]/F");
  tree_->Branch("eleNumberOfValidHits", eleNumberOfValidHits_, "eleNumberOfValidHits[nEle]/I");
  tree_->Branch("eleValidHitInFirstPXB",eleValidHitInFirstPXB_,"eleValidHitInFirstPXB[nEle]/I");
  tree_->Branch("eleTrkExpectHitsInner",eleTrkExpectHitsInner_,"eleTrkExpectHitsInner[nEle]/I");
  tree_->Branch("eleDist",eleDist_,"eleDist[nEle]/F");
  tree_->Branch("eleDcot",eleDcot_,"eleDcot[nEle]/F");
  tree_->Branch("eleConvRadius",eleConvRadius_,"eleConvRadius[nEle]/F");
  tree_->Branch("eleConvPoint",eleConvPoint_,"eleConvPoint[nEle][3]/F");
  // Photon
  tree_->Branch("nPho", &nPho_, "nPho/I");
  tree_->Branch("phoIsPhoton", phoIsPhoton_, "phoIsPhoton[nPho]/O");
  tree_->Branch("phoE", phoE_, "phoE[nPho]/F");
  tree_->Branch("phoEt", phoEt_, "phoEt[nPho]/F");
  tree_->Branch("phoPz", phoPz_, "phoPz[nPho]/F");
  tree_->Branch("phoEta", phoEta_, "phoEta[nPho]/F");
  tree_->Branch("phoPhi", phoPhi_, "phoPhi[nPho]/F");
  tree_->Branch("phoR9", phoR9_, "phoR9[nPho]/F");
  tree_->Branch("phoTrkIsoSolidDR03", phoTrkIsoSolidDR03_, "phoTrkIsoSolidDR03[nPho]/F");
  tree_->Branch("phoTrkIsoHollowDR03", phoTrkIsoHollowDR03_, "phoTrkIsoHollowDR03[nPho]/F");
  tree_->Branch("phoNTrkSolidDR03", phoNTrkSolidDR03_, "phoNTrkSolidDR03[nPho]/I");
  tree_->Branch("phoNTrkHollowDR03", phoNTrkHollowDR03_, "phoNTrkHollowDR03[nPho]/I");
  tree_->Branch("phoEcalIsoDR03", phoEcalIsoDR03_, "phoEcalIsoDR03[nPho]/F");
  tree_->Branch("phoHcalIsoDR03", phoHcalIsoDR03_, "phoHcalIsoDR03[nPho]/F");
  tree_->Branch("phoTrkIsoSolidDR04", phoTrkIsoSolidDR04_, "phoTrkIsoSolidDR04[nPho]/F");
  tree_->Branch("phoTrkIsoHollowDR04", phoTrkIsoHollowDR04_, "phoTrkIsoHollowDR04[nPho]/F");
  tree_->Branch("phoNTrkSolidDR04", phoNTrkSolidDR04_, "phoNTrkSolidDR04[nPho]/I");
  tree_->Branch("phoNTrkHollowDR04", phoNTrkHollowDR04_, "phoNTrkHollowDR04[nPho]/I");
  tree_->Branch("phoEcalIsoDR04", phoEcalIsoDR04_, "phoEcalIsoDR04[nPho]/F");
  tree_->Branch("phoHcalIsoDR04", phoHcalIsoDR04_, "phoHcalIsoDR04[nPho]/F");
  tree_->Branch("phoHoverE", phoHoverE_, "phoHoverE[nPho]/F");
  tree_->Branch("phoSigmaEtaEta", phoSigmaEtaEta_, "phoSigmaEtaEta[nPho]/F");
  tree_->Branch("phoSigmaIEtaIEta", phoSigmaIEtaIEta_, "phoSigmaIEtaIEta[nPho]/F");
  tree_->Branch("phoSeedTime", phoSeedTime_, "phoSeedTime[nPho]/F");
  tree_->Branch("phoPos", phoPos_, "phoPos[nPho]/I");
  tree_->Branch("phoEMax", phoEMax_, "phoEMax[nPho]/F");
  tree_->Branch("phoE3x3", phoE3x3_, "phoE3x3[nPho]/F");
  tree_->Branch("phoERight", phoERight_, "phoERight[nPho]/F");
  tree_->Branch("phoELeft", phoELeft_, "phoELeft[nPho]/F");
  tree_->Branch("phoETop", phoETop_, "phoETop[nPho]/F");
  tree_->Branch("phoEBottom", phoEBottom_, "phoEBottom[nPho]/F");
  tree_->Branch("phoRoundness", phoRoundness_, "phoRoundness[nPho]/F");
  tree_->Branch("phoAngle", phoAngle_, "phoAngle[nPho]/F");
  if (doGenParticles_) {
    tree_->Branch("phoGenIndex", phoGenIndex_, "phoGenIndex[nPho]/I");
    tree_->Branch("phoGenGMomPID", phoGenGMomPID, "phoGenGMomPID[nPho]/I");
    tree_->Branch("phoGenMomPID", phoGenMomPID, "phoGenMomPID[nPho]/I");
    tree_->Branch("phoGenMomPt", phoGenMomPt, "phoGenMomPt[nPho]/F");
  }
  tree_->Branch("phoSCE", phoSCE_, "phoSCE[nPho]/F");
  tree_->Branch("phoSCEt", phoSCEt_, "phoSCEt[nPho]/F");
  tree_->Branch("phoSCEta", phoSCEta_, "phoSCEta[nPho]/F");
  tree_->Branch("phoSCPhi", phoSCPhi_, "phoSCPhi[nPho]/F");
  tree_->Branch("phoSCEtaWidth", phoSCEtaWidth_, "phoSCEtaWidth[nPho]/F");
  tree_->Branch("phoSCPhiWidth", phoSCPhiWidth_, "phoSCPhiWidth[nPho]/F");
  tree_->Branch("phoOverlap", phoOverlap_, "phoOverlap[nPho]/I");
  tree_->Branch("phohasPixelSeed", phohasPixelSeed_, "phohasPixelSeed[nPho]/I");
  tree_->Branch("phoIsConv", phoIsConv_, "phoIsConv[nPho]/I");
  tree_->Branch("phoPi0Disc",phoPi0Disc_ , "phoPi0Disc[nPho]/F");
  // Muon
  tree_->Branch("nMu", &nMu_, "nMu/I");
  tree_->Branch("muEta", muEta_, "muEta[nMu]/F");
  tree_->Branch("muPhi", muPhi_, "muPhi[nMu]/F");
  tree_->Branch("muCharge", muCharge_, "muCharge[nMu]/I");
  tree_->Branch("muPt", muPt_, "muPt[nMu]/F");
  tree_->Branch("muPz", muPz_, "muPz[nMu]/F");
  if (doGenParticles_)
    tree_->Branch("muGenIndex", muGenIndex_, "muGenIndex[nMu]/I");
  tree_->Branch("muIsoTrk", muIsoTrk_, "muIsoTrk[nMu]/F");
  tree_->Branch("muIsoCalo", muIsoCalo_, "muIsoCalo[nMu]/F");
  tree_->Branch("muIsoEcal", muIsoEcal_, "muIsoEcal[nMu]/F");
  tree_->Branch("muIsoHcal", muIsoHcal_, "muIsoHcal[nMu]/F");
  tree_->Branch("muChi2NDF", muChi2NDF_, "muChi2NDF[nMu]/F");
  tree_->Branch("muEmVeto", muEmVeto_, "muEmVeto[nMu]/F");
  tree_->Branch("muHadVeto", muHadVeto_, "muHadVeto[nMu]/F");
  tree_->Branch("muType", muType_, "muType[nMu]/I");
  tree_->Branch("muID", muID_, "muID[nMu][6]/O");
  // [0]: AllArbitrated, [1]: GlobalMuonPromptTight, [2]: TMLSLoose, [3]: TMLSTight, [4]: TM2DCompatLoose, [5]: TM2DCompatTight
  tree_->Branch("muD0", muD0_, "muD0[nMu]/F");
  tree_->Branch("muNumberOfValidTrkHits", muNumberOfValidTrkHits_, "muNumberOfValidTrkHits[nMu]/I");
  tree_->Branch("muNumberOfValidPixelHits", muNumberOfValidPixelHits_, "muNumberOfValidPixelHits[nMu]/I");
  tree_->Branch("muNumberOfValidMuonHits", muNumberOfValidMuonHits_, "muNumberOfValidMuonHits[nMu]/I");
  // Zee candiate
  tree_->Branch("nZee", &nZee_, "nZee/I");
  tree_->Branch("ZeeMass", ZeeMass_, "ZeeMass[nZee]/F");
  tree_->Branch("ZeePt", ZeePt_, "ZeePt[nZee]/F");
  tree_->Branch("ZeeEta", ZeeEta_, "ZeeEta[nZee]/F");
  tree_->Branch("ZeePhi", ZeePhi_, "ZeePhi[nZee]/F");
  tree_->Branch("ZeeLeg1Index", ZeeLeg1Index_, "ZeeLeg1Index[nZee]/I");
  tree_->Branch("ZeeLeg2Index", ZeeLeg2Index_, "ZeeLeg2Index[nZee]/I");
  // Zmumu candiate
  tree_->Branch("nZmumu", &nZmumu_, "nZmumu/I");
  tree_->Branch("ZmumuMass", ZmumuMass_, "ZmumuMass[nZmumu]/F");
  tree_->Branch("ZmumuPt", ZmumuPt_, "ZmumuPt[nZmumu]/F");
  tree_->Branch("ZmumuEta", ZmumuEta_, "ZmumuEta[nZmumu]/F");
  tree_->Branch("ZmumuPhi", ZmumuPhi_, "ZmumuPhi[nZmumu]/F");
  tree_->Branch("ZmumuLeg1Index", ZmumuLeg1Index_, "ZmumuLeg1Index[nZmumu]/I");
  tree_->Branch("ZmumuLeg2Index", ZmumuLeg2Index_, "ZmumuLeg2Index[nZmumu]/I");
  // Wenu candidate
  tree_->Branch("nWenu", &nWenu_, "nWenu/I");
  tree_->Branch("WenuMassTCaloMET", WenuMassTCaloMET_, "WenuMassTCaloMET[nWenu]/F");
  tree_->Branch("WenuEtCaloMET", WenuEtCaloMET_, "WenuEtCaloMET[nWenu]/F");
  tree_->Branch("WenuACopCaloMET", WenuACopCaloMET_, "WenuACopCaloMET[nWenu]/F");
  tree_->Branch("WenuMassTTcMET", WenuMassTTcMET_, "WenuMassTTcMET[nWenu]/F");
  tree_->Branch("WenuEtTcMET", WenuEtTcMET_, "WenuEtTcMET[nWenu]/F");
  tree_->Branch("WenuACopTcMET", WenuACopTcMET_, "WenuACopTcMET[nWenu]/F");
  tree_->Branch("WenuMassTPfMET", WenuMassTPfMET_, "WenuMassTPfMET[nWenu]/F");
  tree_->Branch("WenuEtPfMET", WenuEtPfMET_, "WenuEtPfMET[nWenu]/F");
  tree_->Branch("WenuACopPfMET", WenuACopPfMET_, "WenuACopPfMET[nWenu]/F");
  tree_->Branch("WenuEleIndex", WenuEleIndex_, "WenuEleIndex[nWenu]/I");
  // Wmunu candidate
  tree_->Branch("nWmunu", &nWmunu_, "nWmunu/I");
  tree_->Branch("WmunuMassTCaloMET", WmunuMassTCaloMET_, "WmunuMassTCaloMET[nWmunu]/F");
  tree_->Branch("WmunuEtCaloMET", WmunuEtCaloMET_, "WmunuEtCaloMET[nWmunu]/F");
  tree_->Branch("WmunuACopCaloMET", WmunuACopCaloMET_, "WmunuACopCaloMET[nWmunu]/F");
  tree_->Branch("WmunuMassTTcMET", WmunuMassTTcMET_, "WmunuMassTTcMET[nWmunu]/F");
  tree_->Branch("WmunuEtTcMET", WmunuEtTcMET_, "WmunuEtTcMET[nWmunu]/F");
  tree_->Branch("WmunuACopTcMET", WmunuACopTcMET_, "WmunuACopTcMET[nWmunu]/F");
  tree_->Branch("WmunuMassTPfMET", WmunuMassTPfMET_, "WmunuMassTPfMET[nWmunu]/F");
  tree_->Branch("WmunuEtPfMET", WmunuEtPfMET_, "WmunuEtPfMET[nWmunu]/F");
  tree_->Branch("WmunuACopPfMET", WmunuACopPfMET_, "WmunuACopPfMET[nWmunu]/F");
  tree_->Branch("WmunuMuIndex", WmunuMuIndex_, "WmunuMuIndex[nWmunu]/I");
}

VgAnalyzerKit::~VgAnalyzerKit() {
}

void VgAnalyzerKit::produce(edm::Event & e, const edm::EventSetup & es) {

  hEvents_->Fill(0.5);

  helper_.getHandles(e,
                     muonHandle_,
                     electronHandle_,
                     tauHandle_,
                     jetHandle_,
                     METHandle_,
                     photonHandle_,
                     trackHandle_,
                     genParticlesHandle_,
                     zmumuHandle_);
  if (saveHistograms_) helper_.fillHistograms(e,
                                              muonHandle_,
                                              electronHandle_,
                                              tauHandle_,
                                              jetHandle_,
                                              METHandle_,
                                              photonHandle_,
                                              trackHandle_,
                                              genParticlesHandle_);

  Handle<edm::View<reco::RecoCandidate> > others;
  e.getByLabel("cleanLayer1Electrons", others);

  Handle<EcalRecHitCollection> EBReducedRecHits;
  e.getByLabel(ebReducedRecHitCollection_, EBReducedRecHits);
  Handle<EcalRecHitCollection> EEReducedRecHits;
  e.getByLabel(eeReducedRecHitCollection_, EEReducedRecHits);
  EcalClusterLazyTools lazyTool(e, es, ebReducedRecHitCollection_, eeReducedRecHitCollection_ );
  Handle<EcalRecHitCollection> EBRecHits;
  e.getByLabel("ecalRecHit","EcalRecHitsEB",EBRecHits);

  Handle<reco::BeamSpot> beamSpotHandle;
  e.getByLabel(beamSpotCollection_, beamSpotHandle);

  //Handle<int> genProcessID;
  //e.getByLabel("genEventProcID", genProcessID);
  //processID_ = *genProcessID;
  // cout << "VgAnalyzerKit: produce: event info ..." << endl;

  run_    = e.id().run();
  event_  = e.id().event();
  orbit_  = e.orbitNumber();
  bx_     = e.bunchCrossing();
  lumis_  = e.luminosityBlock();
  isData_ = e.isRealData();

  // technical trigger
  // cout << "VgAnalyzerKit: produce: technical trigger ..." << endl;
  Handle< L1GlobalTriggerReadoutRecord > gtReadoutRecord;
  if (e.getByLabel(gtdigilabel_, gtReadoutRecord) ) {

    const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = gtReadoutRecord->technicalTriggerWord();

    ttbit0_  = (technicalTriggerWordBeforeMask.at(0))  ? 1 : 0;
    ttbit34_ = (technicalTriggerWordBeforeMask.at(34)) ? 1 : 0;
    ttbit40_ = (technicalTriggerWordBeforeMask.at(40)) ? 1 : 0;
    ttbit41_ = (technicalTriggerWordBeforeMask.at(41)) ? 1 : 0;
    ttbitBH_ = (technicalTriggerWordBeforeMask.at(36) || technicalTriggerWordBeforeMask.at(37) || 
technicalTriggerWordBeforeMask.at(38) || technicalTriggerWordBeforeMask.at(39)) ? 1 : 0;
  }

  // Get CaloTower information
  edm::Handle<CaloTowerCollection> pCaloTower;
  if ( e.getByLabel(caloTowerlabel_, pCaloTower) ) {

    const CaloTowerCollection* CaloTowers = pCaloTower.product();

    nHFTowersP_ = 0;
    nHFTowersN_ = 0;
    for (CaloTowerCollection::const_iterator aCalo = CaloTowers->begin(); aCalo != CaloTowers->end(); aCalo++) {

      if (aCalo->energy() > 3) {
        for (size_t i = 0; i < aCalo->constituentsSize(); ++i) {
          const DetId caloId = aCalo->constituent(i);

          if (caloId.det() != DetId::Hcal) continue;

          HcalSubdetector hcalsubdet = (HcalSubdetector(caloId.subdetId()));

          if (hcalsubdet != HcalForward) continue;
          if (aCalo->eta() < 3) nHFTowersP_++;
          if (aCalo->eta() > -3) nHFTowersN_++;
        }
      }
    }
  } // pCalotower.isValid()

  // vertex
  nVtx_ = 0;
  IsVtxGood_ = 0;
  Int_t nGoodVtx = 0;
  Handle<VertexCollection> recVtxs;
  if (e.getByLabel(vtxlabel_, recVtxs)) {

    for (size_t i=0; i<recVtxs->size(); ++i)
      if (!((*recVtxs)[i].isFake())) {
	vtx_[nVtx_][0] = (*recVtxs)[i].x();
	vtx_[nVtx_][1] = (*recVtxs)[i].y();
	vtx_[nVtx_][2] = (*recVtxs)[i].z();
	vtxNTrk_[nVtx_] = (*recVtxs)[i].tracksSize();
	vtxNDF_[nVtx_] = (*recVtxs)[i].ndof();
	vtxD0_[nVtx_] = (*recVtxs)[i].position().rho();

	if (vtxNDF_[nVtx_] > 4 && fabs(vtx_[nVtx_][2]) <= 15 && vtxD0_[nVtx_] <= 2) nGoodVtx++;
	nVtx_++;
      }
  }
  if (nGoodVtx > 0) IsVtxGood_ = 1;

  // track quality
  TrackBase::TrackQuality trkQuality_;
  Handle<reco::TrackCollection> Tracks;
  if (e.getByLabel(tracklabel_, Tracks)) {

    const reco::TrackCollection *track = Tracks.product();

    trkQuality_ = reco::TrackBase::qualityByName("highPurity");

    nTrk_ = 0;
    nGoodTrk_ = 0;
    for (reco::TrackCollection::const_iterator aTrk = track->begin(); aTrk != track->end(); ++aTrk) {

      if (aTrk->quality(trkQuality_)) nGoodTrk_++;
      nTrk_++;
    }
  }
  IsTracksGood_ = 0;
  if (nTrk_ > 10) {
    if (((float)nGoodTrk_/(float)nTrk_) > 0.25) IsTracksGood_ = 1;
  } else {
    IsTracksGood_ = 1;
  }

  // PDF information
  // cout << "VgAnalyzerKit: produce: PDF information..." << endl;
  if (!isData_) {
    Handle<GenEventInfoProduct> pdfInfoHandle;
    bool pdfInfo = e.getByLabel("generator", pdfInfoHandle);
    if (pdfInfo) {
      pdf_[0] = pdfInfoHandle->pdf()->id.first;      // PDG ID of incoming parton #1
      pdf_[1] = pdfInfoHandle->pdf()->id.second;      // PDG ID of incoming parton #2
      pdf_[2] = pdfInfoHandle->pdf()->x.first;       // x value of parton #1
      pdf_[3] = pdfInfoHandle->pdf()->x.second;       // x value of parton #2
      pdf_[4] = pdfInfoHandle->pdf()->xPDF.first;     // PDF weight for parton #1
      pdf_[5] = pdfInfoHandle->pdf()->xPDF.second;     // PDF weight for parton #2
      pdf_[6] = pdfInfoHandle->pdf()->scalePDF; // scale of the hard interaction
    }
  }

  // get pthat value and store in ntuple
  if (!isData_) {
    edm::Handle<GenEventInfoProduct> genEventScale;
    e.getByLabel("generator", genEventScale);
    pthat_ = 0;
    if (genEventScale->hasBinningValues() ) {
       pthat_ = genEventScale->binningValues()[0];
    }
  }

  // GenParticle
  // cout << "VgAnalyzerKit: produce: GenParticle... " << endl;
  if (!isData_ && genParticlesHandle_.isValid() ) {

    nMC_ = 0;
    int genIndex = 0;
    const Candidate *mom = 0;

    for (vector<GenParticle>::const_iterator ip = genParticlesHandle_->begin(); ip != genParticlesHandle_->end(); ++ip) {

      genIndex++;

      if ((ip->status()==3 && (ip->pdgId()==23 || fabs(ip->pdgId())==24)) || (ip->status()==1 && ((fabs(ip->pdgId())>=11 && 
fabs(ip->pdgId())<=14) || ip->pdgId()==22))) {

	const Candidate *p = (const Candidate*)&(*ip);

	if (!p->mother()) continue;

	if (fabs(p->pdgId())==4 && fabs(p->mother()->pdgId())!=7) continue;
	if ((genIndex-1)>20 && ip->pdgId()==22) continue;
	if (fabs(p->pdgId())==12 && fabs(p->mother()->pdgId())>100) continue;
	if (fabs(p->pdgId())==14 && fabs(p->mother()->pdgId())>100) continue;

	mcPID[nMC_] = p->pdgId();
	mcPt[nMC_] = p->pt();
	mcMass[nMC_] = p->mass();
	mcEta[nMC_] = p->eta();
	mcPhi[nMC_] = p->phi();
	mcGMomPID[nMC_] = -999;
	mcMomPID[nMC_] = -999;
	mcMomPt[nMC_] = -999;
	mcMomMass[nMC_] = -999;
	mcMomEta[nMC_] = -999;
	mcMomPhi[nMC_] = -999;
	mcDecayType[nMC_] = -999;

	for (size_t j=0; j<p->numberOfMothers(); ++j) {
	  mom = p->mother(j);
	  mcMomPID[nMC_] = mom->pdgId();
	  mcMomPt[nMC_] = mom->pt();
	  mcMomMass[nMC_] = mom->mass();
	  mcMomEta[nMC_] = mom->eta();
	  mcMomPhi[nMC_] = mom->phi();
	  if (mom->mother()) mcGMomPID[nMC_] = mom->mother()->pdgId();
	}
	mcIndex[nMC_] = genIndex-1;

	// if genParticle is W or Z, check its decay type
	if (ip->pdgId()==23 || fabs(ip->pdgId())==24) {
	  for (size_t k=0; k<p->numberOfDaughters(); ++k) {

	    const Candidate *dp = p->daughter(k);

	    if (fabs(dp->pdgId())<=6)
	      mcDecayType[nMC_] = 1;
	    else if (fabs(dp->pdgId())==11 || fabs(dp->pdgId())==12)
	      mcDecayType[nMC_] = 2;
	    else if (fabs(dp->pdgId())==13 || fabs(dp->pdgId())==14)
	      mcDecayType[nMC_] = 3;
	    else if (fabs(dp->pdgId())==15 || fabs(dp->pdgId())==16)
	      mcDecayType[nMC_] = 4;

	  }
	}

	nMC_++;
      }

    }

  }

  // HLT
  // cout << "VgAnalyzerKit: produce: HLT ... " << endl;
  nHLT_ = 0;
  if (saveHLTInfo_) {
    Handle<TriggerResults> trgResultsHandle;
    e.getByLabel(trgResults_, trgResultsHandle);
    const TriggerNames &trgNames = e.triggerNames(*trgResultsHandle);
    vector<string> hlNames_ = trgNames.triggerNames();
    nHLT_ = trgNames.size();
    for (size_t i=0; i<trgNames.size(); ++i) {
      //int hltBits[20] = {6, 7, 8, 9, 10, 11, 12, 44, 45, 46, 47, 48, 49, 50, 76, 77, 78, 83, 84, 85};
      //for (int i=0; i<20; ++i) {
      //HLT_[i] = (trgResultsHandle->accept(hltBits[i]) == true) ? 1:0;
      //cout<<"HLT bit = "<<hltBits[i]<<"   "<<hlNames_[hltBits[i]]<<" "<<HLT_[i]<<endl;
      HLT_[i] = (trgResultsHandle->accept(i) == true) ? 1:0;
      //cout<<"HLT bit = "<<i<<"   "<<hlNames_[i]<<endl;
    }
  }

  // Gen & PAT MET (caloMET)
  // cout << "VgAnalyzerKit: produce: Gen & PAT MET (caloMET) ..." << endl;
  int nMET = 0;
  if( METHandle_.isValid() )
    for (View<pat::MET>::const_iterator iMET = METHandle_->begin(); iMET != METHandle_->end(); ++iMET) {
      MET_      = iMET->pt();
      METx_     = iMET->px();
      METy_     = iMET->py();
      METPhi_   = iMET->phi();
      METsumEt_ = iMET->sumEt();

      uncorrMET_[0]    = iMET->uncorrectedPt(pat::MET::uncorrALL);
      uncorrMET_[1]    = iMET->uncorrectedPt(pat::MET::uncorrJES);
      uncorrMET_[2]    = iMET->uncorrectedPt(pat::MET::uncorrMUON);

      uncorrMETPhi_[0] = iMET->uncorrectedPhi(pat::MET::uncorrALL);
      uncorrMETPhi_[1] = iMET->uncorrectedPhi(pat::MET::uncorrJES);
      uncorrMETPhi_[2] = iMET->uncorrectedPhi(pat::MET::uncorrMUON);

      uncorrMETSumEt_[0]  = iMET->corSumEt(pat::MET::uncorrALL);
      uncorrMETSumEt_[1]  = iMET->corSumEt(pat::MET::uncorrJES);
      uncorrMETSumEt_[2]  = iMET->corSumEt(pat::MET::uncorrMUON);

      if (!isData_) {
	genMET_    = (*iMET).genMET()->pt();
	genMETx_   = (*iMET).genMET()->px();
	genMETy_   = (*iMET).genMET()->py();
	genMETPhi_ = (*iMET).genMET()->phi();
      }

      nMET++;
    }

  // tcMET
  // cout << "VgAnalyzerKit: produce: tcMET ..." << endl;
  Handle<edm::View<reco::MET> > tcMETcoll;
  const reco::MET *tcMET = 0;
  if (e.getByLabel(tcMETlabel_, tcMETcoll)) {

    tcMET = &(tcMETcoll->front());

    tcMET_       = tcMET->pt();
    tcMETx_      = tcMET->px();
    tcMETy_      = tcMET->py();
    tcMETPhi_    = tcMET->phi();
    tcMETsumEt_  = tcMET->sumEt();
    tcMETmEtSig_ = tcMET->mEtSig();
    tcMETSig_    = tcMET->significance();
  }

  // pfMET
  // cout << "VgAnalyzerKit: produce: pfMET ..." << endl;
  Handle<View<reco::PFMET> > pfMETcoll;
  const reco::PFMET *pfMET = 0;
  if (e.getByLabel(pfMETlabel_, pfMETcoll)) {

    pfMET = &(pfMETcoll->front());

    pfMET_       = pfMET->pt();
    pfMETx_      = pfMET->px();
    pfMETy_      = pfMET->py();
    pfMETPhi_    = pfMET->phi();
    pfMETsumEt_  = pfMET->sumEt();
    pfMETmEtSig_ = pfMET->mEtSig();
    pfMETSig_    = pfMET->significance();
  }

  // Electron

  // cout << "VgAnalyzerKit: produce: Electron ..." << endl;

  //++++

  edm::Handle<DcsStatusCollection> dcsHandle;
  e.getByLabel("scalersRawToDigi", dcsHandle);
  double evt_bField;
  
  if (isData_) {
    // scale factor = 3.801/18166.0 which are
    // average values taken over a stable two
    // week period
    float currentToBFieldScaleFactor = 2.09237036221512717e-04;
    float current = (*dcsHandle)[0].magnetCurrent();
    evt_bField = current*currentToBFieldScaleFactor;
  } else {
      
    ESHandle<MagneticField> magneticField;
    es.get<IdealMagneticFieldRecord>().get(magneticField);
        
    evt_bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
  }
  // cout<< "BField:"<< evt_bField <<endl;
  //=====
  int nElePassCut = 0;
  nEle_ = 0;
  const Candidate *elemom = 0;
  if ( electronHandle_.isValid() )
    for (View<pat::Electron>::const_iterator iEle = electronHandle_->begin(); iEle != electronHandle_->end(); ++iEle) {

      if (iEle->pt() > leadingElePtCut_) nElePassCut++;

      //        new eID with correct isolations and conversion rejection, see https://twiki.cern.ch/twiki/bin/viewauth/CMS/SimpleCutBasedEleID
      //        The value map returns a double with the following meaning:
      //        0: fails
      //	1: passes electron ID only
      //	2: passes electron Isolation only
      //	3: passes electron ID and Isolation only
      //	4: passes conversion rejection
      //	5: passes conversion rejection and ID
      //	6: passes conversion rejection and Isolation
      //	7: passes the whole selection

      for (int i=0; i<12; ++i) eleID_[nEle_][i] = -1;

      // ID sources are following, numbers are simbolic and correspond to WEnu signal efficiency.
      // 0  simpleEleId60cIso
      // 1  simpleEleId70cIso
      // 2  simpleEleId80cIso
      // 3  simpleEleId85cIso
      // 4  simpleEleId90cIso
      // 5  simpleEleId95cIso
      // 6  simpleEleId60relIso
      // 7  simpleEleId70relIso
      // 8  simpleEleId80relIso
      // 9  simpleEleId85relIso
      // 10 simpleEleId90relIso
      // 11 simpleEleId95relIso
      eleID_[nEle_][0] = int (iEle->electronID("simpleEleId60cIso"));
      eleID_[nEle_][1] = int (iEle->electronID("simpleEleId70cIso"));
      eleID_[nEle_][2] = int (iEle->electronID("simpleEleId80cIso"));
      eleID_[nEle_][3] = int (iEle->electronID("simpleEleId85cIso"));
      eleID_[nEle_][4] = int (iEle->electronID("simpleEleId90cIso"));
      eleID_[nEle_][5] = int (iEle->electronID("simpleEleId95cIso"));
      eleID_[nEle_][6] = int (iEle->electronID("simpleEleId60relIso"));
      eleID_[nEle_][7] = int (iEle->electronID("simpleEleId70relIso"));
      eleID_[nEle_][8] = int (iEle->electronID("simpleEleId80relIso"));
      eleID_[nEle_][9] = int (iEle->electronID("simpleEleId85relIso"));
      eleID_[nEle_][10]= int (iEle->electronID("simpleEleId90relIso"));
      eleID_[nEle_][11]= int (iEle->electronID("simpleEleId95relIso"));

      eleClass_[nEle_]   = iEle->classification();
      eleCharge_[nEle_]  = iEle->charge();
      eleEn_[nEle_]      = iEle->energy();
      elePt_[nEle_]      = iEle->pt();
      elePz_[nEle_]      = iEle->pz();
      eleEta_[nEle_]     = iEle->eta();
      elePhi_[nEle_]     = iEle->phi();
      eleHoverE_[nEle_]  = iEle->hadronicOverEm();
      eleEoverP_[nEle_]  = iEle->eSuperClusterOverP();

      elePin_[nEle_]  = iEle->trackMomentumAtVtx().R();
      elePout_[nEle_] = iEle->trackMomentumOut().R();

      eleBrem_[nEle_] = (elePin_[nEle_] - elePout_[nEle_]) / elePin_[nEle_];

      eledEtaAtVtx_[nEle_] = iEle->deltaEtaSuperClusterTrackAtVtx();
      eledPhiAtVtx_[nEle_] = iEle->deltaPhiSuperClusterTrackAtVtx();

      // Acess super cluster
      eleSCEta_[nEle_]   = iEle->superCluster()->eta();
      eleSCPhi_[nEle_]   = iEle->superCluster()->phi();
      eleSCRawEn_[nEle_] = iEle->superCluster()->rawEnergy();
      eleSCEn_[nEle_]    = iEle->superCluster()->energy();
      eleESEn_[nEle_]    = iEle->superCluster()->preshowerEnergy();
      eleSCEtaWidth_[nEle_] = iEle->superCluster()->etaWidth();
      eleSCPhiWidth_[nEle_] = iEle->superCluster()->phiWidth();

      eleVtx_[nEle_][0] = iEle->trackPositionAtVtx().x();
      eleVtx_[nEle_][1] = iEle->trackPositionAtVtx().y();
      eleVtx_[nEle_][2] = iEle->trackPositionAtVtx().z();

      eleCaloPos_[nEle_][0] = iEle->trackPositionAtCalo().x();
      eleCaloPos_[nEle_][1] = iEle->trackPositionAtCalo().y();
      eleCaloPos_[nEle_][2] = iEle->trackPositionAtCalo().z();

      eleSCPos_[nEle_][0] = iEle->superCluster()->x();
      eleSCPos_[nEle_][1] = iEle->superCluster()->y();
      eleSCPos_[nEle_][2] = iEle->superCluster()->z();

      const reco::CaloClusterPtr eleSeed = (*iEle).superCluster()->seed();

      eleEMax_[nEle_] = lazyTool.eMax(*eleSeed);
      eleE3x3_[nEle_] = lazyTool.e3x3(*eleSeed);

      eleERight_[nEle_]     = lazyTool.eRight(*eleSeed);
      eleELeft_[nEle_]      = lazyTool.eLeft(*eleSeed);
      eleETop_[nEle_]       = lazyTool.eTop(*eleSeed);
      eleEBottom_[nEle_]    = lazyTool.eBottom(*eleSeed);

      /*
      //For electron concersion rejection
      // if need a ID optimisation need to use correct tags for later releases 3_6_1_patch2?
      
      ConversionFinder convFinder;
      ConversionInfo convInfo = convFinder.getConversionInfo(*iEle , Tracks, evt_bField);

      eleDist_[nEle_] = convInfo.dist();
      eleDcot_[nEle_] = convInfo.dcot();
      eleConvRadius_[nEle_]    = convInfo.radiusOfConversion();
      eleConvPoint_[nEle_][0]  = convInfo.pointOfConversion().x();
      eleConvPoint_[nEle_][1]  = convInfo.pointOfConversion().y();
      eleConvPoint_[nEle_][2]  = convInfo.pointOfConversion().z();
      */

      // Gen Particle
      eleGenIndex_[nEle_] = -1;
      int EleGenIndex = 0;
      if (!isData_) {
        if ((*iEle).genLepton() && genParticlesHandle_.isValid() ) {
          for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen != genParticlesHandle_->end(); 
++iGen) {
            if (iGen->p4() == (*iEle).genLepton()->p4() && iGen->pdgId() == (*iEle).genLepton()->pdgId() && iGen->status() 
== (*iEle).genLepton()->status()) {
                eleGenIndex_[nEle_] = EleGenIndex;

                const Candidate *elep = (const Candidate*)&(*iGen);

                for (size_t j=0; j<elep->numberOfMothers(); ++j) {
                  elemom = elep->mother(j);
                  eleGenMomPID_[nEle_] = elemom->pdgId();
                  eleGenMomPt_[nEle_] = elemom->pt();
                  if (elemom->mother()) eleGenGMomPID_[nEle_] = elemom->mother()->pdgId();
                }
            }
            EleGenIndex++;
          }
        }
      }

      eleSigmaEtaEta_[nEle_]   = iEle->scSigmaEtaEta();
      eleSigmaIEtaIEta_[nEle_] = iEle->scSigmaIEtaIEta();

      eleIsoTrkDR03_[nEle_]  = iEle->dr03TkSumPt();
      eleIsoEcalDR03_[nEle_] = iEle->dr03EcalRecHitSumEt();
      eleIsoHcalDR03_[nEle_] = iEle->dr03HcalTowerSumEt();

      eleIsoTrkDR04_[nEle_]  = iEle->dr04TkSumPt();
      eleIsoEcalDR04_[nEle_] = iEle->dr04EcalRecHitSumEt();
      eleIsoHcalDR04_[nEle_] = iEle->dr04HcalTowerSumEt();

      eleChi2NDF_[nEle_] = iEle->gsfTrack()->normalizedChi2();

      eleD0_[nEle_] = (*iEle).gsfTrack()->d0();
      eleNumberOfValidHits_[nEle_]  = (*iEle).gsfTrack()->numberOfValidHits();
      eleValidHitInFirstPXB_[nEle_] = iEle->gsfTrack()->hitPattern().hasValidHitInFirstPixelBarrel();
      eleTrkExpectHitsInner_[nEle_] = iEle->gsfTrack()->trackerExpectedHitsInner().numberOfHits();
      
      nEle_++;
    }

  // Photon
   // take the pi0 rejection info from RECO
  Handle<reco::PhotonPi0DiscriminatorAssociationMap>  map;
  e.getByLabel("piZeroDiscriminators","PhotonPi0DiscriminatorAssociationMap",  map);
  reco::PhotonPi0DiscriminatorAssociationMap::const_iterator mapIter;
//
  edm::Handle<reco::PhotonCollection> R_PhotonHandle;
  e.getByLabel("photons", "", R_PhotonHandle);
  const reco::PhotonCollection R_photons = *(R_PhotonHandle.product());   

  const Candidate *phomom = 0;
  int nPhoPassCut = 0;
  nPho_ = 0;
  if ( photonHandle_.isValid() )
    for (View<pat::Photon>::const_iterator iPho = photonHandle_->begin(); iPho != photonHandle_->end(); ++iPho) {
      
      if (iPho->pt() > leadingPhoPtCut_) nPhoPassCut++;
      
      phoIsPhoton_[nPho_] = iPho->isPhoton();
      phoE_[nPho_]   = iPho->energy();
      phoEt_[nPho_]  = iPho->et();
      phoPz_[nPho_]  = iPho->pz();
      phoEta_[nPho_] = iPho->eta();
      phoPhi_[nPho_] = iPho->phi();

      phoR9_[nPho_]               = iPho->r9();
      phoTrkIsoSolidDR03_[nPho_]  = iPho->trkSumPtSolidConeDR03();
      phoTrkIsoHollowDR03_[nPho_] = iPho->trkSumPtHollowConeDR03();
      phoNTrkSolidDR03_[nPho_]    = iPho->nTrkSolidConeDR03();
      phoNTrkHollowDR03_[nPho_]   = iPho->nTrkHollowConeDR03();
      phoEcalIsoDR03_[nPho_]      = iPho->ecalRecHitSumEtConeDR03();
      phoHcalIsoDR03_[nPho_]      = iPho->hcalTowerSumEtConeDR03();

      phoTrkIsoSolidDR04_[nPho_]  = iPho->trkSumPtSolidConeDR04();
      phoTrkIsoHollowDR04_[nPho_] = iPho->trkSumPtHollowConeDR04();
      phoNTrkSolidDR04_[nPho_]    = iPho->nTrkSolidConeDR04();
      phoNTrkHollowDR04_[nPho_]   = iPho->nTrkHollowConeDR04();
      phoEcalIsoDR04_[nPho_]      = iPho->ecalRecHitSumEtConeDR04();
      phoHcalIsoDR04_[nPho_]      = iPho->hcalTowerSumEtConeDR04();

      phoHoverE_[nPho_]        = iPho->hadronicOverEm();
      phoSigmaEtaEta_[nPho_]   = iPho->sigmaEtaEta();
      phoSigmaIEtaIEta_[nPho_] = iPho->sigmaIetaIeta();

      phoOverlap_[nPho_] = (int) iPho->hasOverlaps("electrons");
      phohasPixelSeed_[nPho_] = (int) iPho->hasPixelSeed();
      
      // where is photon ? (0: EB, 1: EE, 2: EBGap, 3: EEGap, 4: EBEEGap)
      phoPos_[nPho_] = -1;
      if (iPho->isEB() == true) phoPos_[nPho_] = 0;
      if (iPho->isEE() == true) phoPos_[nPho_] = 1;
      if (iPho->isEBGap() == true) phoPos_[nPho_] = 2;
      if (iPho->isEEGap() == true) phoPos_[nPho_] = 3;
      if (iPho->isEBEEGap() == true) phoPos_[nPho_] = 4;

      phoSeedTime_[nPho_] = -999.;
      const reco::CaloClusterPtr phoSeed = (*iPho).superCluster()->seed();
      DetId phoSeedDetId = lazyTool.getMaximum(*phoSeed).first;

      if ( iPho->isEB() && EBReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator ebrhit = EBReducedRecHits->find(phoSeedDetId);
        if ( ebrhit != EBReducedRecHits->end() ) { phoSeedTime_[nPho_] = ebrhit->time(); }
      } else if ( EEReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eerhit = EEReducedRecHits->find(phoSeedDetId);
        if ( eerhit != EEReducedRecHits->end() ) { phoSeedTime_[nPho_] = eerhit->time(); }
      }

      phoEMax_[nPho_] = lazyTool.eMax(*phoSeed);
      phoE3x3_[nPho_] = lazyTool.e3x3(*phoSeed);

      phoERight_[nPho_]     = lazyTool.eRight(*phoSeed);
      phoELeft_[nPho_]      = lazyTool.eLeft(*phoSeed);
      phoETop_[nPho_]       = lazyTool.eTop(*phoSeed);
      phoEBottom_[nPho_]    = lazyTool.eBottom(*phoSeed);

      if(iPho->isEB()==true && EBRecHits.isValid()){
            std::vector<float> RoundAndAngle = 
EcalClusterTools::roundnessBarrelSuperClusters(*(iPho->superCluster()),*EBRecHits,0);
            phoRoundness_[nPho_] = RoundAndAngle[0];
            phoAngle_[nPho_] = RoundAndAngle[1];
      } else{
            phoRoundness_[nPho_] = -999;
            phoAngle_[nPho_] = -999;
      }

      // Gen Particle
      // cout << "VgAnalyzerKit: produce: photon " << nPho_ << " gen match ..." << endl;
      phoGenIndex_[nPho_]  = -999;
      phoGenMomPID[nPho_]  = -999;
      phoGenMomPt[nPho_]   = -999;
      phoGenGMomPID[nPho_] = -999;
      int phoGenIndex = 0;
      if ( !isData_ && genParticlesHandle_.isValid() ) {
        if ((*iPho).genPhoton()) {
          for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen != genParticlesHandle_->end(); 
++iGen) {

            if (iGen->p4() == (*iPho).genPhoton()->p4() && iGen->pdgId() == (*iPho).genPhoton()->pdgId() && iGen->status() 
== (*iPho).genPhoton()->status()) {

              phoGenIndex_[nPho_] = phoGenIndex;

              const Candidate *phop = (const Candidate*)&(*iGen);

              for (size_t j=0; j<phop->numberOfMothers(); ++j) {
                phomom = phop->mother(j);
                phoGenMomPID[nPho_] = phomom->pdgId();
                phoGenMomPt[nPho_] = phomom->pt();
                if (phomom->mother()) phoGenGMomPID[nPho_] = phomom->mother()->pdgId();
              }

            }

            phoGenIndex++;
          }
        }
      }

      cout<<" Pho = "<<phoGenMomPID[nPho_]<<"     "<<phoOverlap_[nPho_]<<"     "<<phohasPixelSeed_[nPho_]<<endl;
      // Super Cluster
      phoSCE_[nPho_]   = (*iPho).superCluster()->energy();
      phoSCEta_[nPho_] = (*iPho).superCluster()->eta();
      phoSCPhi_[nPho_] = (*iPho).superCluster()->phi();
      phoSCEt_[nPho_]  = (*iPho).superCluster()->energy()/cosh(phoSCEta_[nPho_]);
      phoSCEtaWidth_[nPho_] = (*iPho).superCluster()->etaWidth();
      phoSCPhiWidth_[nPho_] = (*iPho).superCluster()->phiWidth();


      phoIsConv_[nPho_] = iPho->hasConversionTracks();

      phoPi0Disc_[nPho_] = -1;

      int R_nphot = 0;
      float nn = -1;
      for( reco::PhotonCollection::const_iterator  R_phot_iter = R_photons.begin(); R_phot_iter != R_photons.end(); R_phot_iter++) { 
        mapIter = map->find(edm::Ref<reco::PhotonCollection>(R_PhotonHandle,R_nphot));
        if(mapIter!=map->end()) {
          nn = mapIter->val;
        }
        if(iPho->p4() == R_phot_iter->p4()) phoPi0Disc_[nPho_] = nn;
        R_nphot++;              
      }  
      
       nPho_++;
    }

  // Muon
  int nMuPassCut = 0;
  nMu_ = 0;
  if( muonHandle_.isValid() ) {
    // cout << "VgAnalyzerKit: produce: number of muons: " << muonHandle_->size() << endl;
    for (View<pat::Muon>::const_iterator iMu = muonHandle_->begin(); iMu != muonHandle_->end(); ++iMu) {

      if (iMu->pt() > leadingMuPtCut_) nMuPassCut++;

      //       if (!iMu->isGlobalMuon()) continue;
      //       if (!iMu->isTrackerMuon()) continue;
      //       if (iMu->globalTrack().isNull()) continue;
      //       if (iMu->innerTrack().isNull()) continue;
      //if (iMu->pt()<10) continue;

      for (int i=0; i<6; ++i) muID_[nMu_][i] = 0;
      if (iMu->isGood("AllArbitrated")==1) muID_[nMu_][0] = 1;
      if (iMu->isGood("GlobalMuonPromptTight")==1) muID_[nMu_][1] = 1;
      if (iMu->isGood("TMLastStationLoose")==1) muID_[nMu_][2] = 1;
      if (iMu->isGood("TMLastStationTight")==1) muID_[nMu_][3] = 1;
      if (iMu->isGood("TM2DCompatibilityLoose")==1) muID_[nMu_][4] = 1;
      if (iMu->isGood("TM2DCompatibilityTight")==1) muID_[nMu_][5] = 1;

      const reco::TrackRef trkr = iMu->globalTrack();
      if (trkr.isNull()) {
        muD0_[nMu_] = -99.;
		muNumberOfValidTrkHits_[nMu_] = -99;
		muNumberOfValidPixelHits_[nMu_] = -99;
		muNumberOfValidMuonHits_[nMu_] = -99;
		muChi2NDF_[nMu_] = -99;
	  } else {
        muD0_[nMu_] = trkr->dxy(beamSpotHandle->position());
        muNumberOfValidTrkHits_[nMu_] = trkr->hitPattern().numberOfValidTrackerHits();
        muNumberOfValidPixelHits_[nMu_] = trkr->hitPattern().numberOfValidPixelHits();
        muNumberOfValidMuonHits_[nMu_] = trkr->hitPattern().numberOfValidMuonHits();
	  	muChi2NDF_[nMu_] = trkr->normalizedChi2();
      }

      muEta_[nMu_] = iMu->eta();
      muPhi_[nMu_] = iMu->phi();
      muCharge_[nMu_] = iMu->charge();
      muPt_[nMu_] = iMu->pt();
      muPz_[nMu_] = iMu->pz();

      muType_[nMu_] = iMu->type();

      muIsoTrk_[nMu_] = iMu->trackIso();
      muIsoCalo_[nMu_] = iMu->caloIso();
      muIsoEcal_[nMu_] = iMu->ecalIso();
      muIsoHcal_[nMu_] = iMu->hcalIso();

      muEmVeto_[nMu_]  = iMu->isolationR03().emVetoEt;
      muHadVeto_[nMu_] = iMu->isolationR03().hadVetoEt;

      muGenIndex_[nMu_] = -1;
      int MuGenIndex = 0;
      if (!isData_) {
        if ( (*iMu).genLepton() && genParticlesHandle_.isValid() ) {
          if (fabs((*iMu).genLepton()->pdgId())==13) {
            for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen !=
                  genParticlesHandle_->end(); ++iGen) {

              if (iGen->p4() == (*iMu).genLepton()->p4() && iGen->pdgId() == (*iMu).genLepton()->pdgId() && iGen->status() 
== (*iMu).genLepton()->status()) muGenIndex_[nMu_] = MuGenIndex;
              MuGenIndex++;
            }
          }
        }
      }

      nMu_++;

    }
  }

  // Zee candidate
  // cout << "VgAnalyzerKit: produce: Zee candidate..." << endl;
  nZee_ = 0;
  int leg1Index = 0;
  int leg2Index = 0;
  if (electronHandle_.isValid() && electronHandle_->size() > 1) {
    for (View<pat::Electron>::const_iterator iEle = electronHandle_->begin(); iEle != electronHandle_->end()-1; ++iEle) {

      leg2Index = leg1Index + 1;
      for (View<pat::Electron>::const_iterator jEle = iEle+1; jEle != electronHandle_->end(); ++jEle) {

	if (iEle->charge() * jEle->charge() < 0 ) {
	  pat::CompositeCandidate Zee;
	  Zee.addDaughter(*iEle, "ele1");
	  Zee.addDaughter(*jEle, "ele2");

	  AddFourMomenta addZee;
	  addZee.set(Zee);
	  ZeeMass_[nZee_]      = Zee.mass();
	  ZeePt_[nZee_]        = Zee.pt();
	  ZeeEta_[nZee_]       = Zee.eta();
	  ZeePhi_[nZee_]       = Zee.phi();
	  ZeeLeg1Index_[nZee_] = leg1Index;
	  ZeeLeg2Index_[nZee_] = leg2Index;

	  nZee_++;
	}

	leg2Index++;
      }

      leg1Index++;
    }
  }

  // Zmumu candidate
  // cout << "VgAnalyzerKit: produce: Zmumu candidate..." << endl;

  nZmumu_ = 0;
  leg1Index = 0;
  leg2Index = 0;
  if (zmumuHandle_.isValid() ) {
    // Store Zmumu candidates from the source.
    leg1Index = leg2Index = -1; // index -1 indicates an error
    // Loop over source Zmumu candidates
    for(CandidateView::const_iterator Zmumu = zmumuHandle_->begin();
        Zmumu != zmumuHandle_->end(); ++Zmumu) {
      ZmumuMass_[nZmumu_] = Zmumu->mass();
      ZmumuPt_  [nZmumu_] = Zmumu->pt();
      ZmumuEta_ [nZmumu_] = Zmumu->eta();
      ZmumuPhi_ [nZmumu_] = Zmumu->phi();
      // Get the leg indexes using pointer arithmetics.
      if (muonHandle_.isValid()) {
        // Get the pointer to the first muon
        const pat::Muon * muBegin = &*muonHandle_->begin();
        // Jump through 3 loops to get pointers to the daughters.
        const Candidate * dau1 = Zmumu->daughter(0);
        const Candidate * dau2 = Zmumu->daughter(1);
        if (dau1 == 0 || dau2 == 0)
          throw Exception(errors::InvalidReference) <<
            "One of the two daughters does not exist.\n";
        // Jump through the 2nd loop - get the master of the shallow clones.
        if (dau1->hasMasterClone() && dau2->hasMasterClone() ) {
          dau1 = dau1->masterClone().get();
          dau2 = dau2->masterClone().get();
        } else {
          throw Exception(errors::InvalidReference) <<
            "One of the two daughters is not a shallow clone.\n";
        }
        // Jump throught the 3rd loop - cast up the pointers.
        const pat::Muon * mu1 = dynamic_cast<const pat::Muon*>(dau1);
        const pat::Muon * mu2 = dynamic_cast<const pat::Muon*>(dau2);
        if (mu1 == 0 || mu2 == 0)
          throw Exception(errors::InvalidReference) <<
            "One of the two daughters is not a pat::Muon.\n";
        // Use pointer arithmetics to get the indexes.
        leg1Index = mu1 - muBegin;
        leg2Index = mu2 - muBegin;
        // Check if the indexes make sense.
        if (leg1Index < 0  || (int) muonHandle_->size() <= leg1Index ||
            leg2Index < 0  || (int) muonHandle_->size() <= leg2Index)
          throw Exception(errors::InvalidReference)
            << "One of the two daughters has illegal index.\n"
            << "  daughter 1 index: " << leg1Index << endl
            << "  daughter 2 index: " << leg2Index << endl
            << "  muon source size: " << muonHandle_->size() << endl;
      } // Get the leg indexes using pointer arithmetics.
      ZmumuLeg1Index_[nZmumu_] = leg1Index;
      ZmumuLeg2Index_[nZmumu_] = leg2Index;
      ++nZmumu_;
    } // Loop over source Zmumu candidates
  } // Store Zmumu candidates from the source.
  else if (muonHandle_.isValid() && muonHandle_->size() > 1) {
    // Build Zmumu candidate on the fly based on the source muons
    for (View<pat::Muon>::const_iterator iMu = muonHandle_->begin(); iMu != muonHandle_->end()-1; ++iMu) {

      if (!iMu->isGlobalMuon()) continue;
      if (!iMu->isTrackerMuon()) continue;
      if (iMu->globalTrack().isNull()) continue;
      if (iMu->innerTrack().isNull()) continue;

      for (View<pat::Muon>::const_iterator jMu = iMu+1; jMu != muonHandle_->end(); ++jMu) {

	if (!jMu->isGlobalMuon()) continue;
	if (!jMu->isTrackerMuon()) continue;
	if (jMu->globalTrack().isNull()) continue;
	if (jMu->innerTrack().isNull()) continue;

	if (iMu->charge() * jMu->charge() < 0 ) {
	  pat::CompositeCandidate Zmumu;
	  Zmumu.addDaughter(*iMu, "mu1");
	  Zmumu.addDaughter(*jMu, "mu2");

	  AddFourMomenta addZmumu;
	  addZmumu.set(Zmumu);
	  ZmumuMass_[nZmumu_]      = Zmumu.mass();
	  ZmumuPt_[nZmumu_]        = Zmumu.pt();
	  ZmumuEta_[nZmumu_]       = Zmumu.eta();
	  ZmumuPhi_[nZmumu_]       = Zmumu.phi();

          leg1Index = iMu-(muonHandle_->begin());
          leg2Index = jMu-(muonHandle_->begin());

          ZmumuLeg1Index_[nZmumu_] = leg1Index;
          ZmumuLeg2Index_[nZmumu_] = leg2Index;

	  nZmumu_++;
	}
      }
    }
  } // Build Zmumu candidate on the fly based on the source muons

  // Wenu candiate
  // cout << "VgAnalyzerKit: produce: Wenu candiate..." << endl;
  nWenu_ = 0;
  leg1Index = 0;
  if (electronHandle_.isValid() &&
      electronHandle_->size() > 0 &&
      METHandle_.isValid() &&
      METHandle_->size() > 0) {
    for (View<pat::Electron>::const_iterator iEle = electronHandle_->begin(); iEle != electronHandle_->end(); ++iEle) {

      for (View<pat::MET>::const_iterator iMET = METHandle_->begin(); iMET != METHandle_->end(); ++iMET) {

        pat::CompositeCandidate Wenu;
        Wenu.addDaughter(*iEle, "ele");
        Wenu.addDaughter(*iMET, "met");

        AddFourMomenta addWenu;
        addWenu.set(Wenu);
        WenuMassTCaloMET_[nWenu_] = massT(iEle->pt(), iMET->pt(), Wenu.px(), Wenu.py());
        WenuEtCaloMET_[nWenu_]    = eT(iEle->pt(), iMET->pt());
        WenuACopCaloMET_[nWenu_]  = acop(iEle->phi(), iMET->phi());
      }

      pat::CompositeCandidate WenuTcMET;
      WenuTcMET.addDaughter(*iEle, "ele");
      WenuTcMET.addDaughter(*tcMET, "met");

      AddFourMomenta addWenuTcMET;
      addWenuTcMET.set(WenuTcMET);
      WenuMassTTcMET_[nWenu_] = massT(iEle->pt(), tcMET->pt(), WenuTcMET.px(), WenuTcMET.py());
      WenuEtTcMET_[nWenu_]    = eT(iEle->pt(), tcMET->pt());
      WenuACopTcMET_[nWenu_]  = acop(iEle->phi(), tcMET->phi());

      pat::CompositeCandidate WenuPfMET;
      WenuPfMET.addDaughter(*iEle, "ele");
      WenuPfMET.addDaughter(*pfMET, "met");

      AddFourMomenta addWenuPfMET;
      addWenuPfMET.set(WenuPfMET);
      WenuMassTPfMET_[nWenu_] = massT(iEle->pt(), pfMET->pt(), WenuPfMET.px(), WenuPfMET.py());
      WenuEtPfMET_[nWenu_]    = eT(iEle->pt(), pfMET->pt());
      WenuACopPfMET_[nWenu_]  = acop(iEle->phi(), pfMET->phi());
      WenuEleIndex_[nWenu_]   = leg1Index;
      leg1Index++;
      nWenu_++;
    }
  }

  // Wmunu candiate
  // cout << "VgAnalyzerKit: produce: Wmunu candiate..." << endl;
  nWmunu_ = 0;
  leg1Index = 0;
  if (muonHandle_.isValid() &&
      muonHandle_->size() > 0 &&
      METHandle_.isValid() &&
      METHandle_->size() > 0) {
    for (View<pat::Muon>::const_iterator iMu = muonHandle_->begin(); iMu != muonHandle_->end(); ++iMu) {

      if (!iMu->isGlobalMuon()) continue;

      for (View<pat::MET>::const_iterator iMET = METHandle_->begin(); iMET != METHandle_->end(); ++iMET) {

        pat::CompositeCandidate Wmunu;
        Wmunu.addDaughter(*iMu, "mu");
        Wmunu.addDaughter(*iMET, "met");

        AddFourMomenta addWmunu;
        addWmunu.set(Wmunu);
        WmunuMassTCaloMET_[nWmunu_] = massT(iMu->pt(), iMET->pt(), Wmunu.px(), Wmunu.py());
        WmunuEtCaloMET_[nWmunu_]    = eT(iMu->pt(), iMET->pt());
        WmunuACopCaloMET_[nWmunu_]  = acop(iMu->phi(), iMET->phi());
      }

      pat::CompositeCandidate WmunuTcMET;
      WmunuTcMET.addDaughter(*iMu, "mu");
      WmunuTcMET.addDaughter(*tcMET, "met");

      AddFourMomenta addWmunuTcMET;
      addWmunuTcMET.set(WmunuTcMET);
      WmunuMassTTcMET_[nWmunu_] = massT(iMu->pt(), tcMET->pt(), WmunuTcMET.px(), WmunuTcMET.py());
      WmunuEtTcMET_[nWmunu_]    = eT(iMu->pt(), tcMET->pt());
      WmunuACopTcMET_[nWmunu_]  = acop(iMu->phi(), tcMET->phi());

      pat::CompositeCandidate WmunuPfMET;
      WmunuPfMET.addDaughter(*iMu, "mu");
      WmunuPfMET.addDaughter(*pfMET, "met");

      AddFourMomenta addWmunuPfMET;
      addWmunuPfMET.set(WmunuPfMET);
      WmunuMassTPfMET_[nWmunu_] = massT(iMu->pt(), pfMET->pt(), WmunuPfMET.px(), WmunuPfMET.py());
      WmunuEtPfMET_[nWmunu_]    = eT(iMu->pt(), pfMET->pt());
      WmunuACopPfMET_[nWmunu_]  = acop(iMu->phi(), pfMET->phi());

      leg1Index = iMu-(muonHandle_->begin());
      
      WmunuMuIndex_[nWmunu_]    = leg1Index;
      nWmunu_++;
    }
  }

  if ((nElePassCut > 0 || nMuPassCut > 0) && nPhoPassCut > 0) {
    hEvents_->Fill(1.5);
    tree_->Fill();
  }

}

void VgAnalyzerKit::beginJob() {
}

void VgAnalyzerKit::endJob() {
}

double VgAnalyzerKit::eT(double pt1, double pt2) const {
  double et = pt1 + pt2;
  return et;
}

double VgAnalyzerKit::massT(double pt1, double pt2, double wpx, double wpy) const {
  double mt = eT(pt1, pt2)*eT(pt1, pt2) - wpx*wpx - wpy*wpy;
  mt = (mt>0) ? sqrt(mt) : 0;
  return mt;
}

double VgAnalyzerKit::acop(double phi1, double phi2) const {
  Geom::Phi<double> deltaphi(phi1-phi2);
  double acop = deltaphi.value();
  if (acop<0) acop = - acop;
  acop = M_PI - acop;
  return acop;
}

DEFINE_FWK_MODULE(VgAnalyzerKit);
