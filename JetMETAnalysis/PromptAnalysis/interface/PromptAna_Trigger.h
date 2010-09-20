#ifndef PromptAna_TRIGGER
#define PromptAna_TRIGGER

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include <set>
#include "FWCore/Common/interface/TriggerNames.h"

#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

class PromptAna_Trigger : public edm::EDProducer {
 public:
  explicit PromptAna_Trigger(const edm::ParameterSet&);

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   l1InputTag, hltInputTag;
  const std::vector<std::string> hltPathsOfInterest;
};

#endif

