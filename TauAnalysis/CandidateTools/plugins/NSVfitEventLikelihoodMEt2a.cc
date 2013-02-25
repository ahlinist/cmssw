#include "TauAnalysis/CandidateTools/plugins/NSVfitEventLikelihoodMEt2a.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "DataFormats/METReco/interface/PFMEtSignCovMatrix.h"

#include <TMath.h>
#include <TVectorD.h>

using namespace SVfit_namespace;

const double defaultMEtResolutionX = 10.; // approx. resolution of MVA MET algorithm in 2012
const double defaultMEtResolutionY = 10.;

const double epsilon = 1.e-4;

NSVfitEventLikelihoodMEt2a::NSVfitEventLikelihoodMEt2a(const edm::ParameterSet& cfg)
  : NSVfitEventLikelihood(cfg),
    metCov_(2,2),
    metCovInverse_(2,2),
    tailProbCorrFunction_(0)
{
  power_ = ( cfg.exists("power") ) ?
    cfg.getParameter<double>("power") : 1.0;

  srcMEtCovMatrix_ = cfg.getParameter<edm::InputTag>("srcMEtCovMatrix");

  std::string response_string = cfg.getParameter<std::string>("response");
  response_ = new TFormula(Form("%s_response", pluginName_.data()), response_string.data());

  if ( cfg.exists("tailProbCorr") ) {
    edm::ParameterSet cfgTailProbCorr = cfg.getParameter<edm::ParameterSet>("tailProbCorr");
    tailProbCorrFunction_ = new tailProbCorrFunctionType(pluginName_, cfgTailProbCorr);
  }
}

NSVfitEventLikelihoodMEt2a::~NSVfitEventLikelihoodMEt2a()
{
  delete tailProbCorrFunction_;
}

void NSVfitEventLikelihoodMEt2a::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter("allTauDecays", nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter("allTauDecays", nSVfit_namespace::kTau_phi_lab,    pluginName_);
  algorithm->requestFitParameter("allLeptons",   nSVfit_namespace::kLep_shiftEn,    pluginName_);
  algorithm->requestFitParameter("allNeutrinos", nSVfit_namespace::kNu_energy_lab,  pluginName_);
  algorithm->requestFitParameter("allNeutrinos", nSVfit_namespace::kNu_phi_lab,     pluginName_);
}

void NSVfitEventLikelihoodMEt2a::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  bool isValid = false;
  edm::Handle<PFMEtSignCovMatrix> metCovMatrix;
  evt.getByLabel(srcMEtCovMatrix_, metCovMatrix);
  if ( metCovMatrix.isValid() ) {
    metCov_ = (*metCovMatrix);
    isValid = true;
  } 
  if ( !isValid ) {
    typedef edm::View<reco::MET> METView;
    edm::Handle<METView> met;
    evt.getByLabel(srcMEtCovMatrix_, met);
    if ( met.isValid() ) {
      if ( met->size() == 1 ) {
	metCov_ = met->front().getSignificanceMatrix();
	isValid = true;
      } else {
	throw cms::Exception("InvalidData") 
	  << "Failed to find unique MET object !!\n";
      }
    }
  }
  if ( !isValid ) {
    throw cms::Exception("InvalidData") 
      << "Configuration parameter 'srcMEtCovMatrix' does not refer to valid collection of PFMEtSignCovMatrix or reco::MET objects !!\n";
  }
}

namespace
{
  double determinant(const TMatrixD& metCov)
  {
    assert(metCov.GetNrows() == 2);
    assert(metCov.GetNcols() == 2);
    return (metCov(0,0)*metCov(1,1) - metCov(0,1)*metCov(1,0));
  }
}

