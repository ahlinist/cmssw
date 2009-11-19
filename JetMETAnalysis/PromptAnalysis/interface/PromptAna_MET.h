#ifndef PromptAna_MissingET
#define PromptAna_MissingET

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"

class PromptAna_MET : public edm::EDProducer{
 public: 
  explicit PromptAna_MET(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
};

#endif
