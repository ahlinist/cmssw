#include "TauAnalysis/CandidateTools/plugins/NeuralMtautauNtupleProducer.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneLikelihood.h"
#include "TauAnalysis/CandidateTools/interface/neuralMtautauAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

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
  srcGenTaus_ = cfg.getParameter<edm::InputTag>("srcGenTaus");
  srcGenParticles_ = cfg.getParameter<edm::InputTag>("srcGenParticles");

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
  addBranch_EnPxPyPz("genTau1");
  addBranch_EnPxPyPz("genVis1");
  addBranch_EnPxPyPz("genNu1");
  addBranch("genX1");

  addBranch_EnPxPyPz("genTau2");
  addBranch_EnPxPyPz("genVis2");
  addBranch_EnPxPyPz("genNu2");
  addBranch("genX2");
  
  addBranch_EnPxPyPz("recVis1");

  addBranch_EnPxPyPz("recVis2");

  addBranch("recDPhi12");
  addBranch("recDTheta12");
  addBranch("recDAlpha12");

  addBranch_PxPy("recMEt");
  addBranch_Cov("recMEt");

  addBranch("recSVfitMtautau");
  addBranch("recSigmaSVfit");

  addBranch("recVisMass");
  addBranch_PxPy("genMEt");

  addBranch_EnPxPyPz("genDiTau");
  addBranch("genMtautau");

  addBranch_long("run");
  addBranch_long("lumisection");
  addBranch_long("event");  
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

  double square(double x)
  {
    return x*x;
  }
}

