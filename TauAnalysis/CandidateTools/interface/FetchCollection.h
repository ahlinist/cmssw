#ifndef _TauAnalysis_CandidateTools_FetchCollection_
#define _TauAnalysis_CandidateTools_FetchCollection_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

namespace pf {

template<class T>
void fetchCollection(T& c,
		     const edm::InputTag& tag,
		     const edm::Event& iEvent) {
  
  edm::InputTag empty;
  if( tag==empty ) return;
  
  bool found = iEvent.getByLabel(tag, c);
  
  if(!found ) {
    std::ostringstream  err;
    err<<" cannot get collection: "
       <<tag<<std::endl;
    edm::LogError("PFPAT")<<err.str();
    throw cms::Exception( "MissingProduct", err.str());
  }
  
}
 
}

#endif
