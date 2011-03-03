#include "TauAnalysis/CandidateTools/plugins/NSVfitAlgorithmByIntegration.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TMath.h>

using namespace SVfit_namespace;

double g(double* x, size_t dim, void* param)
{
  return TMath::Exp(-NSVfitAlgorithmBase::gNSVfitAlgorithm->nll(x, (double*)param));
}

NSVfitAlgorithmByIntegration::NSVfitAlgorithmByIntegration(const edm::ParameterSet& cfg)
  : NSVfitAlgorithmBase(cfg),
    fitParameterValues_(0),
    xl_(0),
    xu_(0),
    integrand_(0),
    workspace_(0),
    numMassParameters_(0),
    massParForReplacements_(0)
{
  edm::ParameterSet cfg_replacements = cfg.getParameter<edm::ParameterSet>("parameters");
  std::vector<std::string> replacementNames = cfg_replacements.getParameterNamesForType<edm::ParameterSet>();
  for ( std::vector<std::string>::const_iterator replacementName = replacementNames.begin();
	replacementName != replacementNames.end(); ++replacementName ) {
    edm::ParameterSet cfg_replacement = cfg_replacements.getParameter<edm::ParameterSet>(*replacementName);
    
    fitParameterReplacementType replacement;
    replacement.name_ = (*replacementName);
    replacement.iterLowerLimit_ = cfg_replacement.getParameter<double>("min");
    replacement.iterUpperLimit_ = cfg_replacement.getParameter<double>("max");
    replacement.iterStepSize_ = cfg_replacement.getParameter<double>("stepSize");

    std::string toReplace_string = cfg_replacement.getParameter<std::string>("replace");
    fitParameterType* fitParameterToReplace = getFitParameter(toReplace_string);
    if ( !fitParameterToReplace ) {
      throw cms::Exception("NSVfitAlgorithmByIntegration::NSVfitAlgorithmByIntegration")
	<< " No fitParameter of name = " << replace_string << " defined !!";
    }
    replacement.idxToReplace_ = fitParameterToReplace->idx_;

    replacement.idxMassParameter_ = numMassParameters_;
    ++numMassParameters_;

    std::string replaceBy_string = cfg_replacement.getParameter<std::string>("by");
    size_t pos_token0 = -1;
    size_t pos = 0;
    std::string formula_string = replaceBy_string;
    int errorFlag = 0;    
    replacement.numParForReplacements_ = 0;
    while ( pos < replaceBy_string.length() ) {
      if ( (replaceBy_string[pos] == '(' || replaceBy_string[pos] == ')' ||
	    replaceBy_string[pos] == '*' || replaceBy_string[pos] == '/' ||
	    replaceBy_string[pos] == '+' || replaceBy_string[pos] == '-' || pos == (replaceBy_string.length() - 1)) && 
	   (pos - pos_token0) > 1 ) {
	std::string token = std::string(replaceBy_string, pos_token0, pos - pos_token0);
	if        ( token == (*replacementName) ) {
	  formula_string = replace_string(formula_string, token, "x", 1, 1000, errorFlag);
	} else if ( getFitParameter(token) != 0 ) {
	  replaceParByFitParameter* parForReplacement = new replaceParByFitParameter();	  
	  parForReplacement->idx_ = getFitParameter(token)->idx_;
	  parForReplacement->iPar_ = replacement.numParForReplacements_;
	  replacement.parForReplacements_.push_back(parForReplacement);
	  ++replacement.numParForReplacements_;
	} else {
	  size_t posSeparator = token.find(".");
	  if ( posSeparator == std::string::npos ) {
	    throw cms::Exception("NSVfitAlgorithmByIntegration::NSVfitAlgorithmByIntegration")
	      << " Parameter token = " << token << " has invalid format;" 
	      << " expected format is 'daughter.type' !!";
	  }
	  
	  std::string resonance = std::string(token, posSeparator);
	  std::string value_string = std::string(token, posSeparator + 1);
	  replaceParByResonanceHypothesis* parForReplacement = new replaceParByResonanceHypothesis();	  
	  parForReplacement->resonanceName_ = resonance;
	  parForReplacement->valueExtractor_ = new StringObjectFunction<NSVfitResonanceHypothesis>(value_string);
	  parForReplacement->iPar_ = replacement.numParForReplacements_;
	  replacement.parForReplacements_.push_back(parForReplacement);
	  ++replacement.numParForReplacements_;
	} 
      }
      ++pos;
    }

    std::string formulaName = std::string(*replacementName).append("_formula");
    replacement.replaceBy_ = new TFormula(formulaName.data(), formula_string.data());
  }

  edm::ParameterSet cfg_vegas = cfg.getParameter<edm::ParameterSet>("vegasOptions");
  numCalls_ = cfg_vegas.getParameter<unsigned>("numCalls");

  massParForReplacements_ = new IndepCombinatoricsGeneratorT<double>(numMassParameters_);
  for ( unsigned iMassParameter = 0; iMassParameter < numMassParameters_; ++iMassParameter ) {
    const fitParameterReplacementType& fitParameterReplacement = fitParameterReplacements_[iMassParameter];
    massParForReplacements_->setLowerLimit(iMassParameter, fitParameterReplacement.iterLowerLimit_);
    massParForReplacements_->setUpperLimit(iMassParameter, fitParameterReplacement.iterUpperLimit_);
    massParForReplacements_->setStepSize(iMassParameter,   fitParameterReplacement.iterStepSize_);
  }
}

