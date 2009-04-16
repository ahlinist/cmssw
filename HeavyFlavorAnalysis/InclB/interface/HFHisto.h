// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TFile;
class TH1;


// ----------------------------------------------------------------------
class HFHisto : public edm::EDAnalyzer {
 public:
  explicit HFHisto(const edm::ParameterSet&);
  ~HFHisto();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  TFile        *fFile; 
  TH1          *fHisto;
  TH1          *fErrorHisto;

  int nevt;
};


