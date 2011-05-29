#ifndef TauAnalysis_CandidateTools_NSVfitEventBuilder_h
#define TauAnalysis_CandidateTools_NSVfitEventBuilder_h

/** \class NSVfitEventBuilder
 *
 * Auxiliary class for building NSVfitEventHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: NSVfitEventBuilder.h,v 1.2 2011/04/10 14:46:47 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"

class NSVfitEventBuilder : public NSVfitEventBuilderBase
{
 public:

  NSVfitEventBuilder(const edm::ParameterSet& cfg) 
    : NSVfitEventBuilderBase(cfg)
  {}
  ~NSVfitEventBuilder() {}

  void beginJob(NSVfitAlgorithmBase*);

  typedef edm::Ptr<reco::Candidate> CandidatePtr;
  typedef std::map<std::string, CandidatePtr> inputParticleMap;
  NSVfitEventHypothesis* build(const inputParticleMap&, const reco::Vertex*) const;
  
 private:

  NSVfitAlgorithmBase* algorithm_;
};

#endif


