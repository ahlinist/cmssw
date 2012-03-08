#include "TauAnalysis/CandidateTools/plugins/SVfitNtupleProducer.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/TauReco/interface/PFTauDecayMode.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/neuralMtautauAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include <TMath.h>
#include <TString.h>
#include <TVectorD.h>

typedef edm::View<reco::Candidate> CandidateView;
typedef edm::View<reco::MET> MEtView;

int SVfitNtupleProducer::verbosity_ = 0;

SVfitNtupleProducer::SVfitNtupleProducer(const edm::ParameterSet& cfg) 
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    ntuple_(0)
{
  srcGenTauPair_ = cfg.getParameter<edm::InputTag>("srcGenTauPair");
  srcGenMEt_ = cfg.getParameter<edm::InputTag>("srcGenMEt");
  srcGenTaus_ = cfg.getParameter<edm::InputTag>("srcGenTaus");
  srcGenParticles_ = cfg.getParameter<edm::InputTag>("srcGenParticles");
  
  srcGenLeg1_ = cfg.getParameter<edm::InputTag>("srcGenLeg1");
  srcGenLeg2_ = cfg.getParameter<edm::InputTag>("srcGenLeg2");

  srcRecLeg1_ = cfg.getParameter<edm::InputTag>("srcRecLeg1");
  srcRecLeg2_ = cfg.getParameter<edm::InputTag>("srcRecLeg2");

  srcRecMEt_ = cfg.getParameter<edm::InputTag>("srcRecMEt");
  
  srcPFMEtCovMatrix_ = cfg.getParameter<edm::InputTag>("srcPFMEtCovMatrix");
}

SVfitNtupleProducer::~SVfitNtupleProducer()
{
// nothing to be done yet...
}

void SVfitNtupleProducer::beginJob()
{
//--- create TTree
  edm::Service<TFileService> fs;
  ntuple_ = fs->make<TTree>("svFitNtuple", "svFitNtuple");

//--- add branches storing quantities for neural-net training
  addBranch_EnPxPyPz("genTau1");
  addBranchI("genTauDecayMode1");
  addBranch_EnPxPyPz("genVis1");
  addBranch_EnPxPyPz("genNu1");

  addBranch_EnPxPyPz("genTau2");
  addBranchI("genTauDecayMode2");
  addBranch_EnPxPyPz("genVis2");
  addBranch_EnPxPyPz("genNu2");

  addBranchI("recTauDecayMode1");
  addBranch_EnPxPyPz("recVis1");

  addBranchI("recTauDecayMode2");
  addBranch_EnPxPyPz("recVis2");

  addBranch_PxPy("recMEt");
  addBranch_Cov("recMEt");

  addBranch_PxPy("genMEt");
  addBranchF("genMtautau");
}

namespace
{
  const reco::Candidate* findRecToGenMatch(const edm::View<reco::Candidate>& recCands, 
					   const reco::Candidate::LorentzVector& genParticleP4)
  {
    const reco::Candidate* recCand_matched = 0;

    double dRmatch = 0.3;
    for ( CandidateView::const_iterator recCand = recCands.begin(); 
	  recCand != recCands.end(); ++recCand ) {
      double dR = deltaR(recCand->p4(), genParticleP4);
      if ( dR < dRmatch ) {
	recCand_matched = &(*recCand);
	dRmatch = dR;
      }      
    }

    return recCand_matched;
  }

  double square(double x)
  {
    return x*x;
  }

