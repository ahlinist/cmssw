#ifndef RecoBTag_CombinedSV_TaggingVariablesComputer_h
#define RecoBTag_CombinedSV_TaggingVariablesComputer_h

#include "RecoBTag/CombinedSV/interface/CombinedData.h"
#include "RecoBTag/CombinedSV/interface/CombinedSVTaggingVariables.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"

namespace reco {
  class TaggingVariablesComputer {
    /** 
     *  A class that computes the tagging variables,
     *  given the appropriate data object plus 
     *  a list of what variables to compute.
     */
  public:
    /**
     *  compute the variables 'vars' from 'data'.
     */
    static reco::TaggingVariableList compute (
        const CombinedSVTaggingVariables & vars,
        const combsv::CombinedData & data );
  };
}

#endif
