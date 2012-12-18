#include "TauAnalysis/CandidateTools/plugins/NSVfitAlgorithmByIntegration.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TPRegexp.h>

#include <limits>

using namespace SVfit_namespace;

enum { kMax, kMedian };

namespace 
{
  double g(double* x, size_t dim, void* param)
  {
    double nll = NSVfitAlgorithmBase::gNSVfitAlgorithm->nll(x, (const double*)param);
    double retVal = TMath::Exp(-nll);
    //static long callCounter = 0;
    //if ( (callCounter % 10000) == 0 ) 
    //  std::cout << "<g> (call = " << callCounter << "):" 
    //	  	  << " nll = " << nll << " --> returning retVal = " << retVal << std::endl;
    //++callCounter;
    return retVal;
  }
}

NSVfitAlgorithmByIntegration::NSVfitAlgorithmByIntegration(const edm::ParameterSet& cfg)
  : NSVfitAlgorithmBase(cfg),
    fitParameterValues_(0),
    xl_(0),
    xu_(0),
    integrand_(0),
    workspace_(0),
    rnd_(0),
    numMassParameters_(0),
    massParForReplacements_(0)
{
  edm::ParameterSet cfg_replacements = cfg.getParameter<edm::ParameterSet>("parameters");
  std::vector<std::string> replacementNames = cfg_replacements.getParameterNamesForType<edm::ParameterSet>();
  for ( std::vector<std::string>::const_iterator replacementName = replacementNames.begin();
	replacementName != replacementNames.end(); ++replacementName ) {
    edm::ParameterSet cfg_replacement = cfg_replacements.getParameter<edm::ParameterSet>(*replacementName);
    
    fitParameterReplacementType* replacement = new fitParameterReplacementType();
    replacement->name_ = (*replacementName);
    replacement->iterLowerLimit_ = cfg_replacement.getParameter<double>("min");
    replacement->iterUpperLimit_ = cfg_replacement.getParameter<double>("max");
    replacement->iterStepSizeFactor_ = cfg_replacement.getParameter<double>("stepSizeFactor");
    replacement->iterMinStepSize_ = cfg_replacement.getParameter<double>("minStepSize");

    std::string toReplace_string = cfg_replacement.getParameter<std::string>("replace");
    replacement->toReplace_ = toReplace_string;

    replacement->idxMassParameter_ = numMassParameters_;
    ++numMassParameters_;

    std::string replaceBy_string = cfg_replacement.getParameter<std::string>("by");
    replacement->replaceBy_ = 
      makeReplacementFormula(replaceBy_string, *replacementName, 
			     replacement->parForReplacements_, replacement->numParForReplacements_);
    
    std::string deltaFuncDerrivative_string = cfg_replacement.getParameter<std::string>("deltaFuncDerrivative");
    replacement->deltaFuncDerrivative_ = 
      makeReplacementFormula(deltaFuncDerrivative_string, *replacementName, 
			     replacement->parForDeltaFuncDerrivative_, replacement->numParForDeltaFuncDerrivative_);

    fitParameterReplacements_.push_back(replacement);
  }

  edm::ParameterSet cfg_vegas = cfg.getParameter<edm::ParameterSet>("vegasOptions");
  numCallsGridOpt_ = cfg_vegas.getParameter<unsigned>("numCallsGridOpt");
  numCallsIntEval_ = cfg_vegas.getParameter<unsigned>("numCallsIntEval");
  maxChi2_         = cfg_vegas.getParameter<double>("maxChi2");
  maxIntEvalIter_  = cfg_vegas.getParameter<unsigned>("maxIntEvalIter");
  precision_       = cfg_vegas.getParameter<double>("precision");

  std::string max_or_median_string = cfg.getParameter<std::string>("max_or_median");
  if      ( max_or_median_string == "max"    ) max_or_median_ = kMax;
  else if ( max_or_median_string == "median" ) max_or_median_ = kMedian;
  else throw cms::Exception("NSVfitAlgorithmByIntegration2")
    << " Invalid Configuration Parameter 'max_or_median' = " << max_or_median_string << " !!\n";
}

