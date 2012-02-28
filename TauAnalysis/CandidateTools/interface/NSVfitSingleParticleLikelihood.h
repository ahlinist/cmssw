#ifndef TauAnalysis_CandidateTools_NSVfitSingleParticleLikelihood_h
#define TauAnalysis_CandidateTools_NSVfitSingleParticleLikelihood_h

/** \class NSVfitSingleParticleLikelihoodBase
 *
 * Abstract base-class for plugins computing likelihood of single particle kinematics;
 * used by nSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.6 $
 *
 * $Id: NSVfitSingleParticleLikelihood.h,v 1.6 2012/02/28 09:44:08 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitLikelihoodBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"

#include <TF1.h>
#include <TString.h>

#include <string>
#include <iostream>

class NSVfitSingleParticleLikelihood : public NSVfitLikelihoodBase
{
 public:
  NSVfitSingleParticleLikelihood(const edm::ParameterSet& cfg)
    : NSVfitLikelihoodBase(cfg),
      prodParticleLabel_(cfg.getParameter<std::string>("prodParticleLabel")),
      visPtCutCorrFunction_(0)
  {
    applyVisPtCutCorrection_ = ( cfg.exists("applyVisPtCutCorrection") ) ?
      cfg.getParameter<bool>("applyVisPtCutCorrection") : false;
    if ( applyVisPtCutCorrection_ ) {
      edm::ParameterSet cfgVisPtCutCorrection = cfg.getParameter<edm::ParameterSet>("visPtCutCorrection");
      std::string formula = cfgVisPtCutCorrection.getParameter<std::string>("formula");
      visPtCutCorr_xMin_ = cfgVisPtCutCorrection.getParameter<double>("xMin");
      visPtCutCorr_xMax_ = cfgVisPtCutCorrection.getParameter<double>("xMax");
      std::string name = Form("%s_visPtCutCorrFunction", pluginName_.data());
      visPtCutCorrFunction_ = new TF1(name.data(), formula.data(), visPtCutCorr_xMin_, visPtCutCorr_xMax_);
      int numParameter = visPtCutCorrFunction_->GetNpar();
      for ( int iParameter = 0; iParameter < numParameter; ++iParameter ) {
	std::string parName = Form("par%i", iParameter);
	edm::ParameterSet cfgPar = cfgVisPtCutCorrection.getParameter<edm::ParameterSet>(parName);
	std::string name = Form("%s_visPtCutCorrFunction_par%i", pluginName_.data(), iParameter);
	visPtCutCorrParameters_.push_back(new massDepParameterType(iParameter, name, cfgPar));
      }
    }
  }
  virtual ~NSVfitSingleParticleLikelihood() 
  {
    delete visPtCutCorrFunction_;
    for ( std::vector<massDepParameterType*>::iterator it = visPtCutCorrParameters_.begin();
	  it != visPtCutCorrParameters_.end(); ++it ) {
      delete (*it);
    }
  }

  virtual void beginCandidate(const NSVfitSingleParticleHypothesis*) {}

  virtual double operator()(const NSVfitSingleParticleHypothesis*) const = 0;

 protected:
  std::string prodParticleLabel_;

  double evaluateVisPtCutCorrection(const NSVfitSingleParticleHypothesis* hypothesis) const
  {
//--- compute multiplicative correction to tau decay likelihood
//    in order to account for effect of visible Pt cuts on x (= visible/tau lepton enery) distribution
    double x = hypothesis->p4().energy()/hypothesis->p4_fitted().energy();
    if ( x < 0. ) x = 0.;
    if ( x > 1. ) x = 1.;
    double mass = hypothesis->p4_fitted().mass();
    if ( mass < visPtCutCorr_xMin_ ) mass = visPtCutCorr_xMin_;
    if ( mass > visPtCutCorr_xMax_ ) mass = visPtCutCorr_xMax_;
    for ( std::vector<massDepParameterType*>::const_iterator visPtCutCorrParameter = visPtCutCorrParameters_.begin();
	  visPtCutCorrParameter != visPtCutCorrParameters_.end(); ++visPtCutCorrParameter ) {
      visPtCutCorrFunction_->SetParameter((*visPtCutCorrParameter)->idx_, (*visPtCutCorrParameter)->evaluate(mass));
    }
    return visPtCutCorrFunction_->Eval(x);
  }

  struct massDepParameterType
  {
    massDepParameterType(int idx, const std::string& name, const edm::ParameterSet& cfg)
      : idx_(idx),
	function_(0)
    {
      std::string formula = cfg.getParameter<std::string>("formula");
      xMin_ = cfg.getParameter<double>("xMin");
      xMax_ = cfg.getParameter<double>("xMax");
      function_ = new TF1(name.data(), formula.data(), xMin_, xMax_);
      int numParameter = function_->GetNpar();
      for ( int iParameter = 0; iParameter < numParameter; ++iParameter ) {
	std::string parName = Form("par%i", iParameter);
	double parValue = cfg.getParameter<double>(parName);
	function_->SetParameter(iParameter, parValue);
      }
    }
    ~massDepParameterType() 
    { 
      delete function_; 
    }
    double evaluate(double mass) const
    {
      double x = mass;
      if ( x < xMin_ ) x = xMin_;
      if ( x > xMax_ ) x = xMax_;
      return function_->Eval(x);
    }
    int idx_;
    TF1* function_;
    double xMin_;
    double xMax_;
  };

  mutable TF1* visPtCutCorrFunction_;
  std::vector<massDepParameterType*> visPtCutCorrParameters_;
  double visPtCutCorr_xMin_;
  double visPtCutCorr_xMax_;
  bool applyVisPtCutCorrection_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitSingleParticleLikelihood* (const edm::ParameterSet&)> NSVfitSingleParticleLikelihoodPluginFactory;

#endif
