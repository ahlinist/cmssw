#include "ElectroWeakAnalysis/MultiBosons/interface/VgAnalyzerKit.h"

#include <iostream>
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/ESHandle.h"
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
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "PhysicsTools/CandUtils/interface/AddFourMomenta.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "DataFormats/EgammaCandidates/interface/PhotonPi0DiscriminatorAssociation.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
//#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"

//geometry
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalPreshowerGeometry.h"

// PileupSummaryInfo
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

// Isolation
#include "RecoEgamma/EgammaIsolationAlgos/plugins/EgammaTowerIsolationProducer.h"

using namespace std;
using namespace pat;
using namespace edm;
using namespace reco;
using namespace pat::helper;

VgAnalyzerKit::VgAnalyzerKit(const edm::ParameterSet& ps) : verbosity_(0), helper_(ps) {
  // cout << "VgAnalyzerKit: entering ctor ..." << endl;

  saveHistograms_ = ps.getUntrackedParameter<bool>("saveHistograms", false);
  trgResults_     = ps.getParameter<InputTag>("triggerResults");
  trgEvent_       = ps.getParameter<InputTag>("triggerEvent"); 
  doGenParticles_ = ps.getParameter<bool>("doGenParticles");
  doStoreJets_     = ps.getParameter<bool>("doStoreJets");
  doSkim_         = ps.getParameter<bool>("doSkim");
  gtdigilabel_    = ps.getParameter<InputTag>("GTDigiLabel");
  vtxlabel_       = ps.getParameter<InputTag>("VtxLabel");
  caloTowerlabel_ = ps.getParameter<InputTag>("CaloTowerLabel");
  tracklabel_     = ps.getParameter<InputTag>("TrackLabel");
  tcMETlabel_     = ps.getParameter<InputTag>("tcMETLabel");
  pfMETlabel_     = ps.getParameter<InputTag>("pfMETLabel");
  puInfoLabel_    = ps.getParameter<InputTag>("puInfoLabel");
  rhoLabel_       = ps.getParameter<InputTag>("rhoLabel");
  sigmaLabel_     = ps.getParameter<InputTag>("sigmaLabel");

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
  tree_->Branch("nHLT", &nHLT_, "nHLT/I");
  tree_->Branch("HLT", HLT_, "HLT[nHLT]/I");
  tree_->Branch("HLTIndex", HLTIndex_, "HLTIndex[147]/I");
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
    tree_->Branch("nBX", nBX_, "nBX/I");
    tree_->Branch("nPU", nPU_, "nPU[nBX]/I");
    tree_->Branch("BXPU", BXPU_, "BXPU[nBX]/I");
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
    tree_->Branch("mcIsoDR03", mcIsoDR03, "mcIsoDR03[nMC]/F");
    tree_->Branch("mcCalIsoDR03", mcCalIsoDR03, "mcCalIsoDR03[nMC]/F");
    tree_->Branch("mcTrkIsoDR03", mcTrkIsoDR03, "mcTrkIsoDR03[nMC]/F");
    tree_->Branch("mcIsoDR04", mcIsoDR04, "mcIsoDR04[nMC]/F");
    tree_->Branch("mcCalIsoDR04", mcCalIsoDR04, "mcCalIsoDR04[nMC]/F");
    tree_->Branch("mcTrkIsoDR04", mcTrkIsoDR04, "mcTrkIsoDR04[nMC]/F");
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
  tree_->Branch("eleTrg", eleTrg_, "eleTrg[nEle][21]/I");
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
  tree_->Branch("eleSigmaIEtaIPhi", eleSigmaIEtaIPhi_, "eleSigmaIEtaIPhi[nEle]/F");
  tree_->Branch("eleSigmaIPhiIPhi", eleSigmaIPhiIPhi_, "eleSigmaIPhiIPhi[nEle]/F");
  tree_->Branch("eleE2overE9", eleE2overE9_, "eleE2overE9[nEle]/F");
  tree_->Branch("eleE3x3", eleE3x3_, "eleE3x3[nEle]/F");
  tree_->Branch("eleSeedTime", eleSeedTime_, "eleSeedTime[nEle]/F");
  // If Flag == 2, it means that rechit is out of time
  // https://twiki.cern.ch/twiki/bin/viewauth/CMS/EcalFirstBeam09Anomalous#Spike_identification_in_collisio
  tree_->Branch("eleRecoFlag", eleRecoFlag_, "eleRecoFlag[nEle]/I");
  // If Severity == 3, it is spike. If Severity == 4, it is bad, not sutiable to be used in reconstruction.
  tree_->Branch("eleSeverity", eleSeverity_, "eleSeverity[nEle]/I");
  if (doGenParticles_) {
    tree_->Branch("eleGenIndex", eleGenIndex_, "eleGenIndex[nEle]/I");
    tree_->Branch("eleGenGMomPID", eleGenGMomPID_, "eleGenGMomPID[nEle]/I");
    tree_->Branch("eleGenMomPID", eleGenMomPID_, "eleGenMomPID[nEle]/I");
    tree_->Branch("eleGenMomPt", eleGenMomPt_, "eleGenMomPt[nEle]/F");
  }
  tree_->Branch("eleIsoTrkDR03", eleIsoTrkDR03_, "eleIsoTrkDR03[nEle]/F");
  tree_->Branch("eleIsoEcalDR03", eleIsoEcalDR03_, "eleIsoEcalDR03[nEle]/F");
  tree_->Branch("eleIsoHcalDR03", eleIsoHcalDR03_, "eleIsoHcalDR03[nEle]/F");
  tree_->Branch("eleIsoHcalSolidDR03", eleIsoHcalSolidDR03_, "eleIsoHcalSolidDR03[nEle]/F");
  tree_->Branch("eleIsoTrkDR04", eleIsoTrkDR04_, "eleIsoTrkDR04[nEle]/F");
  tree_->Branch("eleIsoEcalDR04", eleIsoEcalDR04_, "eleIsoEcalDR04[nEle]/F");
  tree_->Branch("eleIsoHcalDR04", eleIsoHcalDR04_, "eleIsoHcalDR04[nEle]/F");
  tree_->Branch("eleIsoHcalSolidDR04", eleIsoHcalSolidDR04_, "eleIsoHcalSolidDR04[nEle]/F");
  tree_->Branch("eleConvDist", eleConvDist_, "eleConvDist[nEle]/F");
  tree_->Branch("eleConvDcot", eleConvDcot_, "eleConvDcot[nEle]/F");
  tree_->Branch("eleConvRadius", eleConvRadius_, "eleConvRadius[nEle]/F");
  tree_->Branch("eleConvFlag", eleConvFlag_, "eleConvFlag[nEle]/I");
  tree_->Branch("eleConvMissinghit", eleConvMissinghit_, "eleConvMissinghit[nEle]/I");
  tree_->Branch("eleESRatio", eleESRatio_, "eleESRatio[nEle]/F");
  tree_->Branch("eleESProfileFront", eleESProfileFront_, "eleESProfileFront[nEle][123]/F");
  tree_->Branch("eleESProfileRear", eleESProfileRear_, "eleESProfileRear[nEle][123]/F");
  // Photon
  tree_->Branch("nPho", &nPho_, "nPho/I");
  tree_->Branch("phoTrg", phoTrg_, "phoTrg[nPho][16]/I");
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
  tree_->Branch("phoHcalIsoSolidDR03", phoHcalIsoSolidDR03_, "phoHcalIsoSolidDR03[nPho]/F");
  tree_->Branch("phoTrkIsoSolidDR04", phoTrkIsoSolidDR04_, "phoTrkIsoSolidDR04[nPho]/F");
  tree_->Branch("phoTrkIsoHollowDR04", phoTrkIsoHollowDR04_, "phoTrkIsoHollowDR04[nPho]/F");
  tree_->Branch("phoNTrkSolidDR04", phoNTrkSolidDR04_, "phoNTrkSolidDR04[nPho]/I");
  tree_->Branch("phoNTrkHollowDR04", phoNTrkHollowDR04_, "phoNTrkHollowDR04[nPho]/I");
  tree_->Branch("phoEcalIsoDR04", phoEcalIsoDR04_, "phoEcalIsoDR04[nPho]/F");
  tree_->Branch("phoHcalIsoDR04", phoHcalIsoDR04_, "phoHcalIsoDR04[nPho]/F");
  tree_->Branch("phoHcalIsoSolidDR04", phoHcalIsoSolidDR04_, "phoHcalIsoSolidDR04[nPho]/F");
  tree_->Branch("phoHoverE", phoHoverE_, "phoHoverE[nPho]/F");
  tree_->Branch("phoSigmaEtaEta", phoSigmaEtaEta_, "phoSigmaEtaEta[nPho]/F");
  tree_->Branch("phoSigmaIEtaIEta", phoSigmaIEtaIEta_, "phoSigmaIEtaIEta[nPho]/F");
  tree_->Branch("phoSigmaIEtaIPhi", phoSigmaIEtaIPhi_, "phoSigmaIEtaIPhi[nPho]/F");
  tree_->Branch("phoSigmaIPhiIPhi", phoSigmaIPhiIPhi_, "phoSigmaIPhiIPhi[nPho]/F");
  tree_->Branch("phoE2overE9", phoE2overE9_, "phoE2overE9[nPho]/F");
  tree_->Branch("phoE3x3", phoE3x3_, "phoE3x3[nPho]/F");
  tree_->Branch("phoSeedTime", phoSeedTime_, "phoSeedTime[nPho]/F");
  // If Flag == 2, it means that rechit is out of time
  tree_->Branch("phoRecoFlag", phoRecoFlag_, "phoRecoFlag[nPho]/I");
  // If Severity == 3, it is spike. If Severity == 4, it is bad, not sutiable to be used in reconstruction.
  tree_->Branch("phoSeverity", phoSeverity_, "phoSeverity[nPho]/I");
  tree_->Branch("phoPos", phoPos_, "phoPos[nPho]/I");
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
  tree_->Branch("phoESRatio", phoESRatio_, "phoESRatio[nPho]/F");
  tree_->Branch("phoESProfileFront", phoESProfileFront_, "phoESProfileFront[nPho][123]/F");
  tree_->Branch("phoESProfileRear", phoESProfileRear_, "phoESProfileRear[nPho][123]/F");
  // Muon
  tree_->Branch("nMu", &nMu_, "nMu/I");
  tree_->Branch("muTrg", muTrg_, "muTrg[nMu][26]/I");
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
  tree_->Branch("muDz", muDz_, "muDz[nMu]/F");
  tree_->Branch("muNumberOfValidTrkHits", muNumberOfValidTrkHits_, "muNumberOfValidTrkHits[nMu]/I");
  tree_->Branch("muNumberOfValidPixelHits", muNumberOfValidPixelHits_, "muNumberOfValidPixelHits[nMu]/I");
  tree_->Branch("muNumberOfValidMuonHits", muNumberOfValidMuonHits_, "muNumberOfValidMuonHits[nMu]/I");
  tree_->Branch("muStations", muStations_, "muStations[nMu]/I");
  tree_->Branch("muChambers", muChambers_, "muChambers[nMu]/I");
  // Jet
  if (doStoreJets_) {
    tree_->Branch("nJet", &nJet_, "nJet/I");
    tree_->Branch("jetTrg", jetTrg_, "jetTrg[nJet][31]/I");
    tree_->Branch("jetEn", jetEn_, "jetEn[nJet]/F");
    tree_->Branch("jetPt", jetPt_, "jetPt[nJet]/F");
    tree_->Branch("jetEta", jetEta_, "jetEta[nJet]/F");
    tree_->Branch("jetPhi", jetPhi_, "jetPhi[nJet]/F");
    tree_->Branch("jetMass", jetMass_, "jetMass[nJet]/F");
    tree_->Branch("jetEt", jetEt_, "jetEt[nJet]/F");
    tree_->Branch("jetpartonFlavour", jetpartonFlavour_, "jetpartonFlavour[nJet]/I");
    tree_->Branch("jetRawPt", jetRawPt_, "jetRawPt[nJet]/F");
    tree_->Branch("jetRawEn", jetRawEn_, "jetRawEn[nJet]/F");
    tree_->Branch("jetCharge", jetCharge_, "jetCharge[nJet]/F");
    //tree_->Branch("jetenergyFractionHadronic", jetenergyFractionHadronic_, "jetenergyFractionHadronic[nJet]/F");
    //tree_->Branch("jetemEnergyFraction", jetemEnergyFraction_, "jetemEnergyFraction[nJet]/F");
    //tree_->Branch("jetfHPD", jetfHPD_, "jetfHPD[nJet]/F");
    //tree_->Branch("jetN60", jetN60_, "jetN60[nJet]/I");
    //tree_->Branch("jetN90", jetN90_, "jetN90[nJet]/I");
    tree_->Branch("jetNeutralEmEnergy", jetNeutralEmEnergy_, "jetNeutralEmEnergy[nJet]/F");
    tree_->Branch("jetNeutralEmEnergyFraction", jetNeutralEmEnergyFraction_, "jetNeutralEmEnergyFraction[nJet]/F");
    tree_->Branch("jetNeutralHadronEnergy", jetNeutralHadronEnergy_, "jetNeutralHadronEnergy[nJet]/F");
    tree_->Branch("jetNeutralHadronEnergyFraction", jetNeutralHadronEnergyFraction_, "jetNeutralHadronEnergyFraction[nJet]/F");
    tree_->Branch("jetNConstituents", jetNConstituents_, "jetNConstituents[nJet]/I");
    tree_->Branch("jetChargedEmEnergy", jetChargedEmEnergy_, "jetChargedEmEnergy[nJet]/F");
    tree_->Branch("jetChargedEmEnergyFraction", jetChargedEmEnergyFraction_, "jetChargedEmEnergyFraction[nJet]/F");
    tree_->Branch("jetChargedHadronEnergy", jetChargedHadronEnergy_, "jetChargedHadronEnergy[nJet]/F");
    tree_->Branch("jetChargedHadronEnergyFraction", jetChargedHadronEnergyFraction_, "jetChargedHadronEnergyFraction[nJet]/F");
    tree_->Branch("jetChargedMuEnergy", jetChargedMuEnergy_, "jetChargedMuEnergy[nJet]/F");
    tree_->Branch("jetChargedMuEnergyFraction", jetChargedMuEnergyFraction_, "jetChargedMuEnergyFraction[nJet]/F");
    if (doGenParticles_) {
      tree_->Branch("jetGenJetIndex", jetGenJetIndex_, "jetGenJetIndex[nJet]/I");
      tree_->Branch("jetGenJetEn", jetGenJetEn_, "jetGenJetEn[nJet]/F");
      tree_->Branch("jetGenJetPt", jetGenJetPt_, "jetGenJetPt[nJet]/F");
      tree_->Branch("jetGenJetEta", jetGenJetEta_, "jetGenJetEta[nJet]/F");
      tree_->Branch("jetGenJetPhi", jetGenJetPhi_, "jetGenJetPhi[nJet]/F");
      tree_->Branch("jetGenJetMass", jetGenJetMass_, "jetGenJetMass[nJet]/F");
      tree_->Branch("jetGenPartonID", jetGenPartonID_, "jetGenPartonID[nJet]/I");
      tree_->Branch("jetGenPartonMomID", jetGenPartonMomID_, "jetGenPartonMomID[nJet]/I");
    }
  }
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

  // get the channel status from the DB
  edm::ESHandle<EcalChannelStatus> chStatus;
  es.get<EcalChannelStatusRcd>().get(chStatus);

  // get the severity status from the DB
  //edm::ESHandle<EcalSeverityLevelAlgo> severityStatus;
  //es.get<EcalSeverityLevelAlgoRcd>().get(severityStatus);

  // PAT Trigger
  edm::Handle< TriggerEvent > triggerEvent;
  e.getByLabel(trgEvent_, triggerEvent);

  const TriggerMatchHelper matchHelper;

  //cout << "VgAnalyzerKit: produce: event info ..." << endl;

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

	if (vtxNDF_[nVtx_] > 4 && fabs(vtx_[nVtx_][2]) <= 24 && vtxD0_[nVtx_] <= 2) nGoodVtx++;
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

    processID_ = genEventScale->signalProcessID();
  }

  // PileupSummaryInfo
  if (!isData_) {
    Handle<std::vector< PileupSummaryInfo > >  PUInfo;
    e.getByLabel(puInfoLabel_, PUInfo);

    std::vector<PileupSummaryInfo>::const_iterator PVI;

    nBX_ = 0;
    for(PVI = PUInfo->begin(); PVI != PUInfo->end(); ++PVI) {

      nPU_[nBX_] = PVI->getPU_NumInteractions();
      BXPU_[nBX_] = PVI->getBunchCrossing();

      nBX_ += 1;
    }
  }

  // GenParticle
  // cout << "VgAnalyzerKit: produce: GenParticle... " << endl;
  edm::Handle<reco::GenParticleCollection> genParticles;
  e.getByLabel( "generator", genParticles);

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
	if ((genIndex-1)>60 && ip->pdgId()==22) continue;
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
        mcIsoDR03[nMC_]    = getGenIso(genParticlesHandle_, ip, 0.3, false, false);
        mcCalIsoDR03[nMC_] = getGenCalIso(genParticlesHandle_, ip, 0.3, false, false);
        mcTrkIsoDR03[nMC_] = getGenTrkIso(genParticlesHandle_, ip, 0.3);
        mcIsoDR04[nMC_]    = getGenIso(genParticlesHandle_, ip, 0.4, false, false);
        mcCalIsoDR04[nMC_] = getGenCalIso(genParticlesHandle_, ip, 0.4, false, false);
        mcTrkIsoDR04[nMC_] = getGenTrkIso(genParticlesHandle_, ip, 0.4);

	nMC_++;
      }
    }
  }

  // HLT
  // cout << "VgAnalyzerKit: produce: HLT ... " << endl;
  // Indicate the index of interesting HLT bits. Even CMS has different HLT table for different runs, we can still use the correct HLT bit
  // 0: HLT_Jet15U
  // 1: HLT_Jet30U
  // 2: HLT_Jet50U
  // 3: HLT_Jet70U
  // 4: HLT_Jet70U_v2
  // 5: HLT_Jet100U
  // 6: HLT_Jet100U_v2
  // 7: HLT_Jet140U_v1
  // 8: HLT_Mu9
  // 9: HLT_Mu11
  //10: HLT_Mu13 
  //11: HLT_Mu13_v1
  //12: HLT_Mu15
  //13: HLT_Mu15_v1
  //14: HLT_DoubleMu3
  //15: HLT_DoubleMu3_v2 
  //16: HLT_DoubleMu5_v1
  //17: HLT_Ele15_LW_L1R
  //18: HLT_Ele20_LW_L1R 
  //19: HLT_Ele15_SW_L1R
  //20: HLT_Ele20_SW_L1R
  //21: HLT_Ele15_SW_EleId_L1R
  //22: HLT_Ele20_SW_EleId_L1R 
  //23: HLT_Ele15_SW_CaloEleId_L1R 
  //24: HLT_Ele20_SW_CaloEleId_L1R 
  //25: HLT_Ele17_SW_CaloEleId_L1R 
  //26: HLT_Ele17_SW_TightEleId_L1R
  //27: HLT_Ele17_SW_TighterEleIdIsol_L1R
  //28: HLT_Ele17_SW_TighterEleIdIsol_L1R_v2
  //29: HLT_Ele22_SW_TighterEleId_L1R_v2
  //30: HLT_DoubleEle10_SW_L1R
  //31: HLT_DoubleEle17_SW_L1R_v1
  //32: HLT_Photon10_Cleaned_L1R
  //33: HLT_Photon15_Cleaned_L1R
  //34: HLT_Photon20_Cleaned_L1R
  //35: HLT_Photon30_Cleaned_L1R 
  //36: HLT_Photon50_Cleaned_L1R_v1
  //37: HLT_Photon70_Cleaned_L1R_v1
  //38: HLT_Jet15U_v3
  //39: HLT_Jet30U_v3
  //40: HLT_Jet50U_v3
  //41: HLT_Jet70U_v3
  //42: HLT_Jet100U_v3
  //43: HLT_Jet140U_v3
  //44: HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1
  //45: HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2
  //46: HLT_Ele17_SW_TighterEleIdIsol_L1R_v3
  //47: HLT_DoubleEle15_SW_L1R_v1 
  //48: HLT_DoublePhoton17_L1R
  //49: HLT_Photon10_L1R
