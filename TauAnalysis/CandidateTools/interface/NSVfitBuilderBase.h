#ifndef TauAnalysis_CandidateTools_NSVfitBuilderBase_h
#define TauAnalysis_CandidateTools_NSVfitBuilderBase_h

/** \class NSVfitBuilderBase
 *
 * Abstract base-class for all plugins building fit hypotheses;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NSVfitBuilderBase.h,v 1.1 2011/02/27 16:46:02 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <iostream>

class NSVfitAlgorithmBase;

class NSVfitBuilderBase
{
 public:
  NSVfitBuilderBase(const edm::ParameterSet& cfg)
    : pluginName_(cfg.getParameter<std::string>("pluginName")),
      pluginType_(cfg.getParameter<std::string>("pluginType")),
      barcodeCounter_(0)
  {}
  virtual ~NSVfitBuilderBase() {}

  virtual void initialize(NSVfitAlgorithmBase*) {}

  virtual void beginJob() {}
  virtual void beginEvent(const edm::Event&, const edm::EventSetup&) { barcodeCounter_ = 0; }

  virtual void print(std::ostream&) const {}

 protected:
  int getFitParameterIdx(NSVfitAlgorithmBase*, const std::string&, int, bool = false);

  std::string pluginName_;
  std::string pluginType_;

  mutable int barcodeCounter_;
};

#endif
