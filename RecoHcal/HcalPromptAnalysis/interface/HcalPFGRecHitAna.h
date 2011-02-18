#ifndef RecoExamples_HcalPFGRecHitAna_h
#define RecoExamples_HcalPFGRecHitAna_h
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


class HcalPFGRecHitAna : public edm::EDAnalyzer {

public:
  HcalPFGRecHitAna( const edm::ParameterSet & );

private:
  void beginJob();
  void analyze ( const edm::Event& , const edm::EventSetup& );
  void endJob();

  edm::Service<TFileService> fs;
                                                                                                                                                                

  TH1F *HEposEne;
  TH1F *HEnegEne;
  TH1F *HEposTime;
  TH1F *HEnegTime;

  TH1F *HBposEne;
  TH1F *HBnegEne;
  TH1F *HBposTime;
  TH1F *HBnegTime;

  TH1F *HFLongEne;
  TH1F *HFShortEne;
  TH1F *HFLongTime;
  TH1F *HFShortTime;

  TH1F *HOrm0Ene;
  TH1F *HOrm0Time;
  TH1F *HOrp0Ene;
  TH1F *HOrp0Time;

  TH1F *HOrm1Ene;
  TH1F *HOrm1Time;
  TH1F *HOrp1Ene;
  TH1F *HOrp1Time;

  TH1F *HOsipmEne;
  TH1F *HOsipmTime;

};

#endif
