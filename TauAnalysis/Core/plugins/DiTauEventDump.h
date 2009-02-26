#ifndef TauAnalysis_Core_DiTauEventDump_h  
#define TauAnalysis_Core_DiTauEventDump_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/Core/interface/GenericEventDump.h"

class DiTauEventDump : public GenericEventDump
{
 public:  
  explicit DiTauEventDump(const edm::ParameterSet&);
  ~DiTauEventDump();

  void printDiTauCandidateInfo(const edm::Event& evt) const {
    printDiTauCandidateInfoImp<pat::Tau, pat::Tau>(evt);
  }
  
 protected:
  void print(const edm::Event&, const edm::EventSetup&, 
	     const filterResults_type&, const filterResults_type&, double) const;
};

#endif  


