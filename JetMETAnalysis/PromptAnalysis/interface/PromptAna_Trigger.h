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
  edm::TriggerResults tr;

 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  void endJob();
  void fillAlgoNameListNoUnderScores();

  const edm::InputTag inputTag;
  int nBxOutput;
  bool storeByName;

  std::vector<std::string> algoNameList;
  std::vector<std::string> algoNameListNoUnderScores;  

  std::set<std::string> algosPresentInData;
  std::set<std::string> menusAlreadyParsed;

  const edm::InputTag   TRGTag;
};

#endif