NSVfitAlgorithmByIntegration::~NSVfitAlgorithmByIntegration() 
{
  delete fitParameterValues_;

  delete [] xl_;
  delete [] xu_;

  delete [] (double*)integrand_->params;
  delete integrand_;
  gsl_monte_vegas_free(workspace_);
  gsl_rng_free(rnd_);

  delete massParForReplacements_;
}

void NSVfitAlgorithmByIntegration::beginJob()
{
  NSVfitAlgorithmBase::beginJob();
  
  numDimensions_ = 0;

  for ( std::vector<fitParameterType>::const_iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    bool isReplaced = false;
    for ( std::vector<fitParameterReplacementType>::const_iterator fitParameterReplacement = fitParameterReplacements_.begin();
	  fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
      if ( fitParameter->idx_ == fitParameterReplacement->idxToReplace_ ) isReplaced = true;
    }
    
    if ( !isReplaced ) {
      fitParameterByIntegrationType fitParameterByIntegration(*fitParameter);
      fitParameterByIntegration.idxByIntegration_ = numDimensions_;
      fitParametersByIntegration_.push_back(fitParameterByIntegration);
      ++numDimensions_;
    }
  }

  fitParameterValues_ = new double[fitParameters_.size()];

  xl_ = new double[numDimensions_];
  xu_ = new double[numDimensions_];
  
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    xl_[iDimension] = fitParametersByIntegration_[iDimension].lowerLimit_;
    xu_[iDimension] = fitParametersByIntegration_[iDimension].upperLimit_;
  }

  integrand_ = new gsl_monte_function;
  integrand_->f = &g;
  integrand_->dim = numDimensions_;
  integrand_->params = new double[numMassParameters_];
  workspace_ = gsl_monte_vegas_alloc(numDimensions_);
  gsl_rng_env_setup();
  rnd_ = gsl_rng_alloc(gsl_rng_default);
}

void NSVfitAlgorithmByIntegration::fitImp() const
{
  std::cout << "<NSVfitAlgorithmByIntegration::fitImp>:" << std::endl;

  gsl_monte_vegas_init(workspace_);
  massParForReplacements_->reset();

  while ( massParForReplacements_->isValid() ) {
//--- set mass parameters
    for ( unsigned iMassParameter = 0; iMassParameter < numMassParameters_; ++iMassParameter ) {
      ((double*)integrand_->params)[iMassParameter] = (*massParForReplacements_)[iMassParameter];
    }

//--- call VEGAS routine (part of GNU scientific library)
//    to perform actual integration
    double p, pErr;
    gsl_monte_vegas_integrate(integrand_, xl_, xu_, numDimensions_, numCalls_, rnd_, workspace_, &p, &pErr);

//--- copy result 
//   (CV: only debug print-out for now...)
    std::cout << "M = " << (*massParForReplacements_) << ": p = " << p << " +/- " << pErr << std::endl;

    massParForReplacements_->next();
  }
}

NSVfitAlgorithmBase::fitParameterType* NSVfitAlgorithmByIntegration::getFitParameter(const std::string& token)
{
  size_t posSeparator = token.find(".");
  if ( posSeparator == std::string::npos || posSeparator == (token.length() - 1) ) {
    throw cms::Exception("NSVfitAlgorithmByIntegration::getFitParameter")
      << " Parameter token = " << token << " passed as function argument has invalid format;" 
      << " expected format is 'daughter.type' !!";
  }

  std::string name = std::string(token, posSeparator);
  std::string type_string = std::string(token, posSeparator + 1);
  int type = -1;
  if ( type_string == "x" ) type = kTau_visEnFracX;
  else throw cms::Exception("NSVfitAlgorithmByIntegration::getFitParameter")
    << " Type = " << type << " not defined !!";

  return NSVfitAlgorithmBase::getFitParameter(name, type);
}

double NSVfitAlgorithmByIntegration::nll(double* x, double* param) const
{
//--- copy fitParameter
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    fitParameterValues_[fitParametersByIntegration_[iDimension].idx_] = x[iDimension];
  }

//--- set additional fitParameters according to mass parameter values
  for ( std::vector<fitParameterReplacementType>::const_iterator fitParameterReplacement = fitParameterReplacements_.begin();
	fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
    TFormula* formula = fitParameterReplacement->replaceBy_;

    for ( int iPar = 0; iPar < fitParameterReplacement->numParForReplacements_; ++iPar ) {
      formula->SetParameter(iPar, (*fitParameterReplacement->parForReplacements_[iPar])(x));
    }

//--- check if fitParameter is within limits;
//    return probability zero if not
    double fitParameterValue = formula->Eval(param[fitParameterReplacement->idxMassParameter_]);
    if ( fitParameterValue >= fitParameterLimits_[fitParameterReplacement->idxToReplace_].first  &&
	 fitParameterValue <= fitParameterLimits_[fitParameterReplacement->idxToReplace_].second ) {
      fitParameterValues_[fitParameterReplacement->idxToReplace_] = fitParameterValue;
    } else {
      return 0.;
    }
  }

//--- build event, resonance and particle hypotheses
  eventModel_->builder_->applyFitParameter(currentEventHypothesis_, fitParameterValues_);

//--- compute likelihood
  return eventModel_->nll(currentEventHypothesis_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitAlgorithmPluginFactory, NSVfitAlgorithmByIntegration, "NSVfitAlgorithmByIntegration");