NSVfitAlgorithmByIntegration::~NSVfitAlgorithmByIntegration() 
{
  for( std::vector<fitParameterReplacementType*>::iterator it = fitParameterReplacements_.begin();
       it != fitParameterReplacements_.end(); ++it ) {
    delete (*it);
  }

  delete [] fitParameterValues_;

  delete [] xl_;
  delete [] xu_;

  if ( integrand_ ) delete [] (double*)integrand_->params;
  delete integrand_;
  if ( workspace_ ) gsl_monte_vegas_free(workspace_);
  if ( rnd_       ) gsl_rng_free(rnd_);

  delete massParForReplacements_;
}

void NSVfitAlgorithmByIntegration::beginJob()
{
  NSVfitAlgorithmBase::beginJob();
  
  for ( std::vector<fitParameterReplacementType*>::iterator fitParameterReplacement = fitParameterReplacements_.begin();
	fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
    (*fitParameterReplacement)->beginJob(this);
  } 

  numDimensions_ = 0;

  for ( std::vector<NSVfitParameter>::const_iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    bool isReplaced = false;
    for ( std::vector<fitParameterReplacementType*>::const_iterator fitParameterReplacement = fitParameterReplacements_.begin();
	  fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
      if ( fitParameter->index() == (*fitParameterReplacement)->idxToReplace_ ) isReplaced = true;
    }
    
    if ( !isReplaced ) {
      NSVfitParameterMappingType fitParameterMapping(&(*fitParameter));
      fitParameterMapping.idxByIntegration_ = numDimensions_;
      fitParameterMappings_.push_back(fitParameterMapping);
      ++numDimensions_;
    }
  }

  fitParameterValues_ = new double[fitParameters_.size()];

  xl_ = new double[numDimensions_];
  xu_ = new double[numDimensions_];

  integrand_ = new gsl_monte_function;
  integrand_->f = &g;
  integrand_->dim = numDimensions_;
  integrand_->params = new double[numMassParameters_];
  workspace_ = gsl_monte_vegas_alloc(numDimensions_);
  gsl_rng_env_setup();
  rnd_ = gsl_rng_alloc(gsl_rng_default);
}

void NSVfitAlgorithmByIntegration::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  NSVfitAlgorithmBase::beginEvent(evt, es);

  currentRunNumber_ = evt.id().run();
  currentLumiSectionNumber_ = evt.luminosityBlock();
  currentEventNumber_ = evt.id().event();
}

