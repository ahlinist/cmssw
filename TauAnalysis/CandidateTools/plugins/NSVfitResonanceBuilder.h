#ifndef TauAnalysis_CandidateTools_NSVfitResonanceBuilder_h
#define TauAnalysis_CandidateTools_NSVfitResonanceBuilder_h

/** \class NSVfitResonanceBuilder
 *
 * Auxiliary class for building NSVfitResonanceHypothesis objects;
 * used by NSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.11 $
 *
 * $Id: NSVfitResonanceBuilder.h,v 1.11 2011/01/18 16:41:35 friis Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceBuilderBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

class NSVfitResonanceBuilder : public NSVfitResonanceBuilderBase
{
 public:
  NSVfitResonanceBuilder(const edm::ParameterSet& cfg) 
    : NSVfitResonanceBuilderBase(cfg)
  {}
  ~NSVfitResonanceBuilder() {}
};

#endif


