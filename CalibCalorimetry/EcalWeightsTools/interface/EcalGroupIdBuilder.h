#ifndef ECALGROUPIDBUILDER_H
#define ECALGROUPIDBUILDER_H

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

class EcalGroupIdBuilder : public edm::EDAnalyzer {
 public:

  explicit EcalGroupIdBuilder(edm::ParameterSet const& pSet);
  ~EcalGroupIdBuilder();
  virtual void analyze(const edm::Event& evt, const edm::EventSetup& evtSetup);

 private:

  unsigned int verbosity_; 
  unsigned int nGroups_;
  unsigned int SuperModule_;
  bool         debug_;
  bool         problem_;
  double       granularity_;
  std::string  inputFile_;
};
#endif
