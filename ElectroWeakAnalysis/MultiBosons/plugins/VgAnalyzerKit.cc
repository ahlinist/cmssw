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
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"
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
#include "RecoEgamma/EgammaIsolationAlgos/interface/PhotonTkIsolation.h"

// Conversion photon
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

// PF candidates
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

// New veto
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

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
  doStoreJets_    = ps.getParameter<bool>("doStoreJets");
  doJetHLTMatch_  = ps.getParameter<bool>("doJetHLTMatch");
  doSkim_         = ps.getParameter<bool>("doSkim");
  gtdigilabel_    = ps.getParameter<InputTag>("GTDigiLabel");
  vtxlabel_       = ps.getParameter<InputTag>("VtxLabel");
  caloTowerlabel_ = ps.getParameter<InputTag>("CaloTowerLabel");
  tracklabel_     = ps.getParameter<InputTag>("TrackLabel");
  tcMETlabel_     = ps.getParameter<InputTag>("tcMETLabel");
  pfMETlabel_     = ps.getParameter<InputTag>("pfMETLabel");
  TypeIpfMETlabel_  = ps.getParameter<InputTag>("TypeIpfMETLabel");
  TypeIpIIpfMETlabel_ = ps.getParameter<InputTag>("TypeIpIIpfMETLabel");
  SmearedpfMETlabel_     = ps.getParameter<InputTag>("SmearedpfMETLabel");
  SmearedTypeIpfMETlabel_  = ps.getParameter<InputTag>("SmearedTypeIpfMETLabel");
  PFCandLabel_    = ps.getParameter<InputTag>("PFCandLabel");
  puInfoLabel_    = ps.getParameter<InputTag>("puInfoLabel");
  rhoLabel_       = ps.getParameter<InputTag>("rhoLabel");
  sigmaLabel_     = ps.getParameter<InputTag>("sigmaLabel");
  rhoNeutralLabel_  = ps.getParameter<InputTag>("rhoNeutralLabel");

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
  tree_->Branch("event", &event_, "event/L");
  tree_->Branch("orbit", &orbit_, "orbit/I");
  tree_->Branch("bx", &bx_, "bx/I");
  tree_->Branch("lumis", &lumis_, "lumis/I");
  tree_->Branch("isData", &isData_, "isData/O");
  tree_->Branch("ttbit0", &ttbit0_, "ttbit0/I");
  tree_->Branch("nHLT", &nHLT_, "nHLT/I");
  tree_->Branch("HLT", HLT_, "HLT[nHLT]/I");
  tree_->Branch("HLTIndex", HLTIndex_, "HLTIndex[125]/I");
  tree_->Branch("HLTprescale", HLTprescale_, "HLTprescale[nHLT]/I");
  tree_->Branch("nHFTowersP", &nHFTowersP_, "nHFTowersP/I");
  tree_->Branch("nHFTowersN", &nHFTowersN_, "nHFTowersN/I");
  tree_->Branch("nVtx", &nVtx_, "nVtx/I");
  tree_->Branch("vtx", vtx_, "vtx[nVtx][3]/F");
  tree_->Branch("vtxNTrk", vtxNTrk_, "vtxNTrk[nVtx]/I");
  tree_->Branch("vtxNDF", vtxNDF_, "vtxNDF[nVtx]/F");
  tree_->Branch("vtxD0", vtxD0_, "vtxD0[nVtx]/F");
  tree_->Branch("nGoodVtx", &nGoodVtx_, "nGoodVtx/I");
  tree_->Branch("IsVtxGood", &IsVtxGood_, "IsVtxGood/I");
  tree_->Branch("nTrk", &nTrk_, "nTrk/I");
  tree_->Branch("nGoodTrk", &nGoodTrk_, "nGoodTrk/I");
  tree_->Branch("IsTracksGood", &IsTracksGood_, "IsTracksGood/I");
  tree_->Branch("rho", &rho_, "rho/F");
  tree_->Branch("sigma", &sigma_, "sigma/F");
  tree_->Branch("rhoNeutral", &rhoNeutral_, "rhoNeutral/F");
  if (doGenParticles_) {
    tree_->Branch("pdf", pdf_, "pdf[7]/F");
    tree_->Branch("pthat", &pthat_, "pthat/F");
    tree_->Branch("processID", &processID_, "processID/F");
    tree_->Branch("TrueNI", &TrueNI_, "TrueNI/F");
    tree_->Branch("nBX", &nBX_, "nBX/I");
    tree_->Branch("nPU", nPU_, "nPU[nBX]/I");
    tree_->Branch("BXPU", BXPU_, "BXPU[nBX]/I");
    // genParticle
    tree_->Branch("nMC", &nMC_, "nMC/I");
    tree_->Branch("mcPID", mcPID, "mcPID[nMC]/I");
    tree_->Branch("mcVtx", mcVtx, "mcVtx[nMC][3]/F");
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
  // TypeI pfMET
  tree_->Branch("TypeIpfMET", &TypeIpfMET_, "TypeIpfMET/F");
  tree_->Branch("TypeIpfMETx", &TypeIpfMETx_, "TypeIpfMETx/F");
  tree_->Branch("TypeIpfMETy", &TypeIpfMETy_, "TypeIpfMETy/F");
  tree_->Branch("TypeIpfMETPhi", &TypeIpfMETPhi_, "TypeIpfMETPhi/F");
  tree_->Branch("TypeIpfMETsumEt", &TypeIpfMETsumEt_, "TypeIpfMETsumEt/F");
  tree_->Branch("TypeIpfMETmEtSig", &TypeIpfMETmEtSig_, "TypeIpfMETmEtSig/F");
  tree_->Branch("TypeIpfMETSig", &TypeIpfMETSig_, "TypeIpfMETSig/F");
  // TypeI + TypeII pfMET 
  tree_->Branch("TypeIpIIpfMET", &TypeIpIIpfMET_, "TypeIpIIpfMET/F");
  tree_->Branch("TypeIpIIpfMETx", &TypeIpIIpfMETx_, "TypeIpIIpfMETx/F");
  tree_->Branch("TypeIpIIpfMETy", &TypeIpIIpfMETy_, "TypeIpIIpfMETy/F");
  tree_->Branch("TypeIpIIpfMETPhi", &TypeIpIIpfMETPhi_, "TypeIpIIpfMETPhi/F");
  tree_->Branch("TypeIpIIpfMETsumEt", &TypeIpIIpfMETsumEt_, "TypeIpIIpfMETsumEt/F");
  tree_->Branch("TypeIpIIpfMETmEtSig", &TypeIpIIpfMETmEtSig_, "TypeIpIIpfMETmEtSig/F");
  tree_->Branch("TypeIpIIpfMETSig", &TypeIpIIpfMETSig_, "TypeIpIIpfMETSig/F");
  if (doGenParticles_) {
    // Smeared pfMET
    tree_->Branch("SmearedpfMET", &SmearedpfMET_, "SmearedpfMET/F");
    tree_->Branch("SmearedpfMETx", &SmearedpfMETx_, "SmearedpfMETx/F");
    tree_->Branch("SmearedpfMETy", &SmearedpfMETy_, "SmearedpfMETy/F");
    tree_->Branch("SmearedpfMETPhi", &SmearedpfMETPhi_, "SmearedpfMETPhi/F");
    tree_->Branch("SmearedpfMETsumEt", &SmearedpfMETsumEt_, "SmearedpfMETsumEt/F");
    tree_->Branch("SmearedpfMETmEtSig", &SmearedpfMETmEtSig_, "SmearedpfMETmEtSig/F");
    tree_->Branch("SmearedpfMETSig", &SmearedpfMETSig_, "SmearedpfMETSig/F");
    // Smeared TypeI pfMET
    tree_->Branch("SmearedTypeIpfMET", &SmearedTypeIpfMET_, "SmearedTypeIpfMET/F");
    tree_->Branch("SmearedTypeIpfMETx", &SmearedTypeIpfMETx_, "SmearedTypeIpfMETx/F");
    tree_->Branch("SmearedTypeIpfMETy", &SmearedTypeIpfMETy_, "SmearedTypeIpfMETy/F");
    tree_->Branch("SmearedTypeIpfMETPhi", &SmearedTypeIpfMETPhi_, "SmearedTypeIpfMETPhi/F");
    tree_->Branch("SmearedTypeIpfMETsumEt", &SmearedTypeIpfMETsumEt_, "SmearedTypeIpfMETsumEt/F");
    tree_->Branch("SmearedTypeIpfMETmEtSig", &SmearedTypeIpfMETmEtSig_, "SmearedTypeIpfMETmEtSig/F");
    tree_->Branch("SmearedTypeIpfMETSig", &SmearedTypeIpfMETSig_, "SmearedTypeIpfMETSig/F");
  }
  // pfCharged and pfNeutral particles
  tree_->Branch("npfCharged", &npfCharged_, "npfCharged/I");
  tree_->Branch("pfChargedSumPt", &pfChargedSumPt_, "pfChargedSumPt/F");
  tree_->Branch("npfChargedHadron", &npfChargedHadron_, "npfChargedHadron/I");
  tree_->Branch("pfChargedHadronSumPt", &pfChargedHadronSumPt_, "pfChargedHadronSumPt/F");
  tree_->Branch("npfLepton", &npfLepton_, "npfLepton/I");
  tree_->Branch("pfLeptonSumPt", &pfLeptonSumPt_, "pfLeptonSumPt/F");
  tree_->Branch("npfNeutral", &npfNeutral_, "npfNeutral/I");
  tree_->Branch("pfNeutralSumPt", &pfNeutralSumPt_, "pfNeutralSumPt/F");
  tree_->Branch("npfNeutralHadron", &npfNeutralHadron_, "npfNeutralHadron/I");
  tree_->Branch("pfNeutralHadronSumPt", &pfNeutralHadronSumPt_, "pfNeutralHadronSumPt/F");
  tree_->Branch("npfPhoton", &npfPhoton_, "npfPhoton/I");
  tree_->Branch("pfPhotonSumPt", &pfPhotonSumPt_, "pfPhotonSumPt/F");
  // Electron
  tree_->Branch("nEle", &nEle_, "nEle/I");
  tree_->Branch("eleTrg", eleTrg_, "eleTrg[nEle][31]/I");
  tree_->Branch("eleID", eleID_, "eleID[nEle][30]/I");
  tree_->Branch("eleIDLH", eleIDLH_, "eleIDLH[nEle]/F");
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
  tree_->Branch("elenBrem", elenBrem_, "elenBrem[nEle]/I");
  tree_->Branch("eledEtaAtVtx", eledEtaAtVtx_, "eledEtaAtVtx[nEle]/F");
  tree_->Branch("eledPhiAtVtx", eledPhiAtVtx_, "eledPhiAtVtx[nEle]/F");
  tree_->Branch("eleSigmaEtaEta", eleSigmaEtaEta_, "eleSigmaEtaEta[nEle]/F");
  tree_->Branch("eleSigmaIEtaIEta", eleSigmaIEtaIEta_, "eleSigmaIEtaIEta[nEle]/F");
  tree_->Branch("eleSigmaIEtaIPhi", eleSigmaIEtaIPhi_, "eleSigmaIEtaIPhi[nEle]/F");
  tree_->Branch("eleSigmaIPhiIPhi", eleSigmaIPhiIPhi_, "eleSigmaIPhiIPhi[nEle]/F");
  tree_->Branch("eleE3x3", eleE3x3_, "eleE3x3[nEle]/F");
  tree_->Branch("eleSeedTime", eleSeedTime_, "eleSeedTime[nEle]/F");
  tree_->Branch("eleSeedEnergy", eleSeedEnergy_, "eleSeedEnergy[nEle]/F");
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
  tree_->Branch("eleConversionveto", eleConversionveto_, "eleConversionveto[nEle]/O");
  tree_->Branch("eleESRatio", eleESRatio_, "eleESRatio[nEle]/F");
  tree_->Branch("eleESProfileFront", eleESProfileFront_, "eleESProfileFront[nEle][123]/F");
  tree_->Branch("eleESProfileRear", eleESProfileRear_, "eleESProfileRear[nEle][123]/F");
  tree_->Branch("elePV2D", elePV2D_, "elePV2D[nEle]/F");
  tree_->Branch("elePV3D", elePV3D_, "elePV3D[nEle]/F");
  tree_->Branch("eleBS2D", eleBS2D_, "eleBS2D[nEle]/F");
  tree_->Branch("eleBS3D", eleBS3D_, "eleBS3D[nEle]/F");
  tree_->Branch("elePVD0", elePVD0_, "elePVD0[nEle]/F");
  tree_->Branch("elePVDz", elePVDz_, "elePVDz[nEle]/F");
  // Photon
  tree_->Branch("nPho", &nPho_, "nPho/I");
  tree_->Branch("phoTrg", phoTrg_, "phoTrg[nPho][14]/I");
  tree_->Branch("phoIsPhoton", phoIsPhoton_, "phoIsPhoton[nPho]/O");
  tree_->Branch("phoElectronveto", phoElectronveto_, "phoElectronveto[nPho]/O");
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
  tree_->Branch("phoEtVtx", phoEtVtx_, "phoEtVtx[nPho][150]/F");
  tree_->Branch("phoEtaVtx", phoEtaVtx_, "phoEtaVtx[nPho][150]/F");
  tree_->Branch("phoPhiVtx", phoPhiVtx_, "phoPhiVtx[nPho][150]/F");
  tree_->Branch("phoTrkIsoSolidDR03Vtx", phoTrkIsoSolidDR03Vtx_, "phoTrkIsoSolidDR03Vtx[nPho][150]/F");
  tree_->Branch("phoTrkIsoHollowDR03Vtx", phoTrkIsoHollowDR03Vtx_, "phoTrkIsoHollowDR03Vtx[nPho][150]/F");
  tree_->Branch("phoTrkIsoSolidDR04Vtx", phoTrkIsoSolidDR04Vtx_, "phoTrkIsoSolidDR04Vtx[nPho][150]/F");
  tree_->Branch("phoTrkIsoHollowDR04Vtx", phoTrkIsoHollowDR04Vtx_, "phoTrkIsoHollowDR04Vtx[nPho][150]/F");
  tree_->Branch("phoHoverE", phoHoverE_, "phoHoverE[nPho]/F");
  tree_->Branch("phoSigmaEtaEta", phoSigmaEtaEta_, "phoSigmaEtaEta[nPho]/F");
  tree_->Branch("phoSigmaIEtaIEta", phoSigmaIEtaIEta_, "phoSigmaIEtaIEta[nPho]/F");
  tree_->Branch("phoSigmaIEtaIPhi", phoSigmaIEtaIPhi_, "phoSigmaIEtaIPhi[nPho]/F");
  tree_->Branch("phoSigmaIPhiIPhi", phoSigmaIPhiIPhi_, "phoSigmaIPhiIPhi[nPho]/F");
  tree_->Branch("phoE3x3", phoE3x3_, "phoE3x3[nPho]/F");
  tree_->Branch("phoE5x5", phoE5x5_, "phoE5x5[nPho]/F");
  tree_->Branch("phoSeedTime", phoSeedTime_, "phoSeedTime[nPho]/F");
  tree_->Branch("phoSeedEnergy", phoSeedEnergy_, "phoSeedEnergy[nPho]/F");
  // If Flag == 2, it means that rechit is out of time
  tree_->Branch("phoRecoFlag", phoRecoFlag_, "phoRecoFlag[nPho]/I");
  // If Severity == 3, it is spike. If Severity == 4, it is bad, not sutiable to be used in reconstruction.
  tree_->Branch("phoSeverity", phoSeverity_, "phoSeverity[nPho]/I");
  tree_->Branch("phoPos", phoPos_, "phoPos[nPho]/I");
  if (doGenParticles_) {
    tree_->Branch("phoGenIndex", phoGenIndex_, "phoGenIndex[nPho]/I");
    tree_->Branch("phoGenGMomPID", phoGenGMomPID, "phoGenGMomPID[nPho]/I");
    tree_->Branch("phoGenMomPID", phoGenMomPID, "phoGenMomPID[nPho]/I");
    tree_->Branch("phoGenMomPt", phoGenMomPt, "phoGenMomPt[nPho]/F");
  }
  tree_->Branch("phoSCE", phoSCE_, "phoSCE[nPho]/F");
  tree_->Branch("phoESE", phoESE_, "phoESE[nPho]/F");
  tree_->Branch("phoSCEt", phoSCEt_, "phoSCEt[nPho]/F");
  tree_->Branch("phoSCEta", phoSCEta_, "phoSCEta[nPho]/F");
  tree_->Branch("phoSCPhi", phoSCPhi_, "phoSCPhi[nPho]/F");
  tree_->Branch("phoSCEtaWidth", phoSCEtaWidth_, "phoSCEtaWidth[nPho]/F");
  tree_->Branch("phoSCPhiWidth", phoSCPhiWidth_, "phoSCPhiWidth[nPho]/F");
  tree_->Branch("phoVtx", phoVtx_, "phoVtx[nPho][3]/F");
  tree_->Branch("phoVtxD0", phoVtxD0_, "phoVtxD0[nPho]/F");
  tree_->Branch("phoOverlap", phoOverlap_, "phoOverlap[nPho]/I");
  tree_->Branch("phohasPixelSeed", phohasPixelSeed_, "phohasPixelSeed[nPho]/I");
  tree_->Branch("phoIsConv", phoIsConv_, "phoIsConv[nPho]/I");
  tree_->Branch("phoESRatio", phoESRatio_, "phoESRatio[nPho]/F");
  tree_->Branch("phoESProfileFront", phoESProfileFront_, "phoESProfileFront[nPho][123]/F");
  tree_->Branch("phoESProfileRear", phoESProfileRear_, "phoESProfileRear[nPho][123]/F");
  tree_->Branch("phoNTracks", phoNTracks_, "phoNTracks[nPho]/I");
  tree_->Branch("phoConvPairInvariantMass", phoConvPairInvariantMass_, "phoConvPairInvariantMass[nPho]/F");
  tree_->Branch("phoConvPairCotThetaSeparation", phoConvPairCotThetaSeparation_, "phoConvPairCotThetaSeparation[nPho]/F");
  tree_->Branch("phoConvPairMomentumEta", phoConvPairMomentumEta_, "phoConvPairMomentumEta[nPho]/F");
  tree_->Branch("phoConvPairMomentumPhi", phoConvPairMomentumPhi_, "phoConvPairMomentumPhi[nPho]/F");
  tree_->Branch("phoConvPairMomentumX", phoConvPairMomentumX_, "phoConvPairMomentumX[nPho]/F");
  tree_->Branch("phoConvPairMomentumY", phoConvPairMomentumY_, "phoConvPairMomentumY[nPho]/F");
  tree_->Branch("phoConvPairMomentumZ", phoConvPairMomentumZ_, "phoConvPairMomentumZ[nPho]/F");
  tree_->Branch("phoConvDistOfMinimumApproach", phoConvDistOfMinimumApproach_, "phoConvDistOfMinimumApproach[nPho]/F");
  tree_->Branch("phoConvDPhiTracksAtVtx", phoConvDPhiTracksAtVtx_, "phoConvDPhiTracksAtVtx[nPho]/F");
  tree_->Branch("phoConvDPhiTracksAtEcal", phoConvDPhiTracksAtEcal_, "phoConvDPhiTracksAtEcal[nPho]/F");
  tree_->Branch("phoConvDEtaTracksAtEcal", phoConvDEtaTracksAtEcal_, "phoConvDEtaTracksAtEcal[nPho]/F");
  tree_->Branch("phoConvVtxValid", phoConvVtxValid_, "phoConvVtxValid[nPho]/F");
  tree_->Branch("phoConvVtxEta", phoConvVtxEta_, "phoConvVtxEta[nPho]/F");
  tree_->Branch("phoConvVtxPhi", phoConvVtxPhi_, "phoConvVtxPhi[nPho]/F");
  tree_->Branch("phoConvVtxR", phoConvVtxR_, "phoConvVtxR[nPho]/F");
  tree_->Branch("phoConvVtxX", phoConvVtxX_, "phoConvVtxX[nPho]/F");
  tree_->Branch("phoConvVtxY", phoConvVtxY_, "phoConvVtxY[nPho]/F");
  tree_->Branch("phoConvVtxZ", phoConvVtxZ_, "phoConvVtxZ[nPho]/F");
  tree_->Branch("phoConvVtxChi2", phoConvVtxChi2_, "phoConvVtxChi2[nPho]/F");
  tree_->Branch("phoConvVtxNdof", phoConvVtxNdof_, "phoConvVtxNdof[nPho]/F");
  tree_->Branch("phoConvChi2Prob", phoConvChi2Prob_, "phoConvChi2Prob[nPho]/F");
  tree_->Branch("phoConvEoverP", phoConvEoverP_, "phoConvEoverP[nPho]/F");
  tree_->Branch("phoNxtal", phoNxtal_, "phoNxtal[nPho]/I");
  tree_->Branch("phoXtalTime", phoXtalTime_, "phoXtalTime_[nPho][200]/F");
  tree_->Branch("phoXtalEnergy", phoXtalEnergy_, "phoXtalEnergy[nPho][200]/F");
  tree_->Branch("phoXtalZ", phoXtalZ_, "phoXtalZ[nPho][200]/I");
  tree_->Branch("phoXtalX", phoXtalX_, "phoXtalX[nPho][200]/I");
  tree_->Branch("phoXtalY", phoXtalY_, "phoXtalY[nPho][200]/I");
  tree_->Branch("phoXtalEta", phoXtalEta_, "phoXtalEta[nPho][200]/I");
  tree_->Branch("phoXtalPhi", phoXtalPhi_, "phoXtalPhi[nPho][200]/I");
  tree_->Branch("pho5x5Time", pho5x5Time_, "pho5x5Time_[nPho][25]/F");
  tree_->Branch("pho5x5Energy", pho5x5Energy_, "pho5x5Energy[nPho][25]/F");
  tree_->Branch("pho5x5Z", pho5x5Z_, "pho5x5Z[nPho][25]/I");
  tree_->Branch("pho5x5X", pho5x5X_, "pho5x5X[nPho][25]/I");
  tree_->Branch("pho5x5Y", pho5x5Y_, "pho5x5Y[nPho][25]/I");
  tree_->Branch("pho5x5Eta", pho5x5Eta_, "pho5x5Eta[nPho][25]/I");
  tree_->Branch("pho5x5Phi", pho5x5Phi_, "pho5x5Phi[nPho][25]/I");

  // Muon
  tree_->Branch("nMu", &nMu_, "nMu/I");
  tree_->Branch("muTrg", muTrg_, "muTrg[nMu][16]/I");
  tree_->Branch("muEta", muEta_, "muEta[nMu]/F");
  tree_->Branch("muPhi", muPhi_, "muPhi[nMu]/F");
  tree_->Branch("muCharge", muCharge_, "muCharge[nMu]/I");
  tree_->Branch("muPt", muPt_, "muPt[nMu]/F");
  tree_->Branch("muPz", muPz_, "muPz[nMu]/F");
  if (doGenParticles_) {
    tree_->Branch("muGenIndex", muGenIndex_, "muGenIndex[nMu]/I");
    tree_->Branch("muGenGMomPID", muGenGMomPID_, "muGenGMomPID[nMu]/I");
    tree_->Branch("muGenMomPID", muGenMomPID_, "muGenMomPID[nMu]/I");
    tree_->Branch("muGenMomPt", muGenMomPt_, "muGenMomPt[nMu]/F");
  }
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
  tree_->Branch("muPVD0", muPVD0_, "muPVD0[nMu]/F");
  tree_->Branch("muPVDz", muPVDz_, "muPVDz[nMu]/F");
  tree_->Branch("muValidFraction", muValidFraction_, "muValidFraction_[nMu]/F");
  tree_->Branch("muTrkdPt", muTrkdPt_, "muTrkdPt[nMu]/F");
  tree_->Branch("muNumberOfHits", muNumberOfHits_, "muNumberOfHits[nMu]/I");
  tree_->Branch("muNumberOfValidHits", muNumberOfValidHits_, "muNumberOfValidHits[nMu]/I");
  tree_->Branch("muNumberOfInactiveHits", muNumberOfInactiveHits_, "muNumberOfInactiveHits[nMu]/I");
  tree_->Branch("muNumberOfValidTrkHits", muNumberOfValidTrkHits_, "muNumberOfValidTrkHits[nMu]/I");
  tree_->Branch("muNumberOfValidPixelHits", muNumberOfValidPixelHits_, "muNumberOfValidPixelHits[nMu]/I");
  tree_->Branch("muNumberOfValidMuonHits", muNumberOfValidMuonHits_, "muNumberOfValidMuonHits[nMu]/I");
  tree_->Branch("muStations", muStations_, "muStations[nMu]/I");
  tree_->Branch("muChambers", muChambers_, "muChambers[nMu]/I");
  tree_->Branch("muPV2D", muPV2D_, "muPV2D[nMu]/F");
  tree_->Branch("muPV3D", muPV3D_, "muPV3D[nMu]/F");
  tree_->Branch("muBS2D", muBS2D_, "muBS2D[nMu]/F");
  tree_->Branch("muBS3D", muBS3D_, "muBS3D[nMu]/F");
  tree_->Branch("muVtx", muVtx_, "muVtx[nMu][3]/F");
  // Jet
  if (doStoreJets_) {
    tree_->Branch("nJet", &nJet_, "nJet/I");
    tree_->Branch("jetTrg", jetTrg_, "jetTrg[nJet][23]/I");
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
    tree_->Branch("jetNeutralEmEnergy", jetNeutralEmEnergy_, "jetNeutralEmEnergy[nJet]/F");
    tree_->Branch("jetNeutralEmEnergyFraction", jetNeutralEmEnergyFraction_, "jetNeutralEmEnergyFraction[nJet]/F");
    tree_->Branch("jetNeutralHadronEnergy", jetNeutralHadronEnergy_, "jetNeutralHadronEnergy[nJet]/F");
    tree_->Branch("jetNeutralHadronEnergyFraction", jetNeutralHadronEnergyFraction_, "jetNeutralHadronEnergyFraction[nJet]/F");
    tree_->Branch("jetNConstituents", jetNConstituents_, "jetNConstituents[nJet]/I");
    tree_->Branch("jetChargedEmEnergy", jetChargedEmEnergy_, "jetChargedEmEnergy[nJet]/F");
    tree_->Branch("jetChargedEmEnergyFraction", jetChargedEmEnergyFraction_, "jetChargedEmEnergyFraction[nJet]/F");
    tree_->Branch("jetChargedHadronEnergy", jetChargedHadronEnergy_, "jetChargedHadronEnergy[nJet]/F");
    tree_->Branch("jetChargedHadronEnergyFraction", jetChargedHadronEnergyFraction_, "jetChargedHadronEnergyFraction[nJet]/F");
    tree_->Branch("jetChargedHadronMultiplicity", jetChargedHadronMultiplicity_, "jetChargedHadronMultiplicity[nJet]/I");
    tree_->Branch("jetChargedMuEnergy", jetChargedMuEnergy_, "jetChargedMuEnergy[nJet]/F");
    tree_->Branch("jetChargedMuEnergyFraction", jetChargedMuEnergyFraction_, "jetChargedMuEnergyFraction[nJet]/F");
    tree_->Branch("jetJVAlpha", jetJVAlpha_, "jetJVAlpha[nJet]/D");
    tree_->Branch("jetJVBeta", jetJVBeta_, "jetJVBeta[nJet]/D");
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

  Handle<reco::BeamSpot> beamSpotHandle;
  e.getByLabel(beamSpotCollection_, beamSpotHandle);

  edm::Handle<reco::ConversionCollection> hConversions;
  e.getByLabel("allConversions", hConversions);

  // get the channel status from the DB
  edm::ESHandle<EcalChannelStatus> chStatus;
  es.get<EcalChannelStatusRcd>().get(chStatus);

  // get the severity status from the DB
  edm::ESHandle<EcalSeverityLevelAlgo> severityStatus;
  es.get<EcalSeverityLevelAlgoRcd>().get(severityStatus);

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
  nGoodVtx_ = 0;
  Handle<VertexCollection> recVtxs;
  if (e.getByLabel(vtxlabel_, recVtxs)) {

    for (size_t i=0; i<recVtxs->size(); ++i) {
      if (!((*recVtxs)[i].isFake())) {
	vtx_[nVtx_][0] = (*recVtxs)[i].x();
	vtx_[nVtx_][1] = (*recVtxs)[i].y();
	vtx_[nVtx_][2] = (*recVtxs)[i].z();
	vtxNTrk_[nVtx_] = (*recVtxs)[i].tracksSize();
	vtxNDF_[nVtx_] = (*recVtxs)[i].ndof();
	vtxD0_[nVtx_] = (*recVtxs)[i].position().rho();

	if (vtxNDF_[nVtx_] >= 4 && fabs(vtx_[nVtx_][2]) <= 24 && fabs(vtxD0_[nVtx_]) <= 2) nGoodVtx_++;
	nVtx_++;
      }
    }
  }
  if (nGoodVtx_ > 0) IsVtxGood_ = 1;

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
    TrueNI_ = -1;
    for(PVI = PUInfo->begin(); PVI != PUInfo->end(); ++PVI) {

      nPU_[nBX_] = PVI->getPU_NumInteractions();
      BXPU_[nBX_] = PVI->getBunchCrossing();

      if (PVI->getBunchCrossing() == 0)
        TrueNI_ = PVI->getTrueNumInteractions(); 

      nBX_ += 1;
    }
  }

  // Rho correction
  edm::Handle<double> rhoHandle;
  e.getByLabel(rhoLabel_, rhoHandle);
  rho_ = *(rhoHandle.product());

  edm::Handle<double> sigmaHandle;
  e.getByLabel(sigmaLabel_, sigmaHandle);
  sigma_ = *(sigmaHandle.product());

  edm::Handle<double> rhoNeutralHandle;
  e.getByLabel(rhoNeutralLabel_, rhoNeutralHandle);
  rhoNeutral_ = *(rhoNeutralHandle.product());

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
	if ((genIndex-1) > 100 && ip->pdgId()==22) continue;
	if (fabs(p->pdgId())==12 && fabs(p->mother()->pdgId())>100) continue;
	if (fabs(p->pdgId())==14 && fabs(p->mother()->pdgId())>100) continue;

	mcPID[nMC_]     = p->pdgId();
        mcVtx[nMC_][0]  = p->vx();
        mcVtx[nMC_][1]  = p->vy();
        mcVtx[nMC_][2]  = p->vz();
	mcPt[nMC_]      = p->pt();
	mcMass[nMC_]    = p->mass();
	mcEta[nMC_]     = p->eta();
	mcPhi[nMC_]     = p->phi();
	mcGMomPID[nMC_] = -999;
	mcMomPID[nMC_]  = -999;
	mcMomPt[nMC_]   = -999;
	mcMomMass[nMC_] = -999;
	mcMomEta[nMC_]  = -999;
	mcMomPhi[nMC_]  = -999;
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
  std::map<unsigned, std::string> HLTIndexPath;  
  HLTIndexPath[0] = "HLT_Jet15U";
  HLTIndexPath[1] = "HLT_Jet30U";
  HLTIndexPath[2] = "HLT_Jet50U";
  HLTIndexPath[3] = "HLT_Jet70U";
  HLTIndexPath[4] = "HLT_Jet70U_v2";
  HLTIndexPath[5] = "HLT_Jet100U";
  HLTIndexPath[6] = "HLT_Jet100U_v2";
  HLTIndexPath[7] = "HLT_Jet140U_v1";
  HLTIndexPath[8] = "HLT_Mu9";
  HLTIndexPath[9] = "HLT_Mu11";
  HLTIndexPath[10] = "HLT_Mu13";
  HLTIndexPath[11] = "HLT_Mu13_v";
  HLTIndexPath[12] = "HLT_Mu15";
  HLTIndexPath[13] = "HLT_Mu15_v";
  HLTIndexPath[14] = "HLT_DoubleMu3";
  HLTIndexPath[15] = "HLT_DoubleMu3_v2";
  HLTIndexPath[16] = "HLT_DoubleMu5_v1";
  HLTIndexPath[17] = "HLT_Ele15_LW_L1R";
  HLTIndexPath[18] = "HLT_Ele20_LW_L1R";
  HLTIndexPath[19] = "HLT_Ele15_SW_L1R";
  HLTIndexPath[20] = "HLT_Ele20_SW_L1R";
  HLTIndexPath[21] = "HLT_Ele15_SW_EleId_L1R";
  HLTIndexPath[22] = "HLT_Ele20_SW_EleId_L1R";
  HLTIndexPath[23] = "HLT_Ele15_SW_CaloEleId_L1R";
  HLTIndexPath[24] = "HLT_Ele20_SW_CaloEleId_L1R";
  HLTIndexPath[25] = "HLT_Ele17_SW_CaloEleId_L1R";
  HLTIndexPath[26] = "HLT_Ele17_SW_TightEleId_L1R";
  HLTIndexPath[27] = "HLT_Ele17_SW_TighterEleIdIsol_L1R";
  HLTIndexPath[28] = "HLT_Ele17_SW_TighterEleIdIsol_L1R_v2";
  HLTIndexPath[29] = "HLT_Ele22_SW_TighterEleId_L1R_v2";
  HLTIndexPath[30] = "HLT_DoubleEle10_SW_L1R";
  HLTIndexPath[31] = "HLT_DoubleEle17_SW_L1R_v1";
  HLTIndexPath[32] = "HLT_Photon10_Cleaned_L1R";
  HLTIndexPath[33] = "HLT_Photon15_Cleaned_L1R";
  HLTIndexPath[34] = "HLT_Photon20_Cleaned_L1R";
  HLTIndexPath[35] = "HLT_Photon30_Cleaned_L1R";
  HLTIndexPath[36] = "HLT_Photon50_Cleaned_L1R_v1";
  HLTIndexPath[37] = "HLT_Photon70_Cleaned_L1R_v1";
  HLTIndexPath[38] = "HLT_Jet15U_v3";
  HLTIndexPath[39] = "HLT_Jet30U_v3";
  HLTIndexPath[40] = "HLT_Jet50U_v3";
  HLTIndexPath[41] = "HLT_Jet70U_v3";
  HLTIndexPath[42] = "HLT_Jet100U_v3";
  HLTIndexPath[43] = "HLT_Jet140U_v3";
  HLTIndexPath[44] = "HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1";
  HLTIndexPath[45] = "HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2";
  HLTIndexPath[46] = "HLT_Ele17_SW_TighterEleIdIsol_L1R_v3";
  HLTIndexPath[47] = "HLT_DoubleEle15_SW_L1R_v1";
  HLTIndexPath[48] = "HLT_DoublePhoton17_L1R";
  HLTIndexPath[49] = "HLT_Photon10_L1R";
  HLTIndexPath[50] = "HLT_IsoMu15_v";
  HLTIndexPath[51] = "HLT_IsoMu17_v";
  HLTIndexPath[52] = "HLT_IsoMu24_v";
  HLTIndexPath[53] = "HLT_IsoMu30_v";
  HLTIndexPath[54] = "HLT_Mu24_v";
  HLTIndexPath[55] = "HLT_Mu30_v";
  // HLT path for 2011A data
  HLTIndexPath[56] = "HLT_Jet30_v";
  HLTIndexPath[57] = "HLT_Jet60_v";
  HLTIndexPath[58] = "HLT_Jet80_v";
  HLTIndexPath[59] = "HLT_Jet110_v";
  HLTIndexPath[60] = "HLT_Jet150_v";
  HLTIndexPath[61] = "HLT_Jet190_v";
  HLTIndexPath[62] = "HLT_Jet240_v";
  HLTIndexPath[63] = "HLT_Jet300_v";
  HLTIndexPath[64] = "HLT_Jet370_v";
  HLTIndexPath[65] = "HLT_Mu30_v";
  HLTIndexPath[66] = "HLT_IsoMu24_v";
  HLTIndexPath[67] = "HLT_IsoMu30_v";
  HLTIndexPath[68] = "HLT_IsoMu24_eta2p1_v";
  HLTIndexPath[69] = "HLT_IsoMu30_eta2p1_v";
  HLTIndexPath[70] = "HLT_IsoMu34_eta2p1_v";
  HLTIndexPath[71] = "HLT_DoubleMu6_v";
  HLTIndexPath[72] = "HLT_DoubleMu7_v";
  HLTIndexPath[73] = "HLT_Mu13_Mu8_v";
  HLTIndexPath[74] = "HLT_Mu17_Mu8_v";
  HLTIndexPath[75] = "HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v";
  HLTIndexPath[76] = "HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v";
  HLTIndexPath[77] = "HLT_Ele42_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v";
  HLTIndexPath[78] = "HLT_Ele27_WP80_PFMT50_v";
  HLTIndexPath[79] = "HLT_Ele32_WP70_PFMT50_v";
  HLTIndexPath[80] = "HLT_Ele8_CaloIdL_CaloIsoVL_v";
  HLTIndexPath[81] = "HLT_Ele17_CaloIdL_CaloIsoVL_v";
  HLTIndexPath[82] = "HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v";
  HLTIndexPath[83] = "HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v";
  HLTIndexPath[84] = "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v";
  HLTIndexPath[85] = "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v";
  HLTIndexPath[86] = "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v";
  HLTIndexPath[87] = "HLT_Ele17_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_Ele8_CaloIdT_TrkIdVL_CaloIsoVL_TrkIsoVL_v";
  HLTIndexPath[88] = "HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_Ele17_v";
  HLTIndexPath[89] = "HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_v";
  HLTIndexPath[90] = "HLT_Photon20_CaloIdVL_IsoL_v";
  HLTIndexPath[91] = "HLT_Photon20_R9Id_Photon18_R9Id_v";
  HLTIndexPath[92] = "HLT_Photon26_Photon18_v";
  HLTIndexPath[93] = "HLT_Photon26_IsoVL_Photon18_v";
  HLTIndexPath[94] = "HLT_Photon26_IsoVL_Photon18_IsoVL_v";
  HLTIndexPath[95] = "HLT_Photon26_CaloIdL_IsoVL_Photon18_v";
  HLTIndexPath[96] = "HLT_Photon26_CaloIdL_IsoVL_Photon18_CaloIdL_IsoVL_v";
  HLTIndexPath[97] = "HLT_Photon26_CaloIdL_IsoVL_Photon18_R9Id_v";
  HLTIndexPath[98] = "HLT_Photon26_R9Id_Photon18_CaloIdL_IsoVL_v";
  HLTIndexPath[99] = "HLT_Photon26_R9Id_Photon18_R9Id_v";
  HLTIndexPath[100]= "HLT_Photon26_R9Id_Photon18_CaloIdXL_IsoXL_v";
  HLTIndexPath[101]= "HLT_Photon26_CaloIdXL_IsoXL_Photon18_v";
  HLTIndexPath[102]= "HLT_Photon26_CaloIdXL_IsoXL_Photon18_R9Id_v";
  HLTIndexPath[103]= "HLT_Photon26_CaloIdXL_IsoXL_Photon18_CaloIdXL_IsoXL_v";
  HLTIndexPath[104]= "HLT_Photon30_CaloIdVL_v";
  HLTIndexPath[105]= "HLT_Photon30_CaloIdVL_IsoL_v";
  HLTIndexPath[106]= "HLT_Photon32_CaloIdL_Photon26_CaloIdL_v";
  HLTIndexPath[107]= "HLT_Photon36_CaloIdL_Photon22_CaloIdL_v";
  HLTIndexPath[108]= "HLT_Photon36_CaloIdL_IsoVL_Photon22_v";
  HLTIndexPath[109]= "HLT_Photon36_Photon22_v";
  HLTIndexPath[110]= "HLT_Photon36_IsoVL_Photon22_v";
  HLTIndexPath[111]= "HLT_Photon36_CaloId_IsoVL_Photon22_R9Id_v";
  HLTIndexPath[112]= "HLT_Photon36_R9Id_Photon22_CaloIdL_IsoVL_v";
  HLTIndexPath[113]= "HLT_Photon36_R9Id_Photon22_R9Id_v";
  HLTIndexPath[114]= "HLT_Photon36_CaloIdL_IsoVL_Photon22_R9Id_v";
  HLTIndexPath[115]= "HLT_Photon36_CaloIdVL_Photon22_CaloIdVL_v";
  HLTIndexPath[116]= "HLT_Photon40_CaloIdL_Photon28_CaloIdL_v";
  HLTIndexPath[117]= "HLT_Photon44_CaloIdL_Photon34_CaloIdL_v";
  HLTIndexPath[118]= "HLT_Photon48_CaloIdL_Photon38_CaloIdL_v";
  HLTIndexPath[119]= "HLT_Photon50_CaloIdVL_v";
  HLTIndexPath[120]= "HLT_Photon50_CaloIdVL_IsoL_v";
  HLTIndexPath[121]= "HLT_Photon75_CaloIdVL_v";
  HLTIndexPath[122]= "HLT_Photon75_CaloIdVL_IsoL_v";
  HLTIndexPath[123]= "HLT_Photon90_CaloIdVL_v";
  HLTIndexPath[124]= "HLT_Photon90_CaloIdVL_IsoL_v";

  for (int a=0; a<125; a++)
    HLTIndex_[a] = -1;
 
  nHLT_ = 0;
  Handle<TriggerResults> trgResultsHandle;
  e.getByLabel(trgResults_, trgResultsHandle);
  const TriggerNames &trgNames = e.triggerNames(*trgResultsHandle);
  vector<string> hlNames = trgNames.triggerNames();
  nHLT_ = trgNames.size();
  for (size_t i=0; i<trgNames.size(); ++i) {
    HLT_[i] = (trgResultsHandle->accept(i) == true) ? 1:0;
    HLTprescale_[i] = hltConfig_.prescaleValue(e, es, hlNames[i]);

    for (size_t j=0; j<HLTIndexPath.size(); j++) {
      if(TString(hlNames[i]).Contains(TRegexp(HLTIndexPath[j])) == 1) HLTIndex_[j] = i;
    }
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

  // TypeI pfMET
  Handle<View<pat::MET> > TypeIpfMETHandle_;
  e.getByLabel(TypeIpfMETlabel_, TypeIpfMETHandle_);
  for (View<pat::MET>::const_iterator iTypeIPFMET = TypeIpfMETHandle_->begin(); iTypeIPFMET != TypeIpfMETHandle_->end(); ++iTypeIPFMET) {

      TypeIpfMET_       = iTypeIPFMET->pt();
      TypeIpfMETx_      = iTypeIPFMET->px();
      TypeIpfMETy_      = iTypeIPFMET->py();
      TypeIpfMETPhi_    = iTypeIPFMET->phi();
      TypeIpfMETsumEt_  = iTypeIPFMET->sumEt();
      TypeIpfMETmEtSig_ = iTypeIPFMET->mEtSig();
      TypeIpfMETSig_    = iTypeIPFMET->significance();
  }

  // Type I + TypeII pfMET
  Handle<View<pat::MET> > TypeIpIIpfMETHandle_;
  e.getByLabel(TypeIpIIpfMETlabel_, TypeIpIIpfMETHandle_);
  for (View<pat::MET>::const_iterator iTypeIpIIPFMET = TypeIpIIpfMETHandle_->begin(); iTypeIpIIPFMET != TypeIpIIpfMETHandle_->end(); ++iTypeIpIIPFMET) {

      TypeIpIIpfMET_       = iTypeIpIIPFMET->pt();
      TypeIpIIpfMETx_      = iTypeIpIIPFMET->px();
      TypeIpIIpfMETy_      = iTypeIpIIPFMET->py();
      TypeIpIIpfMETPhi_    = iTypeIpIIPFMET->phi();
      TypeIpIIpfMETsumEt_  = iTypeIpIIPFMET->sumEt();
      TypeIpIIpfMETmEtSig_ = iTypeIpIIPFMET->mEtSig();
      TypeIpIIpfMETSig_    = iTypeIpIIPFMET->significance();
  }

  if (doGenParticles_) {
    // Smeared pfMET
    Handle<View<pat::MET> > SmearedpfMETHandle_;  
    e.getByLabel(SmearedpfMETlabel_, SmearedpfMETHandle_);
    for (View<pat::MET>::const_iterator iSmearedPFMET = SmearedpfMETHandle_->begin(); iSmearedPFMET != SmearedpfMETHandle_->end(); ++iSmearedPFMET) {

        SmearedpfMET_       = iSmearedPFMET->pt();
        SmearedpfMETx_      = iSmearedPFMET->px();
        SmearedpfMETy_      = iSmearedPFMET->py();
        SmearedpfMETPhi_    = iSmearedPFMET->phi();
        SmearedpfMETsumEt_  = iSmearedPFMET->sumEt();
        SmearedpfMETmEtSig_ = iSmearedPFMET->mEtSig();
        SmearedpfMETSig_    = iSmearedPFMET->significance();
    }

    // Smeared TypeI pfMET
    Handle<View<pat::MET> > SmearedTypeIpfMETHandle_;
    e.getByLabel(SmearedTypeIpfMETlabel_, SmearedTypeIpfMETHandle_);
    for (View<pat::MET>::const_iterator iSmearedTypeIPFMET = SmearedTypeIpfMETHandle_->begin(); iSmearedTypeIPFMET != SmearedTypeIpfMETHandle_->end(); ++iSmearedTypeIPFMET) {

        SmearedTypeIpfMET_       = iSmearedTypeIPFMET->pt();
        SmearedTypeIpfMETx_      = iSmearedTypeIPFMET->px();
        SmearedTypeIpfMETy_      = iSmearedTypeIPFMET->py();
        SmearedTypeIpfMETPhi_    = iSmearedTypeIPFMET->phi();
        SmearedTypeIpfMETsumEt_  = iSmearedTypeIPFMET->sumEt();
        SmearedTypeIpfMETmEtSig_ = iSmearedTypeIPFMET->mEtSig();
        SmearedTypeIpfMETSig_    = iSmearedTypeIPFMET->significance();
    }
  }
  // PF Candidates
  npfCharged_ = 0;
  pfChargedSumPt_ = 0;
  npfChargedHadron_ = 0;
  pfChargedHadronSumPt_ = 0;
  npfLepton_ = 0;
  pfLeptonSumPt_ = 0;
  npfNeutral_ = 0;
  pfNeutralSumPt_ = 0;
  npfNeutralHadron_ = 0;
  pfNeutralHadronSumPt_ = 0;
  npfPhoton_ = 0;
  pfPhotonSumPt_ = 0;

  Handle<reco::PFCandidateCollection> pfCandidatesHandle;
  e.getByLabel(PFCandLabel_, pfCandidatesHandle);
  for (PFCandidateCollection::const_iterator iPF = pfCandidatesHandle->begin(); iPF != pfCandidatesHandle->end(); iPF++) {

    const PFCandidate *particle = &(*iPF);

    if (particle->pdgId() == 22) {
       npfPhoton_ += 1;
       pfPhotonSumPt_ += particle->pt();
    }
    if (particle->pdgId() == 111 || particle->pdgId() == 130 || particle->pdgId() == 310 || particle->pdgId() == 2112) {
       npfNeutralHadron_ += 1;
       pfNeutralHadronSumPt_ += particle->pt();
    }
    if (fabs(particle->pdgId()) == 211 || fabs(particle->pdgId()) == 321 || fabs(particle->pdgId()) == 999211 || fabs(particle->pdgId()) == 2212) {
       npfChargedHadron_ += 1;
       pfChargedHadronSumPt_ += particle->pt();
    }
    if (fabs(particle->pdgId()) == 11 || fabs(particle->pdgId()) == 13) {
       npfLepton_ += 1;
       pfLeptonSumPt_ += particle->pt();
    }

    if (particle->charge() == 0) {
       npfNeutral_ += 1;
       pfNeutralSumPt_ += particle->pt();
    }
    if (particle->charge() != 0) {
       npfCharged_ += 1;
       pfChargedSumPt_ += particle->pt();
    }
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
  const TriggerObjectMatch *eleTriggerMatch11(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle27CaloIdVTCaloIsoTTrkIdTTrkIsoT"));
  const TriggerObjectMatch *eleTriggerMatch12(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoT"));
  const TriggerObjectMatch *eleTriggerMatch13(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle27WP80PFMT50"));
  const TriggerObjectMatch *eleTriggerMatch14(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32WP70PFMT50"));
  const TriggerObjectMatch *eleTriggerMatch15(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle8CaloIdLCaloIsoVL"));
  const TriggerObjectMatch *eleTriggerMatch16(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdLCaloIsoVL"));
  const TriggerObjectMatch *eleTriggerMatch17(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"));
  const TriggerObjectMatch *eleTriggerMatch18(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"));
  const TriggerObjectMatch *eleTriggerMatch19(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdLCaloIsoVLEle8CaloIdLCaloIsoVL"));
  const TriggerObjectMatch *eleTriggerMatch20(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass30"));
  const TriggerObjectMatch *eleTriggerMatch21(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8Mass30"));
  const TriggerObjectMatch *eleTriggerMatch22(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"));
  const TriggerObjectMatch *eleTriggerMatch23(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTEle17"));
  const TriggerObjectMatch *eleTriggerMatch24(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17"));
  const TriggerObjectMatch *eleTriggerMatch25(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32WP70"));
  const TriggerObjectMatch *eleTriggerMatch26(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT"));
  const TriggerObjectMatch *eleTriggerMatch27(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle8Mass30"));
  const TriggerObjectMatch *eleTriggerMatch28(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTSC8Mass30"));
  const TriggerObjectMatch *eleTriggerMatch29(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT"));
  const TriggerObjectMatch *eleTriggerMatch30(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTSC17"));
  const TriggerObjectMatch *eleTriggerMatch31(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17"));

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
      const TriggerObjectRef eleTrigRef22( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch22, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef23( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch23, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef24( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch24, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef25( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch25, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef26( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch26, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef27( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch27, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef28( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch28, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef29( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch29, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef30( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch30, e, *triggerEvent ) );
      const TriggerObjectRef eleTrigRef31( matchHelper.triggerMatchObject( eleBaseRef, eleTriggerMatch31, e, *triggerEvent ) );
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
      eleTrg_[nEle_][21]  = (eleTrigRef22.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][22]  = (eleTrigRef23.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][23]  = (eleTrigRef24.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][24]  = (eleTrigRef25.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][25]  = (eleTrigRef26.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][26]  = (eleTrigRef27.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][27]  = (eleTrigRef28.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][28]  = (eleTrigRef29.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][29]  = (eleTrigRef30.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][30]  = (eleTrigRef31.isAvailable()) ? 1 : -99;

      //        new eID with correct isolations and conversion rejection
      //	https://twiki.cern.ch/twiki/bin/viewauth/CMS/SimpleCutBasedEleID
      //        The value map returns a double with the following meaning:
      //        0: fails
      //	1: passes electron ID only
      //	2: passes electron Isolation only
      //	3: passes electron ID and Isolation only
      //	4: passes conversion rejection
      //	5: passes conversion rejection and ID
      //	6: passes conversion rejection and Isolation
      //	7: passes the whole selection
      //
      //        CIC
      //        https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideCategoryBasedElectronID
      //        The value map returns a double with the following meaning:
      //        0 - no cut passed
      //        1 - eID cuts passed
      //        2 - iso cuts passed
      //        4 - conversion rejection
      //        8 - ip cut
      //        To check eId results:
      //        eID+Iso+ConversionRejection+IP -> ((eleID_[nEle_][] &15) == 15)
      //        Iso only -> ((eleID_[nEle_][] & 2) == 2)
      //        eID+ConversionRejection+IP -> ((eleID_[nEle_][] & 13) == 13)

      for (int i=0; i<30; ++i) 
	eleID_[nEle_][i] = -1;

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
      eleID_[nEle_][12]= int (iEle->electronID("eidVeryLoose"));
      eleID_[nEle_][13]= int (iEle->electronID("eidLoose"));
      eleID_[nEle_][14]= int (iEle->electronID("eidMedium"));
      eleID_[nEle_][15]= int (iEle->electronID("eidTight"));
      eleID_[nEle_][16]= int (iEle->electronID("eidSuperTight"));
      eleID_[nEle_][17]= int (iEle->electronID("eidHyperTight1"));
      eleID_[nEle_][18]= int (iEle->electronID("eidHyperTight2"));
      eleID_[nEle_][19]= int (iEle->electronID("eidHyperTight3"));
      eleID_[nEle_][20]= int (iEle->electronID("eidHyperTight4"));
      eleID_[nEle_][21]= int (iEle->electronID("eidVeryLooseMC"));
      eleID_[nEle_][22]= int (iEle->electronID("eidLooseMC"));
      eleID_[nEle_][23]= int (iEle->electronID("eidMediumMC"));
      eleID_[nEle_][24]= int (iEle->electronID("eidTightMC"));
      eleID_[nEle_][25]= int (iEle->electronID("eidSuperTightMC"));
      eleID_[nEle_][26]= int (iEle->electronID("eidHyperTight1MC"));
      eleID_[nEle_][27]= int (iEle->electronID("eidHyperTight2MC"));
      eleID_[nEle_][28]= int (iEle->electronID("eidHyperTight3MC"));
      eleID_[nEle_][29]= int (iEle->electronID("eidHyperTight4MC"));

      eleIDLH_[nEle_] = iEle->electronID("eidLikelihoodExt");

      eleClass_[nEle_]   = iEle->classification();
      eleCharge_[nEle_]  = iEle->charge();
      eleEn_[nEle_]      = iEle->energy();
      elePt_[nEle_]      = iEle->pt();
      elePz_[nEle_]      = iEle->pz();
      eleEta_[nEle_]     = iEle->eta();
      elePhi_[nEle_]     = iEle->phi();
      eleHoverE_[nEle_]  = iEle->hadronicOverEm();
      eleEoverP_[nEle_]  = iEle->eSuperClusterOverP();

      elePin_[nEle_]   = iEle->trackMomentumAtVtx().R();
      elePout_[nEle_]  = iEle->trackMomentumOut().R();
      //eleBrem_[nEle_] = (elePin_[nEle_] - elePout_[nEle_]) / elePin_[nEle_];
      eleBrem_[nEle_]  = iEle->fbrem();
      elenBrem_[nEle_] = iEle->numberOfBrems();

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

      eleConversionveto_[nEle_] = ConversionTools::hasMatchedConversion(*iEle, hConversions, beamSpotHandle->position());

      const reco::CaloClusterPtr eleSeed = (*iEle).superCluster()->seed();

      vector<float> eleCov;
      eleCov = lazyTool.localCovariances(*eleSeed);
      eleSigmaIEtaIEta_[nEle_] = iEle->sigmaIetaIeta();
      eleSigmaIEtaIPhi_[nEle_] = sqrt(eleCov[1]);
      eleSigmaIPhiIPhi_[nEle_] = sqrt(eleCov[2]);

      eleE3x3_[nEle_] = lazyTool.e3x3(*eleSeed);

      eleSeedTime_[nEle_] = -999.;
      eleSeedEnergy_[nEle_] = -999.;
      eleRecoFlag_[nEle_] = -999.;
      eleSeverity_[nEle_] = -999.;

      DetId eleSeedDetId = lazyTool.getMaximum(*eleSeed).first;

      if ( iEle->isEB() && EBReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eleebrhit = EBReducedRecHits->find(eleSeedDetId);
        if ( eleebrhit != EBReducedRecHits->end() ) { 
	   eleSeedTime_[nEle_] = eleebrhit->time(); 
	   eleSeedEnergy_[nEle_] = eleebrhit->energy(); 
           eleRecoFlag_[nEle_] = eleebrhit->recoFlag();
           eleSeverity_[nEle_] = severityStatus->severityLevel( eleSeedDetId, (*EBReducedRecHits) );
	}
      } else if ( EEReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eleeerhit = EEReducedRecHits->find(eleSeedDetId);
        if ( eleeerhit != EEReducedRecHits->end() ) { 
	   eleSeedTime_[nEle_] = eleeerhit->time(); 
	   eleSeedEnergy_[nEle_] = eleeerhit->energy(); 
           eleRecoFlag_[nEle_] = eleeerhit->recoFlag();
           eleSeverity_[nEle_] = severityStatus->severityLevel( eleSeedDetId, (*EEReducedRecHits) );
	}
      }

      eleESRatio_[nEle_] = getESRatio(iEle, e, es);
      for (int a=0; a<123; a++) {
        eleESProfileFront_[nEle_][a] = getESProfileFront(iEle, e, es)[a];
        eleESProfileRear_[nEle_][a] = getESProfileRear(iEle, e, es)[a];
      }
 
      elePV2D_[nEle_] = iEle->dB(pat::Electron::PV2D);
      elePV3D_[nEle_] = iEle->dB(pat::Electron::PV3D);
      eleBS2D_[nEle_] = iEle->dB(pat::Electron::BS2D);
      eleBS3D_[nEle_] = iEle->dB(pat::Electron::BS3D);
      elePVD0_[nEle_] = iEle->gsfTrack()->dxy((*recVtxs)[0].position());
      elePVDz_[nEle_] = iEle->gsfTrack()->dz((*recVtxs)[0].position());

      nEle_++;
    }

  // Photon

  const Candidate *phomom = 0;
  int nPhoPassCut = 0;
  nPho_ = 0;
  const TriggerObjectMatch *phoTriggerMatch1(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle32CaloIdVTCaloIsoTTrkIdTTrkIsoT"));
  const TriggerObjectMatch *phoTriggerMatch2(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"));
  const TriggerObjectMatch *phoTriggerMatch3(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"));
  const TriggerObjectMatch *phoTriggerMatch4(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass30"));
  const TriggerObjectMatch *phoTriggerMatch5(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC8Mass30"));
  const TriggerObjectMatch *phoTriggerMatch6(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"));
  const TriggerObjectMatch *phoTriggerMatch7(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTEle17"));
  const TriggerObjectMatch *phoTriggerMatch8(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17"));
  const TriggerObjectMatch *phoTriggerMatch9(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT"));
  const TriggerObjectMatch *phoTriggerMatch10(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle8Mass30"));
  const TriggerObjectMatch *phoTriggerMatch11(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTSC8Mass30"));
  const TriggerObjectMatch *phoTriggerMatch12(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT"));
  const TriggerObjectMatch *phoTriggerMatch13(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTSC17"));
  const TriggerObjectMatch *phoTriggerMatch14(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle17"));

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

      for (int i=0; i<150; i++) {
        phoEtVtx_[nPho_][i]  = -999.;
        phoEtaVtx_[nPho_][i] = -999.;
        phoPhiVtx_[nPho_][i] = -999.;
        phoTrkIsoSolidDR03Vtx_[nPho_][i]  = -999.;
        phoTrkIsoHollowDR03Vtx_[nPho_][i] = -999.;
        phoTrkIsoSolidDR04Vtx_[nPho_][i]  = -999.;
        phoTrkIsoHollowDR04Vtx_[nPho_][i] = -999.;
      }

      for (size_t i=0; i<recVtxs->size(); ++i) {
        if (!((*recVtxs)[i].isFake())) {

          math::XYZPoint VTX = math::XYZPoint((*recVtxs)[i].x(), (*recVtxs)[i].y(), (*recVtxs)[i].z());
          math::XYZVector direction = math::XYZVector(iPho->caloPosition().x() - (*recVtxs)[i].x(), iPho->caloPosition().y() - (*recVtxs)[i].y(), iPho->caloPosition().z() - (*recVtxs)[i].z());
          math::XYZVector momentum = direction.unit() * iPho->energy();
	  const reco::Particle::LorentzVector newPho(momentum.x(), momentum.y(), momentum.z(), iPho->energy());

	  phoEtVtx_[nPho_][i]  = newPho.Pt();
	  phoEtaVtx_[nPho_][i] = newPho.Eta();
	  phoPhiVtx_[nPho_][i] = newPho.Phi();
          phoTrkIsoSolidDR03Vtx_[nPho_][i]  = getTrkIso(Tracks, VTX, newPho.Eta(), newPho.Phi(), VTX.z(), 0.3, 0., 0.015, 0.2, 0.1, 0, 0);
          phoTrkIsoHollowDR03Vtx_[nPho_][i] = getTrkIso(Tracks, VTX, newPho.Eta(), newPho.Phi(), VTX.z(), 0.3, 0.04, 0.015, 0.2, 0.1, 0, 0);
          phoTrkIsoSolidDR04Vtx_[nPho_][i]  = getTrkIso(Tracks, VTX, newPho.Eta(), newPho.Phi(), VTX.z(), 0.4, 0., 0.015, 0.2, 0.1, 0, 0);
          phoTrkIsoHollowDR04Vtx_[nPho_][i] = getTrkIso(Tracks, VTX, newPho.Eta(), newPho.Phi(), VTX.z(), 0.4, 0.04, 0.015, 0.2, 0.1, 0, 0);
        }
      }

      phoHoverE_[nPho_]        = iPho->hadronicOverEm();
      phoSigmaEtaEta_[nPho_]   = iPho->sigmaEtaEta();
      phoR9_[nPho_]            = iPho->r9();

      phoOverlap_[nPho_] = (int) iPho->hasOverlaps("electrons");
      phohasPixelSeed_[nPho_] = (int) iPho->hasPixelSeed();
      
      edm::Handle<reco::GsfElectronCollection> hElectrons;
      e.getByLabel("gsfElectrons", hElectrons);
      phoElectronveto_[nPho_] = ConversionTools::hasMatchedPromptElectron(iPho->superCluster(), hElectrons, hConversions, beamSpotHandle->position());

      // where is photon ? (0: EB, 1: EE, 2: EBGap, 3: EEGap, 4: EBEEGap)
      phoPos_[nPho_] = -1;
      if (iPho->isEB() == true) phoPos_[nPho_] = 0;
      if (iPho->isEE() == true) phoPos_[nPho_] = 1;
      if (iPho->isEBGap() == true) phoPos_[nPho_] = 2;
      if (iPho->isEEGap() == true) phoPos_[nPho_] = 3;
      if (iPho->isEBEEGap() == true) phoPos_[nPho_] = 4;

      phoSeedTime_[nPho_] = -999.;
      phoSeedEnergy_[nPho_] = -999.;
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
	   phoSeedEnergy_[nPho_] = ebrhit->energy();
           phoRecoFlag_[nPho_] = ebrhit->recoFlag();
           phoSeverity_[nPho_] = severityStatus->severityLevel( phoSeedDetId, (*EBReducedRecHits));
        }
      } else if ( EEReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eerhit = EEReducedRecHits->find(phoSeedDetId);
        if ( eerhit != EEReducedRecHits->end() ) { 
	   phoSeedTime_[nPho_] = eerhit->time(); 
	   phoSeedEnergy_[nPho_] = eerhit->energy();
           phoRecoFlag_[nPho_] = eerhit->recoFlag();
           phoSeverity_[nPho_] = severityStatus->severityLevel( phoSeedDetId, (*EEReducedRecHits) );
	}
      }
      
      phoE3x3_[nPho_] = lazyTool.e3x3(*phoSeed);
      phoE5x5_[nPho_] = lazyTool.e5x5(*phoSeed);
 
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
      phoESE_[nPho_]   = (*iPho).superCluster()->preshowerEnergy();
      phoSCEta_[nPho_] = (*iPho).superCluster()->eta();
      phoSCPhi_[nPho_] = (*iPho).superCluster()->phi();
      phoSCEt_[nPho_]  = (*iPho).superCluster()->energy()/cosh(phoSCEta_[nPho_]);
      phoSCEtaWidth_[nPho_] = (*iPho).superCluster()->etaWidth();
      phoSCPhiWidth_[nPho_] = (*iPho).superCluster()->phiWidth();

      phoVtx_[nPho_][0] = iPho->vertex().x();
      phoVtx_[nPho_][1] = iPho->vertex().y();
      phoVtx_[nPho_][2] = iPho->vertex().z();
      phoVtxD0_[nPho_]  = iPho->vertex().rho();

      phoIsConv_[nPho_] = iPho->hasConversionTracks();

      phoNTracks_[nPho_] = -1;
      if (iPho->conversions().size() > 0) {
         phoNTracks_[nPho_] = iPho->conversions()[0]->nTracks();

	 if (phoNTracks_[nPho_] == 2) {
           phoConvPairInvariantMass_[nPho_] 	 = iPho->conversions()[0]->pairInvariantMass();
	   phoConvPairCotThetaSeparation_[nPho_] = iPho->conversions()[0]->pairCotThetaSeparation(); 
	   phoConvPairMomentumEta_[nPho_]        = iPho->conversions()[0]->pairMomentum().eta(); 
	   phoConvPairMomentumPhi_[nPho_]        = iPho->conversions()[0]->pairMomentum().phi(); 
	   phoConvPairMomentumX_[nPho_]          = iPho->conversions()[0]->pairMomentum().x(); 
	   phoConvPairMomentumY_[nPho_]          = iPho->conversions()[0]->pairMomentum().y(); 
	   phoConvPairMomentumZ_[nPho_]          = iPho->conversions()[0]->pairMomentum().z();        
	   phoConvDistOfMinimumApproach_[nPho_]  = iPho->conversions()[0]->distOfMinimumApproach(); 
	   phoConvDPhiTracksAtVtx_[nPho_]        = iPho->conversions()[0]->dPhiTracksAtVtx(); 
	   phoConvDPhiTracksAtEcal_[nPho_]       = iPho->conversions()[0]->dPhiTracksAtEcal(); 
	   phoConvDEtaTracksAtEcal_[nPho_]       = iPho->conversions()[0]->dEtaTracksAtEcal(); 

	   phoConvVtxValid_[nPho_]      = iPho->conversions()[0]->conversionVertex().isValid(); 
	   phoConvVtxEta_[nPho_]        = iPho->conversions()[0]->conversionVertex().position().eta(); 
	   phoConvVtxPhi_[nPho_]        = iPho->conversions()[0]->conversionVertex().position().phi(); 
	   phoConvVtxR_[nPho_]          = iPho->conversions()[0]->conversionVertex().position().r(); 
	   phoConvVtxX_[nPho_]          = iPho->conversions()[0]->conversionVertex().position().x(); 
	   phoConvVtxY_[nPho_]          = iPho->conversions()[0]->conversionVertex().position().y(); 
	   phoConvVtxZ_[nPho_]          = iPho->conversions()[0]->conversionVertex().position().z(); 
	   phoConvVtxChi2_[nPho_]       = iPho->conversions()[0]->conversionVertex().chi2(); 
	   phoConvVtxNdof_[nPho_]       = iPho->conversions()[0]->conversionVertex().ndof(); 
	   phoConvChi2Prob_[nPho_]      = ChiSquaredProbability(iPho->conversions()[0]->conversionVertex().chi2(),iPho->conversions()[0]->conversionVertex().ndof());
	   phoConvEoverP_[nPho_]      = iPho->conversions()[0]->EoverP();
	 }
      }

      phoESRatio_[nPho_] = getESRatio(iPho, e, es);
      for (int a=0; a<123; a++) {
        phoESProfileFront_[nPho_][a] = getESProfileFront(iPho, e, es)[a];
        phoESProfileRear_[nPho_][a] = getESProfileRear(iPho, e, es)[a];
      }

      phoNxtal_[nPho_] = 0;
      for (int a=0; a<200; a++) {
	phoXtalTime_[nPho_][a]   = -999;
	phoXtalEnergy_[nPho_][a] = -999;
	phoXtalZ_[nPho_][a]      = -999;
	phoXtalX_[nPho_][a]      = -999;
	phoXtalY_[nPho_][a]      = -999;
	phoXtalEta_[nPho_][a]    = -999;
	phoXtalPhi_[nPho_][a]    = -999;
 
        if (a >= 25) continue;
        pho5x5Time_[nPho_][a]   = -999;
        pho5x5Energy_[nPho_][a] = -999;
        pho5x5Z_[nPho_][a]      = -999;
        pho5x5X_[nPho_][a]      = -999;
        pho5x5Y_[nPho_][a]      = -999;
        pho5x5Eta_[nPho_][a]    = -999;
        pho5x5Phi_[nPho_][a]    = -999;
      }

      int phoN5x5 = 0;
      std::vector<DetId> pho5x5DetId = lazyTool.matrixDetId( phoSeedDetId, -2, 2, -2, 2);
      for ( std::vector<DetId>::const_iterator it_5x5 = pho5x5DetId.begin(); it_5x5 != pho5x5DetId.end(); ++it_5x5 ) {
        if ( iPho->isEB() && EBReducedRecHits.isValid() ) {
          EcalRecHitCollection::const_iterator ebrhit = EBReducedRecHits->find(*it_5x5);
          EBDetId phoSCEBid = EBDetId(*it_5x5);

	  if ( ebrhit != EBReducedRecHits->end() ) { 
	    pho5x5Time_[nPho_][phoN5x5]   = ebrhit->time();
	    pho5x5Energy_[nPho_][phoN5x5] = ebrhit->energy();
	    pho5x5Z_[nPho_][phoN5x5]      = phoSCEBid.zside();
	    pho5x5Eta_[nPho_][phoN5x5]    = phoSCEBid.ieta();
	    pho5x5Phi_[nPho_][phoN5x5]    = phoSCEBid.iphi();

	    phoN5x5 += 1;
	  }
        }
	if ( iPho->isEE() && EEReducedRecHits.isValid() ) {
          EcalRecHitCollection::const_iterator eerhit = EEReducedRecHits->find(*it_5x5);
          EEDetId phoSCEEid = EEDetId(*it_5x5);

          if ( eerhit != EEReducedRecHits->end() ) {
            pho5x5Time_[nPho_][phoN5x5]   = eerhit->time();
            pho5x5Energy_[nPho_][phoN5x5] = eerhit->energy();
            pho5x5Z_[nPho_][phoN5x5]      = phoSCEEid.zside();
            pho5x5X_[nPho_][phoN5x5]      = phoSCEEid.ix();
            pho5x5Y_[nPho_][phoN5x5]      = phoSCEEid.iy();

	    phoN5x5 += 1;
          }
        }
      }

      for (vector< pair<DetId, float> >::const_iterator phoRHIt = (*iPho).superCluster()->hitsAndFractions().begin(); phoRHIt != (*iPho).superCluster()->hitsAndFractions().end(); ++phoRHIt) {

        if ( iPho->isEB() && EBReducedRecHits.isValid() ) {
          EcalRecHitCollection::const_iterator ebrhit = EBReducedRecHits->find(phoRHIt->first);
	  EBDetId phoSCEBid = EBDetId(phoRHIt->first);

          if ( ebrhit != EBReducedRecHits->end() ) { 
	    phoXtalTime_[nPho_][phoNxtal_[nPho_]]   = ebrhit->time();
	    phoXtalEnergy_[nPho_][phoNxtal_[nPho_]] = ebrhit->energy();
	    phoXtalZ_[nPho_][phoNxtal_[nPho_]]      = phoSCEBid.zside();
	    phoXtalEta_[nPho_][phoNxtal_[nPho_]]    = phoSCEBid.ieta();
	    phoXtalPhi_[nPho_][phoNxtal_[nPho_]]    = phoSCEBid.iphi();

	    phoNxtal_[nPho_] += 1;
          }
	}
	else if ( iPho->isEE() && EEReducedRecHits.isValid() ) {
          EcalRecHitCollection::const_iterator eerhit = EEReducedRecHits->find(phoRHIt->first);
	  EEDetId phoSCEEid = EEDetId(phoRHIt->first);

          if ( eerhit != EEReducedRecHits->end() ) { 
	    phoXtalTime_[nPho_][phoNxtal_[nPho_]]   = eerhit->time();
	    phoXtalEnergy_[nPho_][phoNxtal_[nPho_]] = eerhit->energy();
	    phoXtalZ_[nPho_][phoNxtal_[nPho_]]      = phoSCEEid.zside();
	    phoXtalX_[nPho_][phoNxtal_[nPho_]]      = phoSCEEid.ix();
	    phoXtalY_[nPho_][phoNxtal_[nPho_]]      = phoSCEEid.iy();

	    phoNxtal_[nPho_] += 1;
          } 
	}
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
  const TriggerObjectMatch * muTriggerMatch7( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu30" ) );
  const TriggerObjectMatch * muTriggerMatch8( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu24" ) );
  const TriggerObjectMatch * muTriggerMatch9( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu30" ) );
  const TriggerObjectMatch * muTriggerMatch10( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu24eta2p1" ) );
  const TriggerObjectMatch * muTriggerMatch11( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu30eta2p1" ) );
  const TriggerObjectMatch * muTriggerMatch12( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu34eta2p1" ) );
  const TriggerObjectMatch * muTriggerMatch13( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTDoubleMu6" ) );
  const TriggerObjectMatch * muTriggerMatch14( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTDoubleMu7" ) );
  const TriggerObjectMatch * muTriggerMatch15( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu13Mu8" ) );
  const TriggerObjectMatch * muTriggerMatch16( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu17Mu8" ) );

  // Muon
  int nMuPassCut = 0;
  nMu_ = 0;
  const Candidate *mumom = 0;
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
        muDz_[nMu_] = -99.;
	muNumberOfHits_[nMu_] = -99;
	muNumberOfValidHits_[nMu_] = -99;
	muNumberOfInactiveHits_[nMu_] = -99;
	muNumberOfValidTrkHits_[nMu_] = -99;
	muNumberOfValidPixelHits_[nMu_] = -99;
	muNumberOfValidMuonHits_[nMu_] = -99;
	muChi2NDF_[nMu_] = -99;
        muPVD0_[nMu_] = - 99;
        muPVDz_[nMu_] = - 99;
        muValidFraction_[nMu_] = -99;
      } 
      else {
        muD0_[nMu_] = trkr->dxy(beamSpotHandle->position());
        muDz_[nMu_] = trkr->dz(beamSpotHandle->position());
	muNumberOfHits_[nMu_] = trkr->hitPattern().numberOfHits();
	muNumberOfValidHits_[nMu_] = trkr->hitPattern().numberOfValidHits();
	muNumberOfInactiveHits_[nMu_] = trkr->hitPattern().numberOfInactiveHits();
        muNumberOfValidTrkHits_[nMu_] = trkr->hitPattern().numberOfValidTrackerHits();
        muNumberOfValidPixelHits_[nMu_] = trkr->hitPattern().numberOfValidPixelHits();
        muNumberOfValidMuonHits_[nMu_] = trkr->hitPattern().numberOfValidMuonHits();
  	muChi2NDF_[nMu_] = trkr->normalizedChi2();
	muPVD0_[nMu_] = trkr->dxy((*recVtxs)[0].position());
        muPVDz_[nMu_] = trkr->dz((*recVtxs)[0].position());
        muValidFraction_[nMu_] = trkr->validFraction();
      }

      muPV2D_[nMu_] = iMu->dB(pat::Muon::PV2D);
      muPV3D_[nMu_] = iMu->dB(pat::Muon::PV3D);
      muBS2D_[nMu_] = iMu->dB(pat::Muon::BS2D);
      muBS3D_[nMu_] = iMu->dB(pat::Muon::BS3D);

      muVtx_[nMu_][0] = iMu->vertex().x();
      muVtx_[nMu_][1] = iMu->vertex().y();
      muVtx_[nMu_][2] = iMu->vertex().z();

      if (iMu->track().isNull())
        muTrkdPt_[nMu_] = -99;
      else
        muTrkdPt_[nMu_] = iMu->track()->ptError();
      
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

      muGenMomPID_[nMu_]  = -999;
      muGenMomPt_[nMu_]   = -999;
      muGenGMomPID_[nMu_] = -999;
      muGenIndex_[nMu_] = -1;
      int MuGenIndex = 0;
      if (!isData_) {
        if ( (*iMu).genLepton() && genParticlesHandle_.isValid() ) {
          if (fabs((*iMu).genLepton()->pdgId())==13) {
            for (vector<GenParticle>::const_iterator iGen = genParticlesHandle_->begin(); iGen != genParticlesHandle_->end(); ++iGen) {

              if (iGen->p4() == (*iMu).genLepton()->p4() && iGen->pdgId() == (*iMu).genLepton()->pdgId() && iGen->status() == (*iMu).genLepton()->status()) {
	        muGenIndex_[nMu_] = MuGenIndex;

                const Candidate *mup = (const Candidate*)&(*iGen);

                for (size_t j=0; j<mup->numberOfMothers(); ++j) {

                  mumom = mup->mother(j);
                  muGenMomPID_[nMu_] = mumom->pdgId();
                  muGenMomPt_[nMu_] = mumom->pt();
                  if (mumom->mother()) muGenGMomPID_[nMu_] = mumom->mother()->pdgId();
                }
	      }

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
  const TriggerObjectMatch *jetTriggerMatch15(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet30"));
  const TriggerObjectMatch *jetTriggerMatch16(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet60"));
  const TriggerObjectMatch *jetTriggerMatch17(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet80"));
  const TriggerObjectMatch *jetTriggerMatch18(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet110"));
  const TriggerObjectMatch *jetTriggerMatch19(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet150"));
  const TriggerObjectMatch *jetTriggerMatch20(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet190"));
  const TriggerObjectMatch *jetTriggerMatch21(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet240"));
  const TriggerObjectMatch *jetTriggerMatch22(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet300"));
  const TriggerObjectMatch *jetTriggerMatch23(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTJet370"));

  if (doStoreJets_) {
    nJet_ = 0;
    if ( jetHandle_.isValid() )
      for (View<pat::Jet>::const_iterator iJet = jetHandle_->begin(); iJet != jetHandle_->end(); ++iJet) {

        if ( iJet->pt() < 15 ) continue;

	if (doJetHLTMatch_) {
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
	}

	jetEn_[nJet_]     = iJet->energy();
	jetPt_[nJet_]     = iJet->pt();
	jetEta_[nJet_]    = iJet->eta();
	jetPhi_[nJet_]    = iJet->phi();
	jetMass_[nJet_]   = iJet->mass();
	jetCharge_[nJet_] = iJet->jetCharge();
	jetEt_[nJet_]     = iJet->et();
	jetRawEn_[nJet_]  = (*iJet).correctedJet("Uncorrected").energy();
	jetRawPt_[nJet_]  = (*iJet).correctedJet("Uncorrected").pt();
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
        jetChargedHadronMultiplicity_[nJet_]  = iJet->chargedHadronMultiplicity();
	jetChargedMuEnergy_[nJet_]	   = iJet->chargedMuEnergy();
	jetChargedMuEnergyFraction_[nJet_] = iJet->chargedMuEnergyFraction();
	
	// jet-vertex association
	double tracks_x = 0.;
	double tracks_y = 0.;
	double tracks_x_tot = 0.;
	double tracks_y_tot = 0.;
	for (unsigned i = 0;  i <  iJet->numberOfDaughters (); i++) {	     
 	  const reco::PFCandidatePtr pfcand = iJet->getPFConstituent(i);
	  reco::TrackRef trackref = pfcand->trackRef();
	  if ( trackref.isNonnull()) {
 	    tracks_x_tot += (trackref)->px();
	    tracks_y_tot += (trackref)->py();	  
	    if (fabs((trackref)->vz() - vtx_[0][2]) < 0.1) {        	  
	      tracks_x += (trackref)->px();
	      tracks_y += (trackref)->py();		
	    }
	  }
	}

	jetJVAlpha_[nJet_] = sqrt(tracks_x*tracks_x+tracks_y*tracks_y)/iJet->pt();
	if (tracks_x_tot!=0. || tracks_y_tot!=0.)
 	  jetJVBeta_[nJet_] = sqrt(tracks_x*tracks_x+tracks_y*tracks_y)/sqrt(tracks_x_tot*tracks_x_tot+tracks_y_tot*tracks_y_tot);
	else
	  jetJVBeta_[nJet_] = -1;

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

void VgAnalyzerKit::beginRun(edm::Run& iRun, edm::EventSetup const& iSetup) {
  bool changed(true);
  if (hltConfig_.init(iRun, iSetup, "HLT", changed)) {
    // if init returns TRUE, initialisation has succeeded!
    if (changed) {
      // The HLT config has actually changed wrt the previous Run, hence rebook your
      // histograms or do anything else dependent on the revised HLT config
    }
  } else {
    // if init returns FALSE, initialisation has NOT succeeded, which indicates a problem
    // with the file and/or code and needs to be investigated!
    std::cout << " HLT config extraction failure with process name " << "HLT" << std::endl;
    //LogError("MyAnalyzer") << " HLT config extraction failure with process name " << processName_;
    // In this case, all access methods will return empty values!
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
  //e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  e.getByLabel(InputTag("reducedEcalRecHitsES"), PreshowerRecHits);
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
  //e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  e.getByLabel(InputTag("reducedEcalRecHitsES"), PreshowerRecHits);
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
  //e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  e.getByLabel(InputTag("reducedEcalRecHitsES"), PreshowerRecHits);
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
  //e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  e.getByLabel(InputTag("reducedEcalRecHitsES"), PreshowerRecHits);
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
  //e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  e.getByLabel(InputTag("reducedEcalRecHitsES"), PreshowerRecHits);
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
  //e.getByLabel(InputTag("ecalPreshowerRecHit","EcalRecHitsES"), PreshowerRecHits);
  e.getByLabel(InputTag("reducedEcalRecHitsES"), PreshowerRecHits);
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

float VgAnalyzerKit::getTrkIso(edm::Handle<reco::TrackCollection> Tracks, 
			       math::XYZPoint vtx, 
			       double phoEta, 
			       double phoPhi, 
			       double phoVz, 
			       double outerCone = 0.4, 
			       double innerCone = 0.04, 
			       double etaSlice = 0.015, 
			       double dzCut = 0.2, 
			       double d0Cut = 0.1,
			       double ptCut = 0.,
			       int    option = 0)
{

  float trkIso = 0;
  double dz = 0;  
  double d0 = 0;
  double dR = 0;
  double dEta = 0;

  for (reco::TrackCollection::const_iterator aTrk = Tracks.product()->begin(); aTrk != Tracks.product()->end(); ++aTrk) {

    if (option == 0) 
      dz = fabs( (*aTrk).vz() - phoVz );
    else if (option == 1)
      dz = (*aTrk).dz(vtx);
    if (fabs(dz) > dzCut) continue;

    d0 = (*aTrk).dxy(vtx);
    if (fabs(d0) > d0Cut) continue;

    if ((*aTrk).pt() < ptCut) continue;

    dR = deltaR(phoEta, phoPhi, (*aTrk).eta(), (*aTrk).phi());
    dEta = (*aTrk).eta() - phoEta;

    if (dR < outerCone && outerCone >= innerCone && fabs(dEta) >= etaSlice)
      trkIso += (*aTrk).pt();
  }

  return trkIso;
}

DEFINE_FWK_MODULE(VgAnalyzerKit);
