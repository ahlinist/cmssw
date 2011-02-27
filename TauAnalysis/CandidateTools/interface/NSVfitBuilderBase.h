#ifndef TauAnalysis_CandidateTools_NSVfitBuilderBase_h
#define TauAnalysis_CandidateTools_NSVfitBuilderBase_h

/** \class NSVfitBuilderBase
 *
 * Abstract base-class for all plugins building fit hypotheses;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.11 $
 *
 * $Id: NSVfitBuilderBase.h,v 1.11 2011/01/18 16:41:35 friis Exp $
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

  virtual void initialize(NSVfitAlgorithmBase*) const {}

  virtual void beginJob() {}
  virtual void beginEvent(const edm::Event&, const edm::EventSetup&) { barcodeCounter_ = 0; }

  virtual void print(std::ostream& stream) const {}

 protected:
  std::string pluginName_;
  std::string pluginType_;

  mutable int barcodeCounter_;
};

#endif
