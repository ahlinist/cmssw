#ifndef TauAnalysis_CandidateTools_NSVfitSingleParticleLikelihood_h
#define TauAnalysis_CandidateTools_NSVfitSingleParticleLikelihood_h

/** \class NSVfitSingleParticleLikelihoodBase
 *
 * Abstract base-class for plugins computing likelihood of single particle kinematics;
 * used by nSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.8 $
 *
 * $Id: NSVfitSingleParticleLikelihood.h,v 1.8 2012/03/07 15:45:01 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitLikelihoodBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include <TF1.h>
#include <TFormula.h>
#include <TString.h>

#include <string>
#include <iostream>

class NSVfitSingleParticleLikelihood : public NSVfitLikelihoodBase
{
 public:
  NSVfitSingleParticleLikelihood(const edm::ParameterSet& cfg)
    : NSVfitLikelihoodBase(cfg),
      prodParticleLabel_(cfg.getParameter<std::string>("prodParticleLabel")),
      visPtCutCorrFunction_(0),
      visPtCutCorrFunctionOLD_(0)
  {
    //-----------------------------------------------------------------------------
    // CV: NEW method for visible Pt cut correction
    applyVisPtCutCorrection_ = ( cfg.exists("applyVisPtCutCorrection") ) ?
      cfg.getParameter<bool>("applyVisPtCutCorrection") : false;
    if ( applyVisPtCutCorrection_ ) {
      edm::ParameterSet cfgVisPtCutCorrection = cfg.getParameter<edm::ParameterSet>("visPtCutCorrection");
      std::string visPtCutCorrFunctionName = Form("%s_visPtCutCorrFunction", pluginName_.data());
      visPtCutCorrFunction_ = new visPtCutCorrType_formula(visPtCutCorrFunctionName.data(), cfgVisPtCutCorrection);
    }
    //-----------------------------------------------------------------------------
    
    //-----------------------------------------------------------------------------
    // CV: OLD method for visible Pt cut correction
    applyVisPtCutCorrectionOLD_ = ( cfg.exists("applyVisPtCutCorrectionOLD") ) ?
      cfg.getParameter<bool>("applyVisPtCutCorrectionOLD") : false;
    if ( applyVisPtCutCorrectionOLD_ ) {
      applyVisPtCutCorrection_ = true; // CV: temporary hack (for "old" visPtCut correction to be called from derrived classes)
      edm::ParameterSet cfgVisPtCutCorrection = cfg.getParameter<edm::ParameterSet>("visPtCutCorrectionOLD");
      std::string formula = cfgVisPtCutCorrection.getParameter<std::string>("formula");
      visPtCutCorrOLD_xMin_ = cfgVisPtCutCorrection.getParameter<double>("xMin");
      visPtCutCorrOLD_xMax_ = cfgVisPtCutCorrection.getParameter<double>("xMax");
      std::string name = Form("%s_visPtCutCorrFunctionOLD", pluginName_.data());
      visPtCutCorrFunctionOLD_ = new TF1(name.data(), formula.data(), visPtCutCorrOLD_xMin_, visPtCutCorrOLD_xMax_);
      int numParameter = visPtCutCorrFunctionOLD_->GetNpar();
      for ( int iParameter = 0; iParameter < numParameter; ++iParameter ) {
	std::string parName = Form("par%i", iParameter);
	edm::ParameterSet cfgPar = cfgVisPtCutCorrection.getParameter<edm::ParameterSet>(parName);
	std::string name = Form("%s_visPtCutCorrFunctionOLD_par%i", pluginName_.data(), iParameter);
	visPtCutCorrOLD_parameters_.push_back(new massDepParameterType(iParameter, name, cfgPar));
      }
    }
  }

  virtual ~NSVfitSingleParticleLikelihood() 
  {
    //-----------------------------------------------------------------------------
    // CV: NEW method for visible Pt cut correction
    delete visPtCutCorrFunction_;
    //-----------------------------------------------------------------------------
    
    //-----------------------------------------------------------------------------
    // CV: OLD method for visible Pt cut correction
    delete visPtCutCorrFunctionOLD_;
    for ( std::vector<massDepParameterType*>::iterator it = visPtCutCorrOLD_parameters_.begin();
	  it != visPtCutCorrOLD_parameters_.end(); ++it ) {
      delete (*it);
    }
    //-----------------------------------------------------------------------------
  }

  virtual void beginCandidate(const NSVfitSingleParticleHypothesis*) {}

  virtual double operator()(const NSVfitSingleParticleHypothesis*) const = 0;

 protected:
  std::string prodParticleLabel_;

  //-----------------------------------------------------------------------------
  // CV: NEW method for visible Pt cut correction
  double evaluateVisPtCutCorrection(const NSVfitSingleParticleHypothesis* hypothesis) const
  {
//--- compute multiplicative correction to tau decay likelihood
//    in order to account for effect of visible Pt cuts on x (= visible/tau lepton enery) distribution
    if ( applyVisPtCutCorrectionOLD_ ) return evaluateVisPtCutCorrectionOLD(hypothesis);
    else return visPtCutCorrFunction_->eval(hypothesis);
  }

  struct visPtCutCorrTypeBase
  {
    visPtCutCorrTypeBase(const std::string& name, const edm::ParameterSet& cfg)
      : function_(0)
    {
      variable_ = cfg.getParameter<std::string>("variable");
      std::string formula = cfg.getParameter<std::string>("formula");
      std::string functionName = Form("%s_formula", name.data());
      function_ = new TFormula(functionName.data(), formula.data());
      xMin_ = cfg.getParameter<double>("xMin");
      xMax_ = cfg.getParameter<double>("xMax");
      numParameter_ = function_->GetNpar();           
    }
    virtual ~visPtCutCorrTypeBase() 
    {
      delete function_;
    }

    virtual double eval(const NSVfitSingleParticleHypothesis* hypothesis) const = 0;
    
    double get_x(const NSVfitSingleParticleHypothesis* hypothesis) const
    {
      double x = 0.;
      if ( variable_ == "X"    ) {
	x = hypothesis->p4().energy()/hypothesis->p4_fitted().energy();
      } else if ( variable_ == "dPhi" ) {
	const NSVfitResonanceHypothesis* resonance = hypothesis->mother();
	assert(resonance);	
	x = resonance->dPhiVis();
      } else if ( variable_ == "mass" ) {
	const NSVfitResonanceHypothesis* resonance = hypothesis->mother();
	assert(resonance);	
	x = resonance->p4_fitted().mass();
      } else assert(0);
      if      ( x < xMin_ ) x = xMin_;
      else if ( x > xMax_ ) x = xMax_;
      return x;
    }
    
    std::string variable_;
    TFormula* function_;
    double xMin_;
    double xMax_;
    int numParameter_;
  };

  struct visPtCutCorrType_const : public visPtCutCorrTypeBase
  {
    visPtCutCorrType_const(const std::string& name, const edm::ParameterSet& cfg)
      : visPtCutCorrTypeBase(name, cfg)
    {      
      edm::ParameterSet cfgPars = cfg.getParameter<edm::ParameterSet>("parameter");
      parameter_.resize(numParameter_);
      for ( int iParameter = 0; iParameter < numParameter_; ++iParameter ) {
	parameter_[iParameter] = cfgPars.getParameter<double>(Form("par%i", iParameter));
	function_->SetParameter(iParameter, parameter_[iParameter]);
      }
    }
    ~visPtCutCorrType_const() 
    {}

    double eval(const NSVfitSingleParticleHypothesis* hypothesis) const
    {
      double x = get_x(hypothesis);
      return function_->Eval(x);   
    }

    std::vector<double> parameter_;
  };

  struct visPtCutCorrType_formula : public visPtCutCorrTypeBase
  {
    visPtCutCorrType_formula(const std::string& name, const edm::ParameterSet& cfg)
      : visPtCutCorrTypeBase(name, cfg)
    {    
      edm::ParameterSet cfgPars = cfg.getParameter<edm::ParameterSet>("parameter");
      parameter_.resize(numParameter_);
      for ( int iParameter = 0; iParameter < numParameter_; ++iParameter ) {
	edm::ParameterSet cfgPar = cfgPars.getParameter<edm::ParameterSet>(Form("par%i", iParameter));
	std::string parName = Form("%s_par%i", name.data(), iParameter);
	// CV: determine type of parameters on next level
	edm::ParameterSet cfgPar_next = cfgPar.getParameter<edm::ParameterSet>("parameter");
	if ( cfgPar_next.existsAs<double>("par0") ) {
	  parameter_[iParameter] = new visPtCutCorrType_const(parName, cfgPar);
	} else if ( cfgPar_next.existsAs<edm::ParameterSet>("par0") ) {
	  parameter_[iParameter] = new visPtCutCorrType_formula(parName, cfgPar);
	} else throw cms::Exception("NSVfitSingleParticleLikelihood") 
	    << "Failed to determine type of parameter needed by function = " << function_->GetName() << " !!\n";
      }
    }
    ~visPtCutCorrType_formula() 
    {
      for ( std::vector<visPtCutCorrTypeBase*>::iterator it = parameter_.begin();
	    it != parameter_.end(); ++it ) {
	delete (*it);
      }
    }

    double eval(const NSVfitSingleParticleHypothesis* hypothesis) const
    {      
      for ( int iParameter = 0; iParameter < numParameter_; ++iParameter ) {
        function_->SetParameter(iParameter, parameter_[iParameter]->eval(hypothesis));
      }
      double x = get_x(hypothesis);
      return function_->Eval(x);      
    }

    std::vector<visPtCutCorrTypeBase*> parameter_;
  };

  mutable visPtCutCorrType_formula* visPtCutCorrFunction_;
  bool applyVisPtCutCorrection_;
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // CV: OLD method for visible Pt cut correction
  double evaluateVisPtCutCorrectionOLD(const NSVfitSingleParticleHypothesis* hypothesis) const
  {
//--- compute multiplicative correction to tau decay likelihood
//    in order to account for effect of visible Pt cuts on x (= visible/tau lepton enery) distribution
    double x = hypothesis->p4().energy()/hypothesis->p4_fitted().energy();
    if ( x < 0. ) x = 0.;
    if ( x > 1. ) x = 1.;
    //double mass = hypothesis->p4_fitted().mass(); // original (buggy) version
    double mass = hypothesis->mother()->p4_fitted().mass(); // corrected (bug-fixed) version
    if ( mass < visPtCutCorrOLD_xMin_ ) mass = visPtCutCorrOLD_xMin_;
    if ( mass > visPtCutCorrOLD_xMax_ ) mass = visPtCutCorrOLD_xMax_;
    for ( std::vector<massDepParameterType*>::const_iterator visPtCutCorrParameter = visPtCutCorrOLD_parameters_.begin();
	  visPtCutCorrParameter != visPtCutCorrOLD_parameters_.end(); ++visPtCutCorrParameter ) {
      visPtCutCorrFunctionOLD_->SetParameter((*visPtCutCorrParameter)->idx_, (*visPtCutCorrParameter)->evaluate(mass));
    }
    return visPtCutCorrFunctionOLD_->Eval(x);
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

  mutable TF1* visPtCutCorrFunctionOLD_;
  std::vector<massDepParameterType*> visPtCutCorrOLD_parameters_;
  double visPtCutCorrOLD_xMin_;
  double visPtCutCorrOLD_xMax_;
  bool applyVisPtCutCorrectionOLD_;
  //-----------------------------------------------------------------------------
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitSingleParticleLikelihood* (const edm::ParameterSet&)> NSVfitSingleParticleLikelihoodPluginFactory;

#endif
