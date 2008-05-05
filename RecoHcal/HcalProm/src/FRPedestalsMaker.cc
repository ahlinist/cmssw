#include "FRPedestalsMaker.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"



#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalDbASCIIIO.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CondFormats/HcalObjects/interface/HcalPedestals.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"

#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"



#include <math.h>
#include <iostream>
#include <fstream>

  template <class T> 
  void fillHists (const edm::SortedCollection<T>& fCollection, std::map < unsigned, TH1F* > * fHists,
		  int fPreFirst, int fPreLast, int fPostFirst, int fPostLast)
{
    for (unsigned iHit = 0; iHit < fCollection.size(); ++iHit) {
      const T& frame = fCollection[iHit];
      HcalDetId hid = frame.id();
      unsigned id = hid.rawId();
      for (int slice = 0; slice < frame.size(); ++slice) {
	if ((slice >= fPreFirst && slice <= fPreLast) || (slice >= fPostFirst && slice <= fPostLast)) { // consider slices out of signal area
	  int capId = frame[slice].capid();
	  int adc = frame[slice].adc();
	  // search where to store this information
	  if (fHists[capId].find (id) == fHists[capId].end()) {
	    // book new histogram
	    char label [1024];
	    sprintf (label, "adc_%d_%d_%d_%d_%d", hid.subdetId(), hid.ieta(), hid.iphi(), hid.depth(), capId);
	    char title [1024];
	    sprintf (title, "ADC subdet=%d eta=%d phi=%d depth=%d, capId=%d", hid.subdetId(), hid.ieta(), hid.iphi(), hid.depth(), capId);
	    fHists[capId][id] = new TH1F (label, title, 25, -0.5, 24.5);
	  }
	  fHists[capId][id]->Fill (adc);
	}
      }
    }
  }
  
  double getPeak (TH1F* fHist) {
    double mean0 = fHist->GetMean();
    double rms0 = fHist->GetRMS();
    double xmin = mean0 - 2 * rms0;
    double xmax = mean0 + 2 * rms0;
    TF1 gauss ("g","gaus");
    fHist->Fit (&gauss, "Q", "", xmin, xmax);
    double chi2 = gauss.GetChisquare();
    double mean = gauss.GetParameter(1);
    double width = gauss.GetParameter(2);
    double meanError = gauss.GetParError(1);
    double widthError = gauss.GetParError(2);
    return mean;
  }  

  

FRPedestalsMaker::FRPedestalsMaker(const edm::ParameterSet& ps)
  : mPedestalsFile (ps.getParameter<std::string>("PedestalsFile")),
    mPreSliceFirst (ps.getParameter<int>("preSliceFirst")),
    mPreSliceLast (ps.getParameter<int>("preSliceLast")),
    mPostSliceFirst (ps.getParameter<int>("postSliceFirst")),
    mPostSliceLast (ps.getParameter<int>("postSliceLast")),
    mConditions (0),
    mRootFile (0)
{
  std::string histFile = ps.getParameter<std::string>("HistogramsFile");
  if (!histFile.empty()) mRootFile = new TFile (histFile.c_str(), "RECREATE"); 
  // booking summary histograms
  mTimeSlicesHist = new TH1F ("TimeSlicesHist", "Average signal in all time slices", 10, 0, 10);
}

FRPedestalsMaker::~FRPedestalsMaker(){
  delete mRootFile;
}

void FRPedestalsMaker::beginJob(const edm::EventSetup& c){
}


void FRPedestalsMaker::analyze(const edm::Event& e, const edm::EventSetup& eventSetup){

  ///get digis
  edm::Handle<HBHEDigiCollection> hbhe; 
  e.getByType(hbhe);
  fillHists<HBHEDataFrame> (*hbhe, mHists, mPreSliceFirst, mPreSliceLast, mPostSliceFirst, mPostSliceLast);
  edm::Handle<HODigiCollection> ho;     
  e.getByType(ho);
  fillHists<HODataFrame> (*ho, mHists, mPreSliceFirst, mPreSliceLast, mPostSliceFirst, mPostSliceLast);
  edm::Handle<HFDigiCollection> hf;     
  e.getByType(hf);
  fillHists<HFDataFrame> (*hf, mHists, mPreSliceFirst, mPreSliceLast, mPostSliceFirst, mPostSliceLast);
  
  // get conditions
  if (!mConditions) {
    edm::ESHandle<HcalDbService> conditions;
    eventSetup.get<HcalDbRecord>().get(conditions);
    mConditions = &*conditions;
  }
}

void FRPedestalsMaker::endJob(void) {
  const HcalQIEShape* shape = mConditions->getHcalShape ();
  // prepare output
  HcalPedestals pedestals;
  HcalPedestals pedestalsADC;
  
  for (std::map <unsigned, TH1F*>::iterator it = mHists[0].begin(); it != mHists[0].end(); ++it) {
    int id = it->first;
    float value[4] = {0,0,0,0};
    float valueADC[4] = {0,0,0,0};
    for (int capId = 0; capId < 4; ++capId) {
      if (mHists[capId].find (id) == mHists[capId].end()) {
	std::cerr << "strange... Can't find histogram for capId=" << capId << ", " << HcalDetId (id) << std::endl;
      }
      else {
	double ped_adc = getPeak (mHists[capId][id]);
	valueADC[capId] = ped_adc;
	const HcalQIECoder* coder = mConditions->getHcalCoder (id);
	double ped_low = floor (ped_adc);
	if (ped_low < 0) ped_low = 0;
	double ped_high = ped_low+1.;
	
	double ped_fc_low = coder->charge (*shape, int (ped_low), capId);
	double ped_fc_high = coder->charge (*shape, int (ped_high), capId);
	if (ped_high - ped_low > 0)
	  value[capId] = (ped_fc_low * (ped_high - ped_adc) + ped_fc_high * (ped_adc - ped_low)) / (ped_high - ped_low);
	else
	  value[capId]= ped_fc_low;
	// store histograms
	mHists[capId][id]->Write();
      }
    }
    pedestals.addValues (HcalPedestal (DetId(id), value[0], value[1], value[2], value[3])); 
    pedestalsADC.addValues (HcalPedestal (DetId(id), valueADC[0], valueADC[1], valueADC[2], valueADC[3])); 
  }

  std::ofstream stream ((mPedestalsFile+"_fC.txt").c_str());
  HcalDbASCIIIO::dumpObject (stream, pedestals);
  std::ofstream stream2 ((mPedestalsFile+"_ADC.txt").c_str());
  HcalDbASCIIIO::dumpObject (stream2, pedestalsADC);

  mRootFile->Write ();
}
