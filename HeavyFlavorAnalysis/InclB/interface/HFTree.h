
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
class TH1;

// ----------------------------------------------------------------------
class HFTree : public edm::EDAnalyzer {
 public:
  explicit HFTree(const edm::ParameterSet&);
  ~HFTree();
  
 private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  TFile        *fFile; 
  TTree        *fTree;
  TAna00Event  *fEvent;
 
 
  int nevt;
};

