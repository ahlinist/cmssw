#ifndef MyFilter_H
#define MyFilter_H

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"

#include <string>

class MyFilter: public edm::EDFilter {

public:
  explicit MyFilter(const edm::ParameterSet&);
  ~MyFilter() {}
  virtual bool filter(edm::Event&, const edm::EventSetup&);

private:
       double eventCharge_;
       double minJetEt_;
          int debugFlg_;
  std::string PFTaus_;
  std::string hltJetSrc_;
};
#endif
