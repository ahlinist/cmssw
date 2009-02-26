#ifndef TauAnalysis_Core_ElecMuEventDump_h  
#define TauAnalysis_Core_ElecMuEventDump_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "TauAnalysis/Core/interface/GenericEventDump.h"

class ElecMuEventDump : public GenericEventDump
{
 public:  
  explicit ElecMuEventDump(const edm::ParameterSet&);
  ~ElecMuEventDump();

  void printDiTauCandidateInfo(const edm::Event& evt) const {
    printDiTauCandidateInfoImp<pat::Electron, pat::Muon>(evt);
  }
  
 protected:
  void print(const edm::Event&, const edm::EventSetup&, 
	     const filterResults_type&, const filterResults_type&, double) const;
};

#endif  


