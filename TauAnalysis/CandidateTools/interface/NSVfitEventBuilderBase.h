#ifndef TauAnalysis_CandidateTools_NSVfitEventBuilderBase_h
#define TauAnalysis_CandidateTools_NSVfitEventBuilderBase_h

/** \class NSVfitEventBuilderBase
 *
 * Base-class for building NSVfitEventHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: NSVfitEventBuilderBase.h,v 1.5 2011/03/25 14:34:30 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitEventVertexRefitter.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

#include <string>
#include <iostream>

class NSVfitEventBuilderBase : public NSVfitBuilderBase
{
 public:
  NSVfitEventBuilderBase(const edm::ParameterSet&);
  virtual ~NSVfitEventBuilderBase();

  virtual void beginJob(NSVfitAlgorithmBase*);
  virtual void beginEvent(const edm::Event&, const edm::EventSetup&);

  typedef edm::Ptr<reco::Candidate> CandidatePtr;
  typedef std::map<std::string, CandidatePtr> inputParticleMap;
  virtual NSVfitEventHypothesis* build(const inputParticleMap&, const reco::Vertex*) const;

  virtual void applyFitParameter(NSVfitEventHypothesis*, const double*) const;

  virtual void print(std::ostream&) const;

 protected:
  std::vector<NSVfitResonanceBuilderBase*> resonanceBuilders_;
  unsigned numResonanceBuilders_;

  NSVfitEventVertexRefitter* eventVertexRefitAlgorithm_;

  int idxFitParameter_pvShiftX_;
  int idxFitParameter_pvShiftY_;
  int idxFitParameter_pvShiftZ_;

  bool doEventVertexRefit_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitEventBuilderBase* (const edm::ParameterSet&)> NSVfitEventBuilderPluginFactory;

#endif
