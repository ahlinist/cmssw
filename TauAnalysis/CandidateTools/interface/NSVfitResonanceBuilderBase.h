#ifndef TauAnalysis_CandidateTools_NSVfitResonanceBuilderBase_h
#define TauAnalysis_CandidateTools_NSVfitResonanceBuilderBase_h

/** \class NSVfitResonanceBuilderBase
 *
 * Base-class for building NSVfitResonanceHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: NSVfitResonanceBuilderBase.h,v 1.3 2011/03/03 13:04:47 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleBuilderBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

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

  virtual void applyFitParameter(NSVfitResonanceHypothesis*, const double*) const;

  virtual void print(std::ostream&) const;

 protected:
  std::string prodResonanceLabel_;

  std::vector<NSVfitSingleParticleBuilderBase*> daughterBuilders_;
  unsigned numDaughterBuilders_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitResonanceBuilderBase* (const edm::ParameterSet&)> NSVfitResonanceBuilderPluginFactory;

#endif
