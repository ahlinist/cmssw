#include "TauAnalysis/CandidateTools/plugins/NeuralMtautauNtupleProducer.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneLikelihood.h"
#include "TauAnalysis/CandidateTools/interface/neuralMtautauAuxFunctions.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include <TMath.h>
#include <TString.h>
#include <TVectorD.h>

typedef edm::View<reco::Candidate> CandidateView;
typedef edm::View<reco::MET> MEtView;

int NeuralMtautauNtupleProducer::verbosity_ = 0;

using namespace NSVfitStandalone;

NeuralMtautauNtupleProducer::NeuralMtautauNtupleProducer(const edm::ParameterSet& cfg) 
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    ntuple_(0)
{
  srcGenTauPair_ = cfg.getParameter<edm::InputTag>("srcGenTauPair");
  srcGenMEt_ = cfg.getParameter<edm::InputTag>("srcGenMEt");
  
  srcRecPFJets_ = cfg.getParameter<edm::InputTag>("srcRecPFJets");
  srcRecPFCandidatesNotWithinJets_ = cfg.getParameter<edm::InputTag>("srcRecPFCandidatesNotWithinJets");
  srcRecLeg1_ = cfg.getParameter<edm::InputTag>("srcRecLeg1");
  srcRecLeg2_ = cfg.getParameter<edm::InputTag>("srcRecLeg2");
  srcRecMEt_ = cfg.getParameter<edm::InputTag>("srcRecMEt");
  
  srcPFMEtCovMatrix_ = cfg.getParameter<edm::InputTag>("srcPFMEtCovMatrix");
}

NeuralMtautauNtupleProducer::~NeuralMtautauNtupleProducer()
{
// nothing to be done yet...
}

void NeuralMtautauNtupleProducer::beginJob()
{
//--- create TTree
  edm::Service<TFileService> fs;
  ntuple_ = fs->make<TTree>("neuralMtautauNtuple", "neuralMtautauNtuple");

//--- add branches storing quantities for neural-net training
  addBranch_EnPxPyPz("recLeg1");

  addBranch_EnPxPyPz("recLeg2");

  addBranch("recDPhi12");
  addBranch("recDTheta12");
  addBranch("recDAlpha12");

  addBranch_PxPy("recMEt");
  addBranch_Cov("recMEt");

  addBranch("recSVfitMtautau");

  addBranch_PxPy("genMEt");
  addBranch("genMtautau");
}

namespace
{
  double logGaussianNd(const TVectorD& vec, const TMatrixD& cov)
  {
    assert(vec.GetNoElements() == 2);
    assert(cov.GetNrows() == 2);
    assert(cov.GetNcols() == 2);
    if ( cov.Determinant() != 0. ) {
      TMatrixD covInverse = cov;
      covInverse.Invert();
      return 0.5*(vec(0)*(covInverse(0,0)*vec(0) + covInverse(0,1)*vec(1)) + vec(1)*(covInverse(1,0)*vec(0) + covInverse(1,1)*vec(1)));
    } else {
      edm::LogWarning("logGaussianNd")
	<< "Inversion of PFMEt covariance matrix failed, det = " << cov.Determinant() << " !!";
      return 0.;
    }
  }
}

void NeuralMtautauNtupleProducer::produce(edm::Event& evt, const edm::EventSetup& es) 
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

  edm::Handle<CandidateView> recLeg1;
  evt.getByLabel(srcRecLeg1_, recLeg1);
  if ( !recLeg1->size() >= 1 ) return;
  const reco::Candidate::LorentzVector& recLeg1P4 = recLeg1->front().p4();

  edm::Handle<CandidateView> recLeg2;
  evt.getByLabel(srcRecLeg2_, recLeg2);
  if ( !recLeg2->size() >= 1 ) return;
  const reco::Candidate::LorentzVector& recLeg2P4 = recLeg2->front().p4();

  edm::Handle<MEtView> recMEt;
  evt.getByLabel(srcRecMEt_, recMEt);
  if ( !recMEt->size() == 1 ) 
    throw cms::Exception("InvalidData") 
      << "Failed to find unique MET object !!\n";
  const reco::Candidate::LorentzVector& recMEtP4 = recMEt->front().p4();

  edm::Handle<PFMEtSignCovMatrix> pfMEtSignCovMatrix;
  evt.getByLabel(srcPFMEtCovMatrix_, pfMEtSignCovMatrix);

