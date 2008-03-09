#include "CalibCalorimetry/HcalTPGAlgos/interface/HcaluLUTTPGCoder.h"
#include "Geometry/HcalTowerAlgo/src/HcalHardcodeGeometryData.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <iostream>
#include <fstream>
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include <cmath>

const float HcaluLUTTPGCoder::nominal_gain = 0.177; 

HcaluLUTTPGCoder::HcaluLUTTPGCoder(const char* filename, bool read_Ascii_LUTs) {
  AllocateLUTs();
  // std::cout << " filename:" << filename << " read_Ascii_LUTs " << read_Ascii_LUTs << std::endl;
  if (read_Ascii_LUTs == true) {
    update(filename);
  }
  else {
    getRecHitCalib(filename);
  }
}



void HcaluLUTTPGCoder::compress(const IntegerCaloSamples& ics, const std::vector<bool>& featureBits, HcalTriggerPrimitiveDigi& tp) const {
  throw cms::Exception("PROBLEM: This method should never be invoked!");
}

HcaluLUTTPGCoder::~HcaluLUTTPGCoder() {
  for (int i = 0; i < nluts; i++) {
    if (inputLUT[i] != 0) delete [] inputLUT[i];
  }
  delete [] _gain;
  delete [] _ped;
}

void HcaluLUTTPGCoder::AllocateLUTs() {
  HcalTopology theTopo;
  HcalDetId did;

  _ped = new float[nluts];
  _gain = new float[nluts];
  for (int i = 0; i < nluts; i++) inputLUT[i] = 0;
  int maxid = 0, minid = 0x7FFFFFFF, rawid = 0;
  for (int ieta=-41; ieta <= 41; ieta++) {
    for (int iphi = 1; iphi <= 72; iphi++) {
      for (int depth = 1; depth <= 3; depth++) {
	did=HcalDetId(HcalBarrel,ieta,iphi,depth);
	if (theTopo.valid(did)) {
	  rawid = GetLUTID(HcalBarrel, ieta, iphi, depth);
	  if (inputLUT[rawid] != 0) std::cout << "Error: LUT with (ieta,iphi,depth) = (" << ieta << "," << iphi << "," << depth << ") has been previously allocated!" << std::endl;
	  else inputLUT[rawid] = new LUT[INPUT_LUT_SIZE];
	  if (rawid < minid) minid = rawid;
	  if (rawid > maxid) maxid = rawid;
	}

	did=HcalDetId(HcalEndcap,ieta,iphi,depth);
	if (theTopo.valid(did)) {
	  rawid = GetLUTID(HcalEndcap, ieta, iphi, depth);
	  if (inputLUT[rawid] != 0) std::cout << "Error: LUT with (ieta,iphi,depth) = (" << ieta << "," << iphi << "," << depth << ") has been previously allocated!" << std::endl;
	  else inputLUT[rawid] = new LUT[INPUT_LUT_SIZE];
	  if (rawid < minid) minid = rawid;
	  if (rawid > maxid) maxid = rawid;
	}
	did=HcalDetId(HcalForward,ieta,iphi,depth);
	if (theTopo.valid(did)) {
	  rawid = GetLUTID(HcalForward, ieta, iphi, depth);
	  if (inputLUT[rawid] != 0) std::cout << "Error: LUT with (ieta,iphi,depth) = (" << ieta << "," << iphi << "," << depth << ") has been previously allocated!" << std::endl;
	  else inputLUT[rawid] = new LUT[INPUT_LUT_SIZE];
	  if (rawid < minid) minid = rawid;
	  if (rawid > maxid) maxid = rawid;
	}
      }
    }
  }

}

int HcaluLUTTPGCoder::GetLUTID(HcalSubdetector id, int ieta, int iphi, int depth) const {
  int detid = 0;
  if (id == HcalEndcap) detid = 1;
  else if (id == HcalForward) detid = 2;
  return iphi + 72 * ((ieta + 41) + 83 * (depth + 3 * detid)) - 7777;
}

void HcaluLUTTPGCoder::getRecHitCalib(const char* filename) {

   std::ifstream userfile;
   userfile.open(filename);
   int tool;
   float Rec_calib_[87];
 
   if (userfile) {
	       userfile >> tool;

	       if (tool != 86) {
		 std::cout << "Wrong RecHit calibration filesize: " << tool << " (expect 86)" << std::endl;
	       }
     for (int j=1; j<87; j++) {
       userfile >> Rec_calib_[j]; // Read the Calib factors
       Rcalib[j] = Rec_calib_[j] ;
     }
   
     userfile.close();  
   }
   else  std::cout << "File " << filename << " with RecHit calibration factors not found" << std::endl;
}