void NeuralMtautauNtupleProducer::produce(edm::Event& evt, const edm::EventSetup& es) 
{
  if ( verbosity_ ) {
    std::cout << "<NeuralMtautauNtupleProducer::produce>:" << std::endl;
  }

  edm::Handle<CandidateView> genTauPair;
  evt.getByLabel(srcGenTauPair_, genTauPair);
  if ( !genTauPair->size() == 1 ) 
    throw cms::Exception("InvalidData") 
      << "Failed to find unique gen. Tau-pair object !!\n";
  const reco::Candidate::LorentzVector& genDiTauP4 = genTauPair->front().p4();
  double genMtautau = genTauPair->front().mass();
  
  edm::Handle<MEtView> genMEt;
  evt.getByLabel(srcGenMEt_, genMEt);
  if ( !genMEt->size() == 1 ) 
    throw cms::Exception("InvalidData") 
      << "Failed to find unique gen. MET object !!\n";
  const reco::Candidate::LorentzVector& genMEtP4 = genMEt->front().p4();
  
  edm::Handle<CandidateView> recVis1;
  evt.getByLabel(srcRecLeg1_, recVis1);
  if ( !recVis1->size() >= 1 ) return;
  const reco::Candidate::LorentzVector& recVis1P4 = recVis1->front().p4();

  edm::Handle<CandidateView> recVis2;
  evt.getByLabel(srcRecLeg2_, recVis2);
  if ( !recVis2->size() >= 1 ) return;
  const reco::Candidate::LorentzVector& recVis2P4 = recVis2->front().p4();

  edm::Handle<MEtView> recMEt;
  evt.getByLabel(srcRecMEt_, recMEt);
  if ( !recMEt->size() == 1 ) 
    throw cms::Exception("InvalidData") 
      << "Failed to find unique MET object !!\n";
  const reco::Candidate::LorentzVector& recMEtP4 = recMEt->front().p4();

  edm::Handle<PFMEtSignCovMatrix> pfMEtSignCovMatrix;
  evt.getByLabel(srcPFMEtCovMatrix_, pfMEtSignCovMatrix);

//--- compute axis 'zeta' bisecting angle between visible decay products of the two tau leptons
  double zetaPhi = compZetaPhi(recVis1P4, recVis2P4);

  if ( verbosity_ ) {
    std::cout << "recVis1: px = " << recVis1P4.px() << ", py = " << recVis1P4.py() << "," 
	      << " phi = " << recVis1P4.phi() << std::endl;
    std::cout << "recVis2: px = " << recVis2P4.px() << ", py = " << recVis2P4.py() << "," 
	      << " phi = " << recVis2P4.phi() << std::endl;      
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

  setValue_EnPxPyPz("recVis1", recVis1P4, zetaPhi);
  
  setValue_EnPxPyPz("recVis2", recVis2P4, zetaPhi);
  
  double recDPhi12       = TMath::ACos(TMath::Cos(recVis1P4.phi() - recVis2P4.phi()));
  setValue("recDPhi12", recDPhi12);
  double recDTheta12     = TMath::Abs(recVis1P4.theta() - recVis2P4.theta());
  setValue("recDTheta12", recDTheta12);
  double recVis1cosPhi   = TMath::Cos(recVis1P4.phi());
  double recVis1sinPhi   = TMath::Sin(recVis1P4.phi());
  double recVis1cosTheta = TMath::Cos(recVis1P4.theta());
  double recVis1sinTheta = TMath::Sin(recVis1P4.theta());
  double recVis2cosPhi   = TMath::Cos(recVis2P4.phi());
  double recVis2sinPhi   = TMath::Sin(recVis2P4.phi());
  double recVis2cosTheta = TMath::Cos(recVis2P4.theta());
  double recVis2sinTheta = TMath::Sin(recVis2P4.theta());
  double dotProduct12    = (recVis1cosPhi*recVis2cosPhi + recVis1sinPhi*recVis2sinPhi)*recVis1sinTheta*recVis2sinTheta 
                          + recVis1cosTheta*recVis2cosTheta;
  double recDAlpha12     = TMath::ACos(dotProduct12);
  setValue("recDAlpha12", recDAlpha12);

  setValue_PxPy("recMEt", recMEtP4, zetaPhi);
  setValue_Cov("recMEt", *pfMEtSignCovMatrix, zetaPhi);
  
  setValue_PxPy("genMEt", genMEtP4, zetaPhi);

  setValue_EnPxPyPz("genDiTau", genDiTauP4);
  setValue("genMtautau", genMtautau);

  // determine generator level momenta of tau leptons, visible decay products and neutrinos produced in tau decays
  // (not needed for actual neuralMtautau training, but useful for mass-reconstruction studies...)
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
  double dRplusMapsTo1MinusMapsTo2 = square(deltaR(genVisPlusP4, recVis1P4)) + square(deltaR(genVisMinusP4, recVis2P4));
  double dRplusMapsTo2MinusMapsTo1 = square(deltaR(genVisPlusP4, recVis2P4)) + square(deltaR(genVisMinusP4, recVis1P4));
  reco::Candidate::LorentzVector genTau1P4, genVis1P4, genNu1P4, genTau2P4, genVis2P4, genNu2P4;
  if ( dRplusMapsTo1MinusMapsTo2 < dRplusMapsTo2MinusMapsTo1 ) {
    genTau1P4 = genTauPlus->p4();
    genVis1P4 = genVisPlusP4;
    genNu1P4  = getInvisMomentum(genTauPlus);
    genTau2P4 = genTauMinus->p4();
    genVis2P4 = genVisMinusP4;
    genNu2P4  = getInvisMomentum(genTauMinus);
  } else {
    genTau1P4 = genTauMinus->p4();
    genVis1P4 = genVisMinusP4;
    genNu1P4  = getInvisMomentum(genTauMinus);
    genTau2P4 = genTauPlus->p4();
    genVis2P4 = genVisPlusP4;
    genNu2P4  = getInvisMomentum(genTauPlus);
  }
  //assert(TMath::Abs((genTau1P4 + genTau2P4).mass() - genMtautau) < 1.e-1);
  //assert(TMath::Abs((genVis1P4 + genNu1P4).mass() - genTau1P4.mass()) < 1.e-2);
  //assert(TMath::Abs((genVis2P4 + genNu2P4).mass() - genTau2P4.mass()) < 1.e-2);

  setValue_EnPxPyPz("genTau1", genTau1P4, zetaPhi);
  setValue_EnPxPyPz("genVis1", genVis1P4, zetaPhi);
  setValue_EnPxPyPz("genNu1",  genNu1P4,  zetaPhi);
  setValue("genX1", genVis1P4.E()/genTau1P4.E());

  setValue_EnPxPyPz("genTau2", genTau2P4, zetaPhi);
  setValue_EnPxPyPz("genVis2", genVis2P4, zetaPhi);
  setValue_EnPxPyPz("genNu2",  genNu2P4,  zetaPhi);
  setValue("genX2", genVis2P4.E()/genTau2P4.E());

  // reconstruct mTauTau using SVfit for comparisson
  //
  // WARNING: assumes that srcLeg1 (srcLeg2) refers to leptonic (hadronic) tau decays
  //
  std::vector<MeasuredTauLepton> tauDecayProduts;
  tauDecayProduts.push_back(MeasuredTauLepton(kLepDecay, recVis1P4));
  tauDecayProduts.push_back(MeasuredTauLepton(kHadDecay, recVis2P4));
  Vector recMEtP3(recMEtP4.px(), recMEtP4.py(), recMEtP4.pz());
  NSVfitStandaloneAlgorithm svFit(tauDecayProduts, recMEtP3, *pfMEtSignCovMatrix);
  svFit.fit();
  double svFitMtautau = -1.;
  double sigmaSVfit = 1.e+3;
  if ( svFit.isValidSolution() ) {
    svFitMtautau = svFit.mass();
    sigmaSVfit = svFit.massUncert();
  }
  setValue("recSVfitMtautau", svFitMtautau);
  setValue("recSigmaSVfit", sigmaSVfit);

  double visMass = (recVis1P4 + recVis2P4).mass();
  setValue("recVisMass", visMass);

  setValue_long("run", evt.id().run());
  setValue_long("lumisection", evt.luminosityBlock());
  setValue_long("event", evt.id().event());  

//--- finally, fill all computed quantities into TTree
  assert(ntuple_);
  ntuple_->Fill();
}

void NeuralMtautauNtupleProducer::addBranch(const std::string& name) 
{
  assert(branches_.count(name) == 0);
  std::string name_and_format = name + "/F";
  ntuple_->Branch(name.c_str(), &branches_[name].value_float_, name_and_format.c_str());
}

void NeuralMtautauNtupleProducer::addBranch_long(const std::string& name) 
{
  assert(branches_.count(name) == 0);
  std::string name_and_format = name + "/l";
  ntuple_->Branch(name.c_str(), &branches_[name].value_long_, name_and_format.c_str());
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
    branch->second.value_float_ = value;
  } else {
    throw cms::Exception("InvalidParameter") 
      << "No branch with name = " << name << " defined !!\n";
  }
}

