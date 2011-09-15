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
                                                                                                                                                                

  TH1F *HEPEne;
  TH1F *HEMEne;
  TH1F *HEPTime;
  TH1F *HEMTime;

  TH1F *HBPEne;
  TH1F *HBMEne;
  TH1F *HBPTime;
  TH1F *HBMTime;

  TH1F* HEM15BEne   ;  
  TH1F* HEM15BTime  ;  
  TH1F* HEM15AEne   ;  
  TH1F* HEM15ATime  ;  
  TH1F* HEMOtherEne ;  
  TH1F* HEMOtherTime;  

  TH1F* HBP14BEne   ;  
  TH1F* HBP14BTime  ;  
  TH1F* HBP14AEne   ;  
  TH1F* HBP14ATime  ;  
  TH1F* HBPOtherEne ;  
  TH1F* HBPOtherTime;

  TH2F* HEM15BEneVTime  ;
  TH2F* HEM15AEneVTime  ;
  TH2F* HEMOtherEneVTime;
  TH2F* HBP14BEneVTime  ;
  TH2F* HBP14AEneVTime  ;
  TH2F* HBPOtherEneVTime;

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
