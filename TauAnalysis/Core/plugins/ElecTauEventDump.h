#ifndef TauAnalysis_Core_ElecTauEventDump_h  
#define TauAnalysis_Core_ElecTauEventDump_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TauAnalysis/Core/interface/GenericEventDump.h"

class ElecTauEventDump : public GenericEventDump 
{
 public:  
  explicit ElecTauEventDump(const edm::ParameterSet&);
  ~ElecTauEventDump();
  
 protected:
  void print(const edm::Event&, const edm::EventSetup&, 
	     const filterResults_type&, const filterResults_type&, double) const;
};

#endif  