void NSVfitEventLikelihoodMEt2a::beginCandidate(const NSVfitEventHypothesis* hypothesis) const
{
  metCovDet_ = determinant(metCov_);
  metCovInverse_ = metCov_;
  if ( metCovDet_ > epsilon ) {
    metCovInverse_.Invert();
    metCovInverse00_ = metCovInverse_(0, 0);
    metCovInverse01_ = metCovInverse_(0, 1);
    metCovInverse10_ = metCovInverse_(1, 0);
    metCovInverse11_ = metCovInverse_(1, 1);
  } else {
    edm::LogWarning ("NSVfitEventLikelihoodMEt2a::beginCandidate")
      << "Failed to invert MET covariance matrix --> using approximate values for MET resolution instead !!";
    metCovInverse00_ = square(1./defaultMEtResolutionX);
    metCovInverse01_ = 0.;
    metCovInverse10_ = 0.;
    metCovInverse11_ = square(1./defaultMEtResolutionY);
  }
  
  nllConstTerm_ = TMath::Log(2.*TMath::Pi()) + 0.5*TMath::Log(TMath::Abs(metCovDet_));

  hadRecoilPx_measured_ = -(hypothesis->p4MEt().px() + hypothesis->p4().px());
  hadRecoilPy_measured_ = -(hypothesis->p4MEt().py() + hypothesis->p4().py());
}

double NSVfitEventLikelihoodMEt2a::operator()(const NSVfitEventHypothesis* hypothesis) const
{
//--- compute negative log-likelihood for neutrinos produced in tau lepton decays
//    to match missing transverse momentum reconstructed in the event
//
//    NOTE: the computation takes into account the expected response of the MET algorithm for hadrons
//            

//--- compute hadronic recoil corresponding to fitted sum of visible lepton + neutrino momenta
  double hadRecoilPx_fitted = -hypothesis->p4_fitted().px();
  double hadRecoilPy_fitted = -hypothesis->p4_fitted().py();

//--- compute expected components of reconstructed hadronic recoil,
//    taking into account expected response
  double qT = hypothesis->p4_fitted().pt();

  double response_expected = response_->Eval(qT);

  double hadRecoilPx_expected = response_expected*hadRecoilPx_fitted;
  double hadRecoilPy_expected = response_expected*hadRecoilPy_fitted;

//--- compute difference between measured components of hadronic recoil 
//    and components of hadronic recoil expected in case 
//    "true" momenta of visible leptons + neutrinos match fitted values
  double residual0 = hadRecoilPx_expected - hadRecoilPx_measured_;
  double residual1 = hadRecoilPy_expected - hadRecoilPy_measured_;

  if ( this->verbosity_ ) {
    std::cout << "<NSVfitEventLikelihoodMEt2a::operator()>:" << std::endl;
    std::cout << " pxResidual = " << residual0 << std::endl;
    std::cout << " pyResidual = " << residual1 << std::endl;
  }

  double nll = 0.;
  if ( metCovDet_ != 0. ) {
    // CV: avoid usage of TVectorD*(TMatrixD*TVectorD) notation
    //     and write exponent of multivariate Normal distribution in components instead,
    //     to avoid continuous allocation/deallocation of temporary objects (speed!)
    double pull = residual0*(metCovInverse00_*residual0 + metCovInverse01_*residual1)
	        + residual1*(metCovInverse10_*residual0 + metCovInverse11_*residual1);
    if ( tailProbCorrFunction_ ) {
      double tailProbCorr = tailProbCorrFunction_->eval(pull);
      //if ( this->verbosity_ ) std::cout << "pull = " << pull << ": tailProbCorr = " << tailProbCorr << std::endl;
      if ( tailProbCorr > 0.9 ) pull /= tailProbCorr;
    }
    nll = nllConstTerm_ + 0.5*pull;
  } else {
    nll = std::numeric_limits<float>::max();
  }

  double prob = TMath::Exp(-power_*nll);
  if ( this->verbosity_ ) std::cout << "--> prob = " << prob << std::endl;

  return prob;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventLikelihoodPluginFactory, NSVfitEventLikelihoodMEt2a, "NSVfitEventLikelihoodMEt2a");
