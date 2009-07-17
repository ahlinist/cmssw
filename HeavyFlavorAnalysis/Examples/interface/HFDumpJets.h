#ifndef _HFDUMPJETS_h_
#define _HFDUMPJETS_h_

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
class TAna01Event;

class TrackAssociatorBase;

// ----------------------------------------------------------------------
class HFDumpJets : public edm::EDAnalyzer {
 public:
  explicit HFDumpJets(const edm::ParameterSet&);
  ~HFDumpJets();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  std::string fJetsLabel;
  std::string fGenJetsLabel;
  std::string fDisc1;
  std::string fDisc2;
  std::string fDisc3;
  std::string fDisc4;
  std::string fDisc5;
  std::string fDisc6;
  std::string fDisc7;
  edm::InputTag fJetPartonMapAlgo;
  edm::InputTag fJetPartonMapPhys;
  edm::InputTag fGenJetPartonMapAlgo;
  edm::InputTag fGenJetPartonMapPhys;
 

  

};

#endif

