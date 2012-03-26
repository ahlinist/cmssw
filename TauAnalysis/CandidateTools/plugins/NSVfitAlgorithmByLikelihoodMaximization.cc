#include "TauAnalysis/CandidateTools/plugins/NSVfitAlgorithmByLikelihoodMaximization.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <Math/Minimizer.h>
#include <Math/Factory.h>
#include <Math/Functor.h>

#include <TMath.h>

#include <algorithm>
#include <limits>

using namespace SVfit_namespace;

const unsigned defaultMaxObjFunctionCalls = 5000;

NSVfitAlgorithmByLikelihoodMaximization::NSVfitAlgorithmByLikelihoodMaximization(const edm::ParameterSet& cfg)
  : NSVfitAlgorithmBase(cfg),
    minimizer_(0)
{
  typedef std::vector<std::string> vstring;
  vstring minimizer_vstring = cfg.getParameter<vstring>("minimizer");
  if ( minimizer_vstring.size() != 2 ) {
    throw cms::Exception("NSVfitAlgorithmByLikelihoodMaximization::NSVfitAlgorithmByLikelihoodMaximization")
      << " Configuration parameter 'minimizer' has invalid format !!\n";
  }
  minimizer_ = ROOT::Math::Factory::CreateMinimizer(minimizer_vstring[0], minimizer_vstring[1]);

  maxObjFunctionCalls_ = ( cfg.exists("maxObjFunctionCalls") ) ?
    cfg.getParameter<unsigned>("maxObjFunctionCalls") : defaultMaxObjFunctionCalls;
}

NSVfitAlgorithmByLikelihoodMaximization::~NSVfitAlgorithmByLikelihoodMaximization()
{
  delete minimizer_;
}

void NSVfitAlgorithmByLikelihoodMaximization::fitImp() const
{
  //std::cout << "<NSVfitAlgorithmByLikelihoodMaximization::fitImp>:" << std::endl;
  //std::cout << " #fitParameter = " << fitParameters_.size() << std::endl;

  minimizer_->Clear();

  //minimizer_->SetPrintLevel(3);
  minimizer_->SetPrintLevel(-1);
  // Make sure the variables are sorted by index
  ROOT::Math::Functor toMinimize(objectiveFunctionAdapter_, fitParameters_.size());
  minimizer_->SetFunction(toMinimize);
  minimizer_->SetMaxFunctionCalls(maxObjFunctionCalls_);

//--- set Minuit strategy = 2, in order to get reliable error estimates:
//    http://www-cdf.fnal.gov/physics/statistics/recommendations/minuit.html
  minimizer_->SetStrategy(2);

//--- compute uncertainties for increase of objective function by 0.5 wrt. minimum
//   (objective function is log-likelihood function)
  minimizer_->SetErrorDef(0.5);

//--- disable fitParameter limits for azimuthal angles ("cyclic" variables)
//    by setting upper and lower limits to "non-a-number" (NaN)
  idxFitParametersPhi_.clear();
  for ( std::vector<NSVfitParameter>::iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    int fitParameterType = fitParameter->Type();
    if ( fitParameterType == nSVfit_namespace::kTau_phi_lab ||
	 fitParameterType == nSVfit_namespace::kNu_phi_lab  ||
	 fitParameterType == nSVfit_namespace::kW_phi_lab   ) {
      double limit_disabled = std::numeric_limits<float>::quiet_NaN(); // CMSSSW_4_2_x version
      //double limit_disabled = TMath::QuietNaN();                     // CMSSSW_4_4_x version
      fitParameter->setLowerLimit(limit_disabled);
      fitParameter->setUpperLimit(limit_disabled);
      idxFitParametersPhi_.push_back(fitParameter->index());
    }

    fitParameter->reset();
  }

  std::sort(fitParameters_.begin(), fitParameters_.end());
  setupVariables(fitParameters_.begin(), fitParameters_.end());

  idxObjFunctionCall_ = 0;

  if ( verbosity_ ) {
    std::cout << "--> starting ROOT::Math::Minimizer::Minimize..." << std::endl;
    std::cout << " #freeParameters = " << minimizer_->NFree() << ","
  	      << " #constrainedParameters = " << (minimizer_->NDim() - minimizer_->NFree()) << std::endl;
  }
  minimizer_->Minimize();
  if ( verbosity_ ) minimizer_->PrintResults();

//--- set best-fit parameters in event, resonance and particle hypotheses
  const double* bestFitParameterValues = minimizer_->X();
  const double* bestFitParameterErrors = minimizer_->Errors();

  eventModel_->builder_->applyFitParameter(currentEventHypothesis_, bestFitParameterValues);

  for ( std::vector<NSVfitParameter>::iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    fitParameter->setValue(bestFitParameterValues[fitParameter->index()]);
    fitParameter->setErrUp(bestFitParameterErrors[fitParameter->index()]);
    fitParameter->setErrDown(bestFitParameterErrors[fitParameter->index()]);
  }

  unsigned numFitParameter = fitParameters_.size();
  covMatrix_.ResizeTo(numFitParameter, numFitParameter);
  for ( unsigned iRow = 0; iRow < numFitParameter; ++iRow ) {
    for ( unsigned iColumn = 0; iColumn < numFitParameter; ++iColumn ) {
      covMatrix_(iRow, iColumn) = minimizer_->CovMatrix(iRow, iColumn);
    }
  } 

//--- get Minimizer status code, check if solution is valid:
//
//    0: Valid solution
//    1: Covariance matrix was made positive definite
//    2: Hesse matrix is invalid
//    3: Estimated distance to minimum (EDM) is above maximum
//    4: Reached maximum number of function calls before reaching convergence
//    5: Any other failure
//
  int fitStatus = minimizer_->Status();
  //std::cout << " fitStatus = " << fitStatus << std::endl;

  bool isValidSolution = (fitStatus == 0);

  size_t numResonances = currentEventHypothesis_->numResonances();
  for ( size_t iResonance = 0; iResonance < numResonances; ++iResonance ) {
    NSVfitResonanceHypothesis* resonance = 
      dynamic_cast<NSVfitResonanceHypothesis*>(currentEventHypothesis_->resonance(iResonance));
    resonance->isValidSolution_ = isValidSolution;
    setMassResults(*resonance);
  }

  fittedEventHypothesis_ = currentEventHypothesis_;
  fittedEventHypothesis_nll_ = eventModel_->nll(currentEventHypothesis_);
}

