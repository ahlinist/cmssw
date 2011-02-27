#ifndef TauAnalysis_CandidateTools_NSVfitSingleParticleLikelihood_h
#define TauAnalysis_CandidateTools_NSVfitSingleParticleLikelihood_h

/** \class NSVfitSingleParticleLikelihoodBase
 *
 * Abstract base-class for plugins computing likelihood of single particle kinematics;
 * used by nSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.12 $
 *
 * $Id: NSVfitSingleParticleLikelihood.h,v 1.12 2011/01/18 16:42:29 friis Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitLikelihoodBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include <string>
#include <iostream>

class NSVfitSingleParticleLikelihood : public NSVfitLikelihoodBase
{
 public:
  NSVfitSingleParticleLikelihood(const edm::ParameterSet& cfg)
    : NSVfitLikelihoodBase(cfg)
  {}
  virtual ~NSVfitSingleParticleLikelihood() {}

  virtual void beginCandidate(const NSVfitSingleParticleHypothesisBase*) {}

  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitSingleParticleLikelihood::print>:" << std::endl;
    stream << " pluginName = " << pluginName_ << std::endl;
    stream << " pluginType = " << pluginType_ << std::endl;
  }

  virtual double operator()(const NSVfitSingleParticleHypothesisBase*) const = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitSingleParticleLikelihood* (const edm::ParameterSet&)> NSVfitSingleParticleLikelihoodPluginFactory;

#endif
