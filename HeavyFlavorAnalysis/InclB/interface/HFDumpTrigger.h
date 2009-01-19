// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TFile;
class TTree;
class TAna00Event;


// ----------------------------------------------------------------------
class HFDumpTrigger : public edm::EDAnalyzer {
 public:
  explicit HFDumpTrigger(const edm::ParameterSet&);
  ~HFDumpTrigger();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  edm::InputTag fTriggerLabel; 
  edm::InputTag fHLTL1SLabel, fHLTL1FLabel, fHLTL2FLabel, fHLTL2CLabel,fHLTL3FLabel, fHLTL3CLabel;
  std::string   fL1MuLabel;
  std::string   fparticleMap;
  std::string   fTriggerName;
  std::string   fL1TriggerName1,fL1TriggerName2,fL1TriggerName3,fL1TriggerName4,fL1TriggerName5,fL1TriggerName6,fL1TriggerName7; 
  std::string   fHLTriggerName1,fHLTriggerName2,fHLTriggerName3,fHLTriggerName4,fHLTriggerName5;

 

  int nevt;

};

