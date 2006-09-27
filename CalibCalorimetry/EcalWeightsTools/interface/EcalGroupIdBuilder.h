#ifndef ECALGROUPIDBUILDER_H
#define ECALGROUPIDBUILDER_H

//Author: Alexandre Zabi - Imperial College
//$Date: 2006/07/24 15:28:48 $

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>
#include "TH1F.h"
#include "TH2F.h"

class EcalGroupIdBuilder : public edm::EDAnalyzer {
 public:

  explicit EcalGroupIdBuilder(edm::ParameterSet const& pSet);
  ~EcalGroupIdBuilder();
  virtual void analyze(const edm::Event& evt, const edm::EventSetup& evtSetup);

 private:

  //Variable
  unsigned int verbosity_; 
  unsigned int nGroups_;
  unsigned int SuperModule_;
  bool         debug_;
  bool         default_;
  bool         problem_;
  double       granularity_;
  std::string  inputFile_;
  std::string  rootFile_;
  std::string  grouptype_;
  std::string  datatype_;
  std::string  version_;
  std::vector<int>  XtalSpecial_;

  //HISTO
  TH1F *TIMING_;
  TH2F *GROUPIDMAP_;

};
#endif