void NSVfitAlgorithmByIntegration::fitImp() const
{
  //std::cout << "<NSVfitAlgorithmByIntegration::fitImp>:" << std::endl;

  for ( std::vector<fitParameterReplacementType*>::const_iterator fitParameterReplacement = fitParameterReplacements_.begin();
	fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
    double minVisMass = -1.;
    for ( std::vector<replaceParBase*>::const_iterator par = (*fitParameterReplacement)->parForReplacements_.begin();
	  par != (*fitParameterReplacement)->parForReplacements_.end(); ++par ) {
      replaceParByResonanceHypothesis* par_resonance = dynamic_cast<replaceParByResonanceHypothesis*>(*par);
      if ( par_resonance ) {
	const NSVfitResonanceHypothesis* resonance = 
          dynamic_cast<const NSVfitResonanceHypothesis*>(
            currentEventHypothesis_->NSVfitEventHypothesisBase::resonance(par_resonance->resonanceName_));
	par_resonance->value_ = (*par_resonance->valueExtractor_)(*resonance);
	double visMass = resonance->p4().mass();
	if ( minVisMass == -1. || visMass < minVisMass ) minVisMass = visMass;
      }
    }
    (*fitParameterReplacement)->beginEvent(minVisMass);
  }

  delete massParForReplacements_;
  massParForReplacements_ = new IndepCombinatoricsGeneratorT<int>(numMassParameters_);
  for ( unsigned iMassParameter = 0; iMassParameter < numMassParameters_; ++iMassParameter ) {
    const fitParameterReplacementType* fitParameterReplacement = fitParameterReplacements_[iMassParameter];
    massParForReplacements_->setLowerLimit(iMassParameter, 0);
    massParForReplacements_->setUpperLimit(iMassParameter, fitParameterReplacement->gridPoints_->GetSize() - 1);
    massParForReplacements_->setStepSize(iMassParameter, 1);
  }

  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    xl_[iDimension] = fitParameterMappings_[iDimension].base_->LowerLimit(); 
    xu_[iDimension] = fitParameterMappings_[iDimension].base_->UpperLimit();
    if ( verbosity_ >= 2 ) { 
      std::cout << " fitParameter #" << iDimension << " (" << fitParameterMappings_[iDimension].base_->Name() << ":" 
    	        << fitParameterMappings_[iDimension].base_->Type() << "):"
    	        << " xl = " << xl_[iDimension] << ", xu = " << xu_[iDimension] << std::endl;
    }
  }

  TH1* histResults = 0;
  std::ostringstream histResultsName;
  histResultsName << pluginName_;
  histResultsName << "@" << currentRunNumber_ << ":" << currentLumiSectionNumber_ << ":" << currentEventNumber_;
  if ( numMassParameters_ == 1 ) {
    histResults = new TH1F(histResultsName.str().data(), histResultsName.str().data(), 
			   fitParameterReplacements_[0]->numGridPoints_, fitParameterReplacements_[0]->resBinning_->GetArray());
  } else if ( numMassParameters_ == 2 ) {
    histResults = new TH2F(histResultsName.str().data(), histResultsName.str().data(), 
			   fitParameterReplacements_[0]->numGridPoints_, fitParameterReplacements_[0]->resBinning_->GetArray(),
			   fitParameterReplacements_[1]->numGridPoints_, fitParameterReplacements_[1]->resBinning_->GetArray());
  } else {
    throw cms::Exception("NSVfitAlgorithmByIntegration::fitImp")
      << " Only fits in one or two dimensions supported yet "
      << " --> please contact the developers Evan Friis (friis@physics.ucdavis.edu) and Christian Veelken (christian.veelken@cern.ch)"
      << " and request support for more dimensions !!\n";
  }

  std::vector<double> massParameterValues(numMassParameters_);
  double pMax = 0.;
  unsigned numMassParBelowThreshold = 0;
  bool skipHighMassTail = false;

  while ( massParForReplacements_->isValid() ) {

//--- set mass parameters
    for ( unsigned iMassParameter = 0; iMassParameter < numMassParameters_; ++iMassParameter ) {
      int massParameterIdx = (*massParForReplacements_)[iMassParameter];
      double massParameterValue = fitParameterReplacements_[iMassParameter]->gridPoints_->At(massParameterIdx);
      ((double*)integrand_->params)[iMassParameter] = massParameterValue;
      massParameterValues[iMassParameter] = massParameterValue;
    }
    // CV: reset random number generator required by VEGAS (for what ?)
    //     for each event, in order to make mass reconstruction not depend on "processing history"    
    gsl_rng_set(rnd_, 12345); 

//--- call VEGAS routine (part of GNU scientific library)
//    to perform actual integration
    double p    = 0.; 
    double pErr = 0.;
    if ( !skipHighMassTail ) {
      gsl_monte_vegas_init(workspace_);
      workspace_->stage = 0;
      gsl_monte_vegas_integrate(integrand_, xl_, xu_, numDimensions_, 
				numCallsGridOpt_/workspace_->iterations, rnd_, workspace_, &p, &pErr);
      workspace_->stage = 1;

      // CV: repeat integration in case chi2 of estimated integral/uncertainty values
      //     indicates that result of integration cannot be trusted
      //    (up to maxIntEvalIter times in total)
      unsigned iteration = 0;
      double chi2 = -1.;
      do {
	gsl_monte_vegas_integrate(integrand_, xl_, xu_, numDimensions_, 
				  numCallsIntEval_/workspace_->iterations, rnd_, workspace_, &p, &pErr);
	workspace_->stage = 3;
	++iteration;
	//chi2 = gsl_monte_vegas_chisq(workspace_);
	chi2 = workspace_->chisq;
	//std::cout << " chi2 = " << chi2 << std::endl;
      } while ( chi2 > maxChi2_ && iteration < maxIntEvalIter_ );	
      
      if ( verbosity_ >= 2 ) {
	std::cout << "--> M = " << format_vdouble(massParameterValues) << ": p = " << p << " +/- " << pErr 
		  << " (chi2 = " << chi2 << ")" << std::endl;
      }
      
      // CV: in order to reduce computing time, skip precise computation of integral
      //     if in high mass tail and probability negligible anyway
      if ( p > pMax ) pMax = p;
      if ( pMax > 1.e-10 && (p + 3.*TMath::Abs(pErr)) < (pMax*precision_) ) ++numMassParBelowThreshold;
      else numMassParBelowThreshold = 0;
      if ( numMassParBelowThreshold >= 5 ) {
	//std::cout << " integral estimated to be negligible --> skipping integration." << std::endl;
	skipHighMassTail = true;
      }
    }

    if      ( numMassParameters_ == 1 ) histResults->Fill(massParameterValues[0], p);
    else if ( numMassParameters_ == 2 ) {
      TH2* histResults2d = dynamic_cast<TH2*>(histResults);
      assert(histResults2d);
      histResults2d->Fill(massParameterValues[0], massParameterValues[1], p);
    } else assert(0);

    massParForReplacements_->next();
  }

  NSVfitEventHypothesisByIntegration* persistentEventHypothesis = new NSVfitEventHypothesisByIntegration(*currentEventHypothesis_);
  persistentEventHypothesis->histMassResults_.reset(histResults);

