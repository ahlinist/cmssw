#ifndef TauAnalysis_CandidateTools_NSVfitEventBuilder_h
#define TauAnalysis_CandidateTools_NSVfitEventBuilder_h

/** \class NSVfitEventBuilder
 *
 * Auxiliary class for building NSVfitEventHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.11 $
 *
 * $Id: NSVfitEventBuilder.h,v 1.11 2011/01/18 16:41:35 friis Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventBuilderBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"

class NSVfitEventBuilder : public NSVfitEventBuilderBase
{
 public:
  NSVfitEventBuilder(const edm::ParameterSet& cfg) 
    : NSVfitEventBuilderBase(cfg)
  {}
  ~NSVfitEventBuilder() {}
};

#endif


