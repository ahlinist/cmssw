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

  int           fVerbose;
  std::string   fGlobalTriggerLabel;
  std::string   fL1MapLabel;
  std::string   fL1MuLabel;
  std::string   fL1TriggerName; 
  std::string   fL1w1; 
  std::string   fL1w2; 
  std::string   fL1w3; 
  std::string   fL1w4;
  edm::InputTag fHLTriggerLabel; 
  std::string   fHLTriggerObjectLabel;
  std::string   fHLTriggerName; 
  std::string   fHLTw1; 
  std::string   fHLTw2; 
  std::string   fHLTw3; 
  std::string   fHLTw4;
  std::string   fHLTw5; 
  std::string   fHLTw6; 
  std::string   fHLTw7;

  std::string   fHLTFilterObject2;
  std::string   fHLTFilterObject3;

  int           fNevt;

};