//--- set central values and uncertainties on reconstructed masses
  for ( unsigned iMassParameter = 0; iMassParameter < numMassParameters_; ++iMassParameter ) {  
    const std::string& resonanceName = eventModel_->resonances_[iMassParameter]->resonanceName_;
    NSVfitResonanceHypothesisBase* resonance = 
      const_cast<NSVfitResonanceHypothesisBase*>(persistentEventHypothesis->NSVfitEventHypothesisBase::resonance(resonanceName));
    setMassResults(dynamic_cast<NSVfitResonanceHypothesisByIntegration*>(resonance), histResults, iMassParameter);
  }
  
  persistentEventHypothesis->mass_            = 0.;
  persistentEventHypothesis->massErrUp_       = 0.;
  persistentEventHypothesis->massErrDown_     = 0.;
  persistentEventHypothesis->isValidSolution_ = false;

  fittedEventHypothesis_ = persistentEventHypothesis;
  //fittedEventHypothesis_nll_ = eventModel_->nll(currentEventHypothesis_);
  double prob = 0.;
  //if ( histResults->Integral() > 0. ) prob = histResults->GetBinContent(histResults->GetMaximumBin())/histResults->Integral();
  prob = histResults->Integral();
  double nll;
  if ( prob > 0. ) nll = -TMath::Log(prob);
  else nll = std::numeric_limits<float>::max();
  fittedEventHypothesis_nll_ = nll;

  delete currentEventHypothesis_;
}

