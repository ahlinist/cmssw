#ifndef TauAnalysis_BgEstimationTools_PATDiTauChargeSignExtractor_h  
#define TauAnalysis_BgEstimationTools_PATDiTauChargeSignExtractor_h

/** \class PATDiTauChargeSignExtractor
 *
 * Auxiliary class for extracting relative sign of a pair of tau lepton decay products
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATDiTauChargeSignExtractor.h,v 1.1 2009/11/12 15:44:09 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"
#include "TauAnalysis/BgEstimationTools/interface/PATLeptonChargeExtractor.h"

template <typename T1, typename T2>
class PATDiTauChargeSignExtractor : public ObjValExtractorBase
{
 public:
  
  explicit PATDiTauChargeSignExtractor(const edm::ParameterSet&);
  ~PATDiTauChargeSignExtractor();
  
  double operator()(const edm::Event&) const;

 private:

//--- configuration parameters
  edm::InputTag src_;

//--- "helper" class for accessing the track
//    of pat::Electrons and pat::Muons 
//    and the "leading" (i.e. highest Pt) track of pat::Taus
  PATLeptonChargeExtractor<T1> leg1ChargeExtractor_;
  PATLeptonChargeExtractor<T2> leg2ChargeExtractor_;

  unsigned index_;
};

#endif  


