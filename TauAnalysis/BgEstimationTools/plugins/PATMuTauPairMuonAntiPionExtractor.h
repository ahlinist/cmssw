#ifndef TauAnalysis_BgEstimationTools_PATMuTauPairMuonAntiPionExtractor_h  
#define TauAnalysis_BgEstimationTools_PATMuTauPairMuonAntiPionExtractor_h

/** \class PATMuTauPairAntiPionExtractor
 *
 * Auxiliary class for extracting linear combination
 * of muon calorimeter and segment compatibilities
 * (used for Ntuple filling)
 *
 * NOTE: the values are extracted from the first leg of the PATMuTauPair object 
 *       specified by the "index" configuration parameter (**first** PATMuTauPair object in case "index" is not specified)
 *       contained in the collection specified by the "src" configuration parameter;
 *       in case the collection of PAT objects is empty, 
 *       a substitute value of -1. is returned by operator()
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATMuTauPairAntiPionExtractor.h,v 1.1 2009/06/11 07:23:29 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

class PATMuTauPairMuonAntiPionExtractor : public ObjValExtractorBase
{
 public:
  
  explicit PATMuTauPairMuonAntiPionExtractor(const edm::ParameterSet&);
  ~PATMuTauPairMuonAntiPionExtractor();
  
  double operator()(const edm::Event&) const;

 private:

//--- configuration parameters
  edm::InputTag src_;

  double coeffCaloComp_;
  double coeffSegmComp_;

  unsigned index_;
};

#endif  


