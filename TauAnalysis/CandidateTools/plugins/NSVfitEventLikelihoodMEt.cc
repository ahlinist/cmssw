#include "TauAnalysis/CandidateTools/plugins/NSVfitEventLikelihoodMEt.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

#include <string>

using namespace SVfit_namespace;

const double parSigmaMin  = 5.0;
const double perpSigmaMin = 5.0;

NSVfitEventLikelihoodMEt::NSVfitEventLikelihoodMEt(const edm::ParameterSet& cfg)
  : NSVfitEventLikelihood(cfg),
    parSigma_(0),
    parBias_(0),
    perpSigma_(0),
    perpBias_(0),
    qX_(0.),
    qY_(0.),
    qT_(0.)
{
  power_ = ( cfg.exists("power") ) ?
    cfg.getParameter<double>("power") : 1.0;

  edm::ParameterSet cfgResolution = cfg.getParameter<edm::ParameterSet>("resolution");
  parSigma_  = new TFormula("parSigma",  cfgResolution.getParameter<std::string>("parSigma").data());
  parBias_   = new TFormula("parBias",   cfgResolution.getParameter<std::string>("parBias").data());
  perpSigma_ = new TFormula("perpSigma", cfgResolution.getParameter<std::string>("perpSigma").data());
  perpBias_  = new TFormula("perpBias",  cfgResolution.getParameter<std::string>("perpBias").data());
}

NSVfitEventLikelihoodMEt::~NSVfitEventLikelihoodMEt()
{
  delete parSigma_;
  delete parBias_;
  delete perpSigma_;
  delete perpBias_;
}

void NSVfitEventLikelihoodMEt::beginJob(NSVfitAlgorithmBase* algorithm) const 
{
  algorithm->requestFitParameter("allTauDecays", nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter("allTauDecays", nSVfit_namespace::kTau_phi_lab,    pluginName_);
  algorithm->requestFitParameter("allLeptons",   nSVfit_namespace::kLep_shiftEn,    pluginName_);
  algorithm->requestFitParameter("allNeutrinos", nSVfit_namespace::kNu_energy_lab,  pluginName_);
  algorithm->requestFitParameter("allNeutrinos", nSVfit_namespace::kNu_phi_lab,     pluginName_);
}

void NSVfitEventLikelihoodMEt::beginCandidate(const NSVfitEventHypothesis* hypothesis) const
{
  qX_ = hypothesis->p4().px() + hypothesis->p4MEt().px();
  qY_ = hypothesis->p4().py() + hypothesis->p4MEt().py();
  qT_ = TMath::Sqrt(qX_*qX_ + qY_*qY_);
}

double NSVfitEventLikelihoodMEt::operator()(const NSVfitEventHypothesis* hypothesis) const
{
//--- compute negative log-likelihood for neutrinos produced in tau lepton decays
//    to match missing transverse momentum reconstructed in the event
//
//    NB: MET likelihood is split into perp/par components along (leptonic) leg1 of the diTau object
//
  //if ( this->verbosity_ ) std::cout << "<NSVfitEventLikelihoodMEt::operator()>:" << std::endl;

  double parSigma = parSigma_->Eval(qT_);
  if ( parSigma < parSigmaMin ) parSigma = parSigmaMin;
  double parBias = parBias_->Eval(qT_);
  //if ( this->verbosity_ ) std::cout << " parSigma = " << parSigma << ", parBias = " << parBias << std::endl;

  double perpSigma = perpSigma_->Eval(qT_);
  if ( perpSigma < perpSigmaMin ) perpSigma = perpSigmaMin;
  double perpBias = perpBias_->Eval(qT_);
  //if ( this->verbosity_ ) std::cout << " perpSigma = " << perpSigma << ", perpBias = " << perpBias << std::endl;

  double projCosPhi = 0.;
  double projSinPhi = 0.;
  if ( qT_ > 0. ) {
    projCosPhi = (qX_/qT_);
    projSinPhi = (qY_/qT_);
  } 

  double pxResidual_fitted = hypothesis->dp4MEt_fitted().px();
  double pyResidual_fitted = hypothesis->dp4MEt_fitted().py();
  //if ( this->verbosity_ ) {
  //  std::cout << "pxResidual_fitted = " << pxResidual_fitted << std::endl;
  //  std::cout << "pyResidual_fitted = " << pyResidual_fitted << std::endl;
  //}

  double parResidual_fitted  = (pxResidual_fitted*projCosPhi + pyResidual_fitted*projSinPhi) - parBias;
  double perpResidual_fitted = (pxResidual_fitted*projSinPhi - pyResidual_fitted*projCosPhi) - perpBias;
  //if ( this->verbosity_ ) {
  //  std::cout << " parResidual_fitted  = " << parResidual_fitted  << std::endl;
  //  std::cout << " perpResidual_fitted = " << perpResidual_fitted << std::endl;
  //}

  double nll = -(logGaussian(parResidual_fitted, parSigma) + logGaussian(perpResidual_fitted, perpSigma));
  //if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  double prob = TMath::Exp(-power_*nll);

  return prob;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventLikelihoodPluginFactory, NSVfitEventLikelihoodMEt, "NSVfitEventLikelihoodMEt");
