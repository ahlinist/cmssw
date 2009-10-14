#ifndef TauAnalysis_BgEstimationTools_ZtautauVisMassHypothesisValExtractor_h  
#define TauAnalysis_BgEstimationTools_ZtautauVisMassHypothesisValExtractor_h

/** \class ZtautauVisMassHypothesisValExtractor
 *
 * Auxiliary class for extracting value of visible invariant mass 
 * reconstructed from visible decay products of tau-lepton pair
 * (combined value estimated from CompositePtrCandidateT1T2MEt and ZllHypothesisT1T2 data-formats)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ZtautauVisMassHypothesisValExtractor.h,v 1.1 2009/07/30 16:06:52 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

template <typename T1, typename T2>
class ZtautauVisMassHypothesisValExtractor : public ObjValExtractorBase
{
 public:
  
  explicit ZtautauVisMassHypothesisValExtractor(const edm::ParameterSet&);
  ~ZtautauVisMassHypothesisValExtractor();
  
  double operator()(const edm::Event&) const;

 private:

//--- configuration parameters
  edm::InputTag src_;
  edm::InputTag srcDiTauCandidates_;

  unsigned index_;
};

#endif  


