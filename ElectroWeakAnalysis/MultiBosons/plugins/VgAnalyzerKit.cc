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

// PF isolation from alternate code
#include "EGamma/EGammaAnalysisTools/src/PFIsolationEstimator.cc"

// Electron ID in 2012
#include "EGamma/EGammaAnalysisTools/interface/EGammaCutBasedEleId.h"

#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

// BeamHalo information
#include "DataFormats/METReco/interface/BeamHaloSummary.h"

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
  doStoreSCs_    = ps.getParameter<bool>("doStoreSCs");
  doSkim_         = ps.getParameter<bool>("doSkim");
  gtdigilabel_    = ps.getParameter<InputTag>("GTDigiLabel");
  vtxlabel_       = ps.getParameter<InputTag>("VtxLabel");
  caloTowerlabel_ = ps.getParameter<InputTag>("CaloTowerLabel");
  tracklabel_     = ps.getParameter<InputTag>("TrackLabel");
  tcMETlabel_     = ps.getParameter<InputTag>("tcMETLabel");
  pfMETlabel_     = ps.getParameter<InputTag>("pfMETLabel");
  TypeIpfMETlabel_  = ps.getParameter<InputTag>("TypeIpfMETLabel");
  SmearedTypeIpfMETlabel_  = ps.getParameter<InputTag>("SmearedTypeIpfMETLabel");
  PFCandLabel_    = ps.getParameter<InputTag>("PFCandLabel");
  puInfoLabel_    = ps.getParameter<InputTag>("puInfoLabel");
  rhoLabel_       = ps.getParameter<InputTag>("rhoLabel");
  rho2011Label_   = ps.getParameter<InputTag>("rho2011Label");
  rho2012Label_   = ps.getParameter<InputTag>("rho2012Label");
  sigmaLabel_     = ps.getParameter<InputTag>("sigmaLabel");

  leadingElePtCut_ = ps.getParameter<double>("LeadingElePtCut");
  leadingMuPtCut_  = ps.getParameter<double>("LeadingMuPtCut");
  leadingPhoPtCut_ = ps.getParameter<double>("LeadingPhoPtCut");

  ebReducedRecHitCollection_ = ps.getParameter<InputTag>("ebReducedRecHitCollection");
  eeReducedRecHitCollection_ = ps.getParameter<InputTag>("eeReducedRecHitCollection");
  beamSpotCollection_        = ps.getParameter<InputTag>("BeamSpotCollection");

  // PF isolation
  inputTagIsoDepElectrons_ = ps.getParameter< std::vector<edm::InputTag> >("IsoDepElectron");
  inputTagIsoValElectronsPFId_  = ps.getParameter< std::vector<edm::InputTag> >("IsoValElectronPF");

  // skimed HLT 
  if (doSkim_ == true)
     skimedHLTpath_  = ps.getParameter<std::vector<std::string > >("skimedHLTpath");

  if (saveHistograms_) helper_.bookHistos(this);

  // cout << "VgAnalyzerKit: making output tree" << endl;

  Service<TFileService> fs;
  hEvents_ = fs->make<TH1F>("hEvents", "total processed and skimmed events", 2, 0, 2);
  tree_ = fs->make<TTree>("EventTree", "Event data");

  tree_->Branch("METFilter", METFilter_, "METFilter[10]/I");
  tree_->Branch("run", &run_, "run/I");
  tree_->Branch("event", &event_, "event/L");
  tree_->Branch("orbit", &orbit_, "orbit/I");
  tree_->Branch("bx", &bx_, "bx/I");
  tree_->Branch("lumis", &lumis_, "lumis/I");
  tree_->Branch("isData", &isData_, "isData/O");
  tree_->Branch("ttbit0", &ttbit0_, "ttbit0/I");
  tree_->Branch("nHLT", &nHLT_, "nHLT/I");
  tree_->Branch("HLT", HLT_, "HLT[nHLT]/I");
  tree_->Branch("HLTIndex", HLTIndex_, "HLTIndex[34]/I");
  tree_->Branch("HLTprescale", HLTprescale_, "HLTprescale[nHLT]/I");
  tree_->Branch("nVtx", &nVtx_, "nVtx/I");
  tree_->Branch("vtx", vtx_, "vtx[nVtx][3]/F");
  tree_->Branch("vtxNTrk", vtxNTrk_, "vtxNTrk[nVtx]/I");
  tree_->Branch("vtxNDF", vtxNDF_, "vtxNDF[nVtx]/F");
  tree_->Branch("vtxD0", vtxD0_, "vtxD0[nVtx]/F");
  tree_->Branch("nGoodVtx", &nGoodVtx_, "nGoodVtx/I");
  tree_->Branch("nTrk", &nTrk_, "nTrk/I");
  tree_->Branch("nGoodTrk", &nGoodTrk_, "nGoodTrk/I");
  tree_->Branch("IsTracksGood", &IsTracksGood_, "IsTracksGood/I");
  tree_->Branch("rho", &rho_, "rho/D");
  tree_->Branch("rho2011", &rho2011_, "rho2011/D");
  tree_->Branch("rho2012", &rho2012_, "rho2012/D");
  tree_->Branch("sigma", &sigma_, "sigma/F");
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
    tree_->Branch("mcCHIsoDR03", mcCHIsoDR03, "mcCHIsoDR03[nMC]/F");
    tree_->Branch("mcNHIsoDR03", mcNHIsoDR03, "mcNHIsoDR03[nMC]/F");
    tree_->Branch("mcPhIsoDR03", mcPhIsoDR03, "mcPhIsoDR03[nMC]/F");
    tree_->Branch("mcIsoDR04", mcIsoDR04, "mcIsoDR04[nMC]/F");
    tree_->Branch("mcCHIsoDR04", mcCHIsoDR04, "mcCHIsoDR04[nMC]/F");
    tree_->Branch("mcNHIsoDR04", mcNHIsoDR04, "mcNHIsoDR04[nMC]/F");
    tree_->Branch("mcPhIsoDR04", mcPhIsoDR04, "mcPhIsoDR04[nMC]/F");
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
  if (doGenParticles_) {
    // Smeared TypeI pfMET
    tree_->Branch("SmearedTypeIpfMET", &SmearedTypeIpfMET_, "SmearedTypeIpfMET/F");
    tree_->Branch("SmearedTypeIpfMETx", &SmearedTypeIpfMETx_, "SmearedTypeIpfMETx/F");
    tree_->Branch("SmearedTypeIpfMETy", &SmearedTypeIpfMETy_, "SmearedTypeIpfMETy/F");
    tree_->Branch("SmearedTypeIpfMETPhi", &SmearedTypeIpfMETPhi_, "SmearedTypeIpfMETPhi/F");
    tree_->Branch("SmearedTypeIpfMETsumEt", &SmearedTypeIpfMETsumEt_, "SmearedTypeIpfMETsumEt/F");
    tree_->Branch("SmearedTypeIpfMETmEtSig", &SmearedTypeIpfMETmEtSig_, "SmearedTypeIpfMETmEtSig/F");
    tree_->Branch("SmearedTypeIpfMETSig", &SmearedTypeIpfMETSig_, "SmearedTypeIpfMETSig/F");
  }
  // Electron
  tree_->Branch("nEle", &nEle_, "nEle/I");
  tree_->Branch("eleEcalDriven", eleEcalDriven_, "eleEcalDriven[nEle]/O");
  tree_->Branch("eleTrg", eleTrg_, "eleTrg[nEle][14]/I");
  tree_->Branch("eleID", eleID_, "eleID[nEle][30]/I");
  tree_->Branch("eleClass", eleClass_, "eleClass[nEle]/I");
  tree_->Branch("eleCharge", eleCharge_, "eleCharge[nEle]/I");
  tree_->Branch("eleVtx", eleVtx_, "eleVtx[nEle][3]/F");
  tree_->Branch("eleEn", eleEn_, "eleEn[nEle]/F");
  tree_->Branch("eleEcalEn", eleEcalEn_, "eleEcalEn[nEle]/F");
  tree_->Branch("eleGsfP", eleGsfP_, "eleGsfP[nEle]/F");
  tree_->Branch("elePt", elePt_, "elePt[nEle]/F");
  tree_->Branch("elePz", elePz_, "elePz[nEle]/F");
  tree_->Branch("eleEta", eleEta_, "eleEta[nEle]/F");
  tree_->Branch("elePhi", elePhi_, "elePhi[nEle]/F");
  tree_->Branch("eleSCEn", eleSCEn_, "eleSCEn[nEle]/F");
  tree_->Branch("eleSCRawEn", eleSCRawEn_, "eleSCRawEn[nEle]/F");
  tree_->Branch("eleESEn", eleESEn_, "eleESEn[nEle]/F");
  tree_->Branch("eleSCEta", eleSCEta_, "eleSCEta[nEle]/F");
  tree_->Branch("eleSCPhi", eleSCPhi_, "eleSCPhi[nEle]/F");
  tree_->Branch("eleSCEtaWidth", eleSCEtaWidth_, "eleSCEtaWidth[nEle]/F");
  tree_->Branch("eleSCPhiWidth", eleSCPhiWidth_, "eleSCPhiWidth[nEle]/F");
  tree_->Branch("eleHoverE", eleHoverE_, "eleHoverE[nEle]/F");
  tree_->Branch("eleHoverEbc", eleHoverEbc_, "eleHoverEbc[nEle]/F");
  tree_->Branch("eleEoverP", eleEoverP_, "eleEoverP[nEle]/F");
  tree_->Branch("eleTrkPErr", eleTrkPErr_, "eleTrkPErr[nEle]/F");
  tree_->Branch("elePin", elePin_, "elePin[nEle]/F");
  tree_->Branch("elePout", elePout_, "elePout[nEle]/F");
  tree_->Branch("eleBrem", eleBrem_, "eleBrem[nEle]/F");
  tree_->Branch("elenBrem", elenBrem_, "elenBrem[nEle]/I");
  tree_->Branch("eledEtaAtVtx", eledEtaAtVtx_, "eledEtaAtVtx[nEle]/F");
  tree_->Branch("eledPhiAtVtx", eledPhiAtVtx_, "eledPhiAtVtx[nEle]/F");
  tree_->Branch("eledEtaAtCalo", eledEtaAtCalo_, "eledEtaAtCalo[nEle]/F");
  tree_->Branch("eledPhiAtCalo", eledPhiAtCalo_, "eledPhiAtCalo[nEle]/F");
  tree_->Branch("eleSigmaEtaEta", eleSigmaEtaEta_, "eleSigmaEtaEta[nEle]/F");
  tree_->Branch("eleSigmaIEtaIEta", eleSigmaIEtaIEta_, "eleSigmaIEtaIEta[nEle]/F");
  tree_->Branch("eleSigmaIEtaIPhi", eleSigmaIEtaIPhi_, "eleSigmaIEtaIPhi[nEle]/F");
  tree_->Branch("eleSigmaIPhiIPhi", eleSigmaIPhiIPhi_, "eleSigmaIPhiIPhi[nEle]/F");
  tree_->Branch("eleEmax", eleEmax_, "eleEmax[nEle]/F");
  tree_->Branch("eleE1x5", eleE1x5_, "eleE1x5[nEle]/F");
  tree_->Branch("eleE3x3", eleE3x3_, "eleE3x3[nEle]/F");
  tree_->Branch("eleE5x5", eleE5x5_, "eleE5x5[nEle]/F");
  tree_->Branch("eleKfChi2", eleKfChi2_, "eleKfChi2[nEle]/F");
  tree_->Branch("eleKfHits", eleKfHits_, "eleKfHits[nEle]/F");
  tree_->Branch("eleKfHitsAll", eleKfHitsAll_, "eleKfHitsAll[nEle]/F");
  tree_->Branch("eleGsfChi2", eleGsfChi2_, "eleGsfChi2[nEle]/F");
  tree_->Branch("eleSeedTime", eleSeedTime_, "eleSeedTime[nEle]/F");
  tree_->Branch("eleSeedEnergy", eleSeedEnergy_, "eleSeedEnergy[nEle]/F");
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
  tree_->Branch("eleConvMissinghit", eleConvMissinghit_, "eleConvMissinghit[nEle]/I");
  tree_->Branch("eleConversionveto", eleConversionveto_, "eleConversionveto[nEle]/O");
  tree_->Branch("elePVD0", elePVD0_, "elePVD0[nEle]/F");
  tree_->Branch("elePVDz", elePVDz_, "elePVDz[nEle]/F");
  tree_->Branch("eleGVD0", eleGVD0_, "eleGVD0[nEle]/F");
  tree_->Branch("eleGVDz", eleGVDz_, "eleGVDz[nEle]/F");
  tree_->Branch("eleIP3D", eleIP3D_, "eleIP3D[nEle]/F");
  tree_->Branch("eleIP3DErr", eleIP3DErr_, "eleIP3DErr[nEle]/F");
  tree_->Branch("eleIsPFID", eleIsPFID_, "eleIsPFID[nEle]/O");
  tree_->Branch("elePfChargedHadronDR03", elePfChargedHadronDR03_, "elePfChargedHadronDR03[nEle]/D");
  tree_->Branch("elePfNeutralHadronDR03", elePfNeutralHadronDR03_, "elePfNeutralHadronDR03[nEle]/D");
  tree_->Branch("elePfPhotonDR03", elePfPhotonDR03_, "elePfPhotonDR03[nEle]/D");
  tree_->Branch("elePfChargedHadronDR04", elePfChargedHadronDR04_, "elePfChargedHadronDR04[nEle]/D");
  tree_->Branch("elePfNeutralHadronDR04", elePfNeutralHadronDR04_, "elePfNeutralHadronDR04[nEle]/D");
  tree_->Branch("elePfPhotonDR04", elePfPhotonDR04_, "elePfPhotonDR04[nEle]/D");
  // Photon
  tree_->Branch("nPho", &nPho_, "nPho/I");
  tree_->Branch("phoTrg", phoTrg_, "phoTrg[nPho][23]/I");
  tree_->Branch("phoIsPhoton", phoIsPhoton_, "phoIsPhoton[nPho]/O");
  tree_->Branch("phoE", phoE_, "phoE[nPho]/F");
  tree_->Branch("phoEt", phoEt_, "phoEt[nPho]/F");
  tree_->Branch("phoPz", phoPz_, "phoPz[nPho]/F");
  tree_->Branch("phoEta", phoEta_, "phoEta[nPho]/F");
  tree_->Branch("phoPhi", phoPhi_, "phoPhi[nPho]/F");
  tree_->Branch("phoR9", phoR9_, "phoR9[nPho]/F");
  tree_->Branch("phoTrkIsoSolidDR03", phoTrkIsoSolidDR03_, "phoTrkIsoSolidDR03[nPho]/F");
  tree_->Branch("phoTrkIsoHollowDR03", phoTrkIsoHollowDR03_, "phoTrkIsoHollowDR03[nPho]/F");
  tree_->Branch("phoEcalIsoDR03", phoEcalIsoDR03_, "phoEcalIsoDR03[nPho]/F");
  tree_->Branch("phoHcalIsoDR03", phoHcalIsoDR03_, "phoHcalIsoDR03[nPho]/F");
  tree_->Branch("phoTrkIsoSolidDR04", phoTrkIsoSolidDR04_, "phoTrkIsoSolidDR04[nPho]/F");
  tree_->Branch("phoTrkIsoHollowDR04", phoTrkIsoHollowDR04_, "phoTrkIsoHollowDR04[nPho]/F");
  tree_->Branch("phoEcalIsoDR04", phoEcalIsoDR04_, "phoEcalIsoDR04[nPho]/F");
  tree_->Branch("phoHcalIsoDR04", phoHcalIsoDR04_, "phoHcalIsoDR04[nPho]/F");
  tree_->Branch("phoEtVtx", phoEtVtx_, "phoEtVtx[nPho][150]/F");
  tree_->Branch("phoEtaVtx", phoEtaVtx_, "phoEtaVtx[nPho][150]/F");
  tree_->Branch("phoPhiVtx", phoPhiVtx_, "phoPhiVtx[nPho][150]/F");
  tree_->Branch("phoHoverE", phoHoverE_, "phoHoverE[nPho]/F");
  tree_->Branch("phoHoverEbc", phoHoverEbc_, "phoHoverEbc[nPho]/F");
  tree_->Branch("phoSigmaEtaEta", phoSigmaEtaEta_, "phoSigmaEtaEta[nPho]/F");
  tree_->Branch("phoSigmaIEtaIEta", phoSigmaIEtaIEta_, "phoSigmaIEtaIEta[nPho]/F");
  tree_->Branch("phoSigmaIEtaIPhi", phoSigmaIEtaIPhi_, "phoSigmaIEtaIPhi[nPho]/F");
  tree_->Branch("phoSigmaIPhiIPhi", phoSigmaIPhiIPhi_, "phoSigmaIPhiIPhi[nPho]/F");
  tree_->Branch("phoE1x5", phoE1x5_, "phoE1x5[nPho]/F");
  tree_->Branch("phoE2x5", phoE2x5_, "phoE2x5[nPho]/F");
  tree_->Branch("phoE3x3", phoE3x3_, "phoE3x3[nPho]/F");
  tree_->Branch("phoE5x5", phoE5x5_, "phoE5x5[nPho]/F");
  tree_->Branch("phoSeedTime", phoSeedTime_, "phoSeedTime[nPho]/F");
  tree_->Branch("phoSeedEnergy", phoSeedEnergy_, "phoSeedEnergy[nPho]/F");
  tree_->Branch("phoPos", phoPos_, "phoPos[nPho]/I");
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
  tree_->Branch("phoVtx", phoVtx_, "phoVtx[nPho][3]/F");
  tree_->Branch("phoOverlap", phoOverlap_, "phoOverlap[nPho]/I");
  tree_->Branch("phohasPixelSeed", phohasPixelSeed_, "phohasPixelSeed[nPho]/I");
  tree_->Branch("phoElectronveto", phoElectronveto_, "phoElectronveto[nPho]/I");
  tree_->Branch("phoIsPFID", phoIsPFID_, "phoIsPFID[nPho]/O");
  tree_->Branch("phoPfChargedHadron", phoPfChargedHadron_, "phoPfChargedHadron[nPho]/F");
  tree_->Branch("phoPfNeutralHadron", phoPfNeutralHadron_, "phoPfNeutralHadron[nPho]/F");
  tree_->Branch("phoPfPhoton", phoPfPhoton_, "phoPfPhoton[nPho]/F");
  // Muon
  tree_->Branch("nMu", &nMu_, "nMu/I");
  tree_->Branch("muIsPFMu", muIsPFMu_, "muIsPFMu[nMu]/O");
  tree_->Branch("muIsGlobalMu", muIsGlobalMu_, "muIsGlobalMu[nMu]/O");
  tree_->Branch("muIsTrackerMu", muIsTrackerMu_, "muIsTrackerMu[nMu]/O");
  tree_->Branch("muTrg", muTrg_, "muTrg[nMu][11]/I");
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
  tree_->Branch("muType", muType_, "muType[nMu]/I");
  tree_->Branch("muID", muID_, "muID[nMu][6]/O");
  // [0]: AllArbitrated, [1]: GlobalMuonPromptTight, [2]: TMLSLoose, [3]: TMLSTight, [4]: TM2DCompatLoose, [5]: TM2DCompatTight
  tree_->Branch("muPVD0", muPVD0_, "muPVD0[nMu]/F");
  tree_->Branch("muPVDz", muPVDz_, "muPVDz[nMu]/F");
  tree_->Branch("muGVD0", muGVD0_, "muGVD0[nMu]/F");
  tree_->Branch("muGVDz", muGVDz_, "muGVDz[nMu]/F");
  tree_->Branch("muTrkdPt", muTrkdPt_, "muTrkdPt[nMu]/F");
  tree_->Branch("muNumberOfValidTrkHits", muNumberOfValidTrkHits_, "muNumberOfValidTrkHits[nMu]/I");
  tree_->Branch("muNumberOfValidPixelHits", muNumberOfValidPixelHits_, "muNumberOfValidPixelHits[nMu]/I");
  tree_->Branch("muNumberOfValidMuonHits", muNumberOfValidMuonHits_, "muNumberOfValidMuonHits[nMu]/I");
  tree_->Branch("muNumberOfTrackerLayers", muNumberOfTrackerLayers_, "muNumberOfTrackerLayers[nMu]/I");
  tree_->Branch("muNumberOfMatchedStations", muNumberOfMatchedStations_, "muNumberOfMatchedStations[nMu]/I");
  tree_->Branch("muVtx", muVtx_, "muVtx[nMu][3]/F");
  tree_->Branch("muPfChargedHadron", muPfChargedHadron_, "muPfChargedHadron[nMu]/F");
  tree_->Branch("muPfNeutralHadron", muPfNeutralHadron_, "muPfNeutralHadron[nMu]/F");
  tree_->Branch("muPfPhoton", muPfPhoton_, "muPfPhoton[nMu]/F");
  tree_->Branch("muPfSumPUPt", muPfSumPUPt_, "muPfSumPUPt[nMu]/F");
  // Jet
  if (doStoreJets_) {
    tree_->Branch("nJet", &nJet_, "nJet/I");
    tree_->Branch("jetTrg", jetTrg_, "jetTrg[nJet][7]/I");
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
  // SC information
  if (doStoreSCs_) {
    tree_->Branch("nEBSC", &nEBSC_, "nEBSC/I");
    tree_->Branch("scEBEn", scEBEn_, "scEBEn[nEBSC]/F");
    tree_->Branch("scEBEt", scEBEt_, "scEBEt[nEBSC]/F");
    tree_->Branch("scEBEta", scEBEta_, "scEBEta[nEBSC]/F");
    tree_->Branch("scEBPhi", scEBPhi_, "scEBPhi[nEBSC]/F");
    tree_->Branch("nEESC", &nEESC_, "nEESC/I");
    tree_->Branch("scEEEn", scEEEn_, "scEEEn[nEESC]/F");
    tree_->Branch("scEEEt", scEEEt_, "scEEEt[nEESC]/F");
    tree_->Branch("scEEEta", scEEEta_, "scEEEta[nEESC]/F");
    tree_->Branch("scEEPhi", scEEPhi_, "scEEPhi[nEESC]/F");
  }

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
  const reco::BeamSpot &beamSpot = *(beamSpotHandle.product());

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

  // vertex
  Int_t firstGoodVtx = -1;
  nVtx_ = 0;
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

	if (vtxNDF_[nVtx_] >= 4 && fabs(vtx_[nVtx_][2]) <= 24 && fabs(vtxD0_[nVtx_]) <= 2) {
	  nGoodVtx_++;

	  if (nGoodVtx_ == 1) firstGoodVtx = i;
	}

	nVtx_++;
      }
    }
  }

  // Set PV and first good vertex
  if (nGoodVtx_ == 0) firstGoodVtx = 0; 
  math::XYZPoint gv(vtx_[firstGoodVtx][0], vtx_[firstGoodVtx][1], vtx_[firstGoodVtx][2]);

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

  // Rho for electron isolation
  edm::Handle<double> rho2011Handle;
  e.getByLabel(rho2011Label_, rho2011Handle);
  rho2011_ = *(rho2011Handle.product());

  // Rho for 2012 sample
  edm::Handle<double> rho2012Handle;
  e.getByLabel(rho2012Label_, rho2012Handle);
  rho2012_ = *(rho2012Handle.product());

  for (int a=0; a<10; a++)
    METFilter_[a] = -1;

  // The iso-based HBHE noise filter
  edm::Handle<bool> HBHENoiseFilterResultHandle;
  e.getByLabel(edm::InputTag("HBHENoiseFilterResultProducer", "HBHENoiseFilterResult"), HBHENoiseFilterResultHandle);
  METFilter_[0] = (int) (*HBHENoiseFilterResultHandle);

  // The CSC beam halo tight filter
  edm::Handle<BeamHaloSummary> TheBeamHaloSummary;
  e.getByLabel("BeamHaloSummary", TheBeamHaloSummary);
  const BeamHaloSummary TheSummary = (*TheBeamHaloSummary.product() );
  METFilter_[1] = (int) (TheSummary.CSCTightHaloId());  

  // The HCAL laser filter
  edm::Handle<bool> HcalLaserFilter;
  e.getByLabel("hcalLaserEventFilter", HcalLaserFilter);
  METFilter_[2] = (int) (*HcalLaserFilter);

  // The ECAL dead cell trigger primitive filter
  edm::Handle<bool> EcalDeadCellFilter;
  e.getByLabel("EcalDeadCellTriggerPrimitiveFilter", EcalDeadCellFilter);
  METFilter_[3] = (int) (*EcalDeadCellFilter);

  // The EE bad SuperCrystal filter
  edm::Handle<bool> EEBadScFilter;
  e.getByLabel("eeBadScFilter", EEBadScFilter);
  METFilter_[4] = (int) (*EEBadScFilter);

  // The ECAL laser correction filter
  edm::Handle<bool> EcalLaserFilter;
  e.getByLabel("ecalLaserCorrFilter", EcalLaserFilter);
  METFilter_[5] = (int) (*EcalLaserFilter);

  // The tracking failure filter
  edm::Handle<bool> TrackingFailureFilter;
  e.getByLabel("trackingFailureFilter", TrackingFailureFilter);
  METFilter_[6] = (int) (*TrackingFailureFilter);

  // The tracking POG filters
  edm::Handle<bool> Manystripclus53X;
  e.getByLabel("manystripclus53X", Manystripclus53X);
  METFilter_[7] = (int) (*Manystripclus53X);

  edm::Handle<bool> Toomanystripclus53X;
  e.getByLabel("toomanystripclus53X", Toomanystripclus53X);
  METFilter_[8] = (int) (*Toomanystripclus53X);

  edm::Handle<bool> LogErrorTooManyClusters;
  e.getByLabel("logErrorTooManyClusters", LogErrorTooManyClusters);
  METFilter_[9] = (int) (*LogErrorTooManyClusters);

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
        mcCHIsoDR03[nMC_]  = getGenCHIso(genParticlesHandle_, ip, 0.3);
        mcNHIsoDR03[nMC_]  = getGenNHIso(genParticlesHandle_, ip, 0.3);
        mcPhIsoDR03[nMC_]  = getGenPhIso(genParticlesHandle_, ip, 0.3);
	mcIsoDR04[nMC_]    = getGenIso(genParticlesHandle_, ip, 0.4, false, false);
        mcCHIsoDR04[nMC_]  = getGenCHIso(genParticlesHandle_, ip, 0.4);
        mcNHIsoDR04[nMC_]  = getGenNHIso(genParticlesHandle_, ip, 0.4);
        mcPhIsoDR04[nMC_]  = getGenPhIso(genParticlesHandle_, ip, 0.4);

	nMC_++;
      }
    }
  }

  // HLT
  // cout << "VgAnalyzerKit: produce: HLT ... " << endl;
  // Indicate the index of interesting HLT bits. Even CMS has different HLT table for different runs, we can still use the correct HLT bit
  std::map<unsigned, std::string> HLTIndexPath;  
  // HLT path for 2012 analysis
  HLTIndexPath[0]  = "HLT_PFJet40_v";
  HLTIndexPath[1]  = "HLT_PFJet80_v";
  HLTIndexPath[2]  = "HLT_PFJet140_v";
  HLTIndexPath[3]  = "HLT_PFJet200_v";
  HLTIndexPath[4]  = "HLT_PFJet260_v";
  HLTIndexPath[5]  = "HLT_PFJet320_v";
  HLTIndexPath[6]  = "HLT_PFJet400_v";
  HLTIndexPath[7]  = "HLT_IsoMu24_v";
  HLTIndexPath[8]  = "HLT_IsoMu30_v";
  HLTIndexPath[9]  = "HLT_IsoMu24_eta2p1_v";
  HLTIndexPath[10] = "HLT_IsoMu30_eta2p1_v";
  HLTIndexPath[11] = "HLT_IsoMu34_eta2p1_v";
  HLTIndexPath[12] = "HLT_Mu17_Mu8_v";
  HLTIndexPath[13] = "HLT_Mu17_TkMu8_v";
  HLTIndexPath[14] = "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v";
  HLTIndexPath[15] = "HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v";
  HLTIndexPath[16] = "HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v";
  HLTIndexPath[17] = "HLT_Ele32_CaloIdT_CaloIsoT_TrkIdT_TrkIsoT_SC17_Mass50_v";
  HLTIndexPath[18] = "HLT_Ele27_WP80_PFMET_MT50_v";
  HLTIndexPath[19] = "HLT_Ele27_WP80_v";
  HLTIndexPath[20] = "HLT_Photon20_CaloIdVL_IsoL_v";
  HLTIndexPath[21] = "HLT_Photon26_Photon18_v";
  HLTIndexPath[22] = "HLT_Photon30_CaloIdVL_v";
  HLTIndexPath[23] = "HLT_Photon30_CaloIdVL_IsoL_v";
  HLTIndexPath[24] = "HLT_Photon36_Photon22_v";
  HLTIndexPath[25] = "HLT_Photon36_CaloId10_Iso50_Photon22_CaloId10_Iso50_v";
  HLTIndexPath[26] = "HLT_Photon50_CaloIdVL_v";
  HLTIndexPath[27] = "HLT_Photon50_CaloIdVL_IsoL_v";
  HLTIndexPath[28] = "HLT_Photon75_CaloIdVL_v";
  HLTIndexPath[29] = "HLT_Photon75_CaloIdVL_IsoL_v";
  HLTIndexPath[30] = "HLT_Photon90_CaloIdVL_v";
  HLTIndexPath[31] = "HLT_Photon90_CaloIdVL_IsoL_v";
  HLTIndexPath[32] = "HLT_Photon135_v";
  HLTIndexPath[33] = "HLT_Photon150_v";

  for (int a=0; a<34; a++)
    HLTIndex_[a] = -1;
 
  bool skimed_HLT = false;

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

    if (doSkim_ == true) {
      for (size_t j = 0; j<skimedHLTpath_.size(); ++j) {
        if(TString(hlNames[i]).Contains(TRegexp(skimedHLTpath_[j])) == 1 && HLT_[i] == 1) skimed_HLT = true;
      }
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

  if (doGenParticles_) {
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

  // Get the hcal hits
  edm::Handle<CaloTowerCollection> towerHandle;
  e.getByLabel("towerMaker", towerHandle);
  const CaloTowerCollection* towers = towerHandle.product();
  EgammaTowerIsolation myHcalIsoDP1DR03(0.3, 0., 0, 1, towers);
  EgammaTowerIsolation myHcalIsoDP2DR03(0.3, 0., 0, 2, towers);
  EgammaTowerIsolation myHcalIsoDP1DR04(0.4, 0., 0, 1, towers);
  EgammaTowerIsolation myHcalIsoDP2DR04(0.4, 0., 0, 2, towers);

  // get the iso deposits. 3 (charged hadrons, photons, neutral hadrons)
  unsigned nTypes=3;
  IsoDepositMaps electronIsoDep(nTypes);
  for (size_t j = 0; j<inputTagIsoDepElectrons_.size(); ++j) {
    e.getByLabel(inputTagIsoDepElectrons_[j], electronIsoDep[j]);
  }

  IsoDepositVals electronIsoValPFIdDR03(nTypes);
  IsoDepositVals electronIsoValPFIdDR04(nTypes);
  const IsoDepositVals * electronIsoValsDR03 = &electronIsoValPFIdDR03;
  const IsoDepositVals * electronIsoValsDR04 = &electronIsoValPFIdDR04;

  for (size_t j = 0; j<inputTagIsoValElectronsPFId_.size(); ++j) {
    if (j<3)
      e.getByLabel(inputTagIsoValElectronsPFId_[j], electronIsoValPFIdDR03[j]);
    else
      e.getByLabel(inputTagIsoValElectronsPFId_[j], electronIsoValPFIdDR04[j-3]);
  }

  edm::ESHandle<TransientTrackBuilder> builder;
  es.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
  TransientTrackBuilder transientTrackBuilder = *(builder.product());

  // Electron
  edm::Handle<reco::GsfElectronCollection> hElectrons;
  e.getByLabel("gsfElectrons", hElectrons);

  // cout << "VgAnalyzerKit: produce: Electron ..." << endl;
  const TriggerObjectMatch *eleTriggerMatch1(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVLEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"));
  const TriggerObjectMatch *eleTriggerMatch2(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"));
  const TriggerObjectMatch *eleTriggerMatch3(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle8CaloIdTCaloIsoVLTrkIdVLTrkIsoVL"));
  const TriggerObjectMatch *eleTriggerMatch4(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass50"));
  const TriggerObjectMatch *eleTriggerMatch5(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4Mass50"));
  const TriggerObjectMatch *eleTriggerMatch6(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17Mass50"));
  const TriggerObjectMatch *eleTriggerMatch7(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle27WP80PFMETMT50"));
  const TriggerObjectMatch *eleTriggerMatch8(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle27WP80"));
  const TriggerObjectMatch *eleTriggerMatch9(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT"));
  const TriggerObjectMatch *eleTriggerMatch10(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVT"));
  const TriggerObjectMatch *eleTriggerMatch11(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT"));
  const TriggerObjectMatch *eleTriggerMatch12(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTEle8Mass50"));
  const TriggerObjectMatch *eleTriggerMatch13(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTSC4Mass50"));
  const TriggerObjectMatch *eleTriggerMatch14(triggerEvent->triggerObjectMatchResult("electronTriggerMatchHLTSC17Mass50"));

  nEle_ = 0;
  const Candidate *elemom = 0;
  if ( electronHandle_.isValid() )
    for (View<pat::Electron>::const_iterator iEle = electronHandle_->begin(); iEle != electronHandle_->end(); ++iEle) {

      eleEcalDriven_[nEle_] = iEle->ecalDriven();

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
      eleTrg_[nEle_][10] = (eleTrigRef11.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][11] = (eleTrigRef12.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][12] = (eleTrigRef13.isAvailable()) ? 1 : -99;
      eleTrg_[nEle_][13] = (eleTrigRef14.isAvailable()) ? 1 : -99;

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

      eleVtx_[nEle_][0] = iEle->trackPositionAtVtx().x();
      eleVtx_[nEle_][1] = iEle->trackPositionAtVtx().y();
      eleVtx_[nEle_][2] = iEle->trackPositionAtVtx().z();

      eleClass_[nEle_]   = iEle->classification();
      eleCharge_[nEle_]  = iEle->charge();
      eleEn_[nEle_]      = iEle->energy();
      eleEcalEn_[nEle_]  = iEle->ecalEnergy();
      eleGsfP_[nEle_]    = iEle->gsfTrack()->p();
      elePt_[nEle_]      = iEle->pt();
      elePz_[nEle_]      = iEle->pz();
      eleEta_[nEle_]     = iEle->eta();
      elePhi_[nEle_]     = iEle->phi();
      eleHoverE_[nEle_]  = iEle->hadronicOverEm();
      eleHoverEbc_[nEle_]= iEle->hcalOverEcalBc();
      eleEoverP_[nEle_]  = iEle->eSuperClusterOverP();

      eleTrkPErr_[nEle_] = iEle->trackMomentumError();
      elePin_[nEle_]   = iEle->trackMomentumAtVtx().R();
      elePout_[nEle_]  = iEle->trackMomentumOut().R();
      eleBrem_[nEle_]  = iEle->fbrem();
      elenBrem_[nEle_] = iEle->numberOfBrems();

      eledEtaAtVtx_[nEle_] = iEle->deltaEtaSuperClusterTrackAtVtx();
      eledPhiAtVtx_[nEle_] = iEle->deltaPhiSuperClusterTrackAtVtx();

      eledEtaAtCalo_[nEle_] = iEle->deltaEtaSeedClusterTrackAtCalo();
      eledPhiAtCalo_[nEle_] = iEle->deltaPhiSeedClusterTrackAtCalo();

      // Acess super cluster
      eleSCEta_[nEle_]   = iEle->superCluster()->eta();
      eleSCPhi_[nEle_]   = iEle->superCluster()->phi();
      eleSCRawEn_[nEle_] = iEle->superCluster()->rawEnergy();
      eleSCEn_[nEle_]    = iEle->superCluster()->energy();
      eleESEn_[nEle_]    = iEle->superCluster()->preshowerEnergy();
      eleSCEtaWidth_[nEle_] = iEle->superCluster()->etaWidth();
      eleSCPhiWidth_[nEle_] = iEle->superCluster()->phiWidth();

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
      eleIsoHcalSolidDR03_[nEle_] = myHcalIsoDP1DR03.getTowerEtSum(eleSC) + myHcalIsoDP2DR03.getTowerEtSum(eleSC);

      eleIsoTrkDR04_[nEle_]  = iEle->dr04TkSumPt();
      eleIsoEcalDR04_[nEle_] = iEle->dr04EcalRecHitSumEt();
      eleIsoHcalDR04_[nEle_] = iEle->dr04HcalTowerSumEt();
      eleIsoHcalSolidDR04_[nEle_] = myHcalIsoDP1DR04.getTowerEtSum(eleSC) + myHcalIsoDP2DR04.getTowerEtSum(eleSC);

      eleConvDist_[nEle_]       = iEle->convDist();
      eleConvDcot_[nEle_]       = iEle->convDcot();
      eleConvMissinghit_[nEle_] = iEle->gsfTrack()->trackerExpectedHitsInner().numberOfHits();

      const reco::CaloClusterPtr eleSeed = (*iEle).superCluster()->seed();

      vector<float> eleCov;
      eleCov = lazyTool.localCovariances(*eleSeed);
      eleSigmaEtaEta_[nEle_]   = iEle->sigmaEtaEta();
      eleSigmaIEtaIEta_[nEle_] = iEle->sigmaIetaIeta();
      eleSigmaIEtaIPhi_[nEle_] = sqrt(eleCov[1]);
      eleSigmaIPhiIPhi_[nEle_] = sqrt(eleCov[2]);

      eleEmax_[nEle_] = lazyTool.eMax(*eleSeed);
      eleE1x5_[nEle_] = iEle->e1x5();
      eleE3x3_[nEle_] = lazyTool.e3x3(*eleSeed);
      eleE5x5_[nEle_] = iEle->e5x5();

      bool validKF= false; 
      reco::TrackRef myTrackRef = iEle->closestCtfTrackRef();
      validKF = (myTrackRef.isAvailable());
      validKF = (myTrackRef.isNonnull());  
   
      eleKfChi2_[nEle_]    = (validKF) ? myTrackRef->normalizedChi2() : 0 ;
      eleKfHits_[nEle_]    = (validKF) ? myTrackRef->hitPattern().trackerLayersWithMeasurement() : -1. ; 
      eleKfHitsAll_[nEle_] = (validKF) ? myTrackRef->numberOfValidHits() : -1. ;   //  save also this in your ntuple as possible alternative
      eleGsfChi2_[nEle_]   = iEle->gsfTrack()->normalizedChi2();  
   
      eleSeedTime_[nEle_] = -999.;
      eleSeedEnergy_[nEle_] = -999.;

      DetId eleSeedDetId = lazyTool.getMaximum(*eleSeed).first;

      if ( iEle->isEB() && EBReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eleebrhit = EBReducedRecHits->find(eleSeedDetId);
        if ( eleebrhit != EBReducedRecHits->end() ) { 
	   eleSeedTime_[nEle_] = eleebrhit->time(); 
	   eleSeedEnergy_[nEle_] = eleebrhit->energy(); 
	}
      } else if ( EEReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eleeerhit = EEReducedRecHits->find(eleSeedDetId);
        if ( eleeerhit != EEReducedRecHits->end() ) { 
	   eleSeedTime_[nEle_] = eleeerhit->time(); 
	   eleSeedEnergy_[nEle_] = eleeerhit->energy(); 
	}
      }

      elePVD0_[nEle_] = iEle->gsfTrack()->dxy((*recVtxs)[0].position());
      elePVDz_[nEle_] = iEle->gsfTrack()->dz((*recVtxs)[0].position());

      eleGVD0_[nEle_] = iEle->gsfTrack()->dxy(gv);
      eleGVDz_[nEle_] = iEle->gsfTrack()->dz(gv);

      //default values for IP3D
      eleIP3D_[nEle_] = -999.;
      eleIP3DErr_[nEle_] = -99.;
      if (iEle->gsfTrack().isNonnull()) {
	const double gsfsign = ((- iEle->gsfTrack()->dxy((*recVtxs)[0].position())) >= 0) ? 1. : -1.;

	const reco::TransientTrack tt_ele = transientTrackBuilder.build(iEle->gsfTrack());
	const std::pair<bool,Measurement1D> ip3dpv_ele = IPTools::absoluteImpactParameter3D(tt_ele, *recVtxs->begin());
	if (ip3dpv_ele.first) {
	  eleIP3D_[nEle_]    = gsfsign*ip3dpv_ele.second.value();
	  eleIP3DErr_[nEle_] = ip3dpv_ele.second.error();
	}
      }

      eleIsPFID_[nEle_] = iEle->passingMvaPreselection();

      edm::Ptr<reco::Candidate> recoEleRef = iEle->originalObjectRef();                                                                                  
      const reco::GsfElectron *recoElectron = dynamic_cast<const reco::GsfElectron *>(recoEleRef.get());        
      eleConversionveto_[nEle_] = ConversionTools::hasMatchedConversion(*recoElectron, hConversions, beamSpot.position(), true, 2.0, 1e-6, 0);

      // PF isolation
      elePfChargedHadronDR03_[nEle_] = (*(*electronIsoValsDR03)[0])[recoEleRef];
      elePfNeutralHadronDR03_[nEle_] = (*(*electronIsoValsDR03)[1])[recoEleRef];
      elePfPhotonDR03_[nEle_]        = (*(*electronIsoValsDR03)[2])[recoEleRef];

      elePfChargedHadronDR04_[nEle_] = (*(*electronIsoValsDR04)[0])[recoEleRef];
      elePfNeutralHadronDR04_[nEle_] = (*(*electronIsoValsDR04)[1])[recoEleRef];
      elePfPhotonDR04_[nEle_]        = (*(*electronIsoValsDR04)[2])[recoEleRef];

      nEle_++;
    }

  // PF Candidates
  Handle<reco::PFCandidateCollection> pfCandidatesHandle;
  e.getByLabel(PFCandLabel_, pfCandidatesHandle);
  const PFCandidateCollection thePfColl = *(pfCandidatesHandle.product());

  // Photon
  PFIsolationEstimator isolator;
  isolator.initializePhotonIsolation(kTRUE);
  isolator.setConeSize(0.3);
  VertexRef myVtxRef(recVtxs, 0);

  const Candidate *phomom = 0;
  nPho_ = 0;
  const TriggerObjectMatch *phoTriggerMatch1(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVTEle8Mass50"));
  const TriggerObjectMatch *phoTriggerMatch2(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVTSC4Mass50"));
  const TriggerObjectMatch *phoTriggerMatch3(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoTSC17Mass50"));
  const TriggerObjectMatch *phoTriggerMatch4(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle17CaloIdVTCaloIsoVTTrkIdTTrkIsoVT"));
  const TriggerObjectMatch *phoTriggerMatch5(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle20CaloIdVTCaloIsoVTTrkIdTTrkIsoVT"));
  const TriggerObjectMatch *phoTriggerMatch6(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle32CaloIdTCaloIsoTTrkIdTTrkIsoT"));
  const TriggerObjectMatch *phoTriggerMatch7(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTEle8Mass50"));
  const TriggerObjectMatch *phoTriggerMatch8(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTSC4Mass50"));
  const TriggerObjectMatch *phoTriggerMatch9(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTSC17Mass50"));
  const TriggerObjectMatch *phoTriggerMatch10(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton20CaloIdVLIsoL"));
  const TriggerObjectMatch *phoTriggerMatch11(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton26Photon18"));
  const TriggerObjectMatch *phoTriggerMatch12(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton30CaloIdVL"));
  const TriggerObjectMatch *phoTriggerMatch13(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton30CaloIdVLIsoL"));
  const TriggerObjectMatch *phoTriggerMatch14(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton36Photon22"));
  const TriggerObjectMatch *phoTriggerMatch15(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton36CaloId10Iso50Photon22CaloId10Iso50"));
  const TriggerObjectMatch *phoTriggerMatch16(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton50CaloIdVL"));
  const TriggerObjectMatch *phoTriggerMatch17(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton50CaloIdVLIsoL"));
  const TriggerObjectMatch *phoTriggerMatch18(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton75CaloIdVL"));
  const TriggerObjectMatch *phoTriggerMatch19(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton75CaloIdVLIsoL"));
  const TriggerObjectMatch *phoTriggerMatch20(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton90CaloIdVL"));
  const TriggerObjectMatch *phoTriggerMatch21(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton90CaloIdVLIsoL"));
  const TriggerObjectMatch *phoTriggerMatch22(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton135"));
  const TriggerObjectMatch *phoTriggerMatch23(triggerEvent->triggerObjectMatchResult("photonTriggerMatchHLTPhoton150"));

  if ( photonHandle_.isValid() )
    for (View<pat::Photon>::const_iterator iPho = photonHandle_->begin(); iPho != photonHandle_->end(); ++iPho) {
      
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
      const TriggerObjectRef phoTrigRef17( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch17, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef18( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch18, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef19( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch19, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef20( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch20, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef21( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch21, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef22( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch22, e, *triggerEvent ) );
      const TriggerObjectRef phoTrigRef23( matchHelper.triggerMatchObject( phoBaseRef, phoTriggerMatch23, e, *triggerEvent ) );

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
      phoTrg_[nPho_][16] = (phoTrigRef17.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][17] = (phoTrigRef18.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][18] = (phoTrigRef19.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][19] = (phoTrigRef20.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][20] = (phoTrigRef21.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][21] = (phoTrigRef22.isAvailable()) ? 1 : -99;
      phoTrg_[nPho_][22] = (phoTrigRef23.isAvailable()) ? 1 : -99;

      phoIsPhoton_[nPho_] = iPho->isPhoton();
      phoE_[nPho_]   = iPho->energy();
      phoEt_[nPho_]  = iPho->et();
      phoPz_[nPho_]  = iPho->pz();
      phoEta_[nPho_] = iPho->eta();
      phoPhi_[nPho_] = iPho->phi();

      phoTrkIsoSolidDR03_[nPho_]  = iPho->trkSumPtSolidConeDR03();
      phoTrkIsoHollowDR03_[nPho_] = iPho->trkSumPtHollowConeDR03();
      phoEcalIsoDR03_[nPho_]      = iPho->ecalRecHitSumEtConeDR03();
      phoHcalIsoDR03_[nPho_]      = iPho->hcalTowerSumEtConeDR03();

      phoTrkIsoSolidDR04_[nPho_]  = iPho->trkSumPtSolidConeDR04();
      phoTrkIsoHollowDR04_[nPho_] = iPho->trkSumPtHollowConeDR04();
      phoEcalIsoDR04_[nPho_]      = iPho->ecalRecHitSumEtConeDR04();
      phoHcalIsoDR04_[nPho_]      = iPho->hcalTowerSumEtConeDR04();

      // New PF isolation
      edm::Ptr<reco::Candidate> recoPhoRef = iPho->originalObjectRef();
      const reco::Photon *recoPhoton = dynamic_cast<const reco::Photon *>(recoPhoRef.get());

      isolator.fGetIsolation(recoPhoton, &thePfColl, myVtxRef, recVtxs);
      phoPfChargedHadron_[nPho_] = isolator.getIsolationCharged();
      phoPfNeutralHadron_[nPho_] = isolator.getIsolationNeutral();
      phoPfPhoton_[nPho_]        = isolator.getIsolationPhoton();

      for (int i=0; i<150; i++) {
        phoEtVtx_[nPho_][i]  = -999.;
        phoEtaVtx_[nPho_][i] = -999.;
        phoPhiVtx_[nPho_][i] = -999.;
      }

      for (size_t i=0; i<recVtxs->size(); ++i) {
        if (!((*recVtxs)[i].isFake())) {

	  math::XYZVector direction = math::XYZVector(iPho->superCluster()->x() - (*recVtxs)[i].x(), iPho->superCluster()->y() - (*recVtxs)[i].y(), iPho->superCluster()->z() - (*recVtxs)[i].z());
          //math::XYZVector direction = math::XYZVector(iPho->caloPosition().x() - (*recVtxs)[i].x(), iPho->caloPosition().y() - (*recVtxs)[i].y(), iPho->caloPosition().z() - (*recVtxs)[i].z());
          math::XYZVector momentum = direction.unit() * iPho->energy();
	  const reco::Particle::LorentzVector newPho(momentum.x(), momentum.y(), momentum.z(), iPho->energy());

	  phoEtVtx_[nPho_][i]  = newPho.Pt();
	  phoEtaVtx_[nPho_][i] = newPho.Eta();
	  phoPhiVtx_[nPho_][i] = newPho.Phi();
        }
      }

      phoHoverE_[nPho_]        = iPho->hadronicOverEm();
      phoHoverEbc_[nPho_]      = iPho->hadTowOverEm();
      phoSigmaEtaEta_[nPho_]   = iPho->sigmaEtaEta();
      phoR9_[nPho_]            = iPho->r9();

      phoOverlap_[nPho_]      = (int) iPho->hasOverlaps("electrons");
      phohasPixelSeed_[nPho_] = (int) iPho->hasPixelSeed();
      phoElectronveto_[nPho_] = (int) ConversionTools::hasMatchedPromptElectron(recoPhoton->superCluster(), hElectrons, hConversions, beamSpot.position());
      
      phoIsPFID_[nPho_] = iPho->isPFlowPhoton();

      // where is photon ? (0: EB, 1: EE, 2: EBGap, 3: EEGap, 4: EBEEGap)
      phoPos_[nPho_] = -1;
      if (iPho->isEB() == true) phoPos_[nPho_] = 0;
      if (iPho->isEE() == true) phoPos_[nPho_] = 1;
      if (iPho->isEBGap() == true) phoPos_[nPho_] = 2;
      if (iPho->isEEGap() == true) phoPos_[nPho_] = 3;
      if (iPho->isEBEEGap() == true) phoPos_[nPho_] = 4;

      phoSeedTime_[nPho_] = -999.;
      phoSeedEnergy_[nPho_] = -999.;
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
        }
      } else if ( EEReducedRecHits.isValid() ) {
        EcalRecHitCollection::const_iterator eerhit = EEReducedRecHits->find(phoSeedDetId);
        if ( eerhit != EEReducedRecHits->end() ) { 
	   phoSeedTime_[nPho_] = eerhit->time(); 
	   phoSeedEnergy_[nPho_] = eerhit->energy();
	}
      }
      
      phoE1x5_[nPho_] = iPho->e1x5();
      phoE2x5_[nPho_] = iPho->e2x5();
      phoE3x3_[nPho_] = iPho->e3x3();
      phoE5x5_[nPho_] = iPho->e5x5();
 
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

      phoVtx_[nPho_][0] = iPho->vertex().x();
      phoVtx_[nPho_][1] = iPho->vertex().y();
      phoVtx_[nPho_][2] = iPho->vertex().z();

      nPho_++;
    }

  // muon trigger matching
  const TriggerObjectMatch * muTriggerMatch1( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu24" ) );
  const TriggerObjectMatch * muTriggerMatch2( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu30" ) );
  const TriggerObjectMatch * muTriggerMatch3( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu24eta2p1" ) );
  const TriggerObjectMatch * muTriggerMatch4( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu30eta2p1" ) );
  const TriggerObjectMatch * muTriggerMatch5( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTIsoMu34eta2p1" ) );
  const TriggerObjectMatch * muTriggerMatch6( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu17Mu8" ) );
  const TriggerObjectMatch * muTriggerMatch7( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu17TkMu8" ) );
  const TriggerObjectMatch * muTriggerMatch8( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu17forMu17Mu8" ) );
  const TriggerObjectMatch * muTriggerMatch9( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu17forMu17TkMu8" ) );
  const TriggerObjectMatch * muTriggerMatch10( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTMu8forMu17Mu8" ) );
  const TriggerObjectMatch * muTriggerMatch11( triggerEvent->triggerObjectMatchResult( "muonTriggerMatchHLTTkMu8forMu17TkMu8" ) );

  // Muon
  nMu_ = 0;
  const Candidate *mumom = 0;
  if( muonHandle_.isValid() ) {
    for (View<pat::Muon>::const_iterator iMu = muonHandle_->begin(); iMu != muonHandle_->end(); ++iMu) {

      if (! (iMu->isPFMuon() || iMu->isGlobalMuon() || iMu->isTrackerMuon())) continue;

      muIsPFMu_[nMu_] = iMu->isPFMuon();
      muIsGlobalMu_[nMu_] = iMu->isGlobalMuon();
      muIsTrackerMu_[nMu_] = iMu->isTrackerMuon();
      
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
      muTrg_[nMu_][10]= (muTrigRef11.isAvailable()) ? 1 : -99;

      for (int i=0; i<6; ++i) muID_[nMu_][i] = 0;
      if (iMu->isGood("AllArbitrated")==1) muID_[nMu_][0] = 1;
      if (iMu->isGood("GlobalMuonPromptTight")==1) muID_[nMu_][1] = 1;
      if (iMu->isGood("TMLastStationLoose")==1) muID_[nMu_][2] = 1;
      if (iMu->isGood("TMLastStationTight")==1) muID_[nMu_][3] = 1;
      if (iMu->isGood("TM2DCompatibilityLoose")==1) muID_[nMu_][4] = 1;
      if (iMu->isGood("TM2DCompatibilityTight")==1) muID_[nMu_][5] = 1;

      const reco::TrackRef trkr = iMu->globalTrack();
      if (trkr.isNull()) {
	muNumberOfValidMuonHits_[nMu_] = -99;
	muChi2NDF_[nMu_] = -99;
      } 
      else {
        muNumberOfValidMuonHits_[nMu_] = trkr->hitPattern().numberOfValidMuonHits();
  	muChi2NDF_[nMu_] = trkr->normalizedChi2();
      }

      const reco::TrackRef innertrkr = iMu->innerTrack();
      if (innertrkr.isNull()) {
        muPVD0_[nMu_] = - 99;
        muPVDz_[nMu_] = - 99;
        muGVD0_[nMu_] = - 99;
        muGVDz_[nMu_] = - 99;
	muNumberOfValidPixelHits_[nMu_] = -99;
	muNumberOfValidTrkHits_[nMu_] = -99;
	muNumberOfTrackerLayers_[nMu_] = -99;
      }
      else {
	muPVD0_[nMu_] = innertrkr->dxy((*recVtxs)[0].position());
        muPVDz_[nMu_] = innertrkr->dz((*recVtxs)[0].position());
	muGVD0_[nMu_] = innertrkr->dxy(gv);
        muGVDz_[nMu_] = innertrkr->dz(gv);
        muNumberOfValidPixelHits_[nMu_] = innertrkr->hitPattern().numberOfValidPixelHits();
        muNumberOfValidTrkHits_[nMu_]   = innertrkr->hitPattern().numberOfValidTrackerHits();
	muNumberOfTrackerLayers_[nMu_]  = innertrkr->hitPattern().trackerLayersWithMeasurement();
      }
      muNumberOfMatchedStations_[nMu_] = iMu->numberOfMatchedStations();

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

      muPfChargedHadron_[nMu_] = iMu->pfIsolationR04().sumChargedHadronPt;
      muPfNeutralHadron_[nMu_] = iMu->pfIsolationR04().sumNeutralHadronEt;
      muPfPhoton_[nMu_]        = iMu->pfIsolationR04().sumPhotonEt;
      muPfSumPUPt_[nMu_]       = iMu->pfIsolationR04().sumPUPt;

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

  // Jet
  // cout << "VgAnalyzerKit: produce: Jet..." << endl;
  const TriggerObjectMatch *jetTriggerMatch1(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTPFJet40"));
  const TriggerObjectMatch *jetTriggerMatch2(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTPFJet80"));
  const TriggerObjectMatch *jetTriggerMatch3(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTPFJet140"));
  const TriggerObjectMatch *jetTriggerMatch4(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTPFJet200"));
  const TriggerObjectMatch *jetTriggerMatch5(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTPFJet260"));
  const TriggerObjectMatch *jetTriggerMatch6(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTPFJet320"));
  const TriggerObjectMatch *jetTriggerMatch7(triggerEvent->triggerObjectMatchResult("jetTriggerMatchHLTPFJet400"));

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

	  jetTrg_[nJet_][0] = (jetTrigRef1.isAvailable()) ? 1 : -99;
	  jetTrg_[nJet_][1] = (jetTrigRef2.isAvailable()) ? 1 : -99;
	  jetTrg_[nJet_][2] = (jetTrigRef3.isAvailable()) ? 1 : -99;
	  jetTrg_[nJet_][3] = (jetTrigRef4.isAvailable()) ? 1 : -99;
	  jetTrg_[nJet_][4] = (jetTrigRef5.isAvailable()) ? 1 : -99;
	  jetTrg_[nJet_][5] = (jetTrigRef6.isAvailable()) ? 1 : -99;
	  jetTrg_[nJet_][6] = (jetTrigRef7.isAvailable()) ? 1 : -99;
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
	    if (fabs((trackref)->vz() - vtx_[0][2]) < 0.2) {
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

  // SC information
  if (doStoreSCs_ == true) {
    nEBSC_ = 0;
    nEESC_ = 0;
    edm::Handle<reco::SuperClusterCollection> BarrelSuperClusters;
    if (e.getByLabel("correctedHybridSuperClusters", BarrelSuperClusters)) {
      for(reco::SuperClusterCollection::const_iterator aClus = BarrelSuperClusters->begin(); aClus != BarrelSuperClusters->end(); aClus++) {

	if (aClus->energy()/cosh(aClus->position().eta()) < 5) continue;

        scEBEn_[nEBSC_]  = aClus->energy();
	scEBEt_[nEBSC_]  = aClus->energy()/cosh(aClus->position().eta());
	scEBEta_[nEBSC_] = aClus->position().eta();
	scEBPhi_[nEBSC_] = aClus->position().phi();
        
	nEBSC_ += 1;
      }
    }
    edm::Handle<reco::SuperClusterCollection> EndcapSuperClusters;
    if (e.getByLabel("correctedMulti5x5SuperClustersWithPreshower", EndcapSuperClusters)) {
      for(reco::SuperClusterCollection::const_iterator aClus = EndcapSuperClusters->begin(); aClus != EndcapSuperClusters->end(); aClus++) {

	if (aClus->energy()/cosh(aClus->position().eta()) < 5) continue;

        scEEEn_[nEESC_]  = aClus->energy();
	scEEEt_[nEESC_]  = aClus->energy()/cosh(aClus->position().eta());
	scEEEta_[nEESC_] = aClus->position().eta();
	scEEPhi_[nEESC_] = aClus->position().phi();

        nEESC_ += 1;
      }
    }
  }

  if (doSkim_ == true) {
    if (skimed_HLT == true) {
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

float VgAnalyzerKit::getGenCHIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax)
{
  const Float_t ptMin = 0.0;
  Float_t genIsoSum = 0.0;
  if(!doGenParticles_)return genIsoSum;
  if(!handle.isValid())return genIsoSum;

  for (reco::GenParticleCollection::const_iterator it_gen=handle->begin(); it_gen!=handle->end(); it_gen++){

    if(it_gen == thisPho)continue;      // can't be the original photon
    if(it_gen->status() != 1)continue;    // need to be a stable particle
    if(thisPho->collisionId() != it_gen->collisionId()) continue; // has to come from the same collision
    Int_t pdgCode = abs(it_gen->pdgId());
    if(pdgCode != 211) continue;

    Float_t pt = it_gen->pt();
    if(pt < ptMin) continue; // pass a minimum pt threshold, default 0

    Float_t dR = reco::deltaR(thisPho->momentum(), it_gen->momentum());
    if(dR > dRMax) continue; // within deltaR cone
    genIsoSum += pt;

  }// end of loop over gen particles

  return genIsoSum;
}

float VgAnalyzerKit::getGenNHIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax)
{
  const Float_t ptMin = 0.0;
  Float_t genIsoSum = 0.0;
  if(!doGenParticles_)return genIsoSum;
  if(!handle.isValid())return genIsoSum;

  for (reco::GenParticleCollection::const_iterator it_gen=handle->begin(); it_gen!=handle->end(); it_gen++){

    if(it_gen == thisPho)continue;      // can't be the original photon
    if(it_gen->status() != 1)continue;    // need to be a stable particle
    if(thisPho->collisionId() != it_gen->collisionId()) continue; // has to come from the same collision
    Int_t pdgCode = abs(it_gen->pdgId());
    if(pdgCode != 130) continue;

    Float_t pt = it_gen->pt();
    if(pt < ptMin) continue; // pass a minimum pt threshold, default 0

    Float_t dR = reco::deltaR(thisPho->momentum(), it_gen->momentum());
    if(dR > dRMax) continue; // within deltaR cone
    genIsoSum += pt;

  }// end of loop over gen particles

  return genIsoSum;
}

float VgAnalyzerKit::getGenPhIso(edm::Handle<reco::GenParticleCollection> handle, reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax)
{
  const Float_t ptMin = 0.0;
  Float_t genIsoSum = 0.0;
  if(!doGenParticles_)return genIsoSum;
  if(!handle.isValid())return genIsoSum;

  for (reco::GenParticleCollection::const_iterator it_gen=handle->begin(); it_gen!=handle->end(); it_gen++){

    if(it_gen == thisPho)continue;      // can't be the original photon
    if(it_gen->status() != 1)continue;    // need to be a stable particle
    if(thisPho->collisionId() != it_gen->collisionId()) continue; // has to come from the same collision
    Int_t pdgCode = abs(it_gen->pdgId());
    if(pdgCode != 22) continue;

    Float_t pt = it_gen->pt();
    if(pt < ptMin) continue; // pass a minimum pt threshold, default 0

    Float_t dR = reco::deltaR(thisPho->momentum(), it_gen->momentum());
    if(dR > dRMax) continue; // within deltaR cone
    genIsoSum += pt;

  }// end of loop over gen particles

  return genIsoSum;
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