void HcaluLUTTPGCoder::update(const char* filename) {
  int id;
  HcalTopology theTopo;
  int tool;
  
  std::ifstream userfile;
  userfile.open(filename);
  //  std::cout << filename << std::endl;
  if( userfile ) {
    int nluts;
    std::vector<int> loieta,hiieta;
    userfile >> nluts;

    inputluts_.resize(nluts);
    for (int i=0; i<nluts; i++) {
      inputluts_[i].resize(INPUT_LUT_SIZE); 
    }
    
    for (int i=0; i<nluts; i++) {
      userfile >> tool;
      loieta.push_back(tool);
    }
    for (int i=0; i<nluts; i++) {
      userfile >> tool;
      hiieta.push_back(tool);
    }
    
    for (int j=0; j<INPUT_LUT_SIZE; j++) { 
      for(int i = 0; i <nluts; i++) {
	userfile >> inputluts_[i][j];}
    }
    
    userfile.close();
          
    //  std::cout << "nluts:" << nluts << std::endl;

    for (int depth = 1; depth <= 3; depth++) {
     for (int iphi = 1; iphi <= 72; iphi++) {
      
       for (int ieta=-16; ieta <= 16; ieta++) {
	 HcalDetId cell(HcalBarrel,ieta,iphi,depth);
	 if (theTopo.valid(cell)) {  
	   id = GetLUTID(HcalBarrel,ieta,iphi,depth);
	   if (inputLUT[id] == 0) throw cms::Exception("PROBLEM: inputLUT has not been initialized for HB, ieta, iphi, depth, id = ") << ieta << "," << iphi << "," << depth << "," << id << std::endl;
	
	    for (int j = 0; j <= 0x7F; j++) {
	      inputLUT[id][j] = inputluts_[0][j];  //Using LUT 0 for HB
	    }
	 }
       }

       
       for (int ieta=-29; ieta <= 29; ieta++) {
	 HcalDetId cell(HcalEndcap,ieta,iphi,depth);
	 if (theTopo.valid(cell)) { 
	   id = GetLUTID(HcalEndcap,ieta,iphi,depth);
	   if (inputLUT[id] == 0) throw cms::Exception("PROBLEM: inputLUT has not been initialized for HE, ieta, iphi, depth, id = ") << ieta << "," << iphi << "," << depth << "," << id << std::endl;
	   for (int j = 0; j <= 0x7F; j++) {
	     if (abs(ieta) < 21) inputLUT[id][j] = inputluts_[0][j];  // LUT0 for HE upto ieta=21
	     if (abs(ieta) >= 21 && abs(ieta) < 27) inputLUT[id][j] = inputluts_[1][j];  // LUT1 for HE for ieta 21-26
	     if (abs(ieta) >= 27 && abs(ieta) < 29) inputLUT[id][j] = inputluts_[2][j];  // LUT2 for HE for ieta 27-28
	    }
	 }
       }
      
     
       for (int ieta=-41; ieta <= 41; ieta++) {
	 HcalDetId cell(HcalForward,ieta,iphi,depth);
	 if (theTopo.valid(cell)) { 
	   id = GetLUTID(HcalForward,ieta,iphi,depth);
	   if (inputLUT[id] == 0) throw cms::Exception("PROBLEM: inputLUT has not been initialized for HF, ieta, iphi, depth, id = ") << ieta << "," << iphi << "," << depth << "," << id << std::endl;
	   for (int j = 0; j <= 0x7F; j++) {
	     if (depth==1)inputLUT[id][j] = inputluts_[abs(ieta)-26][j];    // LUT for long fibers; use LUT3 for abs(ieta)=29 for eg. 
	     if (depth==2)inputLUT[id][j] = inputluts_[abs(ieta)-26+13][j]; // LUT for short fibers (LUT16 is for abs(ieta)=29 for eg.)
	    }
	 }
       }
       
     }

    }
  }
}


