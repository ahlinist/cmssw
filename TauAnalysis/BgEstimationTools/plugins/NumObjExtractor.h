#ifndef TauAnalysis_BgEstimationTools_NumObjExtractor_h  
#define TauAnalysis_BgEstimationTools_NumObjExtractor_h

/** \class NumObjExtractor
 *
 * Auxiliary class for extracting number of PAT objects
 * (used for Ntuple filling)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NumObjExtractor.h,v 1.1 2009/06/11 07:23:29 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

template<typename T>
class NumObjExtractor : public ObjValExtractorBase
{
 public:
  
  explicit NumObjExtractor(const edm::ParameterSet&);
  ~NumObjExtractor();
  
  double operator()(const edm::Event&) const;

 private:

//--- configuration parameters
  edm::InputTag src_;
};

#endif  