  int getGenTauDecayModeIdx(const std::string& tauDecayMode)
  {
    int tauDecayModeIdx = -1;
    if      ( tauDecayMode == "electron"        ) tauDecayModeIdx = reco::PFTauDecayMode::tauDecaysElectron;
    else if ( tauDecayMode == "muon"            ) tauDecayModeIdx = reco::PFTauDecayMode::tauDecayMuon;
    else if ( tauDecayMode == "oneProng0Pi0"    ) tauDecayModeIdx = reco::PFTauDecayMode::tauDecay1ChargedPion0PiZero;
    else if ( tauDecayMode == "oneProng1Pi0"    ) tauDecayModeIdx = reco::PFTauDecayMode::tauDecay1ChargedPion1PiZero;
    else if ( tauDecayMode == "oneProng2Pi0"    ) tauDecayModeIdx = reco::PFTauDecayMode::tauDecay1ChargedPion2PiZero;
    else if ( tauDecayMode == "oneProngOther"   ) tauDecayModeIdx = reco::PFTauDecayMode::tauDecayOther;
    else if ( tauDecayMode == "threeProng0Pi0"  ) tauDecayModeIdx = reco::PFTauDecayMode::tauDecay3ChargedPion0PiZero;
    else if ( tauDecayMode == "threeProng1Pi0"  ) tauDecayModeIdx = reco::PFTauDecayMode::tauDecay3ChargedPion1PiZero;
    else if ( tauDecayMode == "threeProngOther" ) tauDecayModeIdx = reco::PFTauDecayMode::tauDecayOther;
    else                                          tauDecayModeIdx = reco::PFTauDecayMode::tauDecayOther;
    return tauDecayModeIdx;
  }

  int getRecTauDecayMode(const reco::Candidate* candidate)
  {
    int tauDecayMode = -1;
    if      ( dynamic_cast<const pat::Electron*>(candidate)     ||
	      dynamic_cast<const reco::GsfElectron*>(candidate) ) 
      tauDecayMode = reco::PFTauDecayMode::tauDecaysElectron;
    else if ( dynamic_cast<const pat::Muon*>(candidate)         ||
	      dynamic_cast<const reco::Muon*>(candidate)        ) 
      tauDecayMode = reco::PFTauDecayMode::tauDecayMuon;
    else if ( dynamic_cast<const pat::Tau*>(candidate)          ) 
      tauDecayMode = (dynamic_cast<const pat::Tau*>(candidate))->decayMode();
    else if ( dynamic_cast<const reco::PFTau*>(candidate)       ) 
      tauDecayMode = (dynamic_cast<const reco::PFTau*>(candidate))->decayMode();
    else 
      tauDecayMode = reco::PFTauDecayMode::tauDecayOther;
    return tauDecayMode;
  }
}

