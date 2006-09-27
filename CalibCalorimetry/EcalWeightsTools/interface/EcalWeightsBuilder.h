#ifndef ECALWEIGHTSBUILDER_H
#define ECALWEIGHTSBUILDER_H

//Author: Alexandre Zabi - Imperial College
//Date: 2006/07/10 15:58:06 $

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
#include <map>

class TFile;
class TH2F;
class TProfile;

class EcalWeightsBuilder : public edm::EDAnalyzer {
 public:

  explicit EcalWeightsBuilder(edm::ParameterSet const& pSet);
  ~EcalWeightsBuilder();
  virtual void analyze(const edm::Event& evt, const edm::EventSetup& evtSetup);

 private:

  unsigned int verbosity_; 
  unsigned int nSamples_;
  unsigned int nTdcBins_;
  unsigned int SuperModule_;
  unsigned int BinOfMax_;
  bool         debug_;
  bool         gen_groupID_;
  bool         default_weights_ ;
  double       tMaxRef_;
  std::vector<int>  XtalSpecial_;

  unsigned int nPulseSamples_;
  unsigned int nPrePulseSamples_;
  bool         doFitBaseline_;
  bool         doFitTime_;
  unsigned int nPulseSamples_gain_;
  unsigned int nPrePulseSamples_gain_;
  bool         doFitBaseline_gain_;
  bool         doFitTime_gain_;
};
#endif
