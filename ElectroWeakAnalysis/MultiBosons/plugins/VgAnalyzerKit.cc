#include "ElectroWeakAnalysis/MultiBosons/interface/VgAnalyzerKit.h"

#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HepMCCandidate/interface/PdfInfo.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

using namespace std;
using namespace pat;
using namespace edm;
using namespace reco;

VgAnalyzerKit::VgAnalyzerKit(const edm::ParameterSet& ps) : verbosity_(0), helper_(ps) {

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

  ebReducedRecHitCollection_ = ps.getParameter<InputTag>("ebReducedRecHitCollection");
  eeReducedRecHitCollection_ = ps.getParameter<InputTag>("eeReducedRecHitCollection");

  if (saveHistograms_) helper_.bookHistos(this);

  Service<TFileService> fs;
  tree_ = fs->make<TTree>("EventTree", "Event data");

  tree_->Branch("run", &run_, "run/I");
  tree_->Branch("event", &event_, "event/I");
  tree_->Branch("orbit", &orbit_, "orbit/I");
  tree_->Branch("bx", &bx_, "bx/I");
  tree_->Branch("lumis", &lumis_, "lumis/I");
  tree_->Branch("isData", &isData_, "isData/O");
  tree_->Branch("ttbit", ttbit_, "ttbit[64]/I");
  tree_->Branch("HLT", HLT_, "HLT[102]/I");
  tree_->Branch("nHFTowersP", &nHFTowersP_, "nHFTowersP/I");
  tree_->Branch("nHFTowersN", &nHFTowersN_, "nHFTowersN/I");
  tree_->Branch("nVtx", &nVtx_, "nVtx/I");
  tree_->Branch("vtx", vtx_, "vtx[nVtx][3]/D");
  tree_->Branch("vtxNTrk", vtxNTrk_, "vtxNTrk[nVtx]/I");
  tree_->Branch("vtxNDF", vtxNDF_, "vtxNDF[nVtx]/I");
  tree_->Branch("vtxD0", vtxD0_, "vtxD0[nVtx]/D");
  tree_->Branch("IsVtxGood", &IsVtxGood_, "IsVtxGood/I");
  tree_->Branch("nTrk", &nTrk_, "nTrk/I");
  tree_->Branch("nGoodTrk", &nGoodTrk_, "nGoodTrk/I");
  tree_->Branch("IsTracksGood", &IsTracksGood_, "IsTracksGood/I");
  if (doGenParticles_) {
    tree_->Branch("pdf", pdf_, "pdf[7]/D"); 
    tree_->Branch("processID", &processID_, "processID/D");
    // genParticle
    tree_->Branch("nMC", &nMC_, "nMC/I");
    tree_->Branch("mcPID", mcPID, "mcPID[nMC]/I");
    tree_->Branch("mcPt", mcPt, "mcPt[nMC]/D");
    tree_->Branch("mcMass", mcMass, "mcMass[nMC]/D");
    tree_->Branch("mcEta", mcEta, "mcEta[nMC]/D");
    tree_->Branch("mcPhi", mcPhi, "mcPhi[nMC]/D");
    tree_->Branch("mcGMomPID", mcGMomPID, "mcGMomPID[nMC]/I");
    tree_->Branch("mcMomPID", mcMomPID, "mcMomPID[nMC]/I");
    tree_->Branch("mcMomPt", mcMomPt, "mcMomPt[nMC]/D");
    tree_->Branch("mcMomMass", mcMomMass, "mcMomMass[nMC]/D");
    tree_->Branch("mcMomEta", mcMomEta, "mcMomEta[nMC]/D");
    tree_->Branch("mcMomPhi", mcMomPhi, "mcMomPhi[nMC]/D");
    tree_->Branch("mcIndex", mcIndex, "mcIndex[nMC]/I");
    tree_->Branch("mcDecayType", mcDecayType, "mcDecayType[nMC]/I"); //-999:non W or Z, 1:hardronic, 2:e, 3:mu, 4:tau
    // Gen MET 
    tree_->Branch("genMET", &genMET_, "genMET/D");
    tree_->Branch("genMETx", &genMETx_, "genMETx/D");
    tree_->Branch("genMETy", &genMETy_, "genMETy/D");
    tree_->Branch("genMETPhi", &genMETPhi_, "genMETPhi/D");
  }
  // Calo MET
  tree_->Branch("MET", &MET_, "MET/D");
  tree_->Branch("METx", &METx_, "METx/D");
  tree_->Branch("METy", &METy_, "METy/D");
  tree_->Branch("METPhi", &METPhi_, "METPhi/D");
  tree_->Branch("METsumEt", &METsumEt_, "METsumEt/D");
  tree_->Branch("uncorrMET", uncorrMET_, "uncorrMET[3]/D"); // [0]: uncorrALL, [1]: uncorrJES, [2]: uncorrMUON
  tree_->Branch("uncorrMETPhi", uncorrMETPhi_, "uncorrMETPhi[3]/D"); 
  tree_->Branch("uncorrMETSumEt", uncorrMETSumEt_, "uncorrMETSumEt[3]/D"); 
  // tcMET
  tree_->Branch("tcMET", &tcMET_, "tcMET/D");
  tree_->Branch("tcMETx", &tcMETx_, "tcMETx/D");
  tree_->Branch("tcMETy", &tcMETy_, "tcMETy/D");
  tree_->Branch("tcMETPhi", &tcMETPhi_, "tcMETPhi/D");
  tree_->Branch("tcMETsumEt", &tcMETsumEt_, "tcMETsumEt/D");
  tree_->Branch("tcMETmEtSig", &tcMETmEtSig_, "tcMETmEtSig/D");
  tree_->Branch("tcMETSig", &tcMETSig_, "tcMETSig/D");
  // pfMET
  tree_->Branch("pfMET", &pfMET_, "pfMET/D");
  tree_->Branch("pfMETx", &pfMETx_, "pfMETx/D");
  tree_->Branch("pfMETy", &pfMETy_, "pfMETy/D");
  tree_->Branch("pfMETPhi", &pfMETPhi_, "pfMETPhi/D");
  tree_->Branch("pfMETsumEt", &pfMETsumEt_, "pfMETsumEt/D");
  tree_->Branch("pfMETmEtSig", &pfMETmEtSig_, "pfMETmEtSig/D");
  tree_->Branch("pfMETSig", &pfMETSig_, "pfMETSig/D");
  // Electron
  tree_->Branch("nEle", &nEle_, "nEle/I");
  tree_->Branch("eleID", eleID_, "eleID[nEle][5]/O"); // [0]: eidRobustLoose, [1]: eidRobustTight, [2]: eidLoose, [3]: eidTight, [4]: eidRobustHighEnergy
  tree_->Branch("eleClass", eleClass_, "eleClass[nEle]/I");
  tree_->Branch("eleCharge", eleCharge_, "eleCharge[nEle]/I");
  tree_->Branch("eleEn", eleEn_, "eleEn[nEle]/D");
  tree_->Branch("eleSCEta", eleSCEta_, "eleSCEta[nEle]/D");
  tree_->Branch("eleSCPhi", eleSCPhi_, "eleSCPhi[nEle]/D");
  tree_->Branch("eleSCRawEn", eleSCRawEn_, "eleSCRawEn[nEle]/D");
  tree_->Branch("eleESEn", eleESEn_, "eleESEn[nEle]/D");
  tree_->Branch("eleSCEn", eleSCEn_, "eleSCEn[nEle]/D");
  tree_->Branch("elePt", elePt_, "elePt[nEle]/D");
  tree_->Branch("elePz", elePz_, "elePz[nEle]/D");
  tree_->Branch("eleEta", eleEta_, "eleEta[nEle]/D");
  tree_->Branch("elePhi", elePhi_, "elePhi[nEle]/D");
  tree_->Branch("eleSCEta", eleSCEta_, "eleSCEta[nEle]/D");
  tree_->Branch("eleSCPhi", eleSCPhi_, "eleSCPhi[nEle]/D");
  tree_->Branch("eleSCEtaWidth", eleSCEtaWidth_, "eleSCEtaWidth[nEle]/D");
  tree_->Branch("eleSCPhiWidth", eleSCPhiWidth_, "eleSCPhiWidth[nEle]/D");
  tree_->Branch("eleVtx", eleVtx_, "eleVtxX[nEle][3]/D");
  tree_->Branch("eleCaloPos", eleCaloPos_ ,"eleCaloPos[nEle][3]/D");
  tree_->Branch("eleSCPos", eleSCPos_, "eleSCPos[nEle][3]/D");
  tree_->Branch("eleHoverE", eleHoverE_, "eleHoverE[nEle]/D");
  tree_->Branch("eleHoverE1", eleHoverE1_, "eleHoverE1[nEle]/D");
  tree_->Branch("eleHoverE2", eleHoverE2_, "eleHoverE2[nEle]/D");
  tree_->Branch("eleEoverP", eleEoverP_, "eleEoverP[nEle]/D");
  tree_->Branch("elePin", elePin_, "elePin[nEle]/D");
  tree_->Branch("elePout", elePout_, "elePout[nEle]/D");
  tree_->Branch("eleBrem", eleBrem_, "eleBrem[nEle]/D");
  tree_->Branch("eledEtaAtVtx", eledEtaAtVtx_, "eledEtaAtVtx[nEle]/D");
  tree_->Branch("eledPhiAtVtx", eledPhiAtVtx_, "eledPhiAtVtx[nEle]/D");
  tree_->Branch("eleSigmaEtaEta", eleSigmaEtaEta_, "eleSigmaEtaEta[nEle]/D");
  tree_->Branch("eleSigmaIEtaIEta", eleSigmaIEtaIEta_, "eleSigmaIEtaIEta[nEle]/D");
  tree_->Branch("eleEMax", eleEMax_, "eleEMax_[nEle]/D");
  tree_->Branch("eleE2nd", eleE2nd_, "eleE2nd_[nEle]/D");
  tree_->Branch("eleE2x2", eleE2x2_, "eleE2x2_[nEle]/D");
  tree_->Branch("eleE3x2", eleE3x2_, "eleE3x2_[nEle]/D");
  tree_->Branch("eleE3x3", eleE3x3_, "eleE3x3_[nEle]/D");
  tree_->Branch("eleE4x4", eleE4x4_, "eleE4x4_[nEle]/D");
  tree_->Branch("eleE5x5", eleE5x5_, "eleE5x5_[nEle]/D");
  tree_->Branch("eleE2x5Right", eleE2x5Right_, "eleE2x5Right_[nEle]/D");
  tree_->Branch("eleE2x5Left", eleE2x5Left_, "eleE2x5Left_[nEle]/D");
  tree_->Branch("eleE2x5Top", eleE2x5Top_, "eleE2x5Top_[nEle]/D");
  tree_->Branch("eleE2x5Bottom", eleE2x5Bottom_, "eleE2x5Bottom_[nEle]/D");
  tree_->Branch("eleERight", eleERight_, "eleERight_[nEle]/D");
  tree_->Branch("eleELeft", eleELeft_, "eleELeft_[nEle]/D");
  tree_->Branch("eleETop", eleETop_, "eleETop_[nEle]/D");
  tree_->Branch("eleEBottom", eleEBottom_, "eleEBottom_[nEle]/D");
  if (doGenParticles_) {
    tree_->Branch("eleGenIndex", eleGenIndex_, "eleGenIndex[nEle]/I");
    tree_->Branch("eleGenGMomPID", eleGenGMomPID_, "eleGenGMomPID[nEle]/I");
    tree_->Branch("eleGenMomPID", eleGenMomPID_, "eleGenMomPID[nEle]/I");
    tree_->Branch("eleGenMomPt", eleGenMomPt_, "eleGenMomPt[nEle]/D");
  }
  tree_->Branch("eleIsoTrkDR03", eleIsoTrkDR03_, "eleIsoTrkDR03[nEle]/D");
  tree_->Branch("eleIsoEcalDR03", eleIsoEcalDR03_, "eleIsoEcalDR03[nEle]/D");
  tree_->Branch("eleIsoHcalDR03", eleIsoHcalDR03_, "eleIsoHcalDR03[nEle]/D");
  tree_->Branch("eleIsoTrkDR04", eleIsoTrkDR04_, "eleIsoTrkDR04[nEle]/D");
  tree_->Branch("eleIsoEcalDR04", eleIsoEcalDR04_, "eleIsoEcalDR04[nEle]/D");
  tree_->Branch("eleIsoHcalDR04", eleIsoHcalDR04_, "eleIsoHcalDR04[nEle]/D");
  tree_->Branch("eleChi2NDF", eleChi2NDF_, "eleChi2NDF[nEle]/D");
  tree_->Branch("eleD0", eleD0_, "eleD0[nEle]/D");
  tree_->Branch("eleNumberOfValidHits", eleNumberOfValidHits_, "eleNumberOfValidHits[nEle]/I");
  // Photon
  tree_->Branch("nPho", &nPho_, "nPho/I");
  tree_->Branch("phoIsPhoton", phoIsPhoton_, "phoIsPhoton/O");
  tree_->Branch("phoE", phoE_, "phoE[nPho]/D");
  tree_->Branch("phoEt", phoEt_, "phoEt[nPho]/D");
  tree_->Branch("phoPz", phoPz_, "phoPz[nPho]/D");
  tree_->Branch("phoEta", phoEta_, "phoEta[nPho]/D");
  tree_->Branch("phoPhi", phoPhi_, "phoPhi[nPho]/D");
  tree_->Branch("phoR9", phoR9_, "phoR9[nPho]/D");
  tree_->Branch("phoTrkIsoSolidDR03", phoTrkIsoSolidDR03_, "phoTrkIsoSolidDR03[nPho]/D");
  tree_->Branch("phoTrkIsoHollowDR03", phoTrkIsoHollowDR03_, "phoTrkIsoHollowDR03[nPho]/D");
  tree_->Branch("phoNTrkSolidDR03", phoNTrkSolidDR03_, "phoNTrkSolidDR03[nPho]/I");
  tree_->Branch("phoNTrkHollowDR03", phoNTrkHollowDR03_, "phoNTrkHollowDR03[nPho]/I");
  tree_->Branch("phoEcalIsoDR03", phoEcalIsoDR03_, "phoEcalIsoDR03[nPho]/D");
  tree_->Branch("phoHcalIsoDR03", phoHcalIsoDR03_, "phoHcalIsoDR03[nPho]/D");
  tree_->Branch("phoTrkIsoSolidDR04", phoTrkIsoSolidDR04_, "phoTrkIsoSolidDR04[nPho]/D");
  tree_->Branch("phoTrkIsoHollowDR04", phoTrkIsoHollowDR04_, "phoTrkIsoHollowDR04[nPho]/D");
  tree_->Branch("phoNTrkSolidDR04", phoNTrkSolidDR04_, "phoNTrkSolidDR04[nPho]/I");
  tree_->Branch("phoNTrkHollowDR04", phoNTrkHollowDR04_, "phoNTrkHollowDR04[nPho]/I");
  tree_->Branch("phoEcalIsoDR04", phoEcalIsoDR04_, "phoEcalIsoDR04[nPho]/D");
  tree_->Branch("phoHcalIsoDR04", phoHcalIsoDR04_, "phoHcalIsoDR04[nPho]/D");
  tree_->Branch("phoHoverE", phoHoverE_, "phoHoverE[nPho]/D");
  tree_->Branch("phoHoverE1", phoHoverE1_, "phoHoverE1[nPho]/D");
  tree_->Branch("phoHoverE2", phoHoverE2_, "phoHoverE2[nPho]/D");
  tree_->Branch("phoSigmaEtaEta", phoSigmaEtaEta_, "phoSigmaEtaEta[nPho]/D");
  tree_->Branch("phoSigmaIEtaIEta", phoSigmaIEtaIEta_, "phoSigmaIEtaIEta[nPho]/D");
  tree_->Branch("phoSeedTime", phoSeedTime_, "phoSeedTime[nPho]/D");
  tree_->Branch("phoPos", phoPos_, "phoPos[nPho]/I");
  tree_->Branch("phoEMax", phoEMax_, "phoEMax_[nPho]/D");
  tree_->Branch("phoE2nd", phoE2nd_, "phoE2nd_[nPho]/D");
  tree_->Branch("phoE2x2", phoE2x2_, "phoE2x2_[nPho]/D");
  tree_->Branch("phoE3x2", phoE3x2_, "phoE3x2_[nPho]/D");
  tree_->Branch("phoE3x3", phoE3x3_, "phoE3x3_[nPho]/D");
  tree_->Branch("phoE4x4", phoE4x4_, "phoE4x4_[nPho]/D");
  tree_->Branch("phoE5x5", phoE5x5_, "phoE5x5_[nPho]/D");
  tree_->Branch("phoE2x5Right", phoE2x5Right_, "phoE2x5Right_[nPho]/D");
  tree_->Branch("phoE2x5Left", phoE2x5Left_, "phoE2x5Left_[nPho]/D");
  tree_->Branch("phoE2x5Top", phoE2x5Top_, "phoE2x5Top_[nPho]/D");
  tree_->Branch("phoE2x5Bottom", phoE2x5Bottom_, "phoE2x5Bottom_[nPho]/D");
  tree_->Branch("phoERight", phoERight_, "phoERight_[nPho]/D");
  tree_->Branch("phoELeft", phoELeft_, "phoELeft_[nPho]/D");
  tree_->Branch("phoETop", phoETop_, "phoETop_[nPho]/D");
  tree_->Branch("phoEBottom", phoEBottom_, "phoEBottom_[nPho]/D");
  if (doGenParticles_) {
    tree_->Branch("phoGenIndex", phoGenIndex_, "phoGenIndex[nPho]/I");
    tree_->Branch("phoGenGMomPID", phoGenGMomPID, "phoGenGMomPID[nPho]/I");
    tree_->Branch("phoGenMomPID", phoGenMomPID, "phoGenMomPID[nPho]/I");
    tree_->Branch("phoGenMomPt", phoGenMomPt, "phoGenMomPt[nPho]/D");
  }
  tree_->Branch("phoSCE", phoSCE_, "phoSCE[nPho]/D");
  tree_->Branch("phoSCEt", phoSCEt_, "phoSCEt[nPho]/D");
  tree_->Branch("phoSCEta", phoSCEta_, "phoSCEta[nPho]/D");
  tree_->Branch("phoSCPhi", phoSCPhi_, "phoSCPhi[nPho]/D");
  tree_->Branch("phoSCEtaWidth", phoSCEtaWidth_, "phoSCEtaWidth[nPho]/D");
  tree_->Branch("phoSCPhiWidth", phoSCPhiWidth_, "phoSCPhiWidth[nPho]/D");
  tree_->Branch("phoOverlap", phoOverlap_, "phoOverlap[nPho]/I");
  // Muon
  tree_->Branch("nMu", &nMu_, "nMu/I");
  tree_->Branch("muEta", muEta_, "muEta[nMu]/D");
  tree_->Branch("muPhi", muPhi_, "muPhi[nMu]/D");
  tree_->Branch("muCharge", muCharge_, "muCharge[nMu]/I");
  tree_->Branch("muPt", muPt_, "muPt[nMu]/D");
  tree_->Branch("muPz", muPz_, "muPz[nMu]/D");
  if (doGenParticles_)
    tree_->Branch("muGenIndex", muGenIndex_, "muGenIndex[nMu]/I");
  tree_->Branch("muIsoTrk", muIsoTrk_, "muIsoTrk[nMu]/D");
  tree_->Branch("muIsoCalo", muIsoCalo_, "muIsoCalo[nMu]/D");
  tree_->Branch("muIsoEcal", muIsoEcal_, "muIsoEcal[nMu]/D");
  tree_->Branch("muIsoHcal", muIsoHcal_, "muIsoHcal[nMu]/D");
  tree_->Branch("muEmVeto", muEmVeto_, "muEmVeto[nMu]/D");
  tree_->Branch("muHadVeto", muHadVeto_, "muHadVeto[nMu]/D");
  tree_->Branch("muType", muType_, "muType[nMu]/I");
  tree_->Branch("muID", muID, "muID[nMu][6]/O");
  // [0]: AllArbitrated, [1]: GlobalMuonPromptTight, [2]: TMLSLoose, [3]: TMLSTight, [4]: TM2DCompatLoose, [5]: TM2DCompatTight
  tree_->Branch("muD0", muD0_, "muD0[nMu]/D");
  tree_->Branch("muNumberOfValidHits", muNumberOfValidHits_, "muNumberOfValidHits[nMu]/I");
  // Jet
  tree_->Branch("nJet", &nJet_, "nJet/I");
  tree_->Branch("jetEn", jetEn_, "jetEn[nJet]/D");
  tree_->Branch("jetPt", jetPt_, "jetPt[nJet]/D");
  tree_->Branch("jetEta", jetEta_, "jetEta[nJet]/D");
  tree_->Branch("jetPhi", jetPhi_, "jetPhi[nJet]/D");
  tree_->Branch("jetMass", jetMass_, "jetMass[nJet]/D");
  tree_->Branch("jetEt", jetEt_, "jetEt[nJet]/D");
  tree_->Branch("jetmaxEInEmTowers", jetmaxEInEmTowers_, "jetmaxEInEmTowers[nJet]/D");
  tree_->Branch("jetmaxEInHadTowers", jetmaxEInHadTowers_, "jetmaxEInHadTowers[nJet]/D");
  tree_->Branch("jetenergyFractionHadronic", jetenergyFractionHadronic_, "jetenergyFractionHadronic[nJet]/D");
  tree_->Branch("jetemEnergyFraction", jetemEnergyFraction_, "jetemEnergyFraction[nJet]/D");
  if (doGenParticles_) {
    tree_->Branch("jetGenIndex", jetGenIndex_, "jetGenIndex[nJet]/I");
    tree_->Branch("jetGenJetIndex", jetGenJetIndex_, "jetGenJetIndex[nJet]/I");
    tree_->Branch("jetGenJetEn", jetGenJetEn_, "jetGenJetEn[nJet]/D");
    tree_->Branch("jetGenJetPt", jetGenJetPt_, "jetGenJetPt[nJet]/D");
    tree_->Branch("jetGenJetEta", jetGenJetEta_, "jetGenJetEta[nJet]/D");
    tree_->Branch("jetGenJetPhi", jetGenJetPhi_, "jetGenJetPhi[nJet]/D");
    tree_->Branch("jetGenJetMass", jetGenJetMass_, "jetGenJetMass[nJet]/D");
    tree_->Branch("jetGenPartonID", jetGenPartonID_, "jetGenPartonID[nJet]/I");
    tree_->Branch("jetGenPartonMomID", jetGenPartonMomID_, "jetGenPartonMomID[nJet]/I");
    tree_->Branch("jetGenEn", jetGenEn_, "jetGenEn[nJet]/D");
    tree_->Branch("jetGenPt", jetGenPt_, "jetGenPt[nJet]/D");
    tree_->Branch("jetGenEta", jetGenEta_, "jetGenEta[nJet]/D");
    tree_->Branch("jetGenPhi", jetGenPhi_, "jetGenPhi[nJet]/D");
  }
  tree_->Branch("jetpartonFlavour", jetpartonFlavour_, "jetpartonFlavour[nJet]/I");
  tree_->Branch("jetRawPt", jetRawPt_, "jetRawPt[nJet]/D");
  tree_->Branch("jetRawEn", jetRawEn_, "jetRawEn[nJet]/D");
  tree_->Branch("jetCharge", jetCharge_, "jetCharge[nJet]/D");
  tree_->Branch("jetCombinedSVBJetTags", jetCombinedSVBJetTags_, "jetCombinedSVBJetTags[nJet]/D");
  tree_->Branch("jetCombinedSVMVABJetTags", jetCombinedSVMVABJetTags_, "jetCombinedSVMVABJetTags[nJet]/D");
  tree_->Branch("jetConeIsoTauJetTags", jetConeIsoTauJetTags_, "jetConeIsoTauJetTags[nJet]/D");
  tree_->Branch("jetImpactParaMVABJetTags", jetImpactParaMVABJetTags_, "jetImpactParaMVABJetTags[nJet]/D");
  tree_->Branch("jetJetBProbBJetTags", jetJetBProbBJetTags_, "jetJetBProbBJetTags[nJet]/D");
  tree_->Branch("jetJetProbBJetTags", jetJetProbBJetTags_, "jetJetProbBJetTags[nJet]/D");
  tree_->Branch("jetSimpleSVBJetTags", jetSimpleSVBJetTags_, "jetSimpleSVBJetTags[nJet]/D");
  tree_->Branch("jetSoftElecBJetTags", jetSoftElecBJetTags_, "jetSoftElecBJetTags[nJet]/D");
  tree_->Branch("jetSoftMuonBJetTags", jetSoftMuonBJetTags_, "jetSoftMuonBJetTags[nJet]/D");
  tree_->Branch("jetSoftMuonNoIPBJetTags", jetSoftMuonNoIPBJetTags_, "jetSoftMuonNoIPBJetTags[nJet]/D");
  tree_->Branch("jetTrackCountHiEffBJetTags", jetTrackCountHiEffBJetTags_, "jetTrackCountHiEffBJetTags[nJet]/D");
  tree_->Branch("jetTrackCountHiPurBJetTags", jetTrackCountHiPurBJetTags_, "jetTrackCountHiPurBJetTags[nJet]/D");
  tree_->Branch("jetJetLRval", jetJetLRval_, "jetJetLRval[nJet]/D");
  tree_->Branch("jetJetProb", jetJetProb_, "jetJetProb[nJet]/D");

}

