#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/PFMHTEfficiencyAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/METReco/interface/MET.h"

#include <TTree.h>

// Default constructor
PFMHTEfficiencyAnalyzer::PFMHTEfficiencyAnalyzer() {}

PFMHTEfficiencyAnalyzer::~PFMHTEfficiencyAnalyzer(){}

void PFMHTEfficiencyAnalyzer::Setup(const edm::ParameterSet& iConfig,TTree *trigtree)
{
  edm::ParameterSet pset = iConfig.getParameter<edm::ParameterSet>("HLTPFMHTSources");
  std::vector<std::string> names = pset.getParameterNames();
  values.reserve(names.size());
  for(size_t i=0; i<names.size(); ++i) {
    values.push_back(Value(pset.getParameter<edm::InputTag>(names[i]), names[i]));
  }

  // Setup branches
  for(size_t i=0; i<values.size(); ++i) {
    trigtree->Branch((std::string("HLTPFMHT_")+values[i].branch).c_str(), &(values[i].value));
  }
}

void PFMHTEfficiencyAnalyzer::fill(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  for(size_t i=0; i<values.size(); ++i) {
    edm::Handle<edm::View<reco::MET> > hmht;
    iEvent.getByLabel(values[i].src, hmht);

    values[i].value = hmht->front().et();
  }
} 

