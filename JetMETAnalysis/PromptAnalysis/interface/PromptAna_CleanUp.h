#ifndef PromptAna_CleanUpClass
#define PromptAna_CleanUpClass

#include <memory>
#include <fstream>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DQMOffline/JetMET/interface/CaloMETAnalyzerBase.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
//
#include "DataFormats/HLTReco/interface/TriggerObject.h"
//#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

class PromptAna_CleanUp : public edm::EDProducer{
 public: 
  explicit PromptAna_CleanUp(const edm::ParameterSet&);
  //virtual void beginJob();
  virtual void endJob() {}

 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  
  edm::InputTag HcalNoiseSummaryTag;

  std::string     prefix,suffix;
};

#endif
