#ifndef TauAnalysis_CandidateTools_NSVfitResonanceLikelihoodBase_h
#define TauAnalysis_CandidateTools_NSVfitResonanceLikelihoodBase_h

/** \class NSVfitResonanceLikelihoodBase
 *
 * Abstract base-class for plugins computing likelihood of resonances;
 * used by nSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.12 $
 *
 * $Id: NSVfitResonanceLikelihoodBase.h,v 1.12 2011/01/18 16:42:29 friis Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitLikelihoodBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include <string>
#include <iostream>

class NSVfitResonanceLikelihood : public NSVfitLikelihoodBase
{
 public:
  NSVfitResonanceLikelihood(const edm::ParameterSet& cfg)
    : NSVfitLikelihoodBase(cfg)
  {}
  virtual ~NSVfitResonanceLikelihood() {}

  virtual void beginCandidate(const NSVfitResonanceHypothesis*) {}

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitResonanceLikelihood::print>:" << std::endl;
    stream << " pluginName = " << pluginName_ << std::endl;
    stream << " pluginType = " << pluginType_ << std::endl;
  }

  virtual double operator()(const NSVfitResonanceHypothesis*) const = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitResonanceLikelihood* (const edm::ParameterSet&)> NSVfitResonanceLikelihoodPluginFactory;

#endif