void SVfitNtupleProducer::produce(edm::Event& evt, const edm::EventSetup& es) 
{
  edm::Handle<CandidateView> genTauPair;
  evt.getByLabel(srcGenTauPair_, genTauPair);
  if ( !genTauPair->size() == 1 ) 
    throw cms::Exception("InvalidData") 
      << "Failed to find unique gen. Tau-pair object !!\n";
  double genMtautau = genTauPair->front().mass();
  
  edm::Handle<MEtView> genMEt;
  evt.getByLabel(srcGenMEt_, genMEt);
  if ( !genMEt->size() == 1 ) 
    throw cms::Exception("InvalidData") 
      << "Failed to find unique gen. MET object !!\n";
  const reco::Candidate::LorentzVector& genMEtP4 = genMEt->front().p4();
  
  edm::Handle<CandidateView> genLeg1;
  evt.getByLabel(srcGenLeg1_, genLeg1);
  if ( !genLeg1->size() >= 1 ) return;
  const reco::Candidate::LorentzVector& genLeg1P4 = genLeg1->front().p4();

  edm::Handle<CandidateView> recCandsLeg1;
  evt.getByLabel(srcRecLeg1_, recCandsLeg1);
  const reco::Candidate* recLeg1 = findRecToGenMatch(*recCandsLeg1, genLeg1P4);
  int recTauDecayMode1 = -1;
  reco::Candidate::LorentzVector recVis1P4;
  if ( recLeg1 ) {
    recTauDecayMode1 = getRecTauDecayMode(recLeg1);
    recVis1P4 = recLeg1->p4();
  }
  setValueI("recTauDecayMode1", recTauDecayMode1);
  setValue_EnPxPyPz("recVis1", recVis1P4);

  edm::Handle<CandidateView> genLeg2;
  evt.getByLabel(srcGenLeg2_, genLeg2);
  if ( !genLeg2->size() >= 1 ) return;
  const reco::Candidate::LorentzVector& genLeg2P4 = genLeg2->front().p4();
  
  edm::Handle<CandidateView> recCandsLeg2;
  evt.getByLabel(srcRecLeg2_, recCandsLeg2);
  const reco::Candidate* recLeg2 = findRecToGenMatch(*recCandsLeg2, genLeg2P4);
  int recTauDecayMode2 = -1;
  reco::Candidate::LorentzVector recVis2P4;
  if ( recLeg2 ) {
    recTauDecayMode2 = getRecTauDecayMode(recLeg2);
    recVis2P4 = recLeg2->p4();
  }
  setValueI("recTauDecayMode2", recTauDecayMode2);
  setValue_EnPxPyPz("recVis2", recVis2P4);

  edm::Handle<MEtView> recMEt;
  evt.getByLabel(srcRecMEt_, recMEt);
  if ( !recMEt->size() == 1 ) 
    throw cms::Exception("InvalidData") 
      << "Failed to find unique MET object !!\n";
  const reco::Candidate::LorentzVector& recMEtP4 = recMEt->front().p4();

  edm::Handle<PFMEtSignCovMatrix> pfMEtSignCovMatrix;
  evt.getByLabel(srcPFMEtCovMatrix_, pfMEtSignCovMatrix);

  setValue_PxPy("recMEt", recMEtP4);
  setValue_Cov("recMEt", *pfMEtSignCovMatrix);
  
  setValue_PxPy("genMEt", genMEtP4);
  setValueF("genMtautau", genMtautau);

  edm::Handle<reco::GenParticleCollection> genTaus;
  evt.getByLabel(srcGenTaus_, genTaus);

  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByLabel(srcGenParticles_, genParticles);

  const reco::GenParticle* genTauPlus  = 0;
  const reco::GenParticle* genTauMinus = 0;
  for ( reco::GenParticleCollection::const_iterator genTau = genTaus->begin();
	genTau != genTaus->end(); ++genTau ) {
    // CV: find genParticle corresponding to genTau;
    //     this is neccessary in order to get the mother and daughter references correct
    //    (the genTaus do not have valid daughter references for some reason...)
    for ( reco::GenParticleCollection::const_iterator genParticle = genParticles->begin();
	  genParticle != genParticles->end(); ++genParticle ) {
      if ( genParticle->pdgId() == genTau->pdgId() ) {
	double dR = deltaR(genParticle->p4(), genTau->p4());
	if ( dR < 1.e-2 ) {
	  if      ( genParticle->charge() == +1 ) genTauPlus  = &(*genParticle);
	  else if ( genParticle->charge() == -1 ) genTauMinus = &(*genParticle);
	}
      }
    }
  }
  if ( !(genTauPlus && genTauMinus) )
    throw cms::Exception("InvalidData") 
      << "Failed to find gen. Tau lepton pair !!\n";

  reco::Candidate::LorentzVector genVisPlusP4  = getVisMomentum(genTauPlus);
  reco::Candidate::LorentzVector genVisMinusP4 = getVisMomentum(genTauMinus);
  double dRplusMapsTo1MinusMapsTo2 = square(deltaR(genVisPlusP4, genLeg1P4)) + square(deltaR(genVisMinusP4, genLeg2P4));
  double dRplusMapsTo2MinusMapsTo1 = square(deltaR(genVisPlusP4, genLeg2P4)) + square(deltaR(genVisMinusP4, genLeg1P4));

  const reco::GenParticle* genTau1 = 0;
  const reco::GenParticle* genTau2 = 0;
  if ( dRplusMapsTo1MinusMapsTo2 < dRplusMapsTo2MinusMapsTo1 ) {
    genTau1 = genTauPlus;
    genTau2 = genTauMinus;
  } else {
    genTau1 = genTauMinus;
    genTau2 = genTauPlus;
  }
  assert(genTau1 && genTau2);

  reco::Candidate::LorentzVector genTau1P4 = genTau1->p4(); 
  reco::Candidate::LorentzVector genVis1P4 = getVisMomentum(genTau1);
  reco::Candidate::LorentzVector genNu1P4  = getInvisMomentum(genTau1);
  std::string genTauDecayMode1_string = getGenTauDecayMode(genTau1);
  int genTauDecayMode1 = getGenTauDecayModeIdx(genTauDecayMode1_string);

  reco::Candidate::LorentzVector genTau2P4 = genTau2->p4();  
  reco::Candidate::LorentzVector genVis2P4 = getVisMomentum(genTau2);
  reco::Candidate::LorentzVector genNu2P4  = getInvisMomentum(genTau2);
  std::string genTauDecayMode2_string = getGenTauDecayMode(genTau2);
  int genTauDecayMode2 = getGenTauDecayModeIdx(genTauDecayMode2_string);

  if ( verbosity_ ) {
    std::cout << "genTau1: E = " << genTau1P4.E() << "," 
	      << " Px = " << genTau1P4.Px() << ", Py = " << genTau1P4.Py() << ", Pz = " << genTau1P4.Pz() << std::endl;
    std::cout << "genVis1: E = " << genVis1P4.E() << "," 
	      << " Px = " << genVis1P4.Px() << ", Py = " << genVis1P4.Py() << ", Pz = " << genVis1P4.Pz() << std::endl;
    std::cout << "genNu1: E = " << genNu1P4.E() << "," 
	      << " Px = " << genNu1P4.Px() << ", Py = " << genNu1P4.Py() << ", Pz = " << genNu1P4.Pz() << std::endl;
    std::cout << "genTau2: E = " << genTau2P4.E() << "," 
	      << " Px = " << genTau2P4.Px() << ", Py = " << genTau2P4.Py() << ", Pz = " << genTau2P4.Pz() << std::endl;
    std::cout << "genVis2: E = " << genVis2P4.E() << "," 
	      << " Px = " << genVis2P4.Px() << ", Py = " << genVis2P4.Py() << ", Pz = " << genVis2P4.Pz() << std::endl;
    std::cout << "genNu2: E = " << genNu2P4.E() << "," 
	      << " Px = " << genNu2P4.Px() << ", Py = " << genNu2P4.Py() << ", Pz = " << genNu2P4.Pz() << std::endl;
  }
  //assert(TMath::Abs((genTau1P4 + genTau2P4).mass() - genMtautau) < 1.e-1);
  //assert(TMath::Abs((genVis1P4 + genNu1P4).mass() - genTau1P4.mass()) < 1.e-2);
  //assert(TMath::Abs((genVis2P4 + genNu2P4).mass() - genTau2P4.mass()) < 1.e-2);

  setValue_EnPxPyPz("genTau1", genTau1P4);
  setValue_EnPxPyPz("genVis1", genVis1P4);
  setValue_EnPxPyPz("genNu1",  genNu1P4);
  setValueI("genTauDecayMode1", genTauDecayMode1);

  setValue_EnPxPyPz("genTau2", genTau2P4);
  setValue_EnPxPyPz("genVis2", genVis2P4);
  setValue_EnPxPyPz("genNu2",  genNu2P4);
  setValueI("genTauDecayMode2", genTauDecayMode2);

//--- finally, fill all computed quantities into TTree
  assert(ntuple_);
  ntuple_->Fill();
}

