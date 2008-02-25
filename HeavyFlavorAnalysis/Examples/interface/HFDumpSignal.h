#ifndef _HFDUMPSIGNAL_h_
#define _HFDUMPSIGNAL_h_

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
class HFDumpSignal : public edm::EDAnalyzer {
 public:
  explicit HFDumpSignal(const edm::ParameterSet&);
  ~HFDumpSignal();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  std::string fTracksLabel;

};

#endif