void NeuralMtautauNtupleProducer::setValue_long(const std::string& name, unsigned long value) 
{
  if ( verbosity_ ) std::cout << "branch = " << name << ": value = " << value << std::endl;
  branchMap::iterator branch = branches_.find(name);
  if ( branch != branches_.end() ) {
    branch->second.value_long_ = value;
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
  addBranch(std::string(name).append("Pt"));
  addBranch(std::string(name).append("Eta"));
  addBranch(std::string(name).append("Phi"));
  addBranch(std::string(name).append("M"));
}

void NeuralMtautauNtupleProducer::addBranch_PxPy(const std::string& name) 
{
  addBranch(std::string(name).append("Px"));
  addBranch(std::string(name).append("Py"));
  addBranch(std::string(name).append("Pt"));
  addBranch(std::string(name).append("Phi"));
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

void NeuralMtautauNtupleProducer::setValue_EnPxPyPz(const std::string& name, const reco::Candidate::LorentzVector& p4)
{
  setValue(std::string(name).append("En"), p4.E());
  setValue(std::string(name).append("Px"), p4.px());
  setValue(std::string(name).append("Py"), p4.py());
  setValue(std::string(name).append("Pz"), p4.pz());
  setValue(std::string(name).append("Pt"), p4.pt());
  setValue(std::string(name).append("Eta"), p4.eta());
  setValue(std::string(name).append("Phi"), p4.phi());
  setValue(std::string(name).append("M"), p4.M());
}

void NeuralMtautauNtupleProducer::setValue_EnPxPyPz(const std::string& name, const reco::Candidate::LorentzVector& p4, double zetaPhi)
{
  reco::Candidate::LorentzVector p4inZetaFrame = compP4inZetaFrame(p4, zetaPhi);
  setValue_EnPxPyPz(name, p4inZetaFrame);
}

void NeuralMtautauNtupleProducer::setValue_PxPy(const std::string& name, const reco::Candidate::LorentzVector& p4)
{
  setValue(std::string(name).append("Px"), p4.px());
  setValue(std::string(name).append("Py"), p4.py());
  setValue(std::string(name).append("Pt"), p4.pt());
  setValue(std::string(name).append("Phi"), p4.phi());
}

void NeuralMtautauNtupleProducer::setValue_PxPy(const std::string& name, const reco::Candidate::LorentzVector& p4, double zetaPhi)
{
  reco::Candidate::LorentzVector p4inZetaFrame = compP4inZetaFrame(p4, zetaPhi);
  setValue_PxPy(name, p4inZetaFrame);
}

void NeuralMtautauNtupleProducer::setValue_Cov(const std::string& name, const TMatrixD& cov)
{
  assert(cov.GetNrows() == 2);
  assert(cov.GetNcols() == 2);
  setValue(std::string(name).append("Vxx"), cov(0,0));
  setValue(std::string(name).append("Vxy"), cov(0,1));
  setValue(std::string(name).append("Vyy"), cov(1,1));
  double sigmaX = TMath::Sqrt(TMath::Abs(cov(0,0)));
  double sigmaY = TMath::Sqrt(TMath::Abs(cov(1,1)));
  const double epsilon = 1.e-9;
  double corrXY = ( (sigmaX*sigmaY) > epsilon ) ?
    cov(0,1)/(sigmaX*sigmaY) : 0.;
  setValue(std::string(name).append("SigmaX"), sigmaX);
  setValue(std::string(name).append("SigmaY"), sigmaY);
  setValue(std::string(name).append("CorrXY"), corrXY);
}

void NeuralMtautauNtupleProducer::setValue_Cov(const std::string& name, const TMatrixD& cov, double zetaPhi)
{
  assert(cov.GetNrows() == 2);
  assert(cov.GetNcols() == 2);
  TMatrixD covInZetaFrame = compCovMatrixInZetaFrame(cov, zetaPhi);
  setValue_Cov(name, covInZetaFrame);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NeuralMtautauNtupleProducer);
