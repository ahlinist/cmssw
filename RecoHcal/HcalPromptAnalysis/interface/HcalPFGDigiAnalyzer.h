// -*- C++ -*-
//

// Original Author:  Jim Hirschauer
//         Created:  Fri Mar 11 02:59:06 CET 2011
// $Id$
//
//

// system include files
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <utility>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

class TH1F;
class TH2F;
class TProfile2D;

class HcalPFGDigiAnalyzer : public edm::EDAnalyzer {
public:
  explicit HcalPFGDigiAnalyzer(const edm::ParameterSet&);
  ~HcalPFGDigiAnalyzer();
  
  
private:
  virtual void beginRun(const edm::Run & run,const edm::EventSetup&);
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
//   static const size_t NETA_hf = 13;
//   static const size_t NPHI_hf = 73;
//   static const size_t NDEP_hf = 2;
//   char hnamep_hf[100];
//   char hnamem_hf[100];
//   TH1F *hists_hf_m[NETA_hf][NPHI_hf][NDEP_hf];
//   TH1F *hists_hf_p[NETA_hf][NPHI_hf][NDEP_hf];

  edm::ESHandle<HcalDbService> conditions;
  HcalCalibrations calibs_;
  edm::Service<TFileService> fs;
  //  int getTSMax(double* fDataNew, int fTS);
//   double getSumE2(double fDataNew[9], int ts_max);
//   double getSumE4(double fDataNew[10], int ts_max);

  double getGain(edm::ESHandle<HcalDbService>  mCond, const HFDataFrame& mFrame);

  

};
