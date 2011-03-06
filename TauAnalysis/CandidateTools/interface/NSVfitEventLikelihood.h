#ifndef TauAnalysis_CandidateTools_NSVfitEventLikelihoodBase_h
#define TauAnalysis_CandidateTools_NSVfitEventLikelihoodBase_h

/** \class NSVfitEventLikelihoodBase
 *
 * Abstract base-class for plugins computing likelihood of combinations of resonances (an "event");
 * used by nSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: NSVfitEventLikelihood.h,v 1.2 2011/02/28 10:46:38 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitLikelihoodBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <string>
#include <iostream>

class NSVfitEventLikelihood : public NSVfitLikelihoodBase
{
 public:
  NSVfitEventLikelihood(const edm::ParameterSet& cfg)
    : NSVfitLikelihoodBase(cfg)
  {}
  virtual ~NSVfitEventLikelihood() {}

  virtual void beginCandidate(const NSVfitEventHypothesis*) const {}

  virtual double operator()(const NSVfitEventHypothesis*) const = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitEventLikelihood* (const edm::ParameterSet&)> NSVfitEventLikelihoodPluginFactory;

#endif
