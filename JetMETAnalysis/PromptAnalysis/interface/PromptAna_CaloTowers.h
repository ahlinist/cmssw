#ifndef PromptAna_CALOTOWERS
#define PromptAna_CALOTOWERS

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

class PromptAna_CaloTowers : public edm::EDProducer{
 public: 
  explicit PromptAna_CaloTowers(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
};

#endif