void NSVfitAlgorithmByIntegration::setMassResults(
       NSVfitResonanceHypothesisByIntegration* resonance, const TH1* histMassResults, unsigned iDimension) const
{
  const TH1* histMassResult1d_density = 0;
  if      ( histMassResults->GetDimension() == 1 ) histMassResult1d_density = histMassResults;
  else if ( histMassResults->GetDimension() == 2 ) {
    const TH2* histMassResults2d = dynamic_cast<const TH2*>(histMassResults);
    assert(histMassResults2d);
    if      ( iDimension == 0 ) histMassResult1d_density = histMassResults2d->ProjectionX();
    else if ( iDimension == 1 ) histMassResult1d_density = histMassResults2d->ProjectionY();
  }
  assert(histMassResult1d_density);

  if ( verbosity_ >= 2 ) { 
    for ( int iBin = 1; iBin <= histMassResult1d_density->GetNbinsX(); ++iBin ) {
      std::cout << " iBin " << iBin << " (" << histMassResult1d_density->GetBinCenter(iBin) <<  "):" 
  	        << " " << histMassResult1d_density->GetBinContent(iBin) << std::endl;
    }
  }
  
  std::string histMassResult1dName = std::string(histMassResult1d_density->GetName()).append("_cloned");
  TH1* histMassResult1d = (TH1*)histMassResult1d_density->Clone(histMassResult1dName.data());
  for ( int iBin = 1; iBin <= histMassResult1d->GetNbinsX(); ++iBin ) {
    double binContent = histMassResult1d_density->GetBinContent(iBin);
    double binError = histMassResult1d_density->GetBinError(iBin);
    double binWidth = histMassResult1d_density->GetBinWidth(iBin);
    histMassResult1d->SetBinContent(iBin, binContent*binWidth);
    histMassResult1d->SetBinError(iBin, binError*binWidth);
  }

  if ( histMassResult1d_density->Integral() > 0. ) {
    double massMaximum, massMaximum_interpol, massMean, massQuantile016, massQuantile050, massQuantile084;
    extractHistogramProperties(histMassResult1d, histMassResult1d_density,
			       massMaximum, massMaximum_interpol, massMean, massQuantile016, massQuantile050, massQuantile084);
    
    double mass;
    if      ( max_or_median_ == kMax    ) mass = massMaximum_interpol;
    else if ( max_or_median_ == kMedian ) mass = massQuantile050;
    else assert(0);
    double massErrUp   = TMath::Abs(massQuantile084 - mass);
    double massErrDown = TMath::Abs(mass - massQuantile016);
    NSVfitAlgorithmBase::setMassResults(resonance, mass, massErrUp, massErrDown);

    resonance->massMean_ = massMean;
    resonance->massMedian_ = massQuantile050;
    resonance->massMaximum_ = massMaximum;
    resonance->massMaxInterpol_ = massMaximum_interpol;
    resonance->isValidSolution_ = true;
    
    if ( verbosity_ >= 1 ) { 
      std::cout << "<NSVfitAlgorithmByIntegration::setMassResults>:" << std::endl;
      std::cout << " pluginName = " << pluginName_ << std::endl;
      std::cout << "--> mass = " << resonance->mass_ << std::endl;
      std::cout << " (mean = " << massMean << ", median = " << massQuantile050 << ", max = " << massMaximum << ")" << std::endl;
      //resonance->print(std::cout);
    }
  } else {
    edm::LogWarning("NSVfitAlgorithmByIntegration::setMassResults")
      << "Likelihood functions returned Probability zero for all tested mass hypotheses --> no valid solution found !!";
    resonance->isValidSolution_ = false;
  }
  
  if ( histMassResult1d_density != histMassResults ) delete histMassResult1d_density;
  delete histMassResult1d;
}

bool NSVfitAlgorithmByIntegration::isDaughter(const std::string& daughterName)
{
  bool isDaughter = false;

  for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	resonance != eventModel_->resonances_.end(); ++resonance ) {
    for ( std::vector<daughterModelType*>::const_iterator daughter = (*resonance)->daughters_.begin();
	  daughter != (*resonance)->daughters_.end(); ++daughter ) {
      if ( (*daughter)->daughterName_ == daughterName ) isDaughter = true;
    }
  }

  return isDaughter;
}
 
bool NSVfitAlgorithmByIntegration::isResonance(const std::string& resonanceName)
{
  bool isResonance = false;

  for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	resonance != eventModel_->resonances_.end(); ++resonance ) {
    if ( (*resonance)->resonanceName_ == resonanceName ) isResonance = true;
  }

  return isResonance;
}

