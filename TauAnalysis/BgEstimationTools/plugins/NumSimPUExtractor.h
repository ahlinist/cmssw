#ifndef TauAnalysis_BgEstimationTools_plugins_NumSimPUExtractor_h
#define TauAnalysis_BgEstimationTools_plugins_NumSimPUExtractor_h

/*
 * Extract the number of Sim PU events
 *
 * Author: Evan K. Friis (UC Davis)
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

class NumSimPUExtractor : public ObjValExtractorBase {
  public:

    explicit NumSimPUExtractor(const edm::ParameterSet&);
    virtual ~NumSimPUExtractor() {};

    double operator()(const edm::Event&) const;

  private:
    edm::InputTag src_;
    double minPt_;
};
#endif /* end of include guard: TauAnalysis_BgEstimationTools_plugins_NumSimPUExtractor_h */
