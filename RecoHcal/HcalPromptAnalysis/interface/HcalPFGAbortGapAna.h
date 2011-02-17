#ifndef RecoExamples_HcalPFGAbortGapAna_h
#define RecoExamples_HcalPFGAbortGapAna_h
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TFile.h>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"

class HcalPFGAbortGapAna : public edm::EDAnalyzer {

public:
  HcalPFGAbortGapAna( const edm::ParameterSet & );

private:
  void beginJob( );
  void analyze ( const edm::Event& , const edm::EventSetup& );
  void endJob();


};

#endif
