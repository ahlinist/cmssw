#ifndef _UPSILONTREE_h_
#define _UPSILONTREE_h_

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
class UpsilonTree : public edm::EDAnalyzer {
 public:
  explicit UpsilonTree(const edm::ParameterSet&);
  ~UpsilonTree();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  TFile        *fFile; 
  TTree        *fTree;
  TAna00Event  *fEvent;
};

#endif
