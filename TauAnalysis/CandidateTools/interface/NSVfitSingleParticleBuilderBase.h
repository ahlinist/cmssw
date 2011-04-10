#ifndef TauAnalysis_CandidateTools_NSVfitSingleParticleBuilderBase_h
#define TauAnalysis_CandidateTools_NSVfitSingleParticleBuilderBase_h

/** \class NSVfitSingleParticleBuilderBase
 *
 * Base-class for building objects derrived from NSVfitSingleParticleHypothesis class;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: NSVfitSingleParticleBuilderBase.h,v 1.5 2011/03/28 16:54:00 friis Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitBuilderBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include <string>
#include <iostream>

class NSVfitResonanceHypothesis;

class NSVfitSingleParticleBuilderBase : public NSVfitBuilderBase
{
 public:
  NSVfitSingleParticleBuilderBase(const edm::ParameterSet& cfg)
    : NSVfitBuilderBase(cfg),
      prodParticleLabel_(cfg.getParameter<std::string>("prodParticleLabel"))
  {}
  virtual ~NSVfitSingleParticleBuilderBase() {}

  typedef edm::Ptr<reco::Candidate> CandidatePtr;
  typedef std::map<std::string, CandidatePtr> inputParticleMap;
  virtual NSVfitSingleParticleHypothesisBase* build(const inputParticleMap&) const = 0;

  virtual void applyFitParameter(NSVfitSingleParticleHypothesisBase*, const double*) const = 0;

  virtual void print(std::ostream&) const {}

protected:
  std::string prodParticleLabel_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitSingleParticleBuilderBase* (const edm::ParameterSet&)> NSVfitSingleParticleBuilderPluginFactory;

#endif