void HcaluLUTTPGCoder::update(const HcalDbService& conditions) {
   const HcalQIEShape* shape = conditions.getHcalShape();
   HcalCalibrations calibrations;
   int id;
   float divide;
   HcalTopology theTopo;

   float cosheta_[41], lsb_ = 1./16.;
   for (int i = 0; i < 13; i++) cosheta_[i+29] = cosh((theHFEtaBounds[i+1] + theHFEtaBounds[i])/2.);
    
   for (int depth = 1; depth <= 3; depth++) {
     for (int iphi = 1; iphi <= 72; iphi++) {
       divide = 1.*nominal_gain;
       for (int ieta=-16; ieta <= 16; ieta++) {
	 HcalDetId cell(HcalBarrel,ieta,iphi,depth);
	 if (theTopo.valid(cell)) {  
	   id = GetLUTID(HcalBarrel,ieta,iphi,depth);
	   if (inputLUT[id] == 0) throw cms::Exception("PROBLEM: inputLUT has not been initialized for HB, ieta, iphi, depth, id = ") << ieta << "," << iphi << "," << depth << "," << id << std::endl;
	   //conditions.makeHcalCalibration (cell, &calibrations);
	   HcalCalibrations calibrations = conditions.getHcalCalibrations(cell);

	   const HcalQIECoder* channelCoder = conditions.getHcalCoder (cell);
	   HcalCoderDb coder (*channelCoder, *shape);
	   float ped_ = (calibrations.pedestal(0)+calibrations.pedestal(1)+calibrations.pedestal(2)+calibrations.pedestal(3))/4;
	   float gain_= (calibrations.respcorrgain(0)+calibrations.respcorrgain(1)+calibrations.respcorrgain(2)+calibrations.respcorrgain(3))/4;          
	   HBHEDataFrame frame(cell);
	   frame.setSize(1);
	   CaloSamples samples(cell, 1);
	   _ped[id] = ped_;
           _gain[id] = gain_;
	   for (int j = 0; j <= 0x7F; j++) {
	     HcalQIESample adc(j);
	     frame.setSample(0,adc);
	     coder.adc2fC(frame,samples);
	     float adc2fC_ = samples[0];
	     if (ieta <0 )inputLUT[id][j] = (LUT) std::min(std::max(0,int((adc2fC_ - ped_)*gain_*Rcalib[abs(ieta)]/divide)), 0x3FF);
	     else inputLUT[id][j] = (LUT) std::min(std::max(0,int((adc2fC_ - ped_)*gain_*Rcalib[abs(ieta)+43]/divide)), 0x3FF);
	   }
	 }
       }
       for (int ieta=-29; ieta <= 29; ieta++) {
	 HcalDetId cell(HcalEndcap,ieta,iphi,depth);
	 if (theTopo.valid(cell)) {  
	   if (abs(ieta) < 21) divide = 1.*nominal_gain;
	   else if (abs(ieta) < 27) divide = 2.*nominal_gain;
	   else divide = 5.*nominal_gain;
	   id = GetLUTID(HcalEndcap,ieta,iphi,depth);
	   if (inputLUT[id] == 0) throw cms::Exception("PROBLEM: inputLUT has not been initialized for HE, ieta, iphi, depth, id = ") << ieta << "," << iphi << "," << depth << "," << id << std::endl;
	   //conditions.makeHcalCalibration (cell, &calibrations);
	   HcalCalibrations calibrations = conditions.getHcalCalibrations(cell);
	   const HcalQIECoder* channelCoder = conditions.getHcalCoder (cell);
	   HcalCoderDb coder (*channelCoder, *shape);
	   float ped_ = (calibrations.pedestal(0)+calibrations.pedestal(1)+calibrations.pedestal(2)+calibrations.pedestal(3))/4;
	   float gain_= (calibrations.respcorrgain(0)+calibrations.respcorrgain(1)+calibrations.respcorrgain(2)+calibrations.respcorrgain(3))/4;          
	   HBHEDataFrame frame(cell);
	   frame.setSize(1);
	   CaloSamples samples(cell, 1);
	   _ped[id] = ped_;
           _gain[id] = gain_;
	   for (int j = 0; j <= 0x7F; j++) {
	     HcalQIESample adc(j);
	     frame.setSample(0,adc);
	     coder.adc2fC(frame,samples);
	     float adc2fC_ = samples[0];
	     if ( ieta < 0 ) inputLUT[id][j] = (LUT) std::min(std::max(0,int((adc2fC_ - ped_)*gain_*Rcalib[abs(ieta)+1]/divide)), 0x3FF);
	     else inputLUT[id][j] = (LUT) std::min(std::max(0,int((adc2fC_ - ped_)*gain_*Rcalib[abs(ieta)+44]/divide)), 0x3FF);
	   }
	 }
       }        
       for (int ieta=-41; ieta <= 41; ieta++) {
	 HcalDetId cell(HcalForward,ieta,iphi,depth);
	 if (theTopo.valid(cell)) {  
	   id = GetLUTID(HcalForward,ieta,iphi,depth);
	   if (inputLUT[id] == 0) throw cms::Exception("PROBLEM: inputLUT has not been initialized for HF, ieta, iphi, depth, id = ") << ieta << "," << iphi << "," << depth << "," << id << std::endl;
	   //conditions.makeHcalCalibration (cell, &calibrations);
	   HcalCalibrations calibrations = conditions.getHcalCalibrations(cell);
	   const HcalQIECoder* channelCoder = conditions.getHcalCoder (cell);
	   HcalCoderDb coder (*channelCoder, *shape);
	   float ped_ = (calibrations.pedestal(0)+calibrations.pedestal(1)+calibrations.pedestal(2)+calibrations.pedestal(3))/4;
	   float gain_= (calibrations.respcorrgain(0)+calibrations.respcorrgain(1)+calibrations.respcorrgain(2)+calibrations.respcorrgain(3))/4;          


	   HFDataFrame frame(cell);
	   frame.setSize(1);
	   CaloSamples samples(cell, 1);
	   _ped[id] = ped_;
           _gain[id] = gain_;
	   for (int j = 0; j <= 0x7F; j++) {
	     HcalQIESample adc(j);
	     frame.setSample(0,adc);
	     coder.adc2fC(frame,samples);
	     float adc2fC_ = samples[0];
	     if (ieta < 0 ) inputLUT[id][j] = (LUT) std::min(std::max(0,int((adc2fC_ - ped_)*Rcalib[abs(ieta)+2]*gain_/lsb_/cosheta_[abs(ieta)])), 0x3FF);
	     else inputLUT[id][j] = (LUT) std::min(std::max(0,int((adc2fC_ - ped_)*Rcalib[abs(ieta)+45]*gain_/lsb_/cosheta_[abs(ieta)])), 0x3FF);
	   }
	 }
       }
     }
   }
 }

 void HcaluLUTTPGCoder::adc2Linear(const HBHEDataFrame& df, IntegerCaloSamples& ics) const {
   int id = GetLUTID(df.id().subdet(), df.id().ieta(), df.id().iphi(), df.id().depth());
   if (inputLUT[id]==0) {
     throw cms::Exception("Missing Data") << "No LUT for " << df.id();
   } 
   else {
     for (int i=0; i<df.size(); i++){
       if (df[i].adc() >= INPUT_LUT_SIZE || df[i].adc() < 0) throw cms::Exception("ADC overflow for HBHE tower: ") << i << " adc= " << df[i].adc();
       if (inputLUT[id] !=0) ics[i]=inputLUT[id][df[i].adc()];
     }  
   }
 }

 void HcaluLUTTPGCoder::adc2Linear(const HFDataFrame& df, IntegerCaloSamples& ics)  const{
   int id = GetLUTID(df.id().subdet(), df.id().ieta(), df.id().iphi(), df.id().depth());
   if (inputLUT[id]==0) {
     throw cms::Exception("Missing Data") << "No LUT for " << df.id();
   } else {
     for (int i=0; i<df.size(); i++){
       if (df[i].adc() >= INPUT_LUT_SIZE || df[i].adc() < 0)
	 throw cms::Exception("ADC overflow for HF tower: ") << i << " adc= " << df[i].adc();
       if (inputLUT[id] !=0) ics[i]=inputLUT[id][df[i].adc()];
     }
   }
 }

unsigned short HcaluLUTTPGCoder::adc2Linear(HcalQIESample sample, HcalDetId id) const {
  int ref = GetLUTID(id.subdet(), id.ieta(), id.iphi(), id.depth());
  return inputLUT[ref][sample.adc()];
}

float HcaluLUTTPGCoder::getLUTPedestal(HcalDetId id) const {
  int ref = GetLUTID(id.subdet(), id.ieta(), id.iphi(), id.depth());
  return _ped[ref];
}

float HcaluLUTTPGCoder::getLUTGain(HcalDetId id) const {
  int ref = GetLUTID(id.subdet(), id.ieta(), id.iphi(), id.depth());
  return _gain[ref];
}