// HLT path for 2011A
  //50: HLT_Jet30_v1
  //51: HLT_Jet60_v1
  //52: HLT_Jet80_v1
  //53: HLT_Jet110_v1
  //54: HLT_Jet150_v1
  //55: HLT_Jet190_v1
  //56: HLT_Jet240_v1
  //57: HLT_Jet370_v1
  //58: HLT_Mu12_v1
  //59: HLT_Mu15_v2
  //60: HLT_Mu20_v1
  //61: HLT_Mu24_v1
  //62: HLT_Mu30_v1
  //63: HLT_IsoMu12_v1
  //64: HLT_IsoMu15_v5
  //65: HLT_IsoMu17_v5
  //66: HLT_IsoMu24_v1
  //67: HLT_IsoMu30_v1
  //68: HLT_DoubleMu6_v1
  //69: HLT_DoubleMu7_v1
  //70: HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1
  //71: HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2
  //72: HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1
  //73: HLT_Ele45_CaloIdVT_TrkIdT_v1
  //74: HLT_Ele45_CaloIdVT_TrkIdT_v2
  //75: HLT_Ele90_NoSpikeFilter_v1
  //76: HLT_Ele90_NoSpikeFilter_v2
  //77: HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1
  //78: HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2
  //79: HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v2
  //80: HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v1
  //81: HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v2
  //82: HLT_Ele17_CaloIdL_CaloIsoVL_Ele15_HFL_v1
  //83: HLT_Ele17_CaloIdL_CaloIsoVL_Ele15_HFL_v2
  //84: HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v1
  //85: HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v2
  //86: HLT_Photon30_CaloIdVL_v1
  //87: HLT_Photon30_CaloIdVL_v2
  //88: HLT_Photon30_CaloIdVL_IsoL_v1
  //89: HLT_Photon30_CaloIdVL_IsoL_v2
  //90: HLT_Photon50_CaloIdVL_IsoL_v1
  //91: HLT_Photon75_CaloIdVL_v1
  //92: HLT_Photon75_CaloIdVL_v2
  //93: HLT_Photon75_CaloIdVL_IsoL_v1
  //94: HLT_Photon75_CaloIdVL_IsoL_v2
  //95: HLT_Photon20_R9Id_Photon18_R9Id_v1
  //96: HLT_Photon20_R9Id_Photon18_R9Id_v2
  //97: HLT_Photon26_IsoVL_Photon18_v1
  //98: HLT_Photon26_IsoVL_Photon18_v2
  //99:  HLT_Photon26_IsoVL_Photon18_IsoVL_v1
  //100: HLT_Photon26_IsoVL_Photon18_IsoVL_v2
  //101: HLT_Photon26_CaloIdL_IsoVL_Photon18_v1
  //102: HLT_Photon26_CaloIdL_IsoVL_Photon18_v2
  //103: HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v1
  //104: HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v2
  //105: HLT_Photon26_CaloIdL_IsoVL_Photon18_R9Id_v1
  //106: HLT_Jet30_v2
  //107: HLT_Jet60_v2
  //108: HLT_Jet80_v2
  //109: HLT_Jet110_v2
  //110: HLT_Jet150_v2
  //111: HLT_Jet190_v2
  //112: HLT_Jet240_v2
  //113: HLT_Jet300_v1
  //114: HLT_Jet370_v2
  //115: HLT_Mu12_v2
  //116: HLT_Mu15_v3
  //117: HLT_Mu20_v2
  //118: HLT_Mu24_v2
  //119: HLT_Mu30_v2
  //120: HLT_IsoMu12_v2
  //121: HLT_IsoMu15_v6
  //122: HLT_IsoMu17_v6
  //123: HLT_IsoMu24_v2
  //124: HLT_IsoMu30_v2
  //125: HLT_DoubleMu6_v2
  //126: HLT_DoubleMu7_v2
  //127: HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3
  //128: HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2
  //129: HLT_Ele45_CaloIdVT_TrkIdT_v3
  //130: HLT_Ele90_NoSpikeFilter_v3
  //131: HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3
  //132: HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v3
  //133: HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v3
  //134: HLT_Ele17_CaloIdL_CaloIsoVL_Ele15_HFL_v3
  //135: HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v3
  //136: HLT_Photon30_CaloIdVL_v3
  //137: HLT_Photon30_CaloIdVL_IsoL_v3
  //138: HLT_Photon50_CaloIdVL_IsoL_v2
  //139: HLT_Photon75_CaloIdVL_v3
  //140: HLT_Photon75_CaloIdVL_IsoL_v3
  //141: HLT_Photon20_R9Id_Photon18_R9Id_v3
  //142: HLT_Photon26_IsoVL_Photon18_v3
  //143: HLT_Photon26_IsoVL_Photon18_IsoVL_v3
  //144: HLT_Photon26_CaloIdL_IsoVL_Photon18_v3
  //145: HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v3
  //146: HLT_Photon26_CaloIdL_IsoVL_Photon18_R9Id_v2

  for (int a=0; a<147; a++)
    HLTIndex_[a] = -1;
 
  nHLT_ = 0;
  Handle<TriggerResults> trgResultsHandle;
  e.getByLabel(trgResults_, trgResultsHandle);
  const TriggerNames &trgNames = e.triggerNames(*trgResultsHandle);
  vector<string> hlNames = trgNames.triggerNames();
  nHLT_ = trgNames.size();
  for (size_t i=0; i<trgNames.size(); ++i) {
    HLT_[i] = (trgResultsHandle->accept(i) == true) ? 1:0;

    if (hlNames[i] == "HLT_Jet15U")                       HLTIndex_[0] = i;
    else if (hlNames[i] == "HLT_Jet30U")                  HLTIndex_[1] = i;
    else if (hlNames[i] == "HLT_Jet50U")                  HLTIndex_[2] = i;
    else if (hlNames[i] == "HLT_Jet70U")                  HLTIndex_[3] = i;
    else if (hlNames[i] == "HLT_Jet70U_v2")               HLTIndex_[4] = i;
    else if (hlNames[i] == "HLT_Jet100U")                 HLTIndex_[5] = i;
    else if (hlNames[i] == "HLT_Jet100U_v2")              HLTIndex_[6] = i;
    else if (hlNames[i] == "HLT_Jet100U_v1")              HLTIndex_[7] = i;
    else if (hlNames[i] == "HLT_Mu9")                     HLTIndex_[8] = i;
    else if (hlNames[i] == "HLT_Mu11")                    HLTIndex_[9] = i;
    else if (hlNames[i] == "HLT_Mu13")                    HLTIndex_[10] = i;
    else if (hlNames[i] == "HLT_Mu13_v1")                 HLTIndex_[11] = i;
    else if (hlNames[i] == "HLT_Mu15")                    HLTIndex_[12] = i;
    else if (hlNames[i] == "HLT_Mu15_v1")                 HLTIndex_[13] = i;
    else if (hlNames[i] == "HLT_DoubleMu3")               HLTIndex_[14] = i;
    else if (hlNames[i] == "HLT_DoubleMu3_v2")            HLTIndex_[15] = i;
    else if (hlNames[i] == "HLT_DoubleMu5_v1")            HLTIndex_[16] = i;
    else if (hlNames[i] == "HLT_Ele15_LW_L1R")            HLTIndex_[17] = i;
    else if (hlNames[i] == "HLT_Ele20_LW_L1R")            HLTIndex_[18] = i;
    else if (hlNames[i] == "HLT_Ele15_SW_L1R")            HLTIndex_[19] = i;
    else if (hlNames[i] == "HLT_Ele20_SW_L1R")            HLTIndex_[20] = i;
    else if (hlNames[i] == "HLT_Ele15_SW_EleId_L1R")      HLTIndex_[21] = i;
    else if (hlNames[i] == "HLT_Ele20_SW_EleId_L1R")      HLTIndex_[22] = i;
    else if (hlNames[i] == "HLT_Ele15_SW_CaloEleId_L1R")  HLTIndex_[23] = i;
    else if (hlNames[i] == "HLT_Ele20_SW_CaloEleId_L1R")  HLTIndex_[24] = i;
    else if (hlNames[i] == "HLT_Ele17_SW_CaloEleId_L1R")  HLTIndex_[25] = i;
    else if (hlNames[i] == "HLT_Ele17_SW_TightEleId_L1R") HLTIndex_[26] = i;
    else if (hlNames[i] == "HLT_Ele17_SW_TighterEleIdIsol_L1R")         HLTIndex_[27] = i;
    else if (hlNames[i] == "HLT_Ele17_SW_TighterEleIdIsol_L1R_v2")      HLTIndex_[28] = i;
    else if (hlNames[i] == "HLT_Ele22_SW_TighterEleId_L1R_v2")          HLTIndex_[29] = i;
    else if (hlNames[i] == "HLT_DoubleEle10_SW_L1R")      HLTIndex_[30] = i;
    else if (hlNames[i] == "HLT_DoubleEle17_SW_L1R_v1")   HLTIndex_[31] = i;
    else if (hlNames[i] == "HLT_Photon10_Cleaned_L1R")    HLTIndex_[32] = i;
    else if (hlNames[i] == "HLT_Photon15_Cleaned_L1R")    HLTIndex_[33] = i;
    else if (hlNames[i] == "HLT_Photon20_Cleaned_L1R")    HLTIndex_[34] = i;
    else if (hlNames[i] == "HLT_Photon30_Cleaned_L1R")    HLTIndex_[35] = i;
    else if (hlNames[i] == "HLT_Photon50_Cleaned_L1R_v1") HLTIndex_[36] = i;
    else if (hlNames[i] == "HLT_Photon70_Cleaned_L1R_v1") HLTIndex_[37] = i;
    else if (hlNames[i] == "HLT_Jet15U_v3") 		  HLTIndex_[38] = i;
    else if (hlNames[i] == "HLT_Jet30U_v3") 		  HLTIndex_[39] = i;
    else if (hlNames[i] == "HLT_Jet50U_v3") 		  HLTIndex_[40] = i;
    else if (hlNames[i] == "HLT_Jet70U_v3") 		  HLTIndex_[41] = i;
    else if (hlNames[i] == "HLT_Jet100U_v3") 		  HLTIndex_[42] = i;
    else if (hlNames[i] == "HLT_Jet140U_v3") 		  HLTIndex_[43] = i;
    else if (hlNames[i] == "HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1") HLTIndex_[44] = i;
    else if (hlNames[i] == "HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2") HLTIndex_[45] = i;
    else if (hlNames[i] == "HLT_Ele17_SW_TighterEleIdIsol_L1R_v3")      HLTIndex_[46] = i;
    else if (hlNames[i] == "HLT_DoubleEle15_SW_L1R_v1")   HLTIndex_[47] = i;
    else if (hlNames[i] == "HLT_DoublePhoton17_L1R") 	  HLTIndex_[48] = i;
    else if (hlNames[i] == "HLT_Photon10_L1R")    	  HLTIndex_[49] = i;
    else if (hlNames[i] == "HLT_Jet30_v1")	HLTIndex_[50] = i;
    else if (hlNames[i] == "HLT_Jet60_v1")	HLTIndex_[51] = i;
    else if (hlNames[i] == "HLT_Jet80_v1")	HLTIndex_[52] = i;
    else if (hlNames[i] == "HLT_Jet110_v1")	HLTIndex_[53] = i;
    else if (hlNames[i] == "HLT_Jet150_v1")	HLTIndex_[54] = i;
    else if (hlNames[i] == "HLT_Jet190_v1")	HLTIndex_[55] = i;
    else if (hlNames[i] == "HLT_Jet240_v1")	HLTIndex_[56] = i;
    else if (hlNames[i] == "HLT_Jet370_v1")	HLTIndex_[57] = i;
    else if (hlNames[i] == "HLT_Mu12_v1")	HLTIndex_[58] = i;
    else if (hlNames[i] == "HLT_Mu15_v2")	HLTIndex_[59] = i;
    else if (hlNames[i] == "HLT_Mu20_v1")	HLTIndex_[60] = i;
    else if (hlNames[i] == "HLT_Mu24_v1")	HLTIndex_[61] = i;
    else if (hlNames[i] == "HLT_Mu30_v1")	HLTIndex_[62] = i;
    else if (hlNames[i] == "HLT_IsoMu12_v1")	HLTIndex_[63] = i;
    else if (hlNames[i] == "HLT_IsoMu15_v5")	HLTIndex_[64] = i;
    else if (hlNames[i] == "HLT_IsoMu17_v5")	HLTIndex_[65] = i;
    else if (hlNames[i] == "HLT_IsoMu24_v1")	HLTIndex_[66] = i;
    else if (hlNames[i] == "HLT_IsoMu30_v1")	HLTIndex_[67] = i;
    else if (hlNames[i] == "HLT_DoubleMu6_v1")	HLTIndex_[68] = i;
    else if (hlNames[i] == "HLT_DoubleMu7_v1")	HLTIndex_[69] = i;
    else if (hlNames[i] == "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1")		HLTIndex_[70] = i;
    else if (hlNames[i] == "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2")		HLTIndex_[71] = i;
    else if (hlNames[i] == "HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1")		HLTIndex_[72] = i;
    else if (hlNames[i] == "HLT_Ele45_CaloIdVT_TrkIdT_v1")				HLTIndex_[73] = i;
    else if (hlNames[i] == "HLT_Ele45_CaloIdVT_TrkIdT_v2")				HLTIndex_[74] = i;
    else if (hlNames[i] == "HLT_Ele90_NoSpikeFilter_v1")				HLTIndex_[75] = i;
    else if (hlNames[i] == "HLT_Ele90_NoSpikeFilter_v2")				HLTIndex_[76] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1")	HLTIndex_[77] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2")	HLTIndex_[78] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v2")	HLTIndex_[79] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v1")HLTIndex_[80] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v2")HLTIndex_[81] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdL_CaloIsoVL_Ele15_HFL_v1")			HLTIndex_[82] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdL_CaloIsoVL_Ele15_HFL_v2")			HLTIndex_[83] = i;
    else if (hlNames[i] == "HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v1")			HLTIndex_[84] = i;
    else if (hlNames[i] == "HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v1")			HLTIndex_[85] = i;
    else if (hlNames[i] == "HLT_Photon30_CaloIdVL_v1")		HLTIndex_[86] = i;
    else if (hlNames[i] == "HLT_Photon30_CaloIdVL_v2")		HLTIndex_[87] = i;
    else if (hlNames[i] == "HLT_Photon30_CaloIdVL_IsoL_v1")	HLTIndex_[88] = i;
    else if (hlNames[i] == "HLT_Photon30_CaloIdVL_IsoL_v2")	HLTIndex_[89] = i;
    else if (hlNames[i] == "HLT_Photon50_CaloIdVL_IsoL_v1")	HLTIndex_[90] = i;
    else if (hlNames[i] == "HLT_Photon75_CaloIdVL_v1")		HLTIndex_[91] = i;
    else if (hlNames[i] == "HLT_Photon75_CaloIdVL_v2")		HLTIndex_[92] = i;
    else if (hlNames[i] == "HLT_Photon75_CaloIdVL_IsoL_v1")	HLTIndex_[93] = i;
    else if (hlNames[i] == "HLT_Photon75_CaloIdVL_IsoL_v2")	HLTIndex_[94] = i;
    else if (hlNames[i] == "HLT_Photon20_R9Id_Photon18_R9Id_v1")			HLTIndex_[95] = i;
    else if (hlNames[i] == "HLT_Photon20_R9Id_Photon18_R9Id_v2")			HLTIndex_[96] = i;
    else if (hlNames[i] == "HLT_Photon26_IsoVL_Photon18_v1")				HLTIndex_[97] = i;
    else if (hlNames[i] == "HLT_Photon26_IsoVL_Photon18_v2")				HLTIndex_[98] = i;
    else if (hlNames[i] == "HLT_Photon26_IsoVL_Photon18_IsoVL_v1")			HLTIndex_[99] = i;
    else if (hlNames[i] == "HLT_Photon26_IsoVL_Photon18_IsoVL_v2")			HLTIndex_[100] = i;
    else if (hlNames[i] == "HLT_Photon26_CaloIdL_IsoVL_Photon18_v1")			HLTIndex_[101] = i;
    else if (hlNames[i] == "HLT_Photon26_CaloIdL_IsoVL_Photon18_v2")			HLTIndex_[102] = i;
    else if (hlNames[i] == "HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v1")	HLTIndex_[103] = i;
    else if (hlNames[i] == "HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v2")	HLTIndex_[104] = i;
    else if (hlNames[i] == "HLT_Photon26_CaloIdL_IsoVL_Photon18_R9Id_v1")		HLTIndex_[105] = i;
    else if (hlNames[i] == "HLT_Jet30_v2")		HLTIndex_[106] = i;
    else if (hlNames[i] == "HLT_Jet60_v2")		HLTIndex_[107] = i;
    else if (hlNames[i] == "HLT_Jet80_v2")		HLTIndex_[108] = i;
    else if (hlNames[i] == "HLT_Jet110_v2")		HLTIndex_[109] = i;
    else if (hlNames[i] == "HLT_Jet150_v2")		HLTIndex_[110] = i;
    else if (hlNames[i] == "HLT_Jet190_v2")		HLTIndex_[111] = i;
    else if (hlNames[i] == "HLT_Jet240_v2")		HLTIndex_[112] = i;
    else if (hlNames[i] == "HLT_Jet300_v1")		HLTIndex_[113] = i;
    else if (hlNames[i] == "HLT_Jet370_v2")		HLTIndex_[114] = i;
    else if (hlNames[i] == "HLT_Mu12_v2")		HLTIndex_[115] = i;
    else if (hlNames[i] == "HLT_Mu15_v3")		HLTIndex_[116] = i;
    else if (hlNames[i] == "HLT_Mu20_v2")		HLTIndex_[117] = i;
    else if (hlNames[i] == "HLT_Mu24_v2")		HLTIndex_[118] = i;
    else if (hlNames[i] == "HLT_Mu30_v2")		HLTIndex_[119] = i;
    else if (hlNames[i] == "HLT_IsoMu12_v2")		HLTIndex_[120] = i;
    else if (hlNames[i] == "HLT_IsoMu15_v6")		HLTIndex_[121] = i;
    else if (hlNames[i] == "HLT_IsoMu17_v6")		HLTIndex_[122] = i;
    else if (hlNames[i] == "HLT_IsoMu24_v2")		HLTIndex_[123] = i;
    else if (hlNames[i] == "HLT_IsoMu30_v2")		HLTIndex_[124] = i;
    else if (hlNames[i] == "HLT_DoubleMu6_v2")		HLTIndex_[125] = i;
    else if (hlNames[i] == "HLT_DoubleMu7_v2")		HLTIndex_[126] = i;
    else if (hlNames[i] == "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3")			HLTIndex_[127] = i;
    else if (hlNames[i] == "HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2")			HLTIndex_[128] = i;
    else if (hlNames[i] == "HLT_Ele45_CaloIdVT_TrkIdT_v3")					HLTIndex_[129] = i;
    else if (hlNames[i] == "HLT_Ele90_NoSpikeFilter_v3")					HLTIndex_[130] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v3")		HLTIndex_[131] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v3")		HLTIndex_[132] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v3")	HLTIndex_[133] = i;
    else if (hlNames[i] == "HLT_Ele17_CaloIdL_CaloIsoVL_Ele15_HFL_v3")				HLTIndex_[134] = i;
    else if (hlNames[i] == "HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v3")				HLTIndex_[135] = i;
    else if (hlNames[i] == "HLT_Photon30_CaloIdVL_v3")					HLTIndex_[136] = i;
    else if (hlNames[i] == "HLT_Photon30_CaloIdVL_IsoL_v3")				HLTIndex_[137] = i;
    else if (hlNames[i] == "HLT_Photon50_CaloIdVL_IsoL_v2")				HLTIndex_[138] = i;
    else if (hlNames[i] == "HLT_Photon75_CaloIdVL_v3")					HLTIndex_[139] = i;
    else if (hlNames[i] == "HLT_Photon75_CaloIdVL_IsoL_v3")				HLTIndex_[140] = i;
    else if (hlNames[i] == "HLT_Photon20_R9Id_Photon18_R9Id_v3")			HLTIndex_[141] = i;
    else if (hlNames[i] == "HLT_Photon26_IsoVL_Photon18_v3")				HLTIndex_[142] = i;
    else if (hlNames[i] == "HLT_Photon26_IsoVL_Photon18_IsoVL_v3")			HLTIndex_[143] = i;
    else if (hlNames[i] == "HLT_Photon26_CaloIdL_IsoVL_Photon18_v3")			HLTIndex_[144] = i;
    else if (hlNames[i] == "HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v3")	HLTIndex_[145] = i;
    else if (hlNames[i] == "HLT_Photon26_CaloIdL_IsoVL_Photon18_R9Id_v2")		HLTIndex_[146] = i;
  }

  // Gen & PAT MET (caloMET)
  // cout << "VgAnalyzerKit: produce: Gen & PAT MET (caloMET) ..." << endl;
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
    }

  // tcMET
  // cout << "VgAnalyzerKit: produce: tcMET ..." << endl;
  Handle<View<pat::MET> > tcMETHandle_;
  e.getByLabel(tcMETlabel_, tcMETHandle_);
  for (View<pat::MET>::const_iterator iTCMET = tcMETHandle_->begin(); iTCMET != tcMETHandle_->end(); ++iTCMET) {

      tcMET_       = iTCMET->pt();
      tcMETx_      = iTCMET->px();
      tcMETy_      = iTCMET->py();
      tcMETPhi_    = iTCMET->phi();
      tcMETsumEt_  = iTCMET->sumEt();
      tcMETmEtSig_ = iTCMET->mEtSig();
      tcMETSig_    = iTCMET->significance();
  }

  // pfMET
  // cout << "VgAnalyzerKit: produce: pfMET ..." << endl;
  Handle<View<pat::MET> > pfMETHandle_;
  e.getByLabel(pfMETlabel_, pfMETHandle_);
  for (View<pat::MET>::const_iterator iPFMET = pfMETHandle_->begin(); iPFMET != pfMETHandle_->end(); ++iPFMET) {

      pfMET_       = iPFMET->pt();
      pfMETx_      = iPFMET->px();
      pfMETy_      = iPFMET->py();
      pfMETPhi_    = iPFMET->phi();
      pfMETsumEt_  = iPFMET->sumEt();
      pfMETmEtSig_ = iPFMET->mEtSig();
      pfMETSig_    = iPFMET->significance();
  }

  // Get the hcal hits
  edm::Handle<CaloTowerCollection> towerHandle;
  e.getByLabel("towerMaker", towerHandle);
  const CaloTowerCollection* towers = towerHandle.product();
  EgammaTowerIsolation myHcalIsoDR03(0.3, 0., 0, -1, towers);
  EgammaTowerIsolation myHcalIsoDR04(0.4, 0., 0, -1, towers);

  // Electron
  // cout << "VgAnalyzerKit: produce: Electron ..." << endl;
  const TriggerObjectMatch *eleTriggerMatch1(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton10L1R"));
  const TriggerObjectMatch *eleTriggerMatch2(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton15CleanedL1R"));
  const TriggerObjectMatch *eleTriggerMatch3(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle15LWL1R"));
  const TriggerObjectMatch *eleTriggerMatch4(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle15SWL1R"));
  const TriggerObjectMatch *eleTriggerMatch5(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle15SWCaloEleIdL1R"));
  const TriggerObjectMatch *eleTriggerMatch6(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17SWCaloEleIdL1R"));
  const TriggerObjectMatch *eleTriggerMatch7(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17SWTightEleIdL1R"));
  const TriggerObjectMatch *eleTriggerMatch8(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17SWTighterEleIdIsolL1R"));
  const TriggerObjectMatch *eleTriggerMatch9(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv2"));
  const TriggerObjectMatch *eleTriggerMatch10(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17SWTighterEleIdIsolL1Rv3"));
  const TriggerObjectMatch *eleTriggerMatch11(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTv1"));
  const TriggerObjectMatch *eleTriggerMatch12(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTv2"));
  const TriggerObjectMatch *eleTriggerMatch13(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv1"));
  const TriggerObjectMatch *eleTriggerMatch14(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle45CaloIdVTTrkIdTv1"));
  const TriggerObjectMatch *eleTriggerMatch15(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle45CaloIdVTTrkIdTv2"));
  const TriggerObjectMatch *eleTriggerMatch16(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle90NoSpikeFilterv1"));
  const TriggerObjectMatch *eleTriggerMatch17(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle90NoSpikeFilterv2"));
  const TriggerObjectMatch *eleTriggerMatch18(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoTv3"));
  const TriggerObjectMatch *eleTriggerMatch19(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoTv2"));
  const TriggerObjectMatch *eleTriggerMatch20(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle45CaloIdVTTrkIdTv3"));
  const TriggerObjectMatch *eleTriggerMatch21(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle90NoSpikeFilterv3"));

  int nElePassCut = 0;
  nEle_ = 0;
  const Candidate *elemom = 0;
  if ( electronHandle_.isValid() )
    for (View<pat::Electron>::const_iterator iEle = electronHandle_->begin(); iEle != electronHandle_->end(); ++iEle) {

      if (iEle->pt() > leadingElePtCut_) nElePassCut++;

      edm::RefToBase<pat::Electron> eleRef = electronHandle_->refAt(nEle_);
      reco::CandidateBaseRef eleBaseRef(eleRef);
      const TriggerObjectRef eleTrigRef1( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch1, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef2( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch2, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef3( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch3, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef4( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch4, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef5( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch5, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef6( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch6, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef7( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch7, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef8( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch8, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef9( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch9, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef10( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch10, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef11( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch11, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef12( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch12, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef13( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch13, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef14( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch14, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef15( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch15, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef16( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch16, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef17( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch17, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef18( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch18, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef19( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch19, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef20( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch20, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef21( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch21, e, *triggerEvent ) );
      eleTrg_[nEle_][0]  = (eleTrigRef1.isAvailable())  ? 1 : -99;
      eleTrg_[nEle_][1]  = (eleTrigRef2.isAvailable())  ? 1 : -99;
      eleTrg_[nEle_][2]  = (eleTrigRef3.isAvailable())  ? 1 : -99;
      eleTrg_[nEle_][3]  = (eleTrigRef4.isAvailable())  ? 1 : -99;
      eleTrg_[nEle_][4]  = (eleTrigRef5.isAvailable())  ? 1 : -99;
      eleTrg_[nEle_][5]  = (eleTrigRef6.isAvailable())  ? 1 : -99;
      eleTrg_[nEle_][6]  = (eleTrigRef7.isAvailable())  ? 1 : -99;
      eleTrg_[nEle_][7]  = (eleTrigRef8.isAvailable())  ? 1 : -99;
      eleTrg_[nEle_][8]  = (eleTrigRef9.isAvailable())  ? 1 : -99;
      eleTrg_[nEle_][9]  = (eleTrigRef10.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][10]  = (eleTrigRef11.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][11]  = (eleTrigRef12.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][12]  = (eleTrigRef13.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][13]  = (eleTrigRef14.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][14]  = (eleTrigRef15.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][15]  = (eleTrigRef16.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][16]  = (eleTrigRef17.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][17]  = (eleTrigRef18.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][18]  = (eleTrigRef19.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][19]  = (eleTrigRef20.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][20]  = (eleTrigRef21.isAvailable()) ? 1 : -99;

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

      // Gen Particle
      eleGenIndex_[nEle_]   = -999;
      eleGenMomPID_[nEle_]  = -999;
      eleGenMomPt_[nEle_]   = -999;
      eleGenGMomPID_[nEle_] = -999;
      eleGenIndex_[nEle_]   = -1;
      int EleGenIndex = 0;
      if (!isData_) {
        if ((*iEle).genLepton() && genParticlesHandle_.isValid() ) {

          for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen != genParticlesHandle_->end(); ++iGen) {

            if (iGen->p4() == (*iEle).genLepton()->p4() && iGen->pdgId() == (*iEle).genLepton()->pdgId() && iGen->status() == (*iEle).genLepton()->status()) {

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

      const reco::SuperCluster* eleSC = &(*(iEle->superCluster()));

      eleIsoTrkDR03_[nEle_]  = iEle->dr03TkSumPt();
      eleIsoEcalDR03_[nEle_] = iEle->dr03EcalRecHitSumEt();
      eleIsoHcalDR03_[nEle_] = iEle->dr03HcalTowerSumEt();
      eleIsoHcalSolidDR03_[nEle_] = myHcalIsoDR03.getTowerEtSum(eleSC);

      eleIsoTrkDR04_[nEle_]  = iEle->dr04TkSumPt();
      eleIsoEcalDR04_[nEle_] = iEle->dr04EcalRecHitSumEt();
      eleIsoHcalDR04_[nEle_] = iEle->dr04HcalTowerSumEt();
      eleIsoHcalSolidDR04_[nEle_] = myHcalIsoDR04.getTowerEtSum(eleSC);

      eleSigmaEtaEta_[nEle_] = iEle->sigmaEtaEta();
      
      eleConvDist_[nEle_]    = iEle->convDist();
      eleConvDcot_[nEle_]    = iEle->convDcot();
      eleConvRadius_[nEle_]  = iEle->convRadius();
      eleConvFlag_[nEle_]    = iEle->convFlags();
      const reco::Track *eleTrk = (const reco::Track*)(iEle->gsfTrack().get());  
      const reco::HitPattern& p_inner = eleTrk->trackerExpectedHitsInner(); 
      eleConvMissinghit_[nEle_] = p_inner.numberOfHits();

      const reco::CaloClusterPtr eleSeed = (*iEle).superCluster()->seed();

      vector<float> eleCov;
      eleCov = lazyTool.localCovariances(*eleSeed);
      eleSigmaIEtaIEta_[nEle_] = iEle->sigmaIetaIeta();
      eleSigmaIEtaIPhi_[nEle_] = sqrt(eleCov[1]);
      eleSigmaIPhiIPhi_[nEle_] = sqrt(eleCov[2]);

      eleE3x3_[nEle_] = lazyTool.e3x3(*eleSeed);

      eleSeedTime_[nEle_] = -999.;
      eleRecoFlag_[nEle_] = -999.;
      eleSeverity_[nEle_] = -999.;

      DetId eleSeedDetId = lazyTool.getMaximum(*eleSeed).first;

      if ( iEle->isEB() && EBReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eleebrhit = EBReducedRecHits->find(eleSeedDetId);
        if ( eleebrhit != EBReducedRecHits->end() ) { 
	   eleSeedTime_[nEle_] = eleebrhit->time(); 
           eleRecoFlag_[nEle_] = eleebrhit->recoFlag();
	   eleSeverity_[nEle_] = EcalSeverityLevelAlgo::severityLevel( eleSeedDetId, (*EBReducedRecHits), *chStatus );
	   eleE2overE9_[nEle_] = EcalSeverityLevelAlgo::E2overE9( eleSeedDetId, (*EBReducedRecHits));
           //eleSeverity_[nEle_] = severityStatus->severityLevel( eleSeedDetId, (*EBReducedRecHits) );
	   //eleE2overE9_[nEle_] = EcalSeverityLevelAlgo::E2overE9( eleSeedDetId, (*EBReducedRecHits));
	}
      } else if ( EEReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eleeerhit = EEReducedRecHits->find(eleSeedDetId);
        if ( eleeerhit != EEReducedRecHits->end() ) { 
	   eleSeedTime_[nEle_] = eleeerhit->time(); 
           eleRecoFlag_[nEle_] = eleeerhit->recoFlag();
	   eleSeverity_[nEle_] = EcalSeverityLevelAlgo::severityLevel( eleSeedDetId, (*EEReducedRecHits), *chStatus );
	   eleE2overE9_[nEle_] = EcalSeverityLevelAlgo::E2overE9( eleSeedDetId, (*EEReducedRecHits));
           //eleSeverity_[nEle_] = severityStatus->severityLevel( eleSeedDetId, (*EEReducedRecHits) );
	   //eleE2overE9_[nEle_] = EcalSeverityLevelAlgo::E2overE9( eleSeedDetId, (*EEReducedRecHits));
	}
      }

      eleESRatio_[nEle_] = getESRatio(iEle, e, es);
      for (int a=0; a<123; a++) {
        eleESProfileFront_[nEle_][a] = getESProfileFront(iEle, e, es)[a];
        eleESProfileRear_[nEle_][a] = getESProfileRear(iEle, e, es)[a];
      }

      nEle_++;
    }

  // Photon
  // take the pi0 rejection info from RECO
/*
  Handle<reco::PhotonPi0DiscriminatorAssociationMap>  map;
  e.getByLabel("piZeroDiscriminators","PhotonPi0DiscriminatorAssociationMap",  map);
  reco::PhotonPi0DiscriminatorAssociationMap::const_iterator mapIter;

  edm::Handle<reco::PhotonCollection> R_PhotonHandle;
  e.getByLabel("photons", "", R_PhotonHandle);
  const reco::PhotonCollection R_photons = *(R_PhotonHandle.product());   
*/

  const Candidate *phomom = 0;
  int nPhoPassCut = 0;
  nPho_ = 0;

  const TriggerObjectMatch *phoTriggerMatch1(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton10CleanedL1R"));
  const TriggerObjectMatch *phoTriggerMatch2(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton15CleanedL1R"));
  const TriggerObjectMatch *phoTriggerMatch3(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton20CleanedL1R"));
  const TriggerObjectMatch *phoTriggerMatch4(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton30CleanedL1R"));
  const TriggerObjectMatch *phoTriggerMatch5(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton50CleanedL1Rv1"));
  const TriggerObjectMatch *phoTriggerMatch6(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton70CleanedL1Rv1"));
  const TriggerObjectMatch *phoTriggerMatch7(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTDoublePhoton17L1R"));
  const TriggerObjectMatch *phoTriggerMatch8(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton10L1R"));
  const TriggerObjectMatch *phoTriggerMatch9(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTHLTPhoton50CaloIdVLIsoLv1"));
  const TriggerObjectMatch *phoTriggerMatch10(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton75CaloIdVLIsoLv1"));
  const TriggerObjectMatch *phoTriggerMatch11(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton75CaloIdVLIsoLv2"));
  const TriggerObjectMatch *phoTriggerMatch12(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton75CaloIdVLv1"));
  const TriggerObjectMatch *phoTriggerMatch13(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton75CaloIdVLv2"));
  const TriggerObjectMatch *phoTriggerMatch14(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTHLTPhoton50CaloIdVLIsoLv2"));
  const TriggerObjectMatch *phoTriggerMatch15(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton75CaloIdVLIsoLv3"));
  const TriggerObjectMatch *phoTriggerMatch16(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton75CaloIdVLv3"));


  if ( photonHandle_.isValid() )
    for (View<pat::Photon>::const_iterator iPho = photonHandle_->begin(); iPho != photonHandle_->end(); ++iPho) {
      
      if (iPho->pt() > leadingPhoPtCut_) nPhoPassCut++;

      edm::RefToBase<pat::Photon> phoRef = photonHandle_->refAt(nPho_);
      reco::CandidateBaseRef phoBaseRef(phoRef);
      const TriggerObjectRef phoTrigRef1( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch1, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef2( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch2, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef3( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch3, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef4( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch4, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef5( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch5, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef6( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch6, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef7( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch7, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef8( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch8, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef9( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch9, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef10( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch10, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef11( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch11, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef12( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch12, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef13( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch13, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef14( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch14, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef15( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch15, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef16( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch16, e, *triggerEvent ) );
      phoTrg_[nPho_][0] = (phoTrigRef1.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][1] = (phoTrigRef2.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][2] = (phoTrigRef3.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][3] = (phoTrigRef4.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][4] = (phoTrigRef5.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][5] = (phoTrigRef6.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][6] = (phoTrigRef7.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][7] = (phoTrigRef8.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][8] = (phoTrigRef9.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][9] = (phoTrigRef10.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][10] = (phoTrigRef11.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][11] = (phoTrigRef12.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][12] = (phoTrigRef13.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][13] = (phoTrigRef14.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][14] = (phoTrigRef15.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][15] = (phoTrigRef16.isAvailable()) ? 1 : -99;

      phoIsPhoton_[nPho_] = iPho->isPhoton();
      phoE_[nPho_]   = iPho->energy();
      phoEt_[nPho_]  = iPho->et();
      phoPz_[nPho_]  = iPho->pz();
      phoEta_[nPho_] = iPho->eta();
      phoPhi_[nPho_] = iPho->phi();

      const reco::SuperCluster* phoSC = &(*(iPho->superCluster()));

      phoTrkIsoSolidDR03_[nPho_]  = iPho->trkSumPtSolidConeDR03();
      phoTrkIsoHollowDR03_[nPho_] = iPho->trkSumPtHollowConeDR03();
      phoNTrkSolidDR03_[nPho_]    = iPho->nTrkSolidConeDR03();
      phoNTrkHollowDR03_[nPho_]   = iPho->nTrkHollowConeDR03();
      phoEcalIsoDR03_[nPho_]      = iPho->ecalRecHitSumEtConeDR03();
      phoHcalIsoDR03_[nPho_]      = iPho->hcalTowerSumEtConeDR03();
      phoHcalIsoSolidDR03_[nPho_] = myHcalIsoDR03.getTowerEtSum(phoSC);

      phoTrkIsoSolidDR04_[nPho_]  = iPho->trkSumPtSolidConeDR04();
      phoTrkIsoHollowDR04_[nPho_] = iPho->trkSumPtHollowConeDR04();
      phoNTrkSolidDR04_[nPho_]    = iPho->nTrkSolidConeDR04();
      phoNTrkHollowDR04_[nPho_]   = iPho->nTrkHollowConeDR04();
      phoEcalIsoDR04_[nPho_]      = iPho->ecalRecHitSumEtConeDR04();
      phoHcalIsoDR04_[nPho_]      = iPho->hcalTowerSumEtConeDR04();
      phoHcalIsoSolidDR04_[nPho_] = myHcalIsoDR04.getTowerEtSum(phoSC);

      phoHoverE_[nPho_]        = iPho->hadronicOverEm();
      phoSigmaEtaEta_[nPho_]   = iPho->sigmaEtaEta();
      phoR9_[nPho_]            = iPho->r9();

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
      phoRecoFlag_[nPho_] = -999.;
      phoSeverity_[nPho_] = -999.;
      const reco::CaloClusterPtr phoSeed = (*iPho).superCluster()->seed();
      DetId phoSeedDetId = lazyTool.getMaximum(*phoSeed).first;

      vector<float> phoCov;
      phoCov = lazyTool.localCovariances(*phoSeed);
      phoSigmaIEtaIEta_[nPho_] = iPho->sigmaIetaIeta();
      phoSigmaIEtaIPhi_[nPho_] = sqrt(phoCov[1]);
      phoSigmaIPhiIPhi_[nPho_] = sqrt(phoCov[2]);

      if ( iPho->isEB() && EBReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator ebrhit = EBReducedRecHits->find(phoSeedDetId);
        if ( ebrhit != EBReducedRecHits->end() ) { 
	   phoSeedTime_[nPho_] = ebrhit->time();
           phoRecoFlag_[nPho_] = ebrhit->recoFlag();
	   phoSeverity_[nPho_] = EcalSeverityLevelAlgo::severityLevel( phoSeedDetId, (*EBReducedRecHits), *chStatus );
	   phoE2overE9_[nPho_] = EcalSeverityLevelAlgo::E2overE9( phoSeedDetId, (*EBReducedRecHits));
           //phoSeverity_[nPho_] = severityStatus->severityLevel( phoSeedDetId, (*EBReducedRecHits));
	   //phoE2overE9_[nPho_] = EcalSeverityLevelAlgo::E2overE9( phoSeedDetId, (*EBReducedRecHits));
        }
      } else if ( EEReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eerhit = EEReducedRecHits->find(phoSeedDetId);
        if ( eerhit != EEReducedRecHits->end() ) { 
	   phoSeedTime_[nPho_] = eerhit->time(); 
           phoRecoFlag_[nPho_] = eerhit->recoFlag();
	   phoSeverity_[nPho_] = EcalSeverityLevelAlgo::severityLevel( phoSeedDetId, (*EEReducedRecHits), *chStatus );
	   phoE2overE9_[nPho_] = EcalSeverityLevelAlgo::E2overE9( phoSeedDetId, (*EEReducedRecHits));
           //phoSeverity_[nPho_] = severityStatus->severityLevel( phoSeedDetId, (*EEReducedRecHits) );
	   //phoE2overE9_[nPho_] = EcalSeverityLevelAlgo::E2overE9( phoSeedDetId, (*EEReducedRecHits));
	}
      }

      phoE3x3_[nPho_] = lazyTool.e3x3(*phoSeed);

      if (iPho->isEB()==true && EBRecHits.isValid()) {
	std::vector<float> RoundAndAngle = EcalClusterTools::roundnessBarrelSuperClusters(*(iPho->superCluster()),*EBRecHits,0);
	phoRoundness_[nPho_] = RoundAndAngle[0];
	phoAngle_[nPho_] = RoundAndAngle[1];
      } else {
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

      phoIsConv_[nPho_] = iPho->hasConversionTracks();

      phoPi0Disc_[nPho_] = -1;

/*
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
*/
      phoESRatio_[nPho_] = getESRatio(iPho, e, es);
      for (int a=0; a<123; a++) {
        phoESProfileFront_[nPho_][a] = getESProfileFront(iPho, e, es)[a];
        phoESProfileRear_[nPho_][a] = getESProfileRear(iPho, e, es)[a];
      }

      nPho_++;
    }

  // muon trigger matching
  const TriggerObjectMatch * muTriggerMatch1( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu9" ) );
  const TriggerObjectMatch * muTriggerMatch2( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu11" ) );
  const TriggerObjectMatch * muTriggerMatch3( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu13" ) );
  const TriggerObjectMatch * muTriggerMatch4( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu13v1" ) );
  const TriggerObjectMatch * muTriggerMatch5( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu15" ) );
  const TriggerObjectMatch * muTriggerMatch6( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu15v1" ) );
  const TriggerObjectMatch * muTriggerMatch7( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu12v1" ) );
  const TriggerObjectMatch * muTriggerMatch8( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu15v2" ) );
  const TriggerObjectMatch * muTriggerMatch9( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu20v1" ) );
  const TriggerObjectMatch * muTriggerMatch10( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu24v1" ) );
  const TriggerObjectMatch * muTriggerMatch11( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu30v1" ) );
  const TriggerObjectMatch * muTriggerMatch12( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu12v1" ) );
  const TriggerObjectMatch * muTriggerMatch13( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu15v5" ) );
  const TriggerObjectMatch * muTriggerMatch14( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu17v5" ) );
  const TriggerObjectMatch * muTriggerMatch15( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu24v1" ) );
  const TriggerObjectMatch * muTriggerMatch16( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu30v1" ) );
  const TriggerObjectMatch * muTriggerMatch17( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu12v2" ) );
  const TriggerObjectMatch * muTriggerMatch18( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu15v3" ) );
  const TriggerObjectMatch * muTriggerMatch19( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu20v2" ) );
  const TriggerObjectMatch * muTriggerMatch20( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu24v2" ) );
  const TriggerObjectMatch * muTriggerMatch21( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu30v2" ) );
  const TriggerObjectMatch * muTriggerMatch22( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu12v2" ) );
  const TriggerObjectMatch * muTriggerMatch23( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu15v6" ) );
  const TriggerObjectMatch * muTriggerMatch24( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu17v6" ) );
  const TriggerObjectMatch * muTriggerMatch25( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu24v2" ) );
  const TriggerObjectMatch * muTriggerMatch26( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu30v2" ) );

  // Muon
  int nMuPassCut = 0;
  nMu_ = 0;
  if( muonHandle_.isValid() ) {
    // cout << "VgAnalyzerKit: produce: number of muons: " << muonHandle_->size() << endl;
    for (View<pat::Muon>::const_iterator iMu = muonHandle_->begin(); iMu != muonHandle_->end(); ++iMu) {

      if (iMu->pt() > leadingMuPtCut_) nMuPassCut++;
      
      edm::RefToBase<pat::Muon> muRef = muonHandle_->refAt(nMu_);
      reco::CandidateBaseRef muBaseRef(muRef);
      const TriggerObjectRef muTrigRef1( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch1, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef2( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch2, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef3( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch3, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef4( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch4, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef5( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch5, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef6( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch6, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef7( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch7, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef8( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch8, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef9( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch9, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef10( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch10, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef11( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch11, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef12( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch12, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef13( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch13, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef14( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch14, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef15( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch15, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef16( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch16, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef17( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch17, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef18( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch18, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef19( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch19, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef20( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch20, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef21( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch21, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef22( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch22, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef23( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch23, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef24( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch24, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef25( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch25, e, *triggerEvent ) );
      const TriggerObjectRef muTrigRef26( matchHelper.triggerMatchObject( muBaseRef, muTriggerMatch26, e, *triggerEvent ) );
      muTrg_[nMu_][0] = (muTrigRef1.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][1] = (muTrigRef2.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][2] = (muTrigRef3.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][3] = (muTrigRef4.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][4] = (muTrigRef5.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][5] = (muTrigRef6.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][6] = (muTrigRef7.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][7] = (muTrigRef8.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][8] = (muTrigRef9.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][9] = (muTrigRef10.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][10] = (muTrigRef11.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][11] = (muTrigRef12.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][12] = (muTrigRef13.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][13] = (muTrigRef14.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][14] = (muTrigRef15.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][15] = (muTrigRef16.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][16] = (muTrigRef17.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][17] = (muTrigRef18.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][18] = (muTrigRef19.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][19] = (muTrigRef20.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][20] = (muTrigRef21.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][21] = (muTrigRef22.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][22] = (muTrigRef23.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][23] = (muTrigRef24.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][24] = (muTrigRef25.isAvailable()) ? 1 : -99;
      muTrg_[nMu_][25] = (muTrigRef26.isAvailable()) ? 1 : -99;

      //       if (!iMu->isGlobalMuon()) continue;
      //       if (!iMu->isTrackerMuon()) continue;
      //       if (iMu->globalTrack().isNull()) continue;
      //       if (iMu->innerTrack().isNull()) continue;

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
      } 
      else {
        muD0_[nMu_] = trkr->dxy(beamSpotHandle->position());
        muDz_[nMu_] = trkr->dz(beamSpotHandle->position());
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

      int stations = 0;
      unsigned stationMask(iMu->stationMask());
      for(unsigned i=0; i < 8; ++i)
	if(stationMask & 1 << i) ++stations;
      muStations_[nMu_] = stations;
      muChambers_[nMu_]  = iMu->numberOfMatches();

      muGenIndex_[nMu_] = -1;
      int MuGenIndex = 0;
      if (!isData_) {
        if ( (*iMu).genLepton() && genParticlesHandle_.isValid() ) {
          if (fabs((*iMu).genLepton()->pdgId())==13) {
            for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen != genParticlesHandle_->end(); ++iGen) {

              if (iGen->p4() == (*iMu).genLepton()->p4() && iGen->pdgId() == (*iMu).genLepton()->pdgId() && iGen->status() == (*iMu).genLepton()->status()) 
	        muGenIndex_[nMu_] = MuGenIndex;

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

	//if (iEle->charge() * jEle->charge() < 0 ) {
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
          leg2Index++;
	//}
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
  if (electronHandle_.isValid() && electronHandle_->size() > 0 && METHandle_.isValid() && METHandle_->size() > 0) {

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

      for (View<pat::MET>::const_iterator iTCMET = tcMETHandle_->begin(); iTCMET != tcMETHandle_->end(); ++iTCMET) {

        pat::CompositeCandidate WenuTcMET;
	WenuTcMET.addDaughter(*iEle, "ele");
	WenuTcMET.addDaughter(*iTCMET, "met");

	AddFourMomenta addWenuTcMET;
	addWenuTcMET.set(WenuTcMET);
	WenuMassTTcMET_[nWenu_] = massT(iEle->pt(), iTCMET->pt(), WenuTcMET.px(), WenuTcMET.py());
	WenuEtTcMET_[nWenu_]    = eT(iEle->pt(), iTCMET->pt());
	WenuACopTcMET_[nWenu_]  = acop(iEle->phi(), iTCMET->phi());
      }

      for (View<pat::MET>::const_iterator iPFMET = pfMETHandle_->begin(); iPFMET != pfMETHandle_->end(); ++iPFMET) {
 
        pat::CompositeCandidate WenuPfMET;
	WenuPfMET.addDaughter(*iEle, "ele");
	WenuPfMET.addDaughter(*iPFMET, "met");

	AddFourMomenta addWenuPfMET;
	addWenuPfMET.set(WenuPfMET);
	WenuMassTPfMET_[nWenu_] = massT(iEle->pt(), iPFMET->pt(), WenuPfMET.px(), WenuPfMET.py());
	WenuEtPfMET_[nWenu_]    = eT(iEle->pt(), iPFMET->pt());
	WenuACopPfMET_[nWenu_]  = acop(iEle->phi(), iPFMET->phi());
      }

      WenuEleIndex_[nWenu_]   = leg1Index;
      leg1Index++;
      nWenu_++;
    }
  }

  // Wmunu candiate
  // cout << "VgAnalyzerKit: produce: Wmunu candiate..." << endl;
  nWmunu_ = 0;
  leg1Index = 0;
  if (muonHandle_.isValid() && muonHandle_->size() > 0 && METHandle_.isValid() && METHandle_->size() > 0) {

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

      for (View<pat::MET>::const_iterator iTCMET = tcMETHandle_->begin(); iTCMET != tcMETHandle_->end(); ++iTCMET) {

        pat::CompositeCandidate WmunuTcMET;
	WmunuTcMET.addDaughter(*iMu, "mu");
	WmunuTcMET.addDaughter(*iTCMET, "met");

	AddFourMomenta addWmunuTcMET;
	addWmunuTcMET.set(WmunuTcMET);
	WmunuMassTTcMET_[nWmunu_] = massT(iMu->pt(), iTCMET->pt(), WmunuTcMET.px(), WmunuTcMET.py());
	WmunuEtTcMET_[nWmunu_]    = eT(iMu->pt(), iTCMET->pt());
	WmunuACopTcMET_[nWmunu_]  = acop(iMu->phi(), iTCMET->phi());
      }

      for (View<pat::MET>::const_iterator iPFMET = pfMETHandle_->begin(); iPFMET != pfMETHandle_->end(); ++iPFMET) {
 
        pat::CompositeCandidate WmunuPfMET;
	WmunuPfMET.addDaughter(*iMu, "mu");
	WmunuPfMET.addDaughter(*iPFMET, "met");

	AddFourMomenta addWmunuPfMET;
	addWmunuPfMET.set(WmunuPfMET);
	WmunuMassTPfMET_[nWmunu_] = massT(iMu->pt(), iPFMET->pt(), WmunuPfMET.px(), WmunuPfMET.py());
	WmunuEtPfMET_[nWmunu_]    = eT(iMu->pt(), iPFMET->pt());
	WmunuACopPfMET_[nWmunu_]  = acop(iMu->phi(), iPFMET->phi());
      }

      leg1Index = iMu-(muonHandle_->begin());
      WmunuMuIndex_[nWmunu_]    = leg1Index;
      nWmunu_++;
    }
  }

  // Jet
  // cout << "VgAnalyzerKit: produce: Jet..." << endl;
  const TriggerObjectMatch *jetTriggerMatch1(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet15U"));
  const TriggerObjectMatch *jetTriggerMatch2(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet30U"));
  const TriggerObjectMatch *jetTriggerMatch3(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet50U"));
  const TriggerObjectMatch *jetTriggerMatch4(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet70U"));
  const TriggerObjectMatch *jetTriggerMatch5(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet70Uv2"));
  const TriggerObjectMatch *jetTriggerMatch6(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet100U"));
  const TriggerObjectMatch *jetTriggerMatch7(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet100Uv2"));
  const TriggerObjectMatch *jetTriggerMatch8(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet140Uv1"));
  const TriggerObjectMatch *jetTriggerMatch9(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet15Uv3"));
  const TriggerObjectMatch *jetTriggerMatch10(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet30Uv3"));
  const TriggerObjectMatch *jetTriggerMatch11(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet50Uv3"));
  const TriggerObjectMatch *jetTriggerMatch12(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet70Uv3"));
  const TriggerObjectMatch *jetTriggerMatch13(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet100Uv3"));
  const TriggerObjectMatch *jetTriggerMatch14(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet140Uv3"));
  const TriggerObjectMatch *jetTriggerMatch15(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet30v1"));
  const TriggerObjectMatch *jetTriggerMatch16(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet60v1"));
  const TriggerObjectMatch *jetTriggerMatch17(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet80v1"));
  const TriggerObjectMatch *jetTriggerMatch18(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet110v1"));
  const TriggerObjectMatch *jetTriggerMatch19(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet150v1"));
  const TriggerObjectMatch *jetTriggerMatch20(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet190v1"));
  const TriggerObjectMatch *jetTriggerMatch21(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet240v1"));
  const TriggerObjectMatch *jetTriggerMatch22(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet3700v1"));
  const TriggerObjectMatch *jetTriggerMatch23(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet30v2"));
  const TriggerObjectMatch *jetTriggerMatch24(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet60v2"));
  const TriggerObjectMatch *jetTriggerMatch25(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet80v2"));
  const TriggerObjectMatch *jetTriggerMatch26(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet110v2"));
  const TriggerObjectMatch *jetTriggerMatch27(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet150v2"));
  const TriggerObjectMatch *jetTriggerMatch28(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet190v2"));
  const TriggerObjectMatch *jetTriggerMatch29(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet240v2"));
  const TriggerObjectMatch *jetTriggerMatch30(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet300v1"));
  const TriggerObjectMatch *jetTriggerMatch31(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJetJet3700v2"));

  if (doStoreJets_) {
    nJet_ = 0;
    if ( jetHandle_.isValid() )
      for (View<pat::Jet>::const_iterator iJet = jetHandle_->begin(); iJet != jetHandle_->end(); ++iJet) {

        if ( iJet->pt() < 15 ) continue;

	edm::RefToBase<pat::Jet> jetRef = jetHandle_->refAt(nJet_);
	reco::CandidateBaseRef jetBaseRef(jetRef);
	const TriggerObjectRef jetTrigRef1( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch1, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef2( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch2, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef3( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch3, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef4( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch4, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef5( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch5, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef6( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch6, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef7( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch7, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef8( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch8, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef9( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch9, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef10( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch10, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef11( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch11, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef12( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch12, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef13( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch13, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef14( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch14, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef15( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch15, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef16( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch16, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef17( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch17, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef18( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch18, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef19( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch19, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef20( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch20, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef21( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch21, e, *triggerEvent ) );
	const TriggerObjectRef jetTrigRef22( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch22, e, *triggerEvent ) );
        const TriggerObjectRef jetTrigRef23( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch23, e, *triggerEvent ) );
        const TriggerObjectRef jetTrigRef24( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch24, e, *triggerEvent ) );
        const TriggerObjectRef jetTrigRef25( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch25, e, *triggerEvent ) );
        const TriggerObjectRef jetTrigRef26( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch26, e, *triggerEvent ) );
        const TriggerObjectRef jetTrigRef27( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch27, e, *triggerEvent ) );
        const TriggerObjectRef jetTrigRef28( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch28, e, *triggerEvent ) );
        const TriggerObjectRef jetTrigRef29( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch29, e, *triggerEvent ) );
        const TriggerObjectRef jetTrigRef30( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch30, e, *triggerEvent ) );
        const TriggerObjectRef jetTrigRef31( matchHelper.triggerMatchObject( jetBaseRef, jetTriggerMatch31, e, *triggerEvent ) );

	jetTrg_[nJet_][0] = (jetTrigRef1.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][1] = (jetTrigRef2.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][2] = (jetTrigRef3.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][3] = (jetTrigRef4.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][4] = (jetTrigRef5.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][5] = (jetTrigRef6.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][6] = (jetTrigRef7.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][7] = (jetTrigRef8.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][8] = (jetTrigRef9.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][9] = (jetTrigRef10.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][10] = (jetTrigRef11.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][11] = (jetTrigRef12.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][12] = (jetTrigRef13.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][13] = (jetTrigRef14.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][14] = (jetTrigRef15.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][15] = (jetTrigRef16.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][16] = (jetTrigRef17.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][17] = (jetTrigRef18.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][18] = (jetTrigRef19.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][19] = (jetTrigRef20.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][20] = (jetTrigRef21.isAvailable()) ? 1 : -99;
	jetTrg_[nJet_][21] = (jetTrigRef22.isAvailable()) ? 1 : -99;
        jetTrg_[nJet_][22] = (jetTrigRef23.isAvailable()) ? 1 : -99;
        jetTrg_[nJet_][23] = (jetTrigRef24.isAvailable()) ? 1 : -99;
        jetTrg_[nJet_][24] = (jetTrigRef25.isAvailable()) ? 1 : -99;
        jetTrg_[nJet_][25] = (jetTrigRef26.isAvailable()) ? 1 : -99;
        jetTrg_[nJet_][26] = (jetTrigRef27.isAvailable()) ? 1 : -99;
        jetTrg_[nJet_][27] = (jetTrigRef28.isAvailable()) ? 1 : -99;
        jetTrg_[nJet_][28] = (jetTrigRef29.isAvailable()) ? 1 : -99;
        jetTrg_[nJet_][29] = (jetTrigRef30.isAvailable()) ? 1 : -99;
        jetTrg_[nJet_][30] = (jetTrigRef31.isAvailable()) ? 1 : -99;

	jetEn_[nJet_]     = iJet->energy();
	jetPt_[nJet_]     = iJet->pt();
	jetEta_[nJet_]    = iJet->eta();
	jetPhi_[nJet_]    = iJet->phi();
	jetMass_[nJet_]   = iJet->mass();
	jetCharge_[nJet_] = iJet->jetCharge();
	jetEt_[nJet_]     = iJet->et();
	jetRawPt_[nJet_]  = (*iJet).correctedJet("Uncorrected").pt();
	jetRawEn_[nJet_]  = (*iJet).correctedJet("Uncorrected").energy();
	jetpartonFlavour_[nJet_] = iJet->partonFlavour();

	// Jet Id related
	jetNeutralEmEnergy_[nJet_]         = iJet->neutralEmEnergy();
        jetNeutralEmEnergyFraction_[nJet_] = iJet->neutralEmEnergyFraction();
	jetNeutralHadronEnergy_[nJet_]     = iJet->neutralHadronEnergy();
	jetNeutralHadronEnergyFraction_[nJet_] = iJet->neutralHadronEnergyFraction();
	jetNConstituents_[nJet_]           = iJet->nConstituents();
	jetChargedEmEnergy_[nJet_]         = iJet->chargedEmEnergy();
	jetChargedEmEnergyFraction_[nJet_] = iJet->chargedEmEnergyFraction();
	jetChargedHadronEnergy_[nJet_]     = iJet->chargedHadronEnergy();
	jetChargedHadronEnergyFraction_[nJet_] = iJet->chargedHadronEnergyFraction();
	jetChargedMuEnergy_[nJet_]	   = iJet->chargedMuEnergy();
	jetChargedMuEnergyFraction_[nJet_] = iJet->chargedMuEnergyFraction();
	
	//jetfHPD_[nJet_] = iJet->jetID().fHPD;
	//jetN60_[nJet_]  = iJet->n60();
	//jetN90_[nJet_]  = iJet->n90();
	//jetenergyFractionHadronic_[nJet_] = iJet->energyFractionHadronic();
	//jetemEnergyFraction_[nJet_] = iJet->emEnergyFraction();

	jetGenJetIndex_[nJet_] = -1;
	jetGenJetEn_[nJet_] = -1.0;
	jetGenJetPt_[nJet_] = -99999;
	jetGenJetEta_[nJet_] = -999;
	jetGenJetPhi_[nJet_] = -999;
	jetGenJetMass_[nJet_] = -999;

        if (!isData_ && genParticlesHandle_.isValid() ) {
          if ((*iJet).genJet()) {
            jetGenJetIndex_[nJet_] = 1;
	    jetGenJetEn_[nJet_] = (*iJet).genJet()->energy();
	    jetGenJetPt_[nJet_] = (*iJet).genJet()->pt();
	    jetGenJetEta_[nJet_] = (*iJet).genJet()->eta();
	    jetGenJetPhi_[nJet_] = (*iJet).genJet()->phi();
	    jetGenJetMass_[nJet_] = (*iJet).genJet()->mass();
          }
        }
        nJet_++;
      }
  }

  if (doSkim_ == true) {
    if (doStoreJets_ == false && (nElePassCut > 0 || nMuPassCut > 0)) {
      hEvents_->Fill(1.5);
      tree_->Fill();
    }
    if (doStoreJets_ == true && nPhoPassCut > 0) {
      hEvents_->Fill(1.5);
      tree_->Fill();
    }
  } else {
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

float VgAnalyzerKit::recHitApproxEt( const DetId id, const EcalRecHitCollection &recHits ) {
  // for the time being works only for the barrel
  if ( id.subdetId() == EcalBarrel ) {
    return recHitE( id, recHits ) / cosh( EBDetId::approxEta( id ) );
  }
  return 0;
}

float VgAnalyzerKit::recHitE( const DetId id, const EcalRecHitCollection &recHits )
{
  if ( id == DetId(0) ) {
    return 0;
  } else {
    EcalRecHitCollection::const_iterator it = recHits.find( id );
    if ( it != recHits.end() ) return (*it).energy();
  }
  return 0;
}

float VgAnalyzerKit::recHitE( const DetId id, const EcalRecHitCollection & recHits, int di, int dj )
{
  // in the barrel:   di = dEta   dj = dPhi
  // in the endcap:   di = dX     dj = dY
  
  DetId nid;
  if( id.subdetId() == EcalBarrel) nid = EBDetId::offsetBy( id, di, dj );
  else if( id.subdetId() == EcalEndcap) nid = EEDetId::offsetBy( id, di, dj );

  return ( nid == DetId(0) ? 0 : recHitE( nid, recHits ) );
}

float VgAnalyzerKit::getGenIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho,
					 const Float_t dRMax, bool removeMu, bool removeNu)
{
  const Float_t etMin = 0.0;
  Float_t genIsoSum = 0.0;
  if(!doGenParticles_)return genIsoSum;
  if(!handle.isValid())return genIsoSum;

  for (reco::GenParticleCollection::const_iterator it_gen=handle->begin(); it_gen!=handle->end(); it_gen++){

    if(it_gen == thisPho)continue;      // can't be the original photon
    if(it_gen->status()!=1)continue;    // need to be a stable particle
    if (thisPho->collisionId() != it_gen->collisionId()) continue; // has to come from the same collision
   
    Float_t et = it_gen->et();
    if(et < etMin) continue; // pass a minimum et threshold, default 0

    Float_t dR = reco::deltaR(thisPho->momentum(), it_gen->momentum());
    if(dR > dRMax) continue; // within deltaR cone
    genIsoSum += et;
    
  }// end of loop over gen particles

  return genIsoSum;
}

float VgAnalyzerKit::getGenCalIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho,
					 const Float_t dRMax, bool removeMu, bool removeNu)
{
  const Float_t etMin = 0.0;
  Float_t genCalIsoSum = 0.0;
  if(!doGenParticles_)return genCalIsoSum;
  if(!handle.isValid())return genCalIsoSum;

  for (reco::GenParticleCollection::const_iterator it_gen=handle->begin(); it_gen!=handle->end(); it_gen++){

    if(it_gen == thisPho)continue;      // can't be the original photon
    if(it_gen->status()!=1)continue;    // need to be a stable particle
    if (thisPho->collisionId() != it_gen->collisionId()) continue; // has to come from the same collision
   
    Int_t pdgCode = abs(it_gen->pdgId());
    // we should not count neutrinos, muons
    if( removeMu && pdgCode == 13 ) continue;
    if( removeNu && ( pdgCode == 12 || pdgCode == 14 || pdgCode == 16 ) ) continue;

    Float_t et = it_gen->et();
    if(et < etMin) continue; // pass a minimum et threshold, default 0

    Float_t dR = reco::deltaR(thisPho->momentum(), it_gen->momentum());
    if(dR > dRMax) continue; // within deltaR cone
    genCalIsoSum += et;
    
  }// end of loop over gen particles

  return genCalIsoSum;
}

float VgAnalyzerKit::getGenTrkIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax)
{
  const Float_t ptMin = 0.0;
  Float_t genTrkIsoSum = 0.0;
  if(!doGenParticles_)return genTrkIsoSum;
  if(!handle.isValid())return genTrkIsoSum;

  for (reco::GenParticleCollection::const_iterator it_gen=handle->begin(); it_gen!=handle->end(); it_gen++){

    if(it_gen == thisPho)continue;      // can't be the original photon
    if(it_gen->status()!=1)continue;    // need to be a stable particle
    if (thisPho->collisionId() != it_gen->collisionId()) continue; // has to come from the same collision
   
    if(it_gen->charge()==0)continue;    // we should not count neutral particles
   
    Float_t pt = it_gen->pt();
    if(pt < ptMin) continue; // pass a minimum pt threshold, default 0

    Float_t dR = reco::deltaR(thisPho->momentum(), it_gen->momentum());
    if(dR > dRMax) continue; // within deltaR cone
    genTrkIsoSum += pt;
    
  }// end of loop over gen particles

  return genTrkIsoSum;
}

float VgAnalyzerKit::getESRatio(View<pat::Photon>::const_iterator photon, const edm::Event& e, const edm::EventSetup& iSetup)
{
  //get Geometry
  ESHandle<CaloGeometry> caloGeometry;
  iSetup.get<CaloGeometryRecord>().get(caloGeometry);
  const CaloSubdetectorGeometry *geometry = caloGeometry->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  const CaloSubdetectorGeometry *& geometry_p = geometry;

  Float_t esratio=1.;

  // Get ES rechits
  edm::Handle<EcalRecHitCollection> PreshowerRecHits;
  e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  if (PreshowerRecHits.isValid()) EcalRecHitCollection preshowerHits(*PreshowerRecHits);
  else return esratio;

  const reco::CaloClusterPtr seed = (*photon).superCluster()->seed();    
  reco::CaloCluster cluster = (*seed);
  const GlobalPoint phopoint(cluster.x(), cluster.y(), cluster.z());

  DetId photmp1 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 1);
  DetId photmp2 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 2);
  ESDetId esfid = (photmp1 == DetId(0)) ? ESDetId(0) : ESDetId(photmp1);
  ESDetId esrid = (photmp2 == DetId(0)) ? ESDetId(0) : ESDetId(photmp2);

  if (esfid.strip() != 0 && esrid.strip() != 0) {
    int gs_esfid = -99;
    int gs_esrid = -99;
    gs_esfid = esfid.six()*32 + esfid.strip();
    gs_esrid = esrid.siy()*32 + esrid.strip();

    float esfe3 = 0.; 
    float esfe21 = 0.; 
    float esre3 = 0.; 
    float esre21 = 0.;

    const ESRecHitCollection *ESRH = PreshowerRecHits.product();

    for (EcalRecHitCollection::const_iterator esrh_it = ESRH->begin(); esrh_it != ESRH->end(); esrh_it++) {
      ESDetId esdetid = ESDetId(esrh_it->id());

      if (esrh_it->recoFlag() == 1 || (esrh_it->recoFlag() >= 5 && esrh_it->recoFlag() <= 10)) continue;
      // ES Front plane
      if (esdetid.plane() == 1) {
	if (esdetid.zside() == esfid.zside() && esdetid.siy() == esfid.siy()) {
	  int gs_esid = esdetid.six()*32 + esdetid.strip();
	  int ss = gs_esid - gs_esfid;
	  if (abs(ss) <= 10) {
	    esfe21 += esrh_it->energy();
	  } 
	  if (abs(ss) <= 1) {
	    esfe3 += esrh_it->energy();
	  } 
	}
      }
      // ES Rear plane
      if (esdetid.plane() == 2) {
	if (esdetid.zside() == esrid.zside() && esdetid.six() == esrid.six()) {
	  int gs_esid = esdetid.siy()*32+esdetid.strip();
	  int ss = gs_esid-gs_esrid;
	  if (abs(ss) <= 10) {
	    esre21 += esrh_it->energy();
	  } 
	  if (abs(ss) <= 1) {
	    esre3 += esrh_it->energy();
	  } 
	}
      }
    }
  
    // If denominator == 0, the ratio = 1;
    if((esfe21+esre21) == 0.) {
      esratio = 1.;
    }
    else{
      esratio = (esfe3+esre3) / (esfe21+esre21);
    }
  }
 
  return esratio;
}

float VgAnalyzerKit::getESRatio(View<pat::Electron>::const_iterator photon, const edm::Event& e, const edm::EventSetup& iSetup)
{
  //get Geometry
  ESHandle<CaloGeometry> caloGeometry;
  iSetup.get<CaloGeometryRecord>().get(caloGeometry);
  const CaloSubdetectorGeometry *geometry = caloGeometry->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  const CaloSubdetectorGeometry *& geometry_p = geometry;

  Float_t esratio=1.;
  // Get ES rechits
  edm::Handle<EcalRecHitCollection> PreshowerRecHits;
  e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  if (PreshowerRecHits.isValid()) EcalRecHitCollection preshowerHits(*PreshowerRecHits);
  else return esratio;

  const reco::CaloClusterPtr seed = (*photon).superCluster()->seed();    
  reco::CaloCluster cluster = (*seed);
  const GlobalPoint phopoint(cluster.x(), cluster.y(), cluster.z());

  DetId photmp1 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 1);
  DetId photmp2 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 2);
  ESDetId esfid = (photmp1 == DetId(0)) ? ESDetId(0) : ESDetId(photmp1);
  ESDetId esrid = (photmp2 == DetId(0)) ? ESDetId(0) : ESDetId(photmp2);

  if (esfid.strip() != 0 && esrid.strip() != 0) {
    int gs_esfid = -99;
    int gs_esrid = -99;
    gs_esfid = esfid.six()*32 + esfid.strip();
    gs_esrid = esrid.siy()*32 + esrid.strip();

    float esfe3 = 0.; 
    float esfe21 = 0.; 
    float esre3 = 0.; 
    float esre21 = 0.;

    const ESRecHitCollection *ESRH = PreshowerRecHits.product();

    for (EcalRecHitCollection::const_iterator esrh_it = ESRH->begin(); esrh_it != ESRH->end(); esrh_it++) {
      ESDetId esdetid = ESDetId(esrh_it->id());

      if (esrh_it->recoFlag() == 1 || (esrh_it->recoFlag() >= 5 && esrh_it->recoFlag() <= 10)) continue;
      // ES Front plane
      if (esdetid.plane() == 1) {
	if (esdetid.zside() == esfid.zside() && esdetid.siy() == esfid.siy()) {
	  int gs_esid = esdetid.six()*32 + esdetid.strip();
	  int ss = gs_esid - gs_esfid;
	  if (abs(ss) <= 10) {
	    esfe21 += esrh_it->energy();
	  } 
	  if (abs(ss) <= 1) {
	    esfe3 += esrh_it->energy();
	  } 
	}
      }
      // ES Rear plane
      if (esdetid.plane() == 2) {
	if (esdetid.zside() == esrid.zside() && esdetid.six() == esrid.six()) {
	  int gs_esid = esdetid.siy()*32+esdetid.strip();
	  int ss = gs_esid-gs_esrid;
	  if (abs(ss) <= 10) {
	    esre21 += esrh_it->energy();
	  } 
	  if (abs(ss) <= 1) {
	    esre3 += esrh_it->energy();
	  } 
	}
      }
    }
  
    // If denominator == 0, the ratio = 1;
    if((esfe21+esre21) == 0.) {
      esratio = 1.;
    }
    else{
      esratio = (esfe3+esre3) / (esfe21+esre21);
    }
  }
 
  return esratio;
}

std::vector<float> VgAnalyzerKit::getESProfileFront(View<pat::Photon>::const_iterator photon, const edm::Event& e, const edm::EventSetup& iSetup)
{
  //get Geometry
  ESHandle<CaloGeometry> caloGeometry;
  iSetup.get<CaloGeometryRecord>().get(caloGeometry);
  const CaloSubdetectorGeometry *geometry = caloGeometry->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  const CaloSubdetectorGeometry *& geometry_p = geometry;

  // Store ES rechits energy ±1 sensor an ±20 strips
  //  0 ~ 40: +1 sensor and ±20 strips
  // 41 ~ 81: +0 sensor and ±20 strips
  // 82 ~123: -1 sensor and ±20 strips
  std::vector<float> esprofile;
  for (int a = 0; a<123; a++) 
    esprofile.push_back(0); 

  // Get ES rechits
  edm::Handle<EcalRecHitCollection> PreshowerRecHits;
  e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  if (PreshowerRecHits.isValid()) EcalRecHitCollection preshowerHits(*PreshowerRecHits);
  else return esprofile;

  const reco::CaloClusterPtr seed = (*photon).superCluster()->seed();    
  reco::CaloCluster cluster = (*seed);
  const GlobalPoint phopoint(cluster.x(), cluster.y(), cluster.z());

  DetId photmp1 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 1);
  DetId photmp2 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 2);
  ESDetId esfid = (photmp1 == DetId(0)) ? ESDetId(0) : ESDetId(photmp1);
  ESDetId esrid = (photmp2 == DetId(0)) ? ESDetId(0) : ESDetId(photmp2);

  if (esfid.strip() != 0 && esrid.strip() != 0) {
    int strip[41];
    for (int a=0; a<41; a++) 
      strip[a] = a - 20;

    int gs_esfid = -99;
    int gs_esrid = -99;
    gs_esfid = esfid.six()*32 + esfid.strip();
    gs_esrid = esrid.siy()*32 + esrid.strip();

    const ESRecHitCollection *ESRH = PreshowerRecHits.product();

    for (EcalRecHitCollection::const_iterator esrh_it = ESRH->begin(); esrh_it != ESRH->end(); esrh_it++) {
      ESDetId esdetid = ESDetId(esrh_it->id());

      if (esrh_it->recoFlag() == 1 || (esrh_it->recoFlag() >= 5 && esrh_it->recoFlag() <= 10)) continue;
      // ES Front plane
      if (esdetid.plane() == 1) {
	// +1 sensor and ±20 strip
	if (esdetid.zside() == esfid.zside() && esdetid.siy() == (esfid.siy() + 1)) {
	  int gs_esid = esdetid.six()*32 + esdetid.strip();
	  int ss = gs_esid - gs_esfid;
          for (int a=0; a<41; a++) 
            if (ss == strip[a]) esprofile[a] = esrh_it->energy();
	}
        // +0 sensor and ±20 strip
        else if (esdetid.zside() == esfid.zside() && esdetid.siy() == (esfid.siy())) {
          int gs_esid = esdetid.six()*32 + esdetid.strip();
          int ss = gs_esid - gs_esfid;
          for (int a=0; a<41; a++)
            if (ss == strip[a]) esprofile[a+41] = esrh_it->energy();
        }
        // -1 sensor and ±20 strip
        else if (esdetid.zside() == esfid.zside() && esdetid.siy() == (esfid.siy() - 1)) {
          int gs_esid = esdetid.six()*32 + esdetid.strip();
          int ss = gs_esid - gs_esfid;
          for (int a=0; a<41; a++)
            if (ss == strip[a]) esprofile[a+82] = esrh_it->energy();
        }
      }
    }
  }

  return esprofile;
}

std::vector<float> VgAnalyzerKit::getESProfileRear(View<pat::Photon>::const_iterator photon, const edm::Event& e, const edm::EventSetup& iSetup)
{
  //get Geometry
  ESHandle<CaloGeometry> caloGeometry;
  iSetup.get<CaloGeometryRecord>().get(caloGeometry);
  const CaloSubdetectorGeometry *geometry = caloGeometry->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  const CaloSubdetectorGeometry *& geometry_p = geometry;

  // Store ES rechits energy ±1 sensor an ±20 strips
  //  0 ~ 40: +1 sensor and ±20 strips
  // 41 ~ 81: +0 sensor and ±20 strips
  // 82 ~123: -1 sensor and ±20 strips
  std::vector<float> esprofile;
  for (int a = 0; a<123; a++) 
    esprofile.push_back(0); 

  // Get ES rechits
  edm::Handle<EcalRecHitCollection> PreshowerRecHits;
  e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  if (PreshowerRecHits.isValid()) EcalRecHitCollection preshowerHits(*PreshowerRecHits);
  else return esprofile;

  const reco::CaloClusterPtr seed = (*photon).superCluster()->seed();    
  reco::CaloCluster cluster = (*seed);
  const GlobalPoint phopoint(cluster.x(), cluster.y(), cluster.z());

  DetId photmp1 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 1);
  DetId photmp2 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 2);
  ESDetId esfid = (photmp1 == DetId(0)) ? ESDetId(0) : ESDetId(photmp1);
  ESDetId esrid = (photmp2 == DetId(0)) ? ESDetId(0) : ESDetId(photmp2);

  if (esfid.strip() != 0 && esrid.strip() != 0) {
    int strip[41];
    for (int a=0; a<41; a++) 
      strip[a] = a - 20;

    int gs_esfid = -99;
    int gs_esrid = -99;
    gs_esfid = esfid.six()*32 + esfid.strip();
    gs_esrid = esrid.siy()*32 + esrid.strip();

    const ESRecHitCollection *ESRH = PreshowerRecHits.product();

    for (EcalRecHitCollection::const_iterator esrh_it = ESRH->begin(); esrh_it != ESRH->end(); esrh_it++) {
      ESDetId esdetid = ESDetId(esrh_it->id());

      if (esrh_it->recoFlag() == 1 || (esrh_it->recoFlag() >= 5 && esrh_it->recoFlag() <= 10)) continue;
      // ES Rear plane
      if (esdetid.plane() == 2) {
	// +1 sensor and ±20 strip
	if (esdetid.zside() == esrid.zside() && esdetid.six() == (esrid.six() + 1)) {
	  int gs_esid = esdetid.siy()*32 + esdetid.strip();
	  int ss = gs_esid - gs_esrid;
          for (int a=0; a<41; a++) 
            if (ss == strip[a]) esprofile[a] = esrh_it->energy();
	}
        // +0 sensor and ±20 strip
        else if (esdetid.zside() == esrid.zside() && esdetid.six() == (esrid.six())) {
          int gs_esid = esdetid.siy()*32 + esdetid.strip();
          int ss = gs_esid - gs_esrid;
          for (int a=0; a<41; a++)
            if (ss == strip[a]) esprofile[a+41] = esrh_it->energy();
        }
        // -1 sensor and ±20 strip
        else if (esdetid.zside() == esrid.zside() && esdetid.six() == (esrid.six() - 1)) {
          int gs_esid = esdetid.siy()*32 + esdetid.strip();
          int ss = gs_esid - gs_esrid;
          for (int a=0; a<41; a++)
            if (ss == strip[a]) esprofile[a+82] = esrh_it->energy();
        }
      }
    }
  }

  return esprofile;
}

std::vector<float> VgAnalyzerKit::getESProfileFront(View<pat::Electron>::const_iterator photon, const edm::Event& e, const edm::EventSetup& iSetup)
{
  //get Geometry
  ESHandle<CaloGeometry> caloGeometry;
  iSetup.get<CaloGeometryRecord>().get(caloGeometry);
  const CaloSubdetectorGeometry *geometry = caloGeometry->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  const CaloSubdetectorGeometry *& geometry_p = geometry;

  // Store ES rechits energy ±1 sensor an ±20 strips
  //  0 ~ 40: +1 sensor and ±20 strips
  // 41 ~ 81: +0 sensor and ±20 strips
  // 82 ~123: -1 sensor and ±20 strips
  std::vector<float> esprofile;
  for (int a = 0; a<123; a++) 
    esprofile.push_back(0); 

  // Get ES rechits
  edm::Handle<EcalRecHitCollection> PreshowerRecHits;
  e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  if (PreshowerRecHits.isValid()) EcalRecHitCollection preshowerHits(*PreshowerRecHits);
  else return esprofile;

  const reco::CaloClusterPtr seed = (*photon).superCluster()->seed();    
  reco::CaloCluster cluster = (*seed);
  const GlobalPoint phopoint(cluster.x(), cluster.y(), cluster.z());

  DetId photmp1 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 1);
  DetId photmp2 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 2);
  ESDetId esfid = (photmp1 == DetId(0)) ? ESDetId(0) : ESDetId(photmp1);
  ESDetId esrid = (photmp2 == DetId(0)) ? ESDetId(0) : ESDetId(photmp2);

  if (esfid.strip() != 0 && esrid.strip() != 0) {
    int strip[41];
    for (int a=0; a<41; a++) 
      strip[a] = a - 20;

    int gs_esfid = -99;
    int gs_esrid = -99;
    gs_esfid = esfid.six()*32 + esfid.strip();
    gs_esrid = esrid.siy()*32 + esrid.strip();

    const ESRecHitCollection *ESRH = PreshowerRecHits.product();

    for (EcalRecHitCollection::const_iterator esrh_it = ESRH->begin(); esrh_it != ESRH->end(); esrh_it++) {
      ESDetId esdetid = ESDetId(esrh_it->id());

      if (esrh_it->recoFlag() == 1 || (esrh_it->recoFlag() >= 5 && esrh_it->recoFlag() <= 10)) continue;
      // ES Front plane
      if (esdetid.plane() == 1) {
	// +1 sensor and ±20 strip
	if (esdetid.zside() == esfid.zside() && esdetid.siy() == (esfid.siy() + 1)) {
	  int gs_esid = esdetid.six()*32 + esdetid.strip();
	  int ss = gs_esid - gs_esfid;
          for (int a=0; a<41; a++) 
            if (ss == strip[a]) esprofile[a] = esrh_it->energy();
	}
        // +0 sensor and ±20 strip
        else if (esdetid.zside() == esfid.zside() && esdetid.siy() == (esfid.siy())) {
          int gs_esid = esdetid.six()*32 + esdetid.strip();
          int ss = gs_esid - gs_esfid;
          for (int a=0; a<41; a++)
            if (ss == strip[a]) esprofile[a+41] = esrh_it->energy();
        }
        // -1 sensor and ±20 strip
        else if (esdetid.zside() == esfid.zside() && esdetid.siy() == (esfid.siy() - 1)) {
          int gs_esid = esdetid.six()*32 + esdetid.strip();
          int ss = gs_esid - gs_esfid;
          for (int a=0; a<41; a++)
            if (ss == strip[a]) esprofile[a+82] = esrh_it->energy();
        }
      }
    }
  }

  return esprofile;
}

std::vector<float> VgAnalyzerKit::getESProfileRear(View<pat::Electron>::const_iterator photon, const edm::Event& e, const edm::EventSetup& iSetup)
{
  //get Geometry
  ESHandle<CaloGeometry> caloGeometry;
  iSetup.get<CaloGeometryRecord>().get(caloGeometry);
  const CaloSubdetectorGeometry *geometry = caloGeometry->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  const CaloSubdetectorGeometry *& geometry_p = geometry;

  // Store ES rechits energy ±1 sensor an ±20 strips
  //  0 ~ 40: +1 sensor and ±20 strips
  // 41 ~ 81: +0 sensor and ±20 strips
  // 82 ~123: -1 sensor and ±20 strips
  std::vector<float> esprofile;
  for (int a = 0; a<123; a++) 
    esprofile.push_back(0); 

  // Get ES rechits
  edm::Handle<EcalRecHitCollection> PreshowerRecHits;
  e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  if (PreshowerRecHits.isValid()) EcalRecHitCollection preshowerHits(*PreshowerRecHits);
  else return esprofile;

  const reco::CaloClusterPtr seed = (*photon).superCluster()->seed();    
  reco::CaloCluster cluster = (*seed);
  const GlobalPoint phopoint(cluster.x(), cluster.y(), cluster.z());

  DetId photmp1 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 1);
  DetId photmp2 = (dynamic_cast<const EcalPreshowerGeometry*>(geometry_p))->getClosestCellInPlane(phopoint, 2);
  ESDetId esfid = (photmp1 == DetId(0)) ? ESDetId(0) : ESDetId(photmp1);
  ESDetId esrid = (photmp2 == DetId(0)) ? ESDetId(0) : ESDetId(photmp2);

  if (esfid.strip() != 0 && esrid.strip() != 0) {
    int strip[41];
    for (int a=0; a<41; a++) 
      strip[a] = a - 20;

    int gs_esfid = -99;
    int gs_esrid = -99;
    gs_esfid = esfid.six()*32 + esfid.strip();
    gs_esrid = esrid.siy()*32 + esrid.strip();

    const ESRecHitCollection *ESRH = PreshowerRecHits.product();

    for (EcalRecHitCollection::const_iterator esrh_it = ESRH->begin(); esrh_it != ESRH->end(); esrh_it++) {
      ESDetId esdetid = ESDetId(esrh_it->id());

      if (esrh_it->recoFlag() == 1 || (esrh_it->recoFlag() >= 5 && esrh_it->recoFlag() <= 10)) continue;
      // ES Rear plane
      if (esdetid.plane() == 2) {
	// +1 sensor and ±20 strip
	if (esdetid.zside() == esrid.zside() && esdetid.six() == (esrid.six() + 1)) {
	  int gs_esid = esdetid.siy()*32 + esdetid.strip();
	  int ss = gs_esid - gs_esrid;
          for (int a=0; a<41; a++) 
            if (ss == strip[a]) esprofile[a] = esrh_it->energy();
	}
        // +0 sensor and ±20 strip
        else if (esdetid.zside() == esrid.zside() && esdetid.six() == (esrid.six())) {
          int gs_esid = esdetid.siy()*32 + esdetid.strip();
          int ss = gs_esid - gs_esrid;
          for (int a=0; a<41; a++)
            if (ss == strip[a]) esprofile[a+41] = esrh_it->energy();
        }
        // -1 sensor and ±20 strip
        else if (esdetid.zside() == esrid.zside() && esdetid.six() == (esrid.six() - 1)) {
          int gs_esid = esdetid.siy()*32 + esdetid.strip();
          int ss = gs_esid - gs_esrid;
          for (int a=0; a<41; a++)
            if (ss == strip[a]) esprofile[a+82] = esrh_it->energy();
        }
      }
    }
  }

  return esprofile;
}

DEFINE_FWK_MODULE(VgAnalyzerKit);