//--- compute axis 'zeta' bisecting angle between visible decay products of the two tau leptons
  double zetaPhi = compZetaPhi(recLeg1P4, recLeg2P4);

  if ( verbosity_ ) {
    std::cout << "recLeg1: px = " << recLeg1P4.px() << ", py = " << recLeg1P4.py() << "," 
	      << " phi = " << recLeg1P4.phi() << std::endl;
    std::cout << "recLeg2: px = " << recLeg2P4.px() << ", py = " << recLeg2P4.py() << "," 
	      << " phi = " << recLeg2P4.phi() << std::endl;      
    std::cout << "--> zetaPhi = " << zetaPhi << std::endl;

    TVectorD recMEtVec(2);
    recMEtVec(0) = recMEtP4.px();
    recMEtVec(1) = recMEtP4.py();
    std::cout << "recMEt: px = " << recMEtP4.px() << ", py = " << recMEtP4.py() << "," 
	      << " pt = " << recMEtP4.pt() << ", phi = " << recMEtP4.phi() << std::endl;
    std::cout << "pfMEtSignCovMatrix:" << std::endl;
    std::cout << " | " 
	      << std::setw(10) << (*pfMEtSignCovMatrix)(0,0) 
	      << std::setw(10) << (*pfMEtSignCovMatrix)(0,1) 
	      << "|" << std::endl;
    std::cout << " | " 
	      << std::setw(10) << (*pfMEtSignCovMatrix)(1,0) 
	      << std::setw(10) << (*pfMEtSignCovMatrix)(1,1) 
	      << "|" << std::endl;
    std::cout << "logGaussianNd (frame 1) = " << logGaussianNd(recMEtVec, *pfMEtSignCovMatrix) << std::endl;
    
    reco::Candidate::LorentzVector recMEtP4inZetaFrame = compP4inZetaFrame(recMEtP4, zetaPhi);
    TVectorD recMEtVecInZetaFrame(2);
    recMEtVecInZetaFrame(0) = recMEtP4inZetaFrame.px();
    recMEtVecInZetaFrame(1) = recMEtP4inZetaFrame.py();
    std::cout << "recMEtInZetaFrame: px = " << recMEtP4inZetaFrame.px() << ", py = " << recMEtP4inZetaFrame.py() << ","
	      << " pt = " << recMEtP4inZetaFrame.pt() << ", phi = " << recMEtP4inZetaFrame.phi() << std::endl;
    TMatrixD pfMEtSignCovMatrixInZetaFrame = compCovMatrixInZetaFrame(*pfMEtSignCovMatrix, zetaPhi);
    std::cout << "pfMEtSignCovMatrixInZetaFrame:" << std::endl;
    std::cout << " | " 
	      << std::setw(10) << pfMEtSignCovMatrixInZetaFrame(0,0) 
	      << std::setw(10) << pfMEtSignCovMatrixInZetaFrame(0,1) 
	      << "|" << std::endl;
    std::cout << " | " 
	      << std::setw(10) << pfMEtSignCovMatrixInZetaFrame(1,0) 
	      << std::setw(10) << pfMEtSignCovMatrixInZetaFrame(1,1) 
	      << "|" << std::endl;
    std::cout << "logGaussianNd (frame 2) = " << logGaussianNd(recMEtVecInZetaFrame, pfMEtSignCovMatrixInZetaFrame) << std::endl;
  }

  setValue_EnPxPyPz("recLeg1", recLeg1P4, zetaPhi);
  
  setValue_EnPxPyPz("recLeg2", recLeg2P4, zetaPhi);
  
  double recDPhi12       = TMath::ACos(TMath::Cos(recLeg1P4.phi() - recLeg2P4.phi()));
  setValue("recDPhi12", recDPhi12);
  double recDTheta12     = TMath::Abs(recLeg1P4.theta() - recLeg2P4.theta());
  setValue("recDTheta12", recDTheta12);
  double recLeg1cosPhi   = TMath::Cos(recLeg1P4.phi());
  double recLeg1sinPhi   = TMath::Sin(recLeg1P4.phi());
  double recLeg1cosTheta = TMath::Cos(recLeg1P4.theta());
  double recLeg1sinTheta = TMath::Sin(recLeg1P4.theta());
  double recLeg2cosPhi   = TMath::Cos(recLeg2P4.phi());
  double recLeg2sinPhi   = TMath::Sin(recLeg2P4.phi());
  double recLeg2cosTheta = TMath::Cos(recLeg2P4.theta());
  double recLeg2sinTheta = TMath::Sin(recLeg2P4.theta());
  double dotProduct12    = (recLeg1cosPhi*recLeg2cosPhi + recLeg1sinPhi*recLeg2sinPhi)*recLeg1sinTheta*recLeg2sinTheta 
                          + recLeg1cosTheta*recLeg2cosTheta;
  double recDAlpha12     = TMath::ACos(dotProduct12);
  setValue("recDAlpha12", recDAlpha12);

  setValue_PxPy("recMEt", recMEtP4, zetaPhi);
  setValue_Cov("recMEt", *pfMEtSignCovMatrix, zetaPhi);
  
  setValue_PxPy("genMEt", genMEtP4, zetaPhi);
  setValue("genMtautau", genMtautau);

  // reconstruct mTauTau using SVfit for comparisson
  //
  // WARNING: assumes that srcLeg1 (srcLeg2) refers to leptonic (hadronic) tau decays
  //
  std::vector<MeasuredTauLepton> tauDecayProduts;
  tauDecayProduts.push_back(MeasuredTauLepton(kLepDecay, recLeg1P4));
  tauDecayProduts.push_back(MeasuredTauLepton(kHadDecay, recLeg2P4));
  Vector recMEtP3(recMEtP4.px(), recMEtP4.py(), recMEtP4.pz());
  NSVfitStandaloneAlgorithm svFit(tauDecayProduts, recMEtP3, *pfMEtSignCovMatrix);
  svFit.fit();
  double svFitMtautau = ( svFit.isValidSolution() ) ?
    svFit.mass() : -1.;
  setValue("recSVfitMtautau", svFitMtautau);

