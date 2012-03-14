#ifndef TauAnalysis_CandidateTools_NSVfitAlgorithmByIntegration_h
#define TauAnalysis_CandidateTools_NSVfitAlgorithmByIntegration_h

/** \class SVfitAlgorithmByIntegration
 *
 * Concrete implementation of (n)SVfit algorithm
 * by integration of likelihood functions
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.7 $
 *
 * $Id: NSVfitAlgorithmByIntegration.h,v 1.7 2011/06/13 17:57:28 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/IndepCombinatoricsGeneratorT.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"

#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>

#include <TFormula.h>
#include <TArrayF.h>
#include <TMath.h>

#include <vector>
#include <algorithm>
#include <string>

class NSVfitAlgorithmByIntegration : public NSVfitAlgorithmBase
{
 public:
  NSVfitAlgorithmByIntegration(const edm::ParameterSet&);
  ~NSVfitAlgorithmByIntegration();

  void beginJob();
  void beginEvent(const edm::Event&, const edm::EventSetup&);

  void print(std::ostream&) const {}

  double nll(const double*, const double*) const;

 protected:
  void fitImp() const;

  void setMassResults(NSVfitResonanceHypothesisByIntegration*, const TH1*, unsigned) const;

  bool isDaughter(const std::string&);
  bool isResonance(const std::string&);
  
  NSVfitParameter* getFitParameter(const std::string&);
  
  struct NSVfitParameterMappingType
  {
    NSVfitParameterMappingType(const NSVfitParameter* base)
      : base_(base)
    {}
    const NSVfitParameter* base_;
    int idxByIntegration_;
  };

  std::vector<NSVfitParameterMappingType> fitParameterMappings_;

  edm::RunNumber_t currentRunNumber_;
  edm::LuminosityBlockNumber_t currentLumiSectionNumber_;
  edm::EventNumber_t currentEventNumber_;

  struct replaceParBase
  {
    virtual void beginJob(NSVfitAlgorithmByIntegration*) {}
    virtual double operator()(const double*) const = 0;
    int iPar_;
  };

  struct replaceParByFitParameter : replaceParBase
  {
    void beginJob(NSVfitAlgorithmByIntegration* algorithm)
    {
      idx_ = algorithm->getFitParameter(fitParameterName_)->index();
    }
    double operator()(const double* param) const { return param[idx_]; }
    std::string fitParameterName_;
    int idx_;
  };

  struct replaceParByResonanceHypothesis : replaceParBase
  {
    replaceParByResonanceHypothesis()
      : valueExtractor_(0)
    {}
    ~replaceParByResonanceHypothesis()
    {
      delete valueExtractor_;
    }
    double operator()(const double* param) const { return value_; }
    std::string resonanceName_;
    NSVfitResonanceHypothesis* resonanceHypothesis_;
    StringObjectFunction<NSVfitResonanceHypothesis>* valueExtractor_;
    mutable double value_;
  };

  struct fitParameterReplacementType
  {    
    fitParameterReplacementType()
      : gridPoints_(0),
	resBinning_(0),
	replaceBy_(0)
    {}
    ~fitParameterReplacementType() 
    {
      delete gridPoints_;
      delete resBinning_;
      delete replaceBy_;
      for ( std::vector<replaceParBase*>::iterator it = parForReplacements_.begin();
	    it != parForReplacements_.end(); ++it ) {
	delete (*it);
      }
    }
    void beginJob(NSVfitAlgorithmByIntegration* algorithm)
    {
      std::vector<double> gridPoints_vector;
      double gridPoint = iterLowerLimit_;
      bool isGridComplete = false;
      while ( !isGridComplete ) {
	if ( gridPoint >= iterUpperLimit_ ) isGridComplete = true;
	gridPoints_vector.push_back(gridPoint);
	double stepSize = TMath::Max((iterStepSizeFactor_ - 1.)*gridPoint, iterMinStepSize_);
	gridPoint += stepSize;	
      }
      // CV: always probe Z mass
      gridPoints_vector.push_back(SVfit_namespace::mZ);
      std::sort(gridPoints_vector.begin(), gridPoints_vector.end());
      
      numGridPoints_ = gridPoints_vector.size();
      gridPoints_ = new TArrayF(numGridPoints_);
      resBinning_ = new TArrayF(numGridPoints_ + 1);
      for ( int iGridPoint = 0; iGridPoint < numGridPoints_; ++iGridPoint ) {
	double gridPoint = gridPoints_vector[iGridPoint];
	(*gridPoints_)[iGridPoint] = gridPoint;
	if   ( iGridPoint == 0 ) (*resBinning_)[0]          = gridPoint - 0.5*TMath::Abs(gridPoints_vector[1] - gridPoint);
	else                     (*resBinning_)[iGridPoint] = 0.5*(gridPoints_vector[iGridPoint - 1] + gridPoint);
	if   ( iGridPoint == (numGridPoints_ - 1) ) 
	  (*resBinning_)[numGridPoints_] = gridPoint + 0.5*TMath::Abs(gridPoint - gridPoints_vector[iGridPoint - 1]);
      }

      NSVfitParameter* fitParameterToReplace = algorithm->getFitParameter(toReplace_);
      if ( !fitParameterToReplace ) {
	throw cms::Exception("fitParameterReplacementType::beginJob")
	  << " No fitParameter of name = " << toReplace_ << " defined !!";
      }
      idxToReplace_ = fitParameterToReplace->index();

      for ( std::vector<replaceParBase*>::iterator par = parForReplacements_.begin();
	    par != parForReplacements_.end(); ++par ) {
	(*par)->beginJob(algorithm);
      }
    }
    std::string name_;
    double iterLowerLimit_;
    double iterUpperLimit_;
    double iterStepSizeFactor_;
    double iterMinStepSize_;
    int numGridPoints_;
    TArrayF* gridPoints_;
    TArrayF* resBinning_;
    std::string toReplace_;
    int idxToReplace_;
    TFormula* replaceBy_;
    int idxMassParameter_;
    std::vector<replaceParBase*> parForReplacements_;
    int numParForReplacements_;
  };

  std::vector<fitParameterReplacementType*> fitParameterReplacements_;

  double* fitParameterValues_;

  double* xl_;
  double* xu_;

  gsl_monte_function* integrand_;
  gsl_monte_vegas_state* workspace_;
  gsl_rng* rnd_;
  unsigned numCallsGridOpt_;
  unsigned numCallsIntEval_;
  double maxChi2_;
  unsigned maxIntEvalIter_;
  double precision_;
  size_t numDimensions_;

  size_t numMassParameters_;
  IndepCombinatoricsGeneratorT<int>* massParForReplacements_;
};

#endif