void NSVfitAlgorithmByLikelihoodMaximization::setMassResults(NSVfitResonanceHypothesis& resonance) const
{
  resonance.mass_ = resonance.p4_fitted().mass();
  
  double massErrUp = 0.;
  double massErrDown = 0.;
  bool isMassErrComputed = false;
  if ( resonance.numDaughters() == 2 ) {
//--- special case for computing mass uncertainties:
//    resonance has two daughters, both of which are either hadronic or leptonic tau decays
    NSVfitSingleParticleHypothesisBase* daughter1 = resonance.daughter(0);
    assert(daughter1);
    const std::string daughter1Name = daughter1->name();
    NSVfitSingleParticleHypothesisBase* daughter2 = resonance.daughter(1);
    assert(daughter2);
    const std::string daughter2Name = daughter2->name();
    if ( getFitParameter(daughter1Name, nSVfit_namespace::kTau_visEnFracX) != 0 &&
	 getFitParameter(daughter2Name, nSVfit_namespace::kTau_visEnFracX) != 0 ) {
      NSVfitParameter* fitParameter1 = getFitParameter(daughter1Name, nSVfit_namespace::kTau_visEnFracX);  
      NSVfitParameter* fitParameter2 = getFitParameter(daughter2Name, nSVfit_namespace::kTau_visEnFracX);
    
//--- compute uncertainty on product X1*X2 of visible energy fractions
//   (formula 6 of: George W. Bohrnstedt, Arthur A. Goldberger,
//                  "On the exact covariance of products of random variables",
//                  Journal of the American Statistical Association, Vol. 64, No. 328 (1969), pp. 1439-1442)
      double X1 = fitParameter1->Value();
      double sigmaX1 = TMath::Sqrt(0.5*(square(fitParameter1->ErrDown()) + square(fitParameter1->ErrUp())));
      //std::cout << "X1 = " << X1 << " +/- " << sigmaX1 << std::endl;
      double X2 = fitParameter2->Value();
      double sigmaX2 = TMath::Sqrt(0.5*(square(fitParameter2->ErrDown()) + square(fitParameter2->ErrUp())));
      //std::cout << "X2 = " << X2 << " +/- " << sigmaX2 << std::endl;
      double covX1X2 = covMatrix_(fitParameter1->index(), fitParameter2->index());
      //std::cout << "corr(X1,X2) = " << TMath::Sqrt(covX1X2/(sigmaX1*sigmaX2)) << std::endl;
      double X1timesX2err2 = square(X1*sigmaX2) + square(sigmaX1*X2) 
                            + 2.*X1*X2*covX1X2 + square(sigmaX1*sigmaX2) + square(covX1X2);
      //std::cout << "X1*X2 = " << (X1*X2) << " +/- " << TMath::Sqrt(X1timesX2err2) << std::endl;
      
//--- compute uncertainty on 1/mass^2:
//
//      mass^2 = visMass^2/(X1*X2)
//
//   --> sigma(1/mass^2)/(1/mass^2) = sigma(X1*X2)/(X1*X2)
//
      double massInv2RelErr = TMath::Sqrt(X1timesX2err2)/(X1*X2);
      double massInv = 1./resonance.mass_;
      double massInv2 = square(massInv);
      double massInv2Up = massInv2*(1. + massInv2RelErr);
      double massInv2Down = massInv2*(1. - massInv2RelErr);
      double massInvUp = TMath::Sqrt(massInv2Up);
      double visMassInv = massInv/(X1*X2);
      if ( massInvUp   > visMassInv ) massInvUp   = visMassInv;
      double massInvDown = TMath::Sqrt(massInv2Down);
      const double epsilon = 1.e-6;
      if ( massInvDown < epsilon ) massInvDown = epsilon;
      massErrUp = (1./massInvDown) - resonance.mass_;
      massErrDown = resonance.mass_ - (1./massInvUp);
      //std::cout << "(1): massErrUp = " << massErrUp << ", massErrDown = " << massErrDown << std::endl;
      isMassErrComputed = true;
    }
  }

  if ( !isMassErrComputed ) {
    double massRelErrUp2 = 0.;
    double massRelErrDown2 = 0.;
    for ( edm::OwnVector<NSVfitSingleParticleHypothesisBase>::iterator daughter = resonance.daughters_.begin();
	  daughter != resonance.daughters_.end(); ++daughter ) {
      const std::string daughterName = daughter->name();
      if ( getFitParameter(daughterName, nSVfit_namespace::kTau_visEnFracX) != 0 ) {
	NSVfitParameter* fitParameter = getFitParameter(daughterName, nSVfit_namespace::kTau_visEnFracX);        
	massRelErrUp2   += square(0.5*fitParameter->ErrDown()/fitParameter->Value());
	massRelErrDown2 += square(0.5*fitParameter->ErrUp()/fitParameter->Value());
      } else if ( getFitParameter(daughterName, nSVfit_namespace::kLep_shiftEn) != 0 ) {
	NSVfitParameter* fitParameter = getFitParameter(daughterName, nSVfit_namespace::kLep_shiftEn);
	massRelErrUp2   += square(0.5*fitParameter->ErrUp()/(1. + fitParameter->Value()));
	massRelErrDown2 += square(0.5*fitParameter->ErrDown()/(1. + fitParameter->Value()));
      } else if ( getFitParameter(daughterName, nSVfit_namespace::kNu_energy_lab) != 0 ) {
	edm::LogWarning ("setMassResults") 
	  << " Support for fitParameter type = Nu_energy_lab not implemented yet !!";
      } else if ( getNumFitParameter(daughterName) > 0 ) assert(0);
    }
    massErrUp = resonance.mass_*TMath::Sqrt(massRelErrUp2);
    massErrDown = resonance.mass_*TMath::Sqrt(massRelErrDown2);
    //std::cout << "(2): massErrUp = " << massErrUp << ", massErrDown = " << massErrDown << std::endl;
  }
    
  resonance.massErrUp_ = massErrUp;
  resonance.massErrDown_ = massErrDown;

  //std::cout << "<NSVfitAlgorithmByLikelihoodMaximization::setMassResults>:" << std::endl;
  //std::cout << "--> mass = " << resonance.mass_ 
  //	      << " + " << resonance.massErrUp_ << " - " << resonance.massErrDown_ << std::endl;
  //resonance.print(std::cout);
}