//--- finally, fill all computed quantities into TTree
  assert(ntuple_);
  ntuple_->Fill();
}

void NeuralMtautauNtupleProducer::addBranch(const std::string& name) 
{
  assert(branches_.count(name) == 0);
  std::string name_and_format = name + "/F";
  ntuple_->Branch(name.c_str(), &branches_[name], name_and_format.c_str());
}

void NeuralMtautauNtupleProducer::printBranches(std::ostream& stream)
{
  stream << "<NeuralMtautauNtupleProducer::printBranches>:" << std::endl;
  stream << " registered branches for module = " << moduleLabel_ << std::endl;
  for ( branchMap::const_iterator branch = branches_.begin();
	branch != branches_.end(); ++branch ) {
    stream << " " << branch->first << std::endl;
  }
  stream << std::endl;
}

void NeuralMtautauNtupleProducer::setValue(const std::string& name, double value) 
{
  if ( verbosity_ ) std::cout << "branch = " << name << ": value = " << value << std::endl;
  branchMap::iterator branch = branches_.find(name);
  if ( branch != branches_.end() ) {
    branch->second = value;
  } else {
    throw cms::Exception("InvalidParameter") 
      << "No branch with name = " << name << " defined !!\n";
  }
}

//
//-------------------------------------------------------------------------------
//

void NeuralMtautauNtupleProducer::addBranch_EnPxPyPz(const std::string& name) 
{
  addBranch(std::string(name).append("En"));
  addBranch(std::string(name).append("Px"));
  addBranch(std::string(name).append("Py"));
  addBranch(std::string(name).append("Pz"));
  addBranch(std::string(name).append("M"));
}

void NeuralMtautauNtupleProducer::addBranch_PxPy(const std::string& name) 
{
  addBranch(std::string(name).append("Px"));
  addBranch(std::string(name).append("Py"));
}

void NeuralMtautauNtupleProducer::addBranch_Cov(const std::string& name) 
{
  addBranch(std::string(name).append("Vxx"));
  addBranch(std::string(name).append("Vxy"));
  addBranch(std::string(name).append("Vyy"));
  addBranch(std::string(name).append("SigmaX"));
  addBranch(std::string(name).append("SigmaY"));
  addBranch(std::string(name).append("CorrXY"));
}

//
//-------------------------------------------------------------------------------
//

void NeuralMtautauNtupleProducer::setValue_EnPxPyPz(const std::string& name, const reco::Candidate::LorentzVector& p4, double zetaPhi)
{
  reco::Candidate::LorentzVector p4inZetaFrame = compP4inZetaFrame(p4, zetaPhi);
  setValue(std::string(name).append("En"), p4inZetaFrame.E());
  setValue(std::string(name).append("Px"), p4inZetaFrame.px());
  setValue(std::string(name).append("Py"), p4inZetaFrame.py());
  setValue(std::string(name).append("Pz"), p4inZetaFrame.pz());
  setValue(std::string(name).append("M"), p4inZetaFrame.M());
}

void NeuralMtautauNtupleProducer::setValue_PxPy(const std::string& name, const reco::Candidate::LorentzVector& p4, double zetaPhi)
{
  reco::Candidate::LorentzVector p4inZetaFrame = compP4inZetaFrame(p4, zetaPhi);
  setValue(std::string(name).append("Px"), p4inZetaFrame.px());
  setValue(std::string(name).append("Py"), p4inZetaFrame.py());
}

void NeuralMtautauNtupleProducer::setValue_Cov(const std::string& name, const TMatrixD& cov, double zetaPhi)
{
  assert(cov.GetNrows() == 2);
  assert(cov.GetNcols() == 2);
  TMatrixD covInZetaFrame = compCovMatrixInZetaFrame(cov, zetaPhi);
  setValue(std::string(name).append("Vxx"), covInZetaFrame(0,0));
  setValue(std::string(name).append("Vxy"), covInZetaFrame(0,1));
  setValue(std::string(name).append("Vyy"), covInZetaFrame(1,1));
  double sigmaXinZetaFrame = TMath::Sqrt(TMath::Abs(covInZetaFrame(0,0)));
  double sigmaYinZetaFrame = TMath::Sqrt(TMath::Abs(covInZetaFrame(1,1)));
  const double epsilon = 1.e-9;
  double corrXYinZetaFrame = ( (sigmaXinZetaFrame*sigmaYinZetaFrame) > epsilon ) ?
    covInZetaFrame(0,1)/(sigmaXinZetaFrame*sigmaYinZetaFrame) : 0.;
  setValue(std::string(name).append("SigmaX"), sigmaXinZetaFrame);
  setValue(std::string(name).append("SigmaY"), sigmaYinZetaFrame);
  setValue(std::string(name).append("CorrXY"), corrXYinZetaFrame);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NeuralMtautauNtupleProducer);
