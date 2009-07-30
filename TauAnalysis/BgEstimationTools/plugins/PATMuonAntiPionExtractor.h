#ifndef TauAnalysis_BgEstimationTools_PATMuonAntiPionExtractor_h  
#define TauAnalysis_BgEstimationTools_PATMuonAntiPionExtractor_h

/** \class PATMuonAntiPionExtractor
 *
 * Auxiliary class for extracting linear combination
 * of muon calorimeter and segment compatibilities
 * (used for Ntuple filling)
 *
 * NOTE: the values are extracted from the PAT muon
 *       specified by the "index" configuration parameter (**first** PAT muon in case "index" is not specified)
 *       contained in the collection specified by the "src" configuration parameter;
 *       in case the collection of PAT objects is empty, 
 *       a substitute value of -1. is returned by operator()
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATMuonAntiPionExtractor.h,v 1.1 2009/06/11 07:23:29 veelken Exp $
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

  unsigned index_;
};

#endif  


