#include "RecoLuminosity/LumiCalc/interface/LumiCalc.h"

LumiCalc::LumiCalc(){
  // Run SumHLXs first to load information from LUMI_SECTION.
  lastRun = 0;
  bunchStructure = 1;
  SetNoiseInterval(3564-119, 3564);
}

LumiCalc::LumiCalc(HCAL_HLX::LUMI_SECTION& localSection, u16 start = 3564 - 119, u16 end = 3564, i8 scheme=1, u32 Run=0){
  
  bunchStructure = scheme;
  lastRun = Run;
  SetNoiseInterval(start, end);
  SumHLXs(localSection);
}

//****************** Initialization and Configuration Functions ***************

void LumiCalc::SumHLXs(HCAL_HLX::LUMI_SECTION& localSection){
  // SumHLXs gathers information from LUMI_SECTION and sums over HLX's.
  // If it is nessacary to re-sum a LUMI_SECTION, run ResetRunNumber() before performing any calculation or the noise will be wrong.
  u8 i, j;
  u16 k;

  currentRun = localSection.hdr.runNumber;

  memset(totalOccupancy, 0, sizeof(totalOccupancy)); 
  memset(totalEtSum, 0, sizeof(totalEtSum));

  for(i=0; i<36; i++){ 
    for(k=0; k<3564; k++){	
      for(j=0; j<6; j++){
	totalOccupancy[j][k] += localSection.occupancy[i].data[j][k];
      }
      totalEtSum[k] += localSection.etSum[i].data[k];
    }
  }
}

void LumiCalc::SetNoiseInterval(u16 start, u16 end){
  // must be set before CalcNoise() is called.  Otherwise, it is nessacary to call ResetRunNumber().
  u16 temp;

  if(start==end){   // forces non-zero interval
    start = end - 1;
  }

  if(start > end){ // swaps order 
    temp = start;
    start = end;
    end = temp;
  }

  if(bunchStructure == 1){
    if(start < (3564 -119)){
      startAG = 3564 - 119;  // defaults to beginning of abort gap
    }else{
      startAG = start;
    }

    if(end > 3564 ){
      endAG = 3564;          // defaults to end of abort gap
    } else {
      endAG = end;
    }
  }
}

void LumiCalc::ResetRunNumber(u32 Run=0){
   lastRun = Run;
}

//****************** end Initialization and Configuration Functions ***********

//***************** Luminosity Calculation Functions ***************
// Calculations can be performed in any order.

void LumiCalc::OccLumiBX(){
  u8 i;  
  u16 j;
  
  CalcNoise();

  for(i=0; i<2; i++){
    for(j=0; j < 3564; j++){
      if((noise[i] != 0) && (perBX.OccLumi[i][j] != 0)){
	perBX.OccLumi[i][j] /= noise[i];
	perBX.OccLumi[i][j] = -log(perBX.OccLumi[i][j]); 
      }else{
	perBX.OccLumi[i][j] = 0;
      }
    }
  }
}

void LumiCalc::InstOccLumi(){

  u32 numerator[2] = {0,0};
  u32 denominator[2] = {0,0};
  u16 BXCount =0;
   
  u8 i, k;
  u16 j;
   
  for(i=0; i<2; i++){ 
    for(j=0; j<3564; j++){
      if(isBunch(j, bunchStructure)){  // only count the bunches that should contain particles
	numerator[i] +=  totalOccupancy[3*i+2][j];
	for(k=0; k<3; k++){ 
	  denominator[i] += totalOccupancy[3*i+k][j];
	}
	BXCount++;
      }
    }
  }

  CalcNoise();

  for(i=0; i<2; i++){
    if((denominator[i] != 0) && (BXCount !=0)){
      summary.InstantOccLumi[i] = (double)numerator[i]/denominator[i];  // tower  normalization
      summary.InstantOccLumi[i] /= (double)BXCount; // bunch crossing normalization
    }else{
      summary.InstantOccLumi[i] = 0; // Are the towers off?
    }
    if((noise[i] !=0) && (summary.InstantOccLumi[i] != 0)){
      summary.InstantOccLumi[i] /= noise[i];
      summary.InstantOccLumi[i] = -log(summary.InstantOccLumi[i]);
    }else{
      summary.InstantOccLumi[i]=0;  // Something funny is going on.
    }
  }
}

void LumiCalc::ETLumiBX(){
  u8 k;
  u16 j;
  u32 denominator;

  CalcNoise();
  
  for(j=0; j < 3564; j++){
    for(k=0, denominator=0; k<6; k++){
      denominator += totalOccupancy[k][j];
    }
    perBX.ETLumi[j] = (double)totalEtSum[j]/denominator; // tower normalization
  }
}

