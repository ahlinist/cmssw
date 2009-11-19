#ifndef PromptAna_EVENT
#define PromptAna_EVENT

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

class PromptAna_Event : public edm::EDProducer {
 public: 
  explicit PromptAna_Event(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
};

#endif
