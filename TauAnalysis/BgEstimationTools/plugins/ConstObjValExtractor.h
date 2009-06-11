#ifndef TauAnalysis_BgEstimationTools_ConstObjValExtractor_h  
#define TauAnalysis_BgEstimationTools_ConstObjValExtractor_h

/** \class ConstObjValExtractor
 *
 * Dummy class derrived from ObjValExtractorBase;
 * "extracts" constant value specified by configuration parameter
 * (used for Ntuple filling)
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ObjValExtractorBase.h,v 1.1 2009/02/04 15:53:56 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

class ConstObjValExtractor : public ObjValExtractorBase
{
 public:
  
  explicit ConstObjValExtractor(const edm::ParameterSet&);
  ~ConstObjValExtractor();
  
  double operator()(const edm::Event&) const;

 private:

//--- configuration parameters
  double value_;
};

#endif  


