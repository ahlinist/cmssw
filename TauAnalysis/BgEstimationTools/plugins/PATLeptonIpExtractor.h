#ifndef TauAnalysis_BgEstimationTools_PATLeptonIpExtractor_h  
#define TauAnalysis_BgEstimationTools_PATLeptonIpExtractor_h

/** \class PATLeptonIpExtractor
 *
 * Auxiliary class for extracting transverse impact parameter
 * of electrons, muons and tau-jets based
 * with respect to primary event vertex
 * (used for Ntuple filling)
 *
 * NOTE: the values are extracted from the PAT lepton
 *       specified by the "index" configuration parameter (**first** PAT lepton in case "index" is not specified)
 *       contained in the collection specified by the "src" configuration parameter;
 *       in case the collection of PAT objects is empty, 
 *       a substitute value of 1.e+3 is returned by operator();
 *       the same substitute value is returned in case no primary event vertex is found
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATLeptonIpExtractor.h,v 1.1 2009/06/11 07:23:29 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

#include "TauAnalysis/RecoTools/interface/PATLeptonTrackExtractor.h"

template <typename T>
class PATLeptonIpExtractor : public ObjValExtractorBase
{
 public:
  
  explicit PATLeptonIpExtractor(const edm::ParameterSet&);
  ~PATLeptonIpExtractor();
  
  double operator()(const edm::Event&) const;

 private:

//--- configuration parameters
  edm::InputTag src_;
  edm::InputTag vertexSrc_;

//--- "helper" class for accessing the track
//    of pat::Electrons and pat::Muons 
//    and the "leading" (i.e. highest Pt) track of pat::Taus
  PATLeptonTrackExtractor<T> trackExtractor_;  

  unsigned index_;
};

#endif  


