#ifndef TauAnalysis_CandidateTools_NSVfitAlgorithmByIntegration2_h
#define TauAnalysis_CandidateTools_NSVfitAlgorithmByIntegration2_h

/** \class SVfitAlgorithmByIntegration2
 *
 * Concrete implementation of (n)SVfit algorithm
 * by integration of likelihood functions
 * (based on Markov Chain integration algorithm)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.9 $
 *
 * $Id: NSVfitAlgorithmByIntegration2.h,v 1.9 2012/04/03 10:17:08 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/MarkovChainIntegrator.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <Math/Functor.h>
#include <TH1.h>

#include <vector>
#include <string>

class NSVfitAlgorithmByIntegration2 : public NSVfitAlgorithmBase
{
 public:
  NSVfitAlgorithmByIntegration2(const edm::ParameterSet&);
  ~NSVfitAlgorithmByIntegration2();

  void beginJob();
  void beginEvent(const edm::Event&, const edm::EventSetup&);

  unsigned getNumDimensions() const { return numDimensions_; }

  void print(std::ostream&) const {}

  double nll(const double*, const double* = 0) const;

  void fillProbHistograms(const double*);

 protected:
  TH1* bookMassHistogram(const std::string& name);

  void fitImp() const;

  void setMassResults(NSVfitResonanceHypothesisBase*, const TH1*) const;
    
  ROOT::Math::Functor* integrand_;
  MarkovChainIntegrator* integrator_;
  std::vector<double> intBoundaryLower_;
  std::vector<double> intBoundaryUpper_;
  unsigned numDimensions_;

  double* fitParameterValues_;

  std::vector<TH1*> probHistFitParameter_;
  std::map<std::string, TH1*> probHistResonanceMass_;
  TH1* probHistEventMass_;
  ROOT::Math::Functor* auxFillProbHistograms_;

  edm::RunNumber_t currentRunNumber_;
  edm::LuminosityBlockNumber_t currentLumiSectionNumber_;
  edm::EventNumber_t currentEventNumber_;
};

#endif

