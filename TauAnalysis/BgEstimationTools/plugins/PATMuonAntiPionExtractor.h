#ifndef TauAnalysis_BgEstimationTools_PATMuonAntiPionExtractor_h  
#define TauAnalysis_BgEstimationTools_PATMuonAntiPionExtractor_h

/** \class PATMuonAntiPionExtractor
 *
 * Auxiliary class for extracting linear combination
 * of muon calorimeter and segment compatibilities
 * (used for Ntuple filling)
 *
 * NOTE: the values are extracted from the **first** PAT muon
 *       contained in the collection specified by the "src" configuration parameter;
 *       in case the collection of PAT objects is empty, 
 *       a substitute value of -1. is returned by operator()
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATMuonAntiPionExtractor.h,v 1.1 2009/02/04 15:53:56 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

class PATMuonAntiPionExtractor : public ObjValExtractorBase
{
 public:
  
  explicit PATMuonAntiPionExtractor(const edm::ParameterSet&);
  ~PATMuonAntiPionExtractor();
  
  double operator()(const edm::Event&) const;

 private:

//--- configuration parameters
  edm::InputTag src_;

  double coeffCaloComp_;
  double coeffSegmComp_;
};

#endif  