double NSVfitAlgorithmByLikelihoodMaximization::nll(const double* x, const double* param) const
{
  if ( verbosity_ ) {
    std::cout << "<NSVfitAlgorithmByLikelihoodMaximization::nll>:" << std::endl;
    std::cout << " idxObjFunctionCall = " << idxObjFunctionCall_ << std::endl;
    for ( std::vector<NSVfitParameter>::iterator fitParameter = fitParameters_.begin();
	  fitParameter != fitParameters_.end(); ++fitParameter ) {
      fitParameter->setValue(x[fitParameter->index()]);
      fitParameter->dump(std::cout);
    }
  }

  ++idxObjFunctionCall_;

//--- in order to resolve ambiguities and improve convergence of the fit,
//    add "penalty" terms in case fitParameters corresponding to azimuthal angles
//    are outside of the "physical" interval -pi..+pi
  double penalty = 0.;
  for ( std::vector<unsigned>::const_iterator idxFitParameterPhi = idxFitParametersPhi_.begin();
	idxFitParameterPhi != idxFitParametersPhi_.end(); ++idxFitParameterPhi ) {
    double fitParameterPhiValue = fitParameters_[*idxFitParameterPhi].Value();
    if ( TMath::Abs(fitParameterPhiValue) > TMath::Pi() )
      penalty += square(TMath::Abs(fitParameterPhiValue) - TMath::Pi());
  }

//--- build event, resonance and particle hypotheses
  eventModel_->builder_->applyFitParameter(currentEventHypothesis_, x);

  if ( verbosity_ ) {
    std::cout << " penalty term = " << penalty << std::endl;
    std::cout << " combined nll = " << penalty + eventModel_->nll(currentEventHypothesis_) << std::endl;
  }

//--- compute likelihood
  return penalty + eventModel_->nll(currentEventHypothesis_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitAlgorithmPluginFactory, NSVfitAlgorithmByLikelihoodMaximization, "NSVfitAlgorithmByLikelihoodMaximization");


