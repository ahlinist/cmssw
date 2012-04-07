#ifndef TauAnalysis_CandidateTools_NSVfitEventBuilder_h
#define TauAnalysis_CandidateTools_NSVfitEventBuilder_h

/** \class NSVfitEventBuilder
 *
 * Auxiliary class for building NSVfitEventHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: NSVfitEventBuilder.h,v 1.3 2011/05/29 17:58:22 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"

class NSVfitEventBuilder : public NSVfitEventBuilderBase
{
 public:

  NSVfitEventBuilder(const edm::ParameterSet&);
  ~NSVfitEventBuilder() {}

  void beginJob(NSVfitAlgorithmBase*);

  typedef edm::Ptr<reco::Candidate> CandidatePtr;
  typedef std::map<std::string, CandidatePtr> inputParticleMap;
  NSVfitEventHypothesis* build(const inputParticleMap&, const reco::Vertex*) const;
  
 private:
  /// different possible polarization states of W bosons
  std::vector<int> polHandedness_;
  unsigned numPolStates_;

  NSVfitAlgorithmBase* algorithm_;
};

#endif


