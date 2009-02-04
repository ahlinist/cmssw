#ifndef TauAnalysis_Core_MuTauEventDump_h  
#define TauAnalysis_Core_MuTauEventDump_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TauAnalysis/Core/interface/EventDumpBase.h"

class MuTauEventDump : public EventDumpBase 
{
 public:  
  explicit MuTauEventDump(const edm::ParameterSet&);
  ~MuTauEventDump();
  
 protected:
  void print(const edm::Event&, const edm::EventSetup&, 
	     const filterResults_type&, const filterResults_type&, double) const;

 private:

//--- configuration parameters
  edm::InputTag triggerResultsSrc_;

  typedef std::vector<std::string> vstring;
  vstring triggerPathsToPrint_;

  edm::InputTag genParticleSrc_;
  edm::InputTag genTauJetSrc_;
  edm::InputTag electronSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag tauSrc_;
  edm::InputTag metSrc_;
};

#endif  


