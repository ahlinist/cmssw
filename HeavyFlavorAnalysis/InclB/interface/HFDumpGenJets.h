// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


class TFile;
class TTree;
class TAna00Event;

class TrackAssociatorBase;

// ----------------------------------------------------------------------
class HFDumpGenJets : public edm::EDAnalyzer {
 public:
  explicit HFDumpGenJets(const edm::ParameterSet&);
  ~HFDumpGenJets();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  int           fVerbose; 
  std::string   fGenJetsLabel;
  std::string   fGenCandidatesLabel;
  edm::InputTag fsourceByRefer;

  int nevt;
 

  

};

