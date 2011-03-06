#ifndef TauAnalysis_CandidateTools_NSVfitLikelihoodBase_h
#define TauAnalysis_CandidateTools_NSVfitLikelihoodBase_h

/** \class NSVfitSingleParticleLikelihoodBase
 *
 * Abstract base-class for all likelihood function plugins;
 * used by nSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: NSVfitLikelihoodBase.h,v 1.3 2011/03/03 13:04:47 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <iostream>

class NSVfitAlgorithmBase;

class NSVfitLikelihoodBase
{
 public:
  NSVfitLikelihoodBase(const edm::ParameterSet& cfg)
  {
    pluginType_ = cfg.getParameter<std::string>("pluginType");
    pluginName_ = cfg.getParameter<std::string>("pluginName");

    verbosity_ = cfg.exists("verbosity") ?
      cfg.getParameter<int>("verbosity") : 0;
  }
  virtual ~NSVfitLikelihoodBase() {}

  const std::string& pluginType() const { return pluginType_; }
  const std::string& pluginName() const { return pluginName_; }

  virtual void beginJob(NSVfitAlgorithmBase*) {}
  virtual void beginEvent(const edm::Event&, const edm::EventSetup&) {}
 
  virtual void print(std::ostream& stream) const
  {
    stream << "<NSVfitLikelihoodBase::print>:" << std::endl;
    stream << " pluginName = " << pluginName_ << std::endl;
    stream << " pluginType = " << pluginType_ << std::endl;
  }

 protected:
  std::string pluginType_;
  std::string pluginName_;

  int verbosity_;
};

#endif