void LumiCalc::InstETLumi(){
  CalcNoise();
  u32 numerator = 0;
  u32 denominator = 0;
  u16 j;
  u8 k;

  for(j=0; j < 3564; j++){
    if(isBunch(j, bunchStructure) == 1){
      numerator += totalEtSum[j];
      for(k=0; k < 6; k++){
	denominator += totalOccupancy[k][j];
      }
    }
  }
  summary.InstantETLumi = (double)numerator/denominator - (noise[1]+noise[2])/2;
}

//**************** end Luminosity Calculation Functions *******

// ******************* Private member functions **************

void LumiCalc::ZeroFracPerBX(){
  u8 i, k;
  u16 j;
  double denominator;

  for(i=0; i<2; i++){ 
    for(j=0; j <3564; j++){
      for(k=0, denominator=0; k<3; k++){
	denominator += totalOccupancy[3*i+k][j];
      }
      if(denominator != 0){
	perBX.OccLumi[i][j] = (double)totalOccupancy[3*i+2][j]/denominator;
      }else{
	perBX.OccLumi[i][j] = 0;  // towers are all off
      }
    }
  }
}

void LumiCalc::CalcNoise(){
  // CalcNoise won't perform this calculation if it has already been performed for this run in order to save time. 
  // Use ResetRunNumber() to start fresh.
  // The definition of noise is the integral of the noise distribution function from zero energy to the lower threshold.
  // Since all the bunch crossings in the abort gap should be below threshold, the noise is calculated to be the average
  // fraction of empty towers of a user specified region of the abort gap.
  u8 i;
  u16 j;

  if(currentRun > lastRun){  
    ZeroFracPerBX();
    for(i=0; i<2; i++){
      for(j= startAG; j < endAG; j++){  
	noise[i] += perBX.OccLumi[i][j];
      }
      noise[i] = noise[i]/(endAG-startAG); // (endAG - startAG) is guaranteed, by SetNoiseInterval, to be non-zero and positive.
    }
    lastRun = currentRun;
  }
}


// **************************** end Private member functions **********************************

// *********** General Calculations used independently *************************
void AvgOccupancy(const HCAL_HLX::OCCUPANCY_SECTION& localOcc, float * output, u32 numBunches = 3564, i8 scheme = 1){
  // used by DQMSource
  u8 j;
  u16 i;
  u32 Total[6] = {0,0,0,0,0,0};
  u32 Empty[6] = {0,0,0,0,0,0};
  for(j=0; j< 6; j++){

    if(scheme == 1){
      for(i=0; i< 3564; i++){
	if(isBunch(i,1)==1){
	  Total[j] += localOcc.data[j][i];
	}else{ 
	  Empty[j] += localOcc.data[j][i];
	}
      }
    
      output[j] = (double)Total[j]/numBunches;
    }
  }
}

double AvgETSum(const HCAL_HLX::ET_SUM_SECTION& localEtSum, u32 numBunches = 3564, i8 scheme = 1){
  unsigned short int i;
  unsigned long int Total = 0;
  for(i=0; i<numBunches; i++){
    Total += localEtSum.data[i];
  }
  return (double)Total/numBunches;
}



 u8 Block(u32 BX, u8 numBlock){
   const u8 gap=0;
   const u8 bunch=1;

   if(BX > (u32)numBlock*80){
     return gap;
   }else if(BX%80 > 72){
     return gap;
   }else {
     return bunch;
   }
 }


u8 isBunch(u32 BX, i8 scheme = 1){
  // determine where you are in the bunch structure
  const u8 abortGap =2;
  u16 BXrel;

  if(scheme == 1){
    BXrel = BX%3564;
    
    if(BXrel < 3445){
      if(BXrel < 190){
       	return Block(BXrel,        2);
      }else if(BXrel < 460){
	return Block(BXrel - 190,  3);
      }else if(BXrel < 811){
	return Block(BXrel - 460,  4);
      }else if(BXrel < 1081){
	return Block(BXrel - 811,  3);
      }else if(BXrel < 1351){
	return Block(BXrel - 1081, 3);
      }else if(BXrel < 1702){
	return Block(BXrel - 1351, 4);
      }else if(BXrel < 1972){
	return Block(BXrel - 1702, 3);
      }else if(BXrel < 2242){
	return Block(BXrel - 1972, 3);
      }else if(BXrel < 2593){
	return Block(BXrel - 2242, 4);
      }else if(BXrel < 2863){
	return Block(BXrel - 2593, 3);
      }else if(BXrel < 3133){
	return Block(BXrel - 2863, 3);
      }else if(BXrel < 3484){
	return Block(BXrel - 3133, 4);
      }
    } else{
      return abortGap;
    }
  }
  return 3;    // invalid bunch structure
}

//*****************  end General functions ******************************