void SVfitNtupleProducer::addBranchF(const std::string& name) 
{
  assert(branches_.count(name) == 0);
  std::string name_and_format = name + "/F";
  ntuple_->Branch(name.c_str(), &branches_[name].valueF_, name_and_format.c_str());
}

void SVfitNtupleProducer::addBranchI(const std::string& name) 
{
  assert(branches_.count(name) == 0);
  std::string name_and_format = name + "/I";
  ntuple_->Branch(name.c_str(), &branches_[name].valueI_, name_and_format.c_str());
}

void SVfitNtupleProducer::printBranches(std::ostream& stream)
{
  stream << "<SVfitNtupleProducer::printBranches>:" << std::endl;
  stream << " registered branches for module = " << moduleLabel_ << std::endl;
  for ( branchMap::const_iterator branch = branches_.begin();
	branch != branches_.end(); ++branch ) {
    stream << " " << branch->first << std::endl;
  }
  stream << std::endl;
}

void SVfitNtupleProducer::setValueF(const std::string& name, double value) 
{
  if ( verbosity_ ) std::cout << "branch = " << name << ": value = " << value << std::endl;
  branchMap::iterator branch = branches_.find(name);
  if ( branch != branches_.end() ) {
    branch->second.valueF_ = value;
  } else {
    throw cms::Exception("InvalidParameter") 
      << "No branch with name = " << name << " defined !!\n";
  }
}

