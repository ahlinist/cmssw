// Author: Jim Hirschauer (based on Dima's DQM/HcalMonitorTasks/src/HcalDetDiagLaserMonitor.cc)
// Date:  Feb 2011
// Definitely works in CMSSW 3_11_0

#include "RecoHcal/HcalPromptAnalysis/interface/HcalPFGAbortGapAna.h"
#define _DEBUG_QUIET
#include "RecoHcal/HcalPromptAnalysis/interface/dout.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/HcalDigi/interface/HcalCalibrationEventTypes.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"

#include "FWCore/Framework/interface/Event.h"
//#include "FWCore/Framework/interface/EventSetup.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>
#include <cmath>

#include <sys/time.h>
#include <time.h>


using namespace edm;
//using namespace reco;
using namespace std;


HcalPFGAbortGapAna::HcalPFGAbortGapAna( const ParameterSet & cfg ){
}


// ************************
// ************************

void HcalPFGAbortGapAna::beginJob( ) {
  dout << "Begin job" << std::endl;
  
  //  edm::Service<TFileService> fs;
  
}

// ************************
// ************************
void HcalPFGAbortGapAna::analyze( const edm::Event& evt, const edm::EventSetup& es ) {

  // Abort Gap laser 
  edm::Handle<FEDRawDataCollection> rawdata;
  evt.getByLabel("source",rawdata);
    
  // loop over FEDs 
  //  static bool HBHEseq,HOseq,HFseq, LaserRaddam;
  static bool LaserEvent;

  for (int i=FEDNumbering::MINHCALFEDID;i<=FEDNumbering::MAXHCALFEDID; i++) {
    const FEDRawData& fedData = rawdata->FEDData(i) ;
    
    //  Not sure exactly what this does.  
    if ( fedData.size() < 24 ) continue ;
    
    // Get event header
    int value = ((const HcalDCCHeader*)(fedData.data()))->getCalibType() ;
    
    // Check for Laser events and subdet
    //     if(value==hc_HBHEHPD){HBHEseq=true; HOseq=HFseq=false;}
    //     if(value==hc_HOHPD){HOseq=true; HBHEseq=HFseq=false;}
    //     if(value==hc_HFPMT){HFseq=true; HBHEseq=HOseq=false;}
    if(value==hc_HBHEHPD || value==hc_HOHPD || value==hc_HFPMT){LaserEvent=true; break;}
    //    if(value==hc_RADDAM){LaserEvent=true; LaserRaddam=true; break;}
    
    
  }
  
  // Get BX 
  int bx = evt.bunchCrossing();

  // Do whatever you want here:
  // I print  out BX:
  if (LaserEvent) std::cout << "Laser Event : bx = " << bx<< std::endl;
  if (bx != 3490) std::cout << "non-3490 bx = " << bx<< std::endl;

}

// ***********************************
// ***********************************
void HcalPFGAbortGapAna::endJob() {

}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HcalPFGAbortGapAna);
