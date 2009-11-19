#ifndef PromptAna_JETS
#define PromptAna_JETS

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

class PromptAna_Jet : public edm::EDProducer{
 public: 
  explicit PromptAna_Jet(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
};

#endif
