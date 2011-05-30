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
    pfMEtSign_(0)
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
  
  TMatrixD pfMEtCov = (*pfMEtSign_)(daughterHypothesesList);

  TMatrixD pfMEtCovInverseTemp = pfMEtCov.Invert();

  pfMEtCovInverse_(0, 0) = pfMEtCovInverseTemp(0, 0);
  pfMEtCovInverse_(1, 0) = pfMEtCovInverseTemp(1, 0);
  pfMEtCovInverse_(0, 1) = pfMEtCovInverseTemp(0, 1);
  pfMEtCovInverse_(1, 1) = pfMEtCovInverseTemp(1, 1);
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

  residual_fitted_(0) = hypothesis->dp4MEt_fitted().px();
  residual_fitted_(1) = hypothesis->dp4MEt_fitted().py();

  if ( this->verbosity_ ) {
    std::cout << " pxResidual_fitted = " << residual_fitted_(0) << std::endl;
    std::cout << " pyResidual_fitted = " << residual_fitted_(1) << std::endl;
  }

  // CV: in case pfMEt resolution are accurately modeled,
  //     residual*cov^-1*residual is expected to be distributed
  //     like a chi^2 with 2 degrees of freedom
  //    (cf. CMS AN-10/400, page 1)
  //double chi2 = residual_fitted*(pfMEtCovInverse_*residual_fitted);
  double chi2 = ROOT::Math::Similarity(residual_fitted_, pfMEtCovInverse_);
  double prob = TMath::Prob(chi2, 2);

  double nll = 0.;
  if ( prob > 0. ) {
    nll = -TMath::Log(prob);
  } else {
    nll = std::numeric_limits<float>::max();
  }

  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return power_*nll;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventLikelihoodPluginFactory, NSVfitEventLikelihoodMEt2, "NSVfitEventLikelihoodMEt2");
