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
#include "TH2F.h"
#include "TF1.h"



#include <math.h>
#include <iostream>
#include <fstream>

namespace {
  template <class T> 
  void fillHists (const edm::SortedCollection<T>& fCollection, 
		  std::map < unsigned, TH1F* > * fHists, TH1F* fTimeSlicesHist,
		  int fPreFirst, int fPreLast, int fPostFirst, int fPostLast)
{
    for (unsigned iHit = 0; iHit < fCollection.size(); ++iHit) {
      const T& frame = fCollection[iHit];
      HcalDetId hid = frame.id();
      unsigned id = hid.rawId();
      bool notEmpty = false;
      for (int slice = 0; slice < frame.size(); ++slice) {
	int adc = frame[slice].adc();
	if ((slice >= fPreFirst && slice <= fPreLast) || (slice >= fPostFirst && slice <= fPostLast)) { // consider slices out of signal area
	  int capId = frame[slice].capid();
	  // search where to store this information
	  if (fHists[capId].find (id) == fHists[capId].end()) {
	    // book new histogram
	    char label [1024];
	    sprintf (label, "adc_%c%d_%d_%d_%s_%d", 
		     hid.ieta()>0?'p':'m', hid.ietaAbs(), hid.iphi(), hid.depth(), 
		     hid.subdetId() == 1 ? "HB" : hid.subdetId() == 2 ? "HE" : 
		     hid.subdetId() == 3 ? "HO" : hid.subdetId() == 4 ? "HF" : "NA", 
		     capId);
	    char title [1024];
	    sprintf (title, "ADC subdet=%s eta=%d phi=%d depth=%d, capId=%d", 
		     hid.subdetId() == 1 ? "HB" : hid.subdetId() == 2 ? "HE" : 
		     hid.subdetId() == 3 ? "HO" : hid.subdetId() == 4 ? "HF" : "NA", 
		     hid.ieta(), hid.iphi(), hid.depth(), capId);
	    fHists[capId][id] = new TH1F (label, title, 25, -0.5, 24.5);
	  }
	  fHists[capId][id]->Fill (adc);
	}
	if (adc > 10) notEmpty = true;
      }
      if (notEmpty) {
	for (int slice = 0; slice < frame.size(); ++slice) fTimeSlicesHist->Fill (slice, frame[slice].adc());
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

  void setHcal2DHist (TH2F* fHist[4], const char* fLabel, const char* fTitle) {
    for (unsigned i = 0; i < 4; ++i) {
      char label [1024];
      sprintf (label, "%s_%d", fLabel, i+1);
      char title [1024];
      sprintf (title, "%s, depth=%d", fTitle, i+1);
      if (fHist[i]) {
	std::cerr << "setHcal2DHist-> Histogram " << i << " is booked already. Reset" << std::endl;
	delete fHist[i];
      }
      fHist[i] = new TH2F (label, title, 85, -42.5, 42.5, 72, 0.5, 72.5);
    }
  }

  void setHcal2DValue (TH2F* fHist[4], HcalDetId fId, float fValue) {
    int depth = fId.depth();
    int eta = fId.ieta();
    int phi = fId.iphi();
    if (eta == 29 && fId.subdet() == HcalEndcap) {
      depth = depth + 2;
    }
    int index = depth - 1;
    fHist[index]->Fill (eta, phi, fValue);
    // std::cout << "setHcal2DValue-> " << depth << '/' << index << '/' << eta << '/' << phi << '/' << fValue << std::endl;
    if (fabs(eta) > 20) { // fill also adjacent phi
     fHist[index]->Fill (eta, phi+1, fValue);
    }
    if (fabs(eta) > 39) { // fill also adjacent phi in VF
     fHist[index]->Fill (eta, phi+2, fValue);
     fHist[index]->Fill (eta, phi+3, fValue);
    }
  }
  
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
  mTimeSlicesHist = new TH1F ("AA_TimeSlicesHist", "Average signal in all time slices", 10, 0, 10);
  mTimeSlicesHist->Sumw2();
  mPedDifs[0] = mPedDifs[1] = mPedDifs[2] = mPedDifs[3] = 0; 
  setHcal2DHist (mPedDifs, "AA_pedDifs", "Difference between calculated and set pedestals");
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
  fillHists<HBHEDataFrame> (*hbhe, mHists, mTimeSlicesHist, mPreSliceFirst, mPreSliceLast, mPostSliceFirst, mPostSliceLast);
  edm::Handle<HODigiCollection> ho;     
  e.getByType(ho);
  fillHists<HODataFrame> (*ho, mHists, mTimeSlicesHist, mPreSliceFirst, mPreSliceLast, mPostSliceFirst, mPostSliceLast);
  edm::Handle<HFDigiCollection> hf;     
  e.getByType(hf);
  fillHists<HFDataFrame> (*hf, mHists, mTimeSlicesHist, mPreSliceFirst, mPreSliceLast, mPostSliceFirst, mPostSliceLast);
  
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
    DetId detId (id);
    pedestals.addValues (HcalPedestal (detId, value[0], value[1], value[2], value[3])); 
    pedestalsADC.addValues (HcalPedestal (detId, valueADC[0], valueADC[1], valueADC[2], valueADC[3])); 
    // fill average difference
    const HcalPedestal* p1 = pedestals.getValues (detId);
    const HcalPedestal* p2 = mConditions->getPedestal (detId);
    double diff = 
      0.25 * (p1->getValue(0) + p1->getValue(1) + p1->getValue(2) + p1->getValue(3)) -
      0.25 * (p2->getValue(0) + p2->getValue(1) + p2->getValue(2) + p2->getValue(3));
    setHcal2DValue (mPedDifs, HcalDetId (id), diff);
  }

  mTimeSlicesHist->Write();
  for (unsigned i = 0; i < 4; ++i) mPedDifs[i]->Write();

  std::ofstream stream ((mPedestalsFile+"_fC.txt").c_str());
  HcalDbASCIIIO::dumpObject (stream, pedestals);
  std::ofstream stream2 ((mPedestalsFile+"_ADC.txt").c_str());
  HcalDbASCIIIO::dumpObject (stream2, pedestalsADC);

  mRootFile->Write ();
}
