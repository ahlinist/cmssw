#ifndef ECALPROFILEBUILDER_H
#define ECALPROFILEBUILDER_H

//Author: Alexandre Zabi - Imperial College
//$Date: 2006/07/23 17:13:06 $

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

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

  std::string digiProducer_;
  std::string tdcRecInfoCollection_;
  std::string tdcRecInfoProducer_;
  std::string hodoRecInfoCollection_;
  std::string hodoRecInfoProducer_;
  std::string eventHeaderCollection_;
  std::string eventHeaderProducer_;

  std::string  rootfile_;
  std::string  Shapefile_;
  unsigned int verbosity_; 
  unsigned int SuperModule_;
  bool         debug_;
  bool         scanmode_;
  int          nShape_;
  int          nEntries_[1700];
  int          xtalchosen_;
  int          nentry_;
  int          maxIdRef_;
  int          evt_cnt_;
  int          nIter_;
  double       HodoWindow_;
  double       XlowerCut_[1700];
  double       XupperCut_[1700];
  double       YlowerCut_[1700];
  double       YupperCut_[1700];
  EBDetId      xtalInBeam_;

  std::vector<int>    xtalInfo_;
  std::vector<double> shapeData_;
};
#endif
