#ifndef TauAnalysis_Core_wTauNuEventDump_h  
#define TauAnalysis_Core_wTauNuEventDump_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/View.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/Core/interface/GenericEventDump.h"
#include "TauAnalysis/Core/interface/ObjectDumpBase.h"

class wTauNuEventDump : public GenericEventDump
{
 public:  
  explicit wTauNuEventDump(const edm::ParameterSet&);
  ~wTauNuEventDump();
  
 protected:
  void print(const edm::Event&, const edm::EventSetup&, 
	     const GenericAnalyzer_namespace::filterResults_type&, const GenericAnalyzer_namespace::filterResults_type&, double) const;

  ObjectDumpBase* tauDump_;
};

#endif  


