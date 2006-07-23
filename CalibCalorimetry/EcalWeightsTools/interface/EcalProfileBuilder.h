#ifndef ECALPROFILEBUILDER_H
#define ECALPROFILEBUILDER_H

//Author: Alexandre Zabi - Imperial College
//$Date: 2006/07/19$

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <vector>
#include <map>
#include <string>

class TFile;
class TH2F;
class TProfile;

class EcalProfileBuilder : public edm::EDAnalyzer {
 public:

  explicit EcalProfileBuilder(edm::ParameterSet const& pSet);
  ~EcalProfileBuilder();
  virtual void analyze(const edm::Event& evt, const edm::EventSetup& evtSetup);

 private:

  unsigned int verbosity_; 
  unsigned int SuperModule_;
  bool         debug_;
};
#endif