VgAnalyzerKit::~VgAnalyzerKit() {
}

void VgAnalyzerKit::produce(edm::Event & e, const edm::EventSetup & es) {

  helper_.getHandles(e, muonHandle_, electronHandle_, tauHandle_, jetHandle_, METHandle_, photonHandle_, trackHandle_, genParticlesHandle_);
  if (saveHistograms_) helper_.fillHistograms(e, muonHandle_, electronHandle_, tauHandle_, jetHandle_, METHandle_, photonHandle_, trackHandle_, genParticlesHandle_);

  Handle<edm::View<reco::RecoCandidate> > others;
  e.getByLabel("cleanLayer1Electrons", others);

  Handle<EcalRecHitCollection> EBReducedRecHits;
  e.getByLabel(ebReducedRecHitCollection_, EBReducedRecHits);
  Handle<EcalRecHitCollection> EEReducedRecHits;
  e.getByLabel(eeReducedRecHitCollection_, EEReducedRecHits);
  EcalClusterLazyTools lazyTool(e, es, ebReducedRecHitCollection_, eeReducedRecHitCollection_ );
  
  //Handle<int> genProcessID;
  //e.getByLabel("genEventProcID", genProcessID);
  //processID_ = *genProcessID;
  
  run_    = e.id().run();
  event_  = e.id().event();
  orbit_  = e.orbitNumber();
  bx_     = e.bunchCrossing();
  lumis_  = e.luminosityBlock();
  isData_ = e.isRealData();

  // technical trigger
  Handle< L1GlobalTriggerReadoutRecord > gtReadoutRecord;
  if (e.getByLabel(gtdigilabel_, gtReadoutRecord) ) {

    const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = gtReadoutRecord->technicalTriggerWord();

    for (int i=0; i<64; ++i) {
      if (technicalTriggerWordBeforeMask.at(i)) ttbit_[i] = 1;
      else ttbit_[i] = 0;     
    }
  }

  // Get CaloTower information
  edm::Handle<CaloTowerCollection> pCaloTower;
  e.getByLabel(caloTowerlabel_, pCaloTower);

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
	nVtx_++;

	if (vtxNDF_[nVtx_] > 4 && fabs(vtx_[nVtx_][2]) <= 15 && vtxD0_[nVtx_] <= 2) nGoodVtx++;
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
    if (((float)nGoodTrk_/(float)nTrk_) > 0.25) IsTracksGood_ = 0;
  } else {
    IsTracksGood_ = 0;
  }

  // PDF information
  if (!isData_) {
    Handle<PdfInfo> pdfInfoHandle;
    bool pdfInfo = e.getByLabel("genEventPdfInfo", pdfInfoHandle);
    if (pdfInfo) {
      pdf_[0] = pdfInfoHandle->id1;      // PDG ID of incoming parton #1
      pdf_[1] = pdfInfoHandle->id2;      // PDG ID of incoming parton #2
      pdf_[2] = pdfInfoHandle->x1;       // x value of parton #1
      pdf_[3] = pdfInfoHandle->x2;       // x value of parton #2
      pdf_[4] = pdfInfoHandle->pdf1;     // PDF weight for parton #1
      pdf_[5] = pdfInfoHandle->pdf2;     // PDF weight for parton #2
      pdf_[6] = pdfInfoHandle->scalePDF; // scale of the hard interaction
    } 
  }

  // GenParticle
  nMC_ = 0;
  int genIndex = 0;
  const Candidate *mom = 0;

  if (!isData_) {
    for (vector<GenParticle>::const_iterator ip = genParticlesHandle_->begin(); ip != genParticlesHandle_->end(); ++ip) {
      
      genIndex++;
      
      if ((ip->status()==3 && (ip->pdgId()==23 || fabs(ip->pdgId())==24 || fabs(ip->pdgId())==7 || fabs(ip->pdgId())==4)) || (ip->status()==1 && ((fabs(ip->pdgId())>=11 && fabs(ip->pdgId())<=14) || ip->pdgId()==22))) {
	
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
  if (saveHLTInfo_) {
    Handle<TriggerResults> trgResultsHandle;
    e.getByLabel(trgResults_, trgResultsHandle);
    TriggerNames trgNames(*trgResultsHandle);
    vector<string> hlNames_ = trgNames.triggerNames();
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
  int nMET = 0;
  for (vector<pat::MET>::const_iterator iMET = METHandle_->begin(); iMET != METHandle_->end(); ++iMET) {
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
  Handle<reco::METCollection> tcMETcoll;
  if (e.getByLabel(tcMETlabel_, tcMETcoll)) {
    const reco::METCollection *tcMETcol = tcMETcoll.product();
    const reco::MET *tcMET;
    tcMET = &(tcMETcol->front());

    tcMET_       = tcMET->pt();
    tcMETx_      = tcMET->px();
    tcMETy_      = tcMET->py();
    tcMETPhi_    = tcMET->phi();
    tcMETsumEt_  = tcMET->sumEt();
    tcMETmEtSig_ = tcMET->mEtSig();
    tcMETSig_    = tcMET->significance();
  }

  // pfMET
  Handle<reco::PFMETCollection> pfMETcoll;
  if (e.getByLabel(pfMETlabel_, pfMETcoll)) {
    const reco::PFMETCollection *pfMETcol = pfMETcoll.product();
    const reco::PFMET *pfMET;
    pfMET = &(pfMETcol->front());

    pfMET_       = pfMET->pt();
    pfMETx_      = pfMET->px();
    pfMETy_      = pfMET->py();
    pfMETPhi_    = pfMET->phi();
    pfMETsumEt_  = pfMET->sumEt();
    pfMETmEtSig_ = pfMET->mEtSig();
    pfMETSig_    = pfMET->significance();
  }

  // Electron
  nEle_ = 0;
  const Candidate *elemom = 0;
  for (vector<pat::Electron>::const_iterator iEle = electronHandle_->begin(); iEle != electronHandle_->end(); ++iEle) {
   
    //if (iEle->pt()<10) continue;

    for (int i=0; i<5; ++i) eleID_[nEle_][i] = 0;
    if (iEle->electronID("eidRobustLoose")==1) eleID_[nEle_][0] = 1;
    if (iEle->electronID("eidRobustTight")==1)  eleID_[nEle_][1] = 1;
    if (iEle->electronID("eidLoose")==1)  eleID_[nEle_][2] = 1;
    if (iEle->electronID("eidTight")==1)  eleID_[nEle_][3] = 1;
    if (iEle->electronID("eidRobustHighEnergy")==1)  eleID_[nEle_][4] = 1;

    eleClass_[nEle_]   = iEle->classification();
    eleCharge_[nEle_]  = iEle->charge();
    eleEn_[nEle_]      = iEle->energy();
    elePt_[nEle_]      = iEle->pt();
    elePz_[nEle_]      = iEle->pz();
    eleEta_[nEle_]     = iEle->eta();
    elePhi_[nEle_]     = iEle->phi();
    eleHoverE_[nEle_]  = iEle->hadronicOverEm();
    eleHoverE1_[nEle_] = iEle->hadronicOverEm1();
    eleHoverE2_[nEle_] = iEle->hadronicOverEm2();
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
    eleE2nd_[nEle_] = lazyTool.e2nd(*eleSeed);
    eleE2x2_[nEle_] = lazyTool.e2x2(*eleSeed);
    eleE3x2_[nEle_] = lazyTool.e3x2(*eleSeed);
    eleE3x3_[nEle_] = lazyTool.e3x3(*eleSeed);
    eleE4x4_[nEle_] = lazyTool.e4x4(*eleSeed);
    eleE5x5_[nEle_] = lazyTool.e5x5(*eleSeed);

    eleE2x5Right_[nEle_]  = lazyTool.e2x5Right(*eleSeed);
    eleE2x5Left_[nEle_]   = lazyTool.e2x5Left(*eleSeed);
    eleE2x5Top_[nEle_]    = lazyTool.e2x5Top(*eleSeed);
    eleE2x5Bottom_[nEle_] = lazyTool.e2x5Bottom(*eleSeed);
    eleERight_[nEle_]     = lazyTool.eRight(*eleSeed);
    eleELeft_[nEle_]      = lazyTool.eLeft(*eleSeed);
    eleETop_[nEle_]       = lazyTool.eTop(*eleSeed);
    eleEBottom_[nEle_]    = lazyTool.eBottom(*eleSeed);

    // Gen Particle
    eleGenIndex_[nEle_] = -1;
    int EleGenIndex = 0;
    if (!isData_) {
      if ((*iEle).genLepton()) {
	 for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen != genParticlesHandle_->end(); ++iGen) {
	   if (iGen->p4() == (*iEle).genLepton()->p4() && iGen->pdgId() == (*iEle).genLepton()->pdgId() && iGen->status() == (*iEle).genLepton()->status()) {
              eleGenIndex_[nEle_] = EleGenIndex;
	    
	      EleGenIndex++;

              const Candidate *elep = (const Candidate*)&(*iGen);

              for (size_t j=0; j<elep->numberOfMothers(); ++j) {
                elemom = elep->mother(j);
                eleGenMomPID_[nEle_] = elemom->pdgId();
                eleGenMomPt_[nEle_] = elemom->pt();
                if (elemom->mother()) eleGenGMomPID_[nEle_] = elemom->mother()->pdgId();
              }
           }
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
    eleNumberOfValidHits_[nEle_] = (*iEle).gsfTrack()->numberOfValidHits();

    nEle_++;
  }

  // Photon
  const Candidate *phomom = 0;
  nPho_ = 0;
  for (vector<pat::Photon>::const_iterator iPho = photonHandle_->begin(); iPho != photonHandle_->end(); ++iPho) {

    phoIsPhoton_[nPho_] = iPho->isPhoton();
    phoE_[nPho_]   = iPho->energy();
    phoEt_[nPho_]  = iPho->et();       
    phoPz_[nPho_]  = iPho->pz();
    phoEta_[nPho_] = iPho->eta();
    phoPhi_[nPho_] = iPho->phi();

    phoR9_[nPho_]          = iPho->r9();
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
    phoHoverE1_[nPho_]       = iPho->hadronicDepth1OverEm();
    phoHoverE2_[nPho_]       = iPho->hadronicDepth2OverEm();
    phoSigmaEtaEta_[nPho_]   = iPho->sigmaEtaEta();
    phoSigmaIEtaIEta_[nPho_] = iPho->sigmaIetaIeta();

    phoOverlap_[nPho_] = (int) iPho->hasOverlaps("electrons");

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

    if (iPho->isEB()) { 
      EcalRecHitCollection::const_iterator ebrhit = EBReducedRecHits->find(phoSeedDetId);
      if ( ebrhit != EBReducedRecHits->end() ) { phoSeedTime_[nPho_] = ebrhit->time(); }
    } else {
      EcalRecHitCollection::const_iterator eerhit = EEReducedRecHits->find(phoSeedDetId);
      if ( eerhit != EEReducedRecHits->end() ) { phoSeedTime_[nPho_] = eerhit->time(); }
    }

    phoEMax_[nPho_] = lazyTool.eMax(*phoSeed);
    phoE2nd_[nPho_] = lazyTool.e2nd(*phoSeed);
    phoE2x2_[nPho_] = lazyTool.e2x2(*phoSeed);
    phoE3x2_[nPho_] = lazyTool.e3x2(*phoSeed);
    phoE3x3_[nPho_] = lazyTool.e3x3(*phoSeed);
    phoE4x4_[nPho_] = lazyTool.e4x4(*phoSeed);
    phoE5x5_[nPho_] = lazyTool.e5x5(*phoSeed);

    phoE2x5Right_[nPho_]  = lazyTool.e2x5Right(*phoSeed);
    phoE2x5Left_[nPho_]   = lazyTool.e2x5Left(*phoSeed);
    phoE2x5Top_[nPho_]    = lazyTool.e2x5Top(*phoSeed);
    phoE2x5Bottom_[nPho_] = lazyTool.e2x5Bottom(*phoSeed);
    phoERight_[nPho_]     = lazyTool.eRight(*phoSeed);
    phoELeft_[nPho_]      = lazyTool.eLeft(*phoSeed);
    phoETop_[nPho_]       = lazyTool.eTop(*phoSeed);
    phoEBottom_[nPho_]    = lazyTool.eBottom(*phoSeed);

    // Gen Particle
    phoGenIndex_[nPho_]  = -999; 
    phoGenMomPID[nPho_]  = -999;
    phoGenMomPt[nPho_]   = -999;
    phoGenGMomPID[nPho_] = -999;
    int phoGenIndex = 0;
    if (!isData_) {
      if ((*iPho).genPhoton()) {
	for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen != genParticlesHandle_->end(); ++iGen) {
	  
	  if (iGen->p4() == (*iPho).genPhoton()->p4() && iGen->pdgId() == (*iPho).genPhoton()->pdgId() && iGen->status() == (*iPho).genPhoton()->status()) {
	    
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
  
    // Super Cluster
    phoSCE_[nPho_]   = (*iPho).superCluster()->energy();
    phoSCEta_[nPho_] = (*iPho).superCluster()->eta();
    phoSCPhi_[nPho_] = (*iPho).superCluster()->phi();
    phoSCEt_[nPho_]  = (*iPho).superCluster()->energy()/cosh(phoSCEta_[nPho_]);
    phoSCEtaWidth_[nPho_] = (*iPho).superCluster()->etaWidth();
    phoSCPhiWidth_[nPho_] = (*iPho).superCluster()->phiWidth();

    nPho_++; 
  }

  // Muon
  nMu_ = 0;
  for (vector<pat::Muon>::const_iterator iMu = muonHandle_->begin(); iMu != muonHandle_->end(); ++iMu) {

    if (!iMu->isGlobalMuon()) continue;
    //if (iMu->pt()<10) continue;

    if (iMu->isGood("AllArbitrated")==1) muID[nMu_][0] = 1;
    else muID[nMu_][0] = 0;
    if (iMu->isGood("GlobalMuonPromptTight")==1) muID[nMu_][1] = 1;
    else muID[nMu_][1] = 0;
    if (iMu->isGood("TMLastStationLoose")==1) muID[nMu_][2] = 1;
    else muID[nMu_][2] = 0;
    if (iMu->isGood("TMLastStationTight")==1) muID[nMu_][3] = 1;
    else muID[nMu_][3] = 0;
    if (iMu->isGood("TM2DCompatibilityLoose")==1) muID[nMu_][4] = 1;
    else muID[nMu_][4] = 0;
    if (iMu->isGood("TM2DCompatibilityTight")==1) muID[nMu_][5] = 1;
    else muID[nMu_][5] = 0;

    const reco::TrackRef trkr = iMu->track();
    if (trkr.isNull()) {
      muD0_[nMu_] = -99.;
      muNumberOfValidHits_[nMu_] = -99;
    } else {
      muD0_[nMu_] = (*iMu).innerTrack()->d0();
      muNumberOfValidHits_[nMu_] = (*iMu).innerTrack()->numberOfValidHits();
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
      if ((*iMu).genLepton()) {
	if (fabs((*iMu).genLepton()->pdgId())==13) {
	  for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen !=
		 genParticlesHandle_->end(); ++iGen) {
	    
	    if (iGen->p4() == (*iMu).genLepton()->p4() && iGen->pdgId() == (*iMu).genLepton()->pdgId() && iGen->status() == (*iMu).genLepton()->status()) muGenIndex_[nMu_] = MuGenIndex;
	    MuGenIndex++;
	  }
	}
      }
    }

    nMu_++;

  }

  // Jet
  nJet_ = 0;
  for (vector<pat::Jet>::const_iterator iJet = jetHandle_->begin(); iJet != jetHandle_->end(); ++iJet) {

    if ( iJet->pt() < 15 ) continue;

    jetEn_[nJet_]     = iJet->energy();
    jetPt_[nJet_]     = iJet->pt();
    jetEta_[nJet_]    = iJet->eta();
    jetPhi_[nJet_]    = iJet->phi();
    jetMass_[nJet_]   = iJet->mass();
    jetCharge_[nJet_] = iJet->jetCharge();
    jetEt_[nJet_]     = iJet->et();
    jetRawPt_[nJet_]  = (*iJet).correctedJet("RAW").pt();
    jetRawEn_[nJet_]  = (*iJet).correctedJet("RAW").energy();
    jetpartonFlavour_[nJet_] = iJet->partonFlavour();

    // b tagging information
    jetCombinedSVBJetTags_[nJet_] = iJet->bDiscriminator("combinedSecondaryVertexBJetTags");
    jetCombinedSVMVABJetTags_[nJet_] = iJet->bDiscriminator("combinedSecondaryVertexMVABJetTags");
    jetConeIsoTauJetTags_[nJet_] = iJet->bDiscriminator("coneIsolationTauJetTags");
    jetImpactParaMVABJetTags_[nJet_] = iJet->bDiscriminator("impactParameterMVABJetTags");
    jetJetBProbBJetTags_[nJet_] = iJet->bDiscriminator("jetBProbabilityBJetTags");
    jetJetProbBJetTags_[nJet_] = iJet->bDiscriminator("jetProbabilityBJetTags");
    jetSimpleSVBJetTags_[nJet_] = iJet->bDiscriminator("simpleSecondaryVertexBJetTags");
    jetSoftElecBJetTags_[nJet_] = iJet->bDiscriminator("softElectronBJetTags");
    jetSoftMuonBJetTags_[nJet_] = iJet->bDiscriminator("softMuonBJetTags");
    jetSoftMuonNoIPBJetTags_[nJet_] = iJet->bDiscriminator("softMuonNoIPBJetTags");
    jetTrackCountHiEffBJetTags_[nJet_] = iJet->bDiscriminator("trackCountingHighEffBJetTags");
    jetTrackCountHiPurBJetTags_[nJet_] = iJet->bDiscriminator("trackCountingHighPurBJetTags");
    // End of b-tagging information
    
    jetmaxEInEmTowers_[nJet_] = iJet->maxEInEmTowers();
    jetmaxEInHadTowers_[nJet_] = iJet->maxEInHadTowers();
    jetenergyFractionHadronic_[nJet_] = iJet->energyFractionHadronic();
    jetemEnergyFraction_[nJet_] = iJet->emEnergyFraction();

    jetGenIndex_[nJet_] = -1;
    jetGenPartonID_[nJet_] = 0;
    jetGenPartonMomID_[nJet_] = 0;
    int JetGenIndex = 0;
    if (!isData_) {
      if ((*iJet).genParton()) {
	jetGenPartonID_[nJet_] = (*iJet).genParton()->pdgId();
	jetGenEn_[nJet_] = (*iJet).genParton()->energy();
	jetGenPt_[nJet_] = (*iJet).genParton()->pt();
	jetGenEta_[nJet_] = (*iJet).genParton()->eta();
	jetGenPhi_[nJet_] = (*iJet).genParton()->phi();
	for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen != genParticlesHandle_->end(); ++iGen) {
	  if (iGen->p4() == (*iJet).genParticle()->p4() && iGen->pdgId() == (*iJet).genParticle()->pdgId() && iGen->status() ==  (*iJet).genParticle()->status()) {
	    jetGenIndex_[nJet_] = JetGenIndex;
	    if (iGen->mother()) jetGenPartonMomID_[nJet_] = iGen->mother()->pdgId();
	  }
	  JetGenIndex++;
	}
      }
    }

    jetGenJetIndex_[nJet_] = -1;
    jetGenJetEn_[nJet_] = -1.0;
    jetGenJetPt_[nJet_] = -99999;
    jetGenJetEta_[nJet_] = -999;
    jetGenJetPhi_[nJet_] = -999;
    jetGenJetMass_[nJet_] = -999;

    if ((*iJet).genJet()) {
      jetGenJetIndex_[nJet_] = 1;
      jetGenJetEn_[nJet_] = (*iJet).genJet()->energy();
      jetGenJetPt_[nJet_] = (*iJet).genJet()->pt();
      jetGenJetEta_[nJet_] = (*iJet).genJet()->eta();
      jetGenJetPhi_[nJet_] = (*iJet).genJet()->phi();
      jetGenJetMass_[nJet_] = (*iJet).genJet()->mass();
    }

    nJet_++;
  }
  tree_->Fill();
}

void VgAnalyzerKit::beginJob() {
}

void VgAnalyzerKit::endJob() {
}

DEFINE_FWK_MODULE(VgAnalyzerKit);
