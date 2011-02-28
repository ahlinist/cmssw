#ifndef TauAnalysis_CandidateTools_NSVfitEventBuilderBase_h
#define TauAnalysis_CandidateTools_NSVfitEventBuilderBase_h

/** \class NSVfitEventBuilderBase
 *
 * Base-class for building NSVfitEventHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NSVfitEventBuilderBase.h,v 1.1 2011/02/27 16:45:16 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceBuilderBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <string>
#include <iostream>

class NSVfitEventBuilderBase : public NSVfitBuilderBase
{
 public:
  NSVfitEventBuilderBase(const edm::ParameterSet&);
  virtual ~NSVfitEventBuilderBase();

  virtual void beginJob();
  virtual void beginEvent(const edm::Event&, const edm::EventSetup&);

  typedef edm::Ptr<reco::Candidate> CandidatePtr;
  typedef std::map<std::string, CandidatePtr> inputParticleMap;
  virtual NSVfitEventHypothesis* build(const inputParticleMap&) const;

  virtual void applyFitParameter(NSVfitEventHypothesis*, double*) const;

  virtual void print(std::ostream&) const;

 protected:
  std::vector<NSVfitResonanceBuilderBase*> resonanceBuilders_;
  unsigned numResonanceBuilders_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitEventBuilderBase* (const edm::ParameterSet&)> NSVfitEventBuilderPluginFactory;

#endif
