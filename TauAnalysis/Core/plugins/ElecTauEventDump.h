#ifndef TauAnalysis_Core_ElecTauEventDump_h  
#define TauAnalysis_Core_ElecTauEventDump_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/Core/interface/GenericEventDump.h"

class ElecTauEventDump : public GenericEventDump
{
 public:  
  explicit ElecTauEventDump(const edm::ParameterSet&);
  ~ElecTauEventDump();

  void printDiTauCandidateInfo(const edm::Event& evt) const {
    printDiTauCandidateInfoImp<pat::Electron, pat::Tau>(evt);
  }
  
 protected:
  void print(const edm::Event&, const edm::EventSetup&, 
	     const filterResults_type&, const filterResults_type&, double) const;

  void printZeeInfo(const edm::Event&) const;
};

#endif  


