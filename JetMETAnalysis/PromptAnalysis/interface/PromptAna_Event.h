#ifndef PromptAna_EVENT
#define PromptAna_EVENT

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "TString.h"
#include <fstream>
#include <iostream>

class PromptAna_Event : public edm::EDProducer {
 public: 
  explicit PromptAna_Event(const edm::ParameterSet&);
  edm::TriggerResults tr;

 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   TRGTag;

};

#endif