void SVfitNtupleProducer::setValueI(const std::string& name, int value) 
{
  if ( verbosity_ ) std::cout << "branch = " << name << ": value = " << value << std::endl;
  branchMap::iterator branch = branches_.find(name);
  if ( branch != branches_.end() ) {
    branch->second.valueI_ = value;
  } else {
    throw cms::Exception("InvalidParameter") 
      << "No branch with name = " << name << " defined !!\n";
  }
}

//
//-------------------------------------------------------------------------------
//

void SVfitNtupleProducer::addBranch_EnPxPyPz(const std::string& name) 
{
  addBranchF(std::string(name).append("En"));
  addBranchF(std::string(name).append("Px"));
  addBranchF(std::string(name).append("Py"));
  addBranchF(std::string(name).append("Pz"));
  addBranchF(std::string(name).append("M"));
}

void SVfitNtupleProducer::addBranch_PxPy(const std::string& name) 
{
  addBranchF(std::string(name).append("Px"));
  addBranchF(std::string(name).append("Py"));
}

void SVfitNtupleProducer::addBranch_Cov(const std::string& name) 
{
  addBranchF(std::string(name).append("Vxx"));
  addBranchF(std::string(name).append("Vxy"));
  addBranchF(std::string(name).append("Vyy"));
  addBranchF(std::string(name).append("SigmaX"));
  addBranchF(std::string(name).append("SigmaY"));
  addBranchF(std::string(name).append("CorrXY"));
}

//
//-------------------------------------------------------------------------------
//

void SVfitNtupleProducer::setValue_EnPxPyPz(const std::string& name, const reco::Candidate::LorentzVector& p4)
{
  setValueF(std::string(name).append("En"), p4.E());
  setValueF(std::string(name).append("Px"), p4.px());
  setValueF(std::string(name).append("Py"), p4.py());
  setValueF(std::string(name).append("Pz"), p4.pz());
  setValueF(std::string(name).append("M"), p4.M());
}

void SVfitNtupleProducer::setValue_PxPy(const std::string& name, const reco::Candidate::LorentzVector& p4)
{
  setValueF(std::string(name).append("Px"), p4.px());
  setValueF(std::string(name).append("Py"), p4.py());
}

void SVfitNtupleProducer::setValue_Cov(const std::string& name, const TMatrixD& cov)
{
  assert(cov.GetNrows() == 2);
  assert(cov.GetNcols() == 2);
  setValueF(std::string(name).append("Vxx"), cov(0,0));
  setValueF(std::string(name).append("Vxy"), cov(0,1));
  setValueF(std::string(name).append("Vyy"), cov(1,1));
  double sigmaX = TMath::Sqrt(TMath::Abs(cov(0,0)));
  double sigmaY = TMath::Sqrt(TMath::Abs(cov(1,1)));
  const double epsilon = 1.e-9;
  double corrXY = ( (sigmaX*sigmaY) > epsilon ) ?
    cov(0,1)/(sigmaX*sigmaY) : 0.;
  setValueF(std::string(name).append("SigmaX"), sigmaX);
  setValueF(std::string(name).append("SigmaY"), sigmaY);
  setValueF(std::string(name).append("CorrXY"), corrXY);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SVfitNtupleProducer);
