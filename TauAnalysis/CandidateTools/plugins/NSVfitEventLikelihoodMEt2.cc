#include "TauAnalysis/CandidateTools/plugins/NSVfitEventLikelihoodMEt2.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>
#include <TVectorD.h>

using namespace SVfit_namespace;

const double defaultPFMEtResolutionX = 10.;
const double defaultPFMEtResolutionY = 10.;

const double epsilon = 1.e-4;

NSVfitEventLikelihoodMEt2::NSVfitEventLikelihoodMEt2(const edm::ParameterSet& cfg)
  : NSVfitEventLikelihood(cfg),
    pfMEtSign_(0),
    pfMEtCov_(2, 2),
    pfMEtCovInverse_(2, 2)
{
  power_ = ( cfg.exists("power") ) ?
    cfg.getParameter<double>("power") : 1.0;

  pfMEtSign_ = new PFMEtSignInterface(cfg);
}

NSVfitEventLikelihoodMEt2::~NSVfitEventLikelihoodMEt2()
{
  delete pfMEtSign_;
}

void NSVfitEventLikelihoodMEt2::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter("allTauDecays", nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter("allTauDecays", nSVfit_namespace::kTau_phi_lab,    pluginName_);
  algorithm->requestFitParameter("allLeptons",   nSVfit_namespace::kLep_shiftEn,    pluginName_);
  algorithm->requestFitParameter("allNeutrinos", nSVfit_namespace::kNu_energy_lab,  pluginName_);
  algorithm->requestFitParameter("allNeutrinos", nSVfit_namespace::kNu_phi_lab,     pluginName_);
}

void NSVfitEventLikelihoodMEt2::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  pfMEtSign_->beginEvent(evt, es);
}

void NSVfitEventLikelihoodMEt2::beginCandidate(const NSVfitEventHypothesis* hypothesis) const
{
  if ( this->verbosity_ ) {
    std::cout << "<NSVfitEventLikelihoodMEt2::beginCandidate>:" << std::endl;
    std::cout << " hypothesis = " << hypothesis << std::endl;
  }

  std::list<const reco::Candidate*> daughterHypothesesList;

  size_t numResonances = hypothesis->numResonances();
  for ( size_t iResonance = 0; iResonance < numResonances; ++iResonance ) {
    const NSVfitResonanceHypothesis* resonance = hypothesis->resonance(iResonance);
    size_t numDaughters = resonance->numDaughters();
    for ( size_t iDaughter = 0; iDaughter < numDaughters; ++iDaughter ) {
      const NSVfitSingleParticleHypothesis* daughter = resonance->daughter(iDaughter);
      daughterHypothesesList.push_back(daughter->particle().get());
    }
  }
  
  pfMEtCov_ = (*pfMEtSign_)(daughterHypothesesList);
  pfMEtCovDet_ = pfMEtCov_.Determinant();
  pfMEtCovInverse_ = pfMEtCov_;
  if ( pfMEtCovDet_ != 0. ) pfMEtCovInverse_.Invert();

  pfMEtCovInverse00_ = pfMEtCovInverse_(0, 0);
  pfMEtCovInverse01_ = pfMEtCovInverse_(0, 1);
  pfMEtCovInverse10_ = pfMEtCovInverse_(1, 0);
  pfMEtCovInverse11_ = pfMEtCovInverse_(1, 1);
  
  nllConstTerm_ = TMath::Log(2.*TMath::Pi()) + 0.5*TMath::Log(TMath::Abs(pfMEtCovDet_));
}

double NSVfitEventLikelihoodMEt2::operator()(const NSVfitEventHypothesis* hypothesis) const
{
//--- compute negative log-likelihood for neutrinos produced in tau lepton decays
//    to match missing transverse momentum reconstructed in the event
//
//    NB: MET likelihood is split into perp/par components along (leptonic) leg1 of the diTau object
//
  if ( this->verbosity_ ) {
    std::cout << "<NSVfitEventLikelihoodMEt2::operator()>:" << std::endl;
    std::cout << " hypothesis = " << hypothesis << std::endl;
  }
  
  residual_fitted0_ = hypothesis->dp4MEt_fitted().px();
  residual_fitted1_ = hypothesis->dp4MEt_fitted().py();

  if ( this->verbosity_ ) {
    std::cout << " pxResidual_fitted = " << residual_fitted0_ << std::endl;
    std::cout << " pyResidual_fitted = " << residual_fitted1_ << std::endl;
  }

  double nll = 0.;
  if ( pfMEtCovDet_ != 0. ) {
    // CV: avoid usage of TVectorD*(TMatrixD*TVectorD) notation
    //     and write exponent of multivariate Normal distribution in components instead,
    //     to avoid continuous allocation/deallocation of temporary objects (speed!)
    nll = nllConstTerm_ 
         + 0.5*(residual_fitted0_*(pfMEtCovInverse00_*residual_fitted0_ + pfMEtCovInverse01_*residual_fitted1_)
	      + residual_fitted1_*(pfMEtCovInverse10_*residual_fitted0_ + pfMEtCovInverse11_*residual_fitted1_));
  } else {
    nll = std::numeric_limits<float>::max();
  }

  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return power_*nll;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventLikelihoodPluginFactory, NSVfitEventLikelihoodMEt2, "NSVfitEventLikelihoodMEt2");
