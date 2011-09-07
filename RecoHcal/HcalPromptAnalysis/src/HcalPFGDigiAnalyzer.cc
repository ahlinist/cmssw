// -*- C++ -*-

//
// Original Author:  Jim Hirschauer     
//         Created:  Fri Mar 11 02:59:06 CET 2011
// $Id$
//
//

// system include files
#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <sys/time.h>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "DataFormats/Candidate/interface/Candidate.h"

//
#include "TH1.h"
#include "TH2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include <TROOT.h>
#include <TSystem.h>
#include <TStyle.h>
#include "TFile.h"
#include <TCanvas.h>
#include <TMath.h>
#include <cmath>

#include "RecoHcal/HcalPromptAnalysis/interface/HcalPFGDigiAnalyzer.h"

using namespace edm;
using namespace std;
// static const float adc2fC[128]={-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5, 10.5,11.5,12.5,
// 				13.5,15.,17.,19.,21.,23.,25.,27.,29.5,32.5,35.5,38.5,42.,46.,50.,54.5,59.5,
// 				64.5,59.5,64.5,69.5,74.5,79.5,84.5,89.5,94.5,99.5,104.5,109.5,114.5,119.5,
// 				124.5,129.5,137.,147.,157.,167.,177.,187.,197.,209.5,224.5,239.5,254.5,272.,
// 				292.,312.,334.5,359.5,384.5,359.5,384.5,409.5,434.5,459.5,484.5,509.5,534.5,
// 				559.5,584.5,609.5,634.5,659.5,684.5,709.5,747.,797.,847.,897.,947.,997.,
// 				1047.,1109.5,1184.5,1259.5,1334.5,1422.,1522.,1622.,1734.5,1859.5,1984.5,
// 				1859.5,1984.5,2109.5,2234.5,2359.5,2484.5,2609.5,2734.5,2859.5,2984.5,
// 				3109.5,3234.5,3359.5,3484.5,3609.5,3797.,4047.,4297.,4547.,4797.,5047.,
// 				5297.,5609.5,5984.5,6359.5,6734.5,7172.,7672.,8172.,8734.5,9359.5,9984.5};

HcalPFGDigiAnalyzer::HcalPFGDigiAnalyzer(const edm::ParameterSet& iConfig)

{
  //    Runno = iConfig.getUntrackedParameter < int >("RunNo", 1);
}


HcalPFGDigiAnalyzer::~HcalPFGDigiAnalyzer()
{

}

void HcalPFGDigiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  
  Handle < HFDigiCollection > hf_digis;
  iEvent.getByType(hf_digis);
  
  for (unsigned ihit = 0; ihit < hf_digis->size (); ++ihit) {
    const HFDataFrame& frame = (*hf_digis)[ihit];
    int ieta=frame.id().ieta();
    int iphi=frame.id().iphi();
    int idep=frame.id().depth();
    
    
    double fData[10] = { -999. };
    double fDataNew[9] = { -999. };
    
    double fSum = 0.;
    
    for (int isample = 0; isample < frame.size(); ++isample)
    {
      //      fData[isample] = frame[isample].nominal_fC()-2.5;  // 2.5 is to subtract ped
      fData[isample] = frame[isample].adc();
      
      fSum += fData[isample];
    }
    
    
  }
  
}




			

void HcalPFGDigiAnalyzer::beginRun(const edm::Run & run,const edm::EventSetup& iSetup){
  iSetup.get < HcalDbRecord > ().get(conditions);
}

double HcalPFGDigiAnalyzer::getGain(edm::ESHandle < HcalDbService > mCond, const HFDataFrame & mFrame) {
  double mGain = 1.;
  HcalCalibrations calib = mCond->getHcalCalibrations(mFrame.id());

  // this will return the first value since gain is same for all CapIds
  mGain = calib.respcorrgain(mFrame[4].capid());
  return mGain;
}

void HcalPFGDigiAnalyzer::beginJob()
{
  
//   for (int ieta=lo_ieta_hf; ieta<hi_ieta_hf; ieta++){
//     for (int iphi=lo_iphi; iphi<hi_iphi; iphi++){
//       for (int idep=lo_idep_hf; idep<hi_idep_hf; idep++){
//         sprintf(hnamem_hf, "h_energy_hfm_eta%i_phi%i_dep%i",ieta, iphi, idep);
//         sprintf(hnamep_hf, "h_energy_hfp_eta%i_phi%i_dep%i",ieta, iphi, idep);
//         hists_hf_m[ieta-lo_ieta_hf][iphi][idep-lo_idep_hf] = fs->make<TH1F>( hnamem_hf, hnamem_hf, 100, 0., 300.);
//         hists_hf_p[ieta-lo_ieta_hf][iphi][idep-lo_idep_hf] = fs->make<TH1F>( hnamep_hf, hnamep_hf, 100, 0., 300.);
//       }
//     }
//   }
  
}


void HcalPFGDigiAnalyzer::endJob() {
}


//define this as a plug-in
DEFINE_FWK_MODULE(HcalPFGDigiAnalyzer);