TFormula* NSVfitAlgorithmByIntegration::makeReplacementFormula(
            const std::string& expression, const std::string& replacementName,
	    std::vector<replaceParBase*>& parForReplacements, int& numParForReplacements)
{
  size_t pos_token0 = -1;
  size_t pos = 0;
  std::set<std::string> tokens;
  while ( pos < expression.length() ) {
    bool isSymbol = (expression[pos] == '(' || expression[pos] == ')' ||
		     expression[pos] == '*' || expression[pos] == '/' ||
		     expression[pos] == '+' || expression[pos] == '-');      
    if ( (isSymbol || pos == (expression.length() - 1)) && (pos - pos_token0) > 1 ) {
      size_t num = ( pos != (expression.length() - 1) ) ? pos - (pos_token0 + 1) : pos - pos_token0;
      std::string token = std::string(expression, pos_token0 + 1, num);	
//--- skip tokens that are constant numbers     
      std::string regexpParser_notNumber_string = std::string("[^0-9.]+");
      TPRegexp regexpParser_notNumber(regexpParser_notNumber_string.data());
      if ( regexpParser_notNumber.Match(token.data()) ) {
	//std::cout << "adding token = " << token << std::endl;
	tokens.insert(token);
      } else {
	//std::cout << "skipping token = " << token << std::endl;
      }
    }
    if ( isSymbol ) pos_token0 = pos;
    ++pos;
  }

  std::string formula_string = expression;
  int errorFlag = 0;    
  numParForReplacements = 0;
  for ( std::set<std::string>::const_iterator token = tokens.begin();
	token != tokens.end(); ++token ) {
    if ( (*token) == replacementName ) {
      formula_string = replace_string(formula_string, *token, "x", 0, 1000, errorFlag);
    } else {
      size_t posSeparator = token->find(".");
      if ( posSeparator == std::string::npos ) {
	throw cms::Exception("NSVfitAlgorithmByIntegration::NSVfitAlgorithmByIntegration")
	  << " Parameter token = " << (*token) << " has invalid format;" 
	  << " expected format is 'daughter.type' !!\n";
      }
	
      std::string particleName = std::string(*token, 0, posSeparator);
      std::string value_string = std::string(*token, posSeparator + 1);
	
      if ( isDaughter(particleName)  ) {
	replaceParByFitParameter* parForReplacement = new replaceParByFitParameter();	  
	parForReplacement->fitParameterName_ = (*token);
	parForReplacement->iPar_ = numParForReplacements;
	std::ostringstream par_string;
	par_string << "[" << parForReplacement->iPar_ << "]";
	formula_string = replace_string(formula_string, *token, par_string.str(), 0, 1000, errorFlag);
	parForReplacements.push_back(parForReplacement);
	++numParForReplacements;
      } else if ( isResonance(particleName) ) {
	replaceParByResonanceHypothesis* parForReplacement = new replaceParByResonanceHypothesis();	  
	parForReplacement->resonanceName_ = particleName;
	parForReplacement->valueExtractor_ = new StringObjectFunction<NSVfitResonanceHypothesis>(value_string);
	parForReplacement->iPar_ = numParForReplacements;
	std::ostringstream par_string;
	par_string << "[" << parForReplacement->iPar_ << "]";
	formula_string = replace_string(formula_string, *token, par_string.str(), 0, 1000, errorFlag);
	parForReplacements.push_back(parForReplacement);
	++numParForReplacements;
      } else {
	throw cms::Exception("NSVfitAlgorithmByIntegration::NSVfitAlgorithmByIntegration")
	  << " No resonance/daughter of name = " << particleName << " defined !!\n";
      }
    } 
  }

  std::string formulaName = std::string(replacementName).append("_formula");    
  TFormula* formula = new TFormula(formulaName.data(), formula_string.data());

  return formula;
}

