#ifndef TauAnalysis_Core_ElecMuEventDump_h  
#define TauAnalysis_Core_ElecMuEventDump_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "TauAnalysis/Core/interface/GenericEventDump.h"
#include "TauAnalysis/Core/interface/ObjectDumpBase.h"

class ElecMuEventDump : public GenericEventDump
{
 public:  
  explicit ElecMuEventDump(const edm::ParameterSet&);
  ~ElecMuEventDump();
  
 protected:
  void print(const edm::Event&, const edm::EventSetup&, 
	     const GenericAnalyzer_namespace::filterResults_type&, const GenericAnalyzer_namespace::filterResults_type&, double) const;

  ObjectDumpBase* electronDump_;
  ObjectDumpBase* muonDump_;
  ObjectDumpBase* elecMuDump_;
};

#endif  


