#ifndef AnalysisDataFormats_TauAnalysis_tauAnalysisAuxFunctions_h
#define AnalysisDataFormats_TauAnalysis_tauAnalysisAuxFunctions_h

#include "DataFormats/Candidate/interface/Candidate.h"

namespace TauAnalysis_namespace 
{
  reco::Candidate::Point operator+(const reco::Candidate::Point&, const reco::Candidate::Point&);

  template<typename keyT1, typename keyT2, typename valueT>
  const valueT* findMapElement(const std::map<keyT1, std::map<keyT2, valueT> >& map, const keyT1& key1, const keyT2& key2)
  {
    typename std::map<keyT1, std::map<keyT2, valueT> >::const_iterator map12_iterator = map.find(key1);
    if ( map12_iterator != map.end() ) {
      typename std::map<keyT2, valueT>::const_iterator map2_iterator = map12_iterator->second.find(key2);
      if ( map2_iterator != map12_iterator->second.end() ) {
	return &map2_iterator->second;
      }
    }
//--- return NULL pointer in case no element found in map for combination of key1 and key2
//    (error handling needs to be taken care of by calling code...)
    return 0;
  } 
}

#endif
