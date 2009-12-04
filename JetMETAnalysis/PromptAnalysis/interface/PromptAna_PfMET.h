#ifndef PromptAna_PFMET
#define PromptAna_PFMET

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"

class PromptAna_PfMET : public edm::EDProducer{
 public: 
  explicit PromptAna_PfMET(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
};

#endif
