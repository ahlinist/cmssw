#ifndef TauAnalysis_CandidateTools_NSVfitResonanceBuilderBase_h
#define TauAnalysis_CandidateTools_NSVfitResonanceBuilderBase_h

/** \class NSVfitResonanceBuilderBase
 *
 * Base-class for building NSVfitResonanceHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: NSVfitResonanceBuilderBase.h,v 1.4 2011/04/10 14:46:47 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleBuilderBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <string>
#include <iostream>

class NSVfitResonanceBuilderBase : public NSVfitBuilderBase
{
 public:
  NSVfitResonanceBuilderBase(const edm::ParameterSet&);
  virtual ~NSVfitResonanceBuilderBase();

  virtual void beginJob(NSVfitAlgorithmBase*);
  virtual void beginEvent(const edm::Event&, const edm::EventSetup&);

  typedef edm::Ptr<reco::Candidate> CandidatePtr;
  typedef std::map<std::string, CandidatePtr> inputParticleMap;
  virtual NSVfitResonanceHypothesis* build(const inputParticleMap&) const;

  virtual void finalize(NSVfitResonanceHypothesis*) const;

  virtual bool applyFitParameter(NSVfitResonanceHypothesis*, const double*) const;

  virtual void print(std::ostream&) const;

 protected:
  std::string prodResonanceLabel_;

  std::vector<NSVfitSingleParticleBuilderBase*> daughterBuilders_;
  unsigned numDaughterBuilders_;

  mutable std::map<NSVfitSingleParticleBuilderBase*, int> lastBuiltDaughters_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitResonanceBuilderBase* (const edm::ParameterSet&)> NSVfitResonanceBuilderPluginFactory;

#endif