NSVfitParameter* NSVfitAlgorithmByIntegration::getFitParameter(const std::string& token)
{
  size_t posSeparator = token.find(".");
  if ( posSeparator == std::string::npos || posSeparator == (token.length() - 1) ) {
    throw cms::Exception("NSVfitAlgorithmByIntegration::getFitParameter")
      << " Parameter token = " << token << " passed as function argument has invalid format;" 
      << " expected format is 'daughter.type' !!\n";
  }

  std::string name = std::string(token, 0, posSeparator);
  std::string type_string = std::string(token, posSeparator + 1);
  int type = -1;
  if ( type_string == "x" ) type = nSVfit_namespace::kTau_visEnFracX;
  else throw cms::Exception("NSVfitAlgorithmByIntegration::getFitParameter")
    << " Type = " << type << " not defined !!\n";

  return NSVfitAlgorithmBase::getFitParameter(name, type);
}

double NSVfitAlgorithmByIntegration::nll(const double* x, const double* param) const
{
//--- copy fitParameter
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    fitParameterValues_[fitParameterMappings_[iDimension].base_->index()] = x[iDimension];
  }

//--- set additional fitParameters according to mass parameter values
  for ( std::vector<fitParameterReplacementType*>::const_iterator fitParameterReplacement = fitParameterReplacements_.begin();
	fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
    TFormula* formula = (*fitParameterReplacement)->replaceBy_;
    //std::cout << "formula = " << formula->GetTitle() << std::endl;

    for ( int iPar = 0; iPar < (*fitParameterReplacement)->numParForReplacements_; ++iPar ) {
      formula->SetParameter(iPar, (*(*fitParameterReplacement)->parForReplacements_[iPar])(fitParameterValues_));
      //std::cout << "par #" << iPar << " = " << formula->GetParameter(iPar) << std::endl;
    }

//--- check if fitParameter is within limits;
//    return probability zero if not
    double fitParameterValue = formula->Eval(param[(*fitParameterReplacement)->idxMassParameter_]);
    //std::cout << "value = " << fitParameterValue << std::endl;
    if ( fitParameterValue >= fitParameters_[(*fitParameterReplacement)->idxToReplace_].LowerLimit() &&
	 fitParameterValue <= fitParameters_[(*fitParameterReplacement)->idxToReplace_].UpperLimit() ) {
      fitParameterValues_[(*fitParameterReplacement)->idxToReplace_] = fitParameterValue;
    } else {
      return std::numeric_limits<float>::max();
    }
  }

//--- build event, resonance and particle hypotheses
  currentEventHypothesis_isValidSolution_ = eventModel_->builder_->applyFitParameter(currentEventHypothesis_, fitParameterValues_);

//--- compute likelihood;
//    add derrivatives of delta functions for each fit parameter that is replaced in integration
//   (cf. http://en.wikipedia.org/wiki/Dirac_delta_function )
  double nll = std::numeric_limits<float>::max();
  if ( currentEventHypothesis_isValidSolution_ ) {
    nll = eventModel_->nll(currentEventHypothesis_);

    for ( std::vector<fitParameterReplacementType*>::const_iterator fitParameterReplacement = fitParameterReplacements_.begin();
	  fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
      TFormula* formula = (*fitParameterReplacement)->deltaFuncDerrivative_;
      //std::cout << "formula = " << formula->GetTitle() << std::endl;

      for ( int iPar = 0; iPar < (*fitParameterReplacement)->numParForDeltaFuncDerrivative_; ++iPar ) {
	formula->SetParameter(iPar, (*(*fitParameterReplacement)->parForDeltaFuncDerrivative_[iPar])(fitParameterValues_));
	//std::cout << "par #" << iPar << " = " << formula->GetParameter(iPar) << std::endl;
      }

      double probCorr = formula->Eval(param[(*fitParameterReplacement)->idxMassParameter_]);
      //std::cout << "value = " << probCorr << std::endl;
      if ( probCorr > 0. && !TMath::IsNaN(probCorr) ) {
	nll -= TMath::Log(probCorr);
      }
    }
  }

  return nll;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitAlgorithmPluginFactory, NSVfitAlgorithmByIntegration, "NSVfitAlgorithmByIntegration");


