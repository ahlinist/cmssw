/****************************************************************************

Author:  Adam Hunt
         ahunt@princeton.edu

Summary:  Library to calculate relative luminosity from HLX data

*****************************************************************************/

#include "RecoLuminosity/TCPReceiver/interface/ICTypeDefs.hh"
#include "RecoLuminosity/TCPReceiver/interface/LumiStructures.hh"
#include "RecoLuminosity/LumiCalc/interface/LumiCalc.hh"

namespace HCAL_HLX {
  
   LumiCalc::LumiCalc(){
    
      unsigned int BXMask[HCAL_HLX_NUM_BUNCHES];
      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ) BXMask[iBX] = 0;
      unsigned int HLXMask[HCAL_HLX_NUM_HLXS];
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ) HLXMask[iHLX] = 1;
      // Should these be combined to form a matrix so that different BXs are masked for different HLXs?

      //std::cout << "In ctor set arrays" << std::endl;

      // Masking convension: Masked 0, Data 1, Noise 2.  Anything marked as 2 will be used to calculate noise.
      for( unsigned int iBX = 0;  iBX < 120; ++iBX )                    BXMask[iBX] = 0; // Mask
      for( unsigned int iBX = 120;  iBX < 3400; ++iBX )                 BXMask[iBX] = 1; // UnMask
      for( unsigned int iBX = 3400; iBX < 3524; ++iBX )                 BXMask[iBX] = 2; // Noise
      for( unsigned int iBX = 3524; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ) BXMask[iBX] = 0; // Mask

      SetBXMask( BXMask, HCAL_HLX_NUM_BUNCHES );
      SetHLXMask( HLXMask, HCAL_HLX_NUM_HLXS );

      // Et sum error parameters
      sigmaEt_ = 0.0;
      sigmaEtWeight_ = 0.0;
      sigmaEtNoise_ = 0.0;
      sigmaEtNoiseWeight_ = 0.0;
      etStoreSize_ = 0;
      etNoiseStoreSize_ = 0;

      // Initialize error arrays
      for( unsigned int iLB = 0; iLB < ET_SUM_STORE_SIZE; ++iLB ){
	 for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    etSumAvgPerLB_[iLB][iBX] = 0.0;
	    etSumNoiseAvgPerLB_[iLB][iBX] = 0.0;
	 }
      }

      // Et sum tower counting constants
      noMasks_ = false;
      numOrbitsPerNB_ = 4096;
      //numOrbitsPerNB_ = 512;

   }
  
   //****************** Initialization and Configuration Functions ***************
    
   unsigned int LumiCalc::SetBXMask( unsigned int BXMask[], unsigned int maskSize ){
      // Copy given mask into member variable and count number of active bunches

      numBX_ = 0;
      sizeNoise_[0] = 0;
      sizeNoise_[1] = 0;
      sizeNoise_[2] = 0;
      sizeNoise_[3] = 0;
    
      for( unsigned int iBX = 0; iBX < maskSize ; ++iBX ){
	 if( BXMask[iBX] < 3 ) {
	    BXMask_[iBX] = BXMask[iBX];
	 } else {
	    BXMask_[iBX] = 1;
	 }
	 if( BXMask_[iBX] == 1 ) 
	    ++numBX_;
	 if( BXMask_[iBX] == 2 ) 
	    ++sizeNoise_[iBX % 4];
      }

      for( unsigned int iBX = maskSize; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 BXMask_[iBX] = 0;
      }
    
      return numBX_;
   }

   unsigned int LumiCalc::SetHLXMask( unsigned int HLXMask[], unsigned int maskSize ){
      // Copy given mask into member variable and count number of active HLXs

      numHLX_ = 0;

      for( unsigned int iHLX = 0; iHLX < maskSize ; ++iHLX ){
	 HLXMask_[iHLX] = ( HLXMask[iHLX] == 0 )? 0:1;
	 numHLX_ += HLXMask_[iHLX];
      }      

      for( unsigned int iHLX = maskSize; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX )
	 HLXMask_[iHLX] = 0;
   
      return numHLX_;
   }

   //****************** end Initialization and Configuration Functions ***********

   void LumiCalc::CountActiveTowers( HCAL_HLX::LUMI_SECTION &localSection ){
      // Should only be calculated once per run.
    
      TotalActiveTowers_ = 0;

      //find first unmasked BX
      unsigned int iBX;
      for( iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( BXMask_[iBX] == 1 )
	    break;
      }

      if( iBX >= HCAL_HLX_NUM_BUNCHES ) return;
 
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( HLXMask_[iHLX] ){
	    numActiveTowers_[iHLX][0] =  localSection.occupancy[iHLX].data[set1AboveIndex][iBX] +
	       localSection.occupancy[iHLX].data[set1BetweenIndex][iBX] +
	       localSection.occupancy[iHLX].data[set1BelowIndex][iBX] ;
	
	    numActiveTowers_[iHLX][1] = localSection.occupancy[iHLX].data[set2AboveIndex][iBX] +
	       localSection.occupancy[iHLX].data[set2BetweenIndex][iBX] +
	       localSection.occupancy[iHLX].data[set2BelowIndex][iBX];
	 } else {
	    numActiveTowers_[iHLX][0] = 0;
	    numActiveTowers_[iHLX][1] = 0;
	 }
	 TotalActiveTowers_ += (numActiveTowers_[iHLX][0] + numActiveTowers_[iHLX][1]);
      }
      //std::cout << "Total active towers " << TotalActiveTowers_ << std::endl;

      for( iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 numActiveTowersBX_[iBX][0] = 0;
	 numActiveTowersBX_[iBX][1] = 0;
	 if( BXMask_[iBX] >= 1 ){
	    for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       numActiveTowersBX_[iBX][0] += localSection.occupancy[iHLX].data[set1AboveIndex][iBX] +
		  localSection.occupancy[iHLX].data[set1BetweenIndex][iBX] +
		  localSection.occupancy[iHLX].data[set1BelowIndex][iBX] ;

	       numActiveTowersBX_[iBX][1] += localSection.occupancy[iHLX].data[set2AboveIndex][iBX] +
		  localSection.occupancy[iHLX].data[set2BetweenIndex][iBX] +
		  localSection.occupancy[iHLX].data[set2BelowIndex][iBX] ;
	    } 
	 }
	 else{
	       numActiveTowersBX_[iBX][0] = 0;
	       numActiveTowersBX_[iBX][1] = 0;
	 }
      }
   }

   void LumiCalc::CountActiveTowersEt( HCAL_HLX::LUMI_SECTION &localSection ){
    
      // Renormalize to the number of nibbles in the Et sum hdr
      unsigned int toccNibbles = 0;
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 toccNibbles += localSection.occupancy[iHLX].hdr.numNibbles;
      }
      unsigned int etNibbles = 0;
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 etNibbles += localSection.etSum[iHLX].hdr.numNibbles;
      }


      TotalActiveTowersEt_ = 0;
      float reNorm = (float)etNibbles/(float)toccNibbles;
      float totalBX = float(toccNibbles*numOrbitsPerNB_*numBX_);
      float totalBXEt = float(etNibbles*numOrbitsPerNB_*numBX_);

      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( HLXMask_[iHLX] ){
	    float twrSum = float(numActiveTowers_[iHLX][0] + numActiveTowers_[iHLX][1]);
	    numActiveTowersEt_[iHLX] = (unsigned int)(twrSum*reNorm);
	    // If there are no masks ...
	    if( twrSum/totalBX > 8 ){ numActiveTowersEt_[iHLX] = (unsigned int)(48*totalBXEt); }
	 } else {
	    numActiveTowersEt_[iHLX] = 0;
	 }
	 TotalActiveTowersEt_ += (numActiveTowersEt_[iHLX]);
      }
      //std::cout << "Total active towers " << TotalActiveTowers_ << std::endl;

      float totalHLX = float(toccNibbles*numOrbitsPerNB_*numHLX_);
      float totalHLXEt = float(etNibbles*numOrbitsPerNB_*numHLX_);
      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( BXMask_[iBX] >= 1 ){
	       float twrSum = float(numActiveTowersBX_[iBX][0] + numActiveTowersBX_[iBX][1]);
	       numActiveTowersBXEt_[iBX] = (unsigned int)(twrSum*reNorm);
	       if( twrSum/totalHLX > 8 ){ numActiveTowersBXEt_[iBX] = (unsigned int)(48*totalHLXEt); }
	 }        
	 else{
	       numActiveTowersBXEt_[iBX] = 0;
	 }
      }
   }
  
   void LumiCalc::CalcLHCLumi( HCAL_HLX::LUMI_SECTION &localSection ){
      // masking does not apply to LHC Lumi

      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 localSection.lumiDetail.LHCLumi[iBX] = 0;
	 for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    localSection.lumiDetail.LHCLumi[iBX] += localSection.lhc[iHLX].data[iBX];
	 }
      }
   }

   void LumiCalc::CalcETSumLumi(HCAL_HLX::LUMI_SECTION &localSection){

      float TotalEtSumNoise[NUM_CAP_BANKS];
      float TotalEtSum[HCAL_HLX_NUM_BUNCHES];

      // Fill Normalization and quality
      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 localSection.lumiDetail.ETLumiQlty[iBX]            = 1;
	 localSection.lumiDetail.ETBXNormalization[iBX]     = 1.0;      
      }

      localSection.lumiSummary.InstantETLumiQlty = 1;
      localSection.lumiSummary.ETNormalization = 1.0;
   
      // count nibbles.
      unsigned int TotalNibbles = 0;
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 TotalNibbles += localSection.etSum[iHLX].hdr.numNibbles;
      }

      // Sum over HLXs
      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 TotalEtSum[iBX] = 0.0;
	 for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    TotalEtSum[iBX] += localSection.etSum[iHLX].data[iBX];
	 }
      }

      float ETSumError = -995;
      if( TotalNibbles > 0 ) ETSumError = CalcETSumError( localSection ); 

      // Calculate noise per cap bank
      for( unsigned int iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ){
	 TotalEtSumNoise[iCap] = 0.0;
	 if( sizeNoise_[iCap] == 0 ) continue;
	 for( unsigned int iBX = iCap; iBX < HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
	    if( BXMask_[iBX] == 2 )
	       TotalEtSumNoise[iCap] += TotalEtSum[iBX];
	 }
	 TotalEtSumNoise[iCap] /= (float)sizeNoise_[iCap];
      }
      float ETSumNoiseError = -995;
      if( TotalNibbles > 0 ) ETSumNoiseError = CalcETSumNoiseError( localSection );

      //std::cout << "Active towers " << TotalActiveTowers_ << std::endl;
      //std::cout << "Active towers Et " << TotalActiveTowersEt_ << std::endl;
      if( TotalActiveTowersEt_ > 0 ){
	 for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    localSection.lumiDetail.ETLumi[iBX]
	       = (TotalEtSum[iBX] - TotalEtSumNoise[iBX % 4])/(float)TotalActiveTowersEt_;
	    localSection.lumiDetail.ETLumiErr[iBX] = sqrt(ETSumError*ETSumError + ETSumNoiseError*ETSumNoiseError);

	    // Check for over flow
	    if( localSection.lumiDetail.ETLumi[iBX] > 999 )
	       localSection.lumiDetail.ETLumi[iBX] = 999;
	 
	    if( localSection.lumiDetail.ETLumi[iBX] < -999 )
	       localSection.lumiDetail.ETLumi[iBX] = -999;

	    if( isinf(localSection.lumiDetail.ETLumi[iBX]) )
	       localSection.lumiDetail.ETLumi[iBX] = 998;

	    if( isnan(localSection.lumiDetail.ETLumi[iBX]) )
	       localSection.lumiDetail.ETLumi[iBX] = 0;
	 
	    if( localSection.lumiDetail.ETLumiErr[iBX] > 999 )
	       localSection.lumiDetail.ETLumiErr[iBX] = 999;
	 
	    if( localSection.lumiDetail.ETLumiErr[iBX] < -999 )
	       localSection.lumiDetail.ETLumiErr[iBX] = -999;
	 }
      }

      if( TotalNibbles > 0 ){
	 if( numBX_ ){ // TODO: check for TotalNibbles = 0. This will be a big problem.
	    localSection.lumiSummary.InstantETLumi = 0.0;
	    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( BXMask_[iBX] == 1 ) localSection.lumiSummary.InstantETLumi += localSection.lumiDetail.ETLumi[iBX];
	    }
	    localSection.lumiSummary.InstantETLumi /= (float)numBX_;
	    localSection.lumiSummary.InstantETLumiErr = sqrt( ETSumError*ETSumError/(float)numBX_ + ETSumNoiseError*ETSumNoiseError );
	 } else {
	    localSection.lumiSummary.InstantETLumi = -994;  // error
	    localSection.lumiSummary.InstantETLumiErr = -994; // error
	 }
      } else {
	 localSection.lumiSummary.InstantETLumi = -995; // error
	 localSection.lumiSummary.InstantETLumiErr = -995; // error
      }
   }

   // Calculate the per BX Et sum error
   float LumiCalc::CalcETSumError( HCAL_HLX::LUMI_SECTION &localSection ){

      // Check for the case where we have no data ...
      if( numBX_ == 0 || numHLX_ == 0 ) return -995;

      // General case 
     
      // 1. Calculate the mean for each BX in this event (and the total mean ...)
      float mean = 0;
      // If the store is maxed out, move everything back by 1 and zero the
      // final entry
      if( etStoreSize_ <= ET_SUM_STORE_SIZE ) ++etStoreSize_;
      if( etStoreSize_ > ET_SUM_STORE_SIZE ){
	 --etStoreSize_;
	 std::cout << "Here in pop back errors! " << etStoreSize_ << std::endl;
	 for( unsigned int iLB = 0; iLB < ET_SUM_STORE_SIZE; ++iLB ){
	    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( iLB < (ET_SUM_STORE_SIZE-1) ) etSumAvgPerLB_[iLB][iBX] = etSumAvgPerLB_[iLB+1][iBX];
	       else                              etSumAvgPerLB_[iLB][iBX] = 0.0;
	    }
	 }
      }
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( !HLXMask_[iHLX] ) continue;
	 float nTower = (float)(numActiveTowersEt_[iHLX]);
	 if( nTower == 0 ) continue;
	 for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] != 1 ) continue;
	    mean += localSection.etSum[iHLX].data[iBX]/nTower;
	    float nTowerBX = (float)(numActiveTowersBXEt_[iBX]);
	    if( nTowerBX == 0 ) continue;
	    etSumAvgPerLB_[etStoreSize_-1][iBX] += localSection.etSum[iHLX].data[iBX]/nTowerBX;
	 }
      }
      mean = mean/(float)HCAL_HLX_NUM_HLXS;

      // 2. Calculate the final error ... for the special case of the first
      // LB take this from the HLX spread, if there are less BX's than HLX's.
      float deviation = 0.0;
      float sigma = 0.0;
      if( (numBX_ < numHLX_) && etStoreSize_ <= 1 ){
	 for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( !HLXMask_[iHLX] ) continue; 
	    float totalBX = 0.0;
	    float nTower = (float)(numActiveTowersEt_[iHLX]);
	    if( nTower == 0 ) continue;
	    for( int iBX=0; iBX<HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( BXMask_[iBX] != 1 ) continue;
	       totalBX += localSection.etSum[iHLX].data[iBX]/nTower;
	    }
	    totalBX /= numBX_;
	    float diff = totalBX - mean;
	    deviation += diff*diff;
	 }    
	 sigma = sqrt(deviation)/(float)numHLX_;
      }
      // Otherwise use the stored values from previous LB's to get a spread.
      else{
	 float totalMean = 0.0;
	 float norm = 0;
	 for( unsigned int iLB = 0; iLB < etStoreSize_; ++iLB ){
	    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( BXMask_[iBX] != 1 ) continue;
	       totalMean += etSumAvgPerLB_[iLB][iBX];
	       ++norm; 
	    }
	 }
	 totalMean /= norm;
	 //std::cout << "Total mean " << totalMean << " norm " << norm << std::endl;

	 for( unsigned int iLB = 0; iLB < etStoreSize_; ++iLB ){
	    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( BXMask_[iBX] != 1 ) continue;
	       float diff = totalMean - etSumAvgPerLB_[iLB][iBX];
	       deviation += diff*diff;
	    }
	 }
	 sigma = sqrt(deviation/norm);

	 if( sigmaEtWeight_ > ET_SUM_STORE_SIZE ){
	    float weight = sigmaEtWeight_/(sigmaEtWeight_+(float)ET_SUM_STORE_SIZE); 
	    sigma = (weight*sigmaEt_ + (1-weight)*sigma);
	 }
      }
      //std::cout << "Et sum " << mean << " +/- " << sigma << std::endl;

      sigmaEt_ = sigma;
      ++sigmaEtWeight_;

      return sigma; // error
   } 

   // Calculate the total noise error (for all noise BX's in each cap).
   float LumiCalc::CalcETSumNoiseError( HCAL_HLX::LUMI_SECTION &localSection ){

      // Check for the case where we have no data ...
      if( numBX_ == 0 || numHLX_ == 0 ) return -995;

      // General case 
     
      // 1. Calculate the mean for each BX in this event (and the total mean ...)
      float mean[4] = {0,0,0,0};
      // If the noise store is maxed out, move everything back by 1 and zero the
      // final entry
      if( etNoiseStoreSize_ <= ET_SUM_STORE_SIZE ) ++etNoiseStoreSize_;
      if( etNoiseStoreSize_ > ET_SUM_STORE_SIZE ){
	 --etNoiseStoreSize_;
	 for( unsigned int iLB = 0; iLB < ET_SUM_STORE_SIZE; ++iLB ){
	    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( iLB < (ET_SUM_STORE_SIZE-1) ) etSumNoiseAvgPerLB_[iLB][iBX] = etSumNoiseAvgPerLB_[iLB+1][iBX];
	       else                              etSumNoiseAvgPerLB_[iLB][iBX] = 0.0;
	    }
	 }
      }

      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( !HLXMask_[iHLX] ) continue;
	 float nTower = (float)(numActiveTowersEt_[iHLX]);
	 if( nTower == 0 ) continue;
	 for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] != 2 ) continue;
	    if( sizeNoise_[iBX % NUM_CAP_BANKS] == 0 ) continue;
	    mean[iBX % NUM_CAP_BANKS] += localSection.etSum[iHLX].data[iBX]/(nTower*(float)sizeNoise_[iBX % NUM_CAP_BANKS]);
	    float nTowerBX = (float)(numActiveTowersBXEt_[iBX]);
	    if( nTowerBX == 0 ) continue;
	    etSumNoiseAvgPerLB_[etNoiseStoreSize_-1][iBX] += localSection.etSum[iHLX].data[iBX]/nTowerBX;
	 }
      }
      for( int iCap=0; iCap<NUM_CAP_BANKS; ++iCap ){
	 mean[iCap] = mean[iCap]/(float)numHLX_;
      }


      // 2. Calculate the final error ... for the special case of the first
      // LB take this from the HLX spread, if there are less BX's than HLX's.
      float sigma = 0.0;
      for( unsigned int iCap=0; iCap<NUM_CAP_BANKS; ++iCap ){
	 if( sizeNoise_[iCap] == 0 ) continue;
	 if( (sizeNoise_[iCap] < numHLX_) && etNoiseStoreSize_ <= 1 ){
	    float deviation = 0.0;
	    for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       if( !HLXMask_[iHLX] ) continue; 
	       float totalBX = 0.0;
	       float nTower = (float)(numActiveTowersEt_[iHLX]);
	       if( nTower == 0 ) continue;
	       for( unsigned int iBX=0; iBX<HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX] != 2 ) continue;
		  if( (iBX % NUM_CAP_BANKS) != iCap ) continue;
		  totalBX += localSection.etSum[iHLX].data[iBX]/(nTower*(float)sizeNoise_[iCap]);
	       }
	       float diff = totalBX - mean[iCap];
	       deviation += diff*diff;
	    }    
	    sigma += (sqrt(deviation)/(float)numHLX_)/sqrt((float)sizeNoise_[iCap]);
	 }
	 // Otherwise use the stored values from previous LB's to get a spread.
	 else{
	    float totalMean = 0.0;
	    float norm = 0;
	    for( unsigned int iLB = 0; iLB < etNoiseStoreSize_; ++iLB ){
	       for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX] != 2 ) continue;
		  if( (iBX % NUM_CAP_BANKS) != iCap ) continue;
		  totalMean += etSumNoiseAvgPerLB_[iLB][iBX];
		  ++norm; 
	       }
	    }
	    if( norm > 0 ) totalMean /= norm;
	    //std::cout << "Total mean " << totalMean << " norm " << norm << std::endl;

	    float deviation = 0.0;
	    for( unsigned int iLB = 0; iLB < etNoiseStoreSize_; ++iLB ){
	       for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX] != 2 ) continue;
		  if( (iBX % NUM_CAP_BANKS) != iCap ) continue;
		  float diff = totalMean - etSumNoiseAvgPerLB_[iLB][iBX];
		  deviation += diff*diff;
	       }
	    }
	    sigma += sqrt(deviation/norm)/sqrt((float)sizeNoise_[iCap]);  
	 }
      }
      sigma /= (float)NUM_CAP_BANKS;

      if( sigmaEtNoiseWeight_ > ET_SUM_STORE_SIZE ){
	 float weight = sigmaEtNoiseWeight_/(sigmaEtNoiseWeight_+(float)ET_SUM_STORE_SIZE); 
	 sigma = (weight*sigmaEtNoise_ + (1-weight)*sigma);
      }
      //std::cout << "Et sum " << mean[0] << " +/- " << sigma << std::endl;

      sigmaEtNoise_ = sigma;
      ++sigmaEtNoiseWeight_;

      return sigma; // error
   } 


   // Calculate the occupancy
   void LumiCalc::CalcOccLumi( HCAL_HLX::LUMI_SECTION &localSection ){

      float occNoise[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
    
      // Calculate noise per cap bank
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 for( unsigned int iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ){
	    if( sizeNoise_[iCap] == 0 ) continue;
	    occNoise[iHLX][0][iCap] = 0.0;
	    occNoise[iHLX][1][iCap] = 0.0;
	    for(unsigned int iBX = iCap ; iBX < HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
	       if( BXMask_[iBX] != 2 ) continue;
	       occNoise[iHLX][0][iCap] += localSection.occupancy[iHLX].data[set1BelowIndex][iBX];
	       occNoise[iHLX][1][iCap] += localSection.occupancy[iHLX].data[set2BelowIndex][iBX];
	    }
	    // Average num towers below threshold per tower per BX
	    occNoise[iHLX][0][iCap] /= (float)sizeNoise_[iCap];
	    occNoise[iHLX][1][iCap] /= (float)sizeNoise_[iCap];
	 }
      }

      localSection.lumiSummary.InstantOccLumi[0] = 0.0;
      localSection.lumiSummary.InstantOccLumi[1] = 0.0;
      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 localSection.lumiDetail.OccLumi[0][iBX] = 0.0;
	 localSection.lumiDetail.OccLumi[1][iBX] = 0.0;
	 if( BXMask_[iBX] != 1 ) continue;

	 for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( !HLXMask_[iHLX] ) continue;

	    // Ring Set 1
	    if( localSection.occupancy[iHLX].data[set1BelowIndex][iBX] != 0 ){ // empty towers is non-zero
	       if( occNoise[iHLX][0][iBX % NUM_CAP_BANKS] > 0.0 ){ // for non-zero noise ... 
		  localSection.lumiDetail.OccLumi[0][iBX]
		     += log( occNoise[iHLX][0][iBX % NUM_CAP_BANKS] / 
		     (float)localSection.occupancy[iHLX].data[set1BelowIndex][iBX]);
	       } else { // for zero noise ...
		  localSection.lumiDetail.OccLumi[0][iBX]
		     += log( numActiveTowers_[iHLX][0] /
		     (float)localSection.occupancy[iHLX].data[set1BelowIndex][iBX]);
	       }
	    } else {
	       // What should we do when there are no empty towers?  
	       //Should we not use that HLX? No empty towers means infinite lumi?
	       localSection.lumiDetail.OccLumi[0][iBX]  = 999;
	    }
	
	    // Ring Set 2
	    if( localSection.occupancy[iHLX].data[set2BelowIndex][iBX] != 0 ){ // empty towers is non-zero
	       if( occNoise[iHLX][1][iBX % NUM_CAP_BANKS] > 0.0 ){ // for non-zero noise ... 
		  localSection.lumiDetail.OccLumi[1][iBX]
		     += log( occNoise[iHLX][1][iBX % NUM_CAP_BANKS] / 
		     (float)localSection.occupancy[iHLX].data[set2BelowIndex][iBX]);
	       } else { // for zero noise ...
		  localSection.lumiDetail.OccLumi[1][iBX]
		     += log( numActiveTowers_[iHLX][1] /
		     (float)localSection.occupancy[iHLX].data[set2BelowIndex][iBX]);
	       }
	    } else {
	       // What should we do when there are no empty towers?  
	       //Should we not use that HLX? No empty towers means infinite lumi?
	       localSection.lumiDetail.OccLumi[1][iBX] = 999;
	    }
	    // TO DO: Take into account different numbers of active towers in HLX.
	    // Check for dropped nibbles.
	 }
	 // normailze to number of active HLXs
	 if( numHLX_ > 0 ){
	    localSection.lumiDetail.OccLumi[0][iBX] /= (float)numHLX_;
	    localSection.lumiDetail.OccLumi[1][iBX] /= (float)numHLX_;
	 }

	 localSection.lumiDetail.OccLumiErr[0][iBX] = CalcOccErrorBX( localSection, 0 );
	 localSection.lumiDetail.OccLumiErr[1][iBX] = CalcOccErrorBX( localSection, 1 );


	 // Check for infinity
	 if( isinf(localSection.lumiDetail.OccLumi[0][iBX]) )
	    localSection.lumiDetail.OccLumi[0][iBX] = 998;

	 if( isinf(localSection.lumiDetail.OccLumi[1][iBX]) )
	    localSection.lumiDetail.OccLumi[1][iBX] = 998;

	 // Check for NaN
	 if( isnan(localSection.lumiDetail.OccLumi[0][iBX]) )
	    localSection.lumiDetail.OccLumi[0][iBX] = 0;

	 if( isnan(localSection.lumiDetail.OccLumi[1][iBX]) )
	    localSection.lumiDetail.OccLumi[1][iBX] = 0;

	 // Check upper bound
	 if( localSection.lumiDetail.OccLumi[0][iBX] > 999 )
	    localSection.lumiDetail.OccLumi[0][iBX] = 999;

	 if( localSection.lumiDetail.OccLumi[1][iBX] > 999 )
	    localSection.lumiDetail.OccLumi[1][iBX] = 999;

	 // Check lower bound
	 if( localSection.lumiDetail.OccLumi[0][iBX] < -999 )
	    localSection.lumiDetail.OccLumi[0][iBX] = -999;

	 if( localSection.lumiDetail.OccLumi[1][iBX] < -999 )
	    localSection.lumiDetail.OccLumi[1][iBX] = -999;

	 // Errors
	 // Check for infinity
	 if( isinf(localSection.lumiDetail.OccLumiErr[0][iBX]) )
	    localSection.lumiDetail.OccLumiErr[0][iBX] = 998;

	 if( isinf(localSection.lumiDetail.OccLumiErr[1][iBX]) )
	    localSection.lumiDetail.OccLumiErr[1][iBX] = 998;

	 // Check for NaN
	 if( isnan(localSection.lumiDetail.OccLumiErr[0][iBX]) )
	    localSection.lumiDetail.OccLumiErr[0][iBX] = 0;

	 if( isnan(localSection.lumiDetail.OccLumiErr[1][iBX]) )
	    localSection.lumiDetail.OccLumiErr[1][iBX] = 0;

	 // Check upper bound
	 if( localSection.lumiDetail.OccLumiErr[0][iBX] > 999 )
	    localSection.lumiDetail.OccLumiErr[0][iBX] = 999;

	 if( localSection.lumiDetail.OccLumiErr[1][iBX] > 999 )
	    localSection.lumiDetail.OccLumiErr[1][iBX] = 999;

	 // Check lower bound
	 if( localSection.lumiDetail.OccLumiErr[0][iBX] < -999 )
	    localSection.lumiDetail.OccLumiErr[0][iBX] = -999;

	 if( localSection.lumiDetail.OccLumiErr[1][iBX] < -999 )
	    localSection.lumiDetail.OccLumiErr[1][iBX] = -999;
      
	 // Sum over bunch crossings
	 if( BXMask_[iBX] == 1 ){
	    localSection.lumiSummary.InstantOccLumi[0] += localSection.lumiDetail.OccLumi[0][iBX];
	    localSection.lumiSummary.InstantOccLumi[1] += localSection.lumiDetail.OccLumi[1][iBX];
	 }

      }
      // normailze to the number of active bunches
      if( numBX_ > 0 ){
	 localSection.lumiSummary.InstantOccLumi[0] /= (float)numBX_;
	 localSection.lumiSummary.InstantOccLumi[1] /= (float)numBX_;
      }

      localSection.lumiSummary.InstantOccLumiErr[0] = CalcOccErrorTotal( localSection, 0 );
      localSection.lumiSummary.InstantOccLumiErr[1] = CalcOccErrorTotal( localSection, 1 );

      // Check for infinity
      if( isinf( localSection.lumiSummary.InstantOccLumi[0] ) ) localSection.lumiSummary.InstantOccLumi[0] = -996;
      if( isinf( localSection.lumiSummary.InstantOccLumi[1] ) ) localSection.lumiSummary.InstantOccLumi[1] = -996;

      // Check for NaN
      if( isnan( localSection.lumiSummary.InstantOccLumi[0] ) ) localSection.lumiSummary.InstantOccLumi[0] = -997;
      if( isnan( localSection.lumiSummary.InstantOccLumi[1] ) ) localSection.lumiSummary.InstantOccLumi[1] = -997;
    
      // Check upper bound
      if( localSection.lumiSummary.InstantOccLumi[0] > 999 )
	 localSection.lumiSummary.InstantOccLumi[0] = 999;

      if( localSection.lumiSummary.InstantOccLumi[1] > 999 )
	 localSection.lumiSummary.InstantOccLumi[1] = 999;

      // Check lower bound
      if( localSection.lumiSummary.InstantOccLumi[0] < -999 )
	 localSection.lumiSummary.InstantOccLumi[0] = -999;

      if( localSection.lumiSummary.InstantOccLumi[1] < -999 )
	 localSection.lumiSummary.InstantOccLumi[1] = -999;

      // Check for infinity
      if( isinf( localSection.lumiSummary.InstantOccLumiErr[0] ) ) localSection.lumiSummary.InstantOccLumiErr[0] = -996;
      if( isinf( localSection.lumiSummary.InstantOccLumiErr[1] ) ) localSection.lumiSummary.InstantOccLumiErr[1] = -996;

      // Check for NaN
      if( isnan( localSection.lumiSummary.InstantOccLumiErr[0] ) ) localSection.lumiSummary.InstantOccLumiErr[0] = -997;
      if( isnan( localSection.lumiSummary.InstantOccLumiErr[1] ) ) localSection.lumiSummary.InstantOccLumiErr[1] = -997;
    
      // Check upper bound
      if( localSection.lumiSummary.InstantOccLumiErr[0] > 999 )
	 localSection.lumiSummary.InstantOccLumiErr[0] = 999;

      if( localSection.lumiSummary.InstantOccLumiErr[1] > 999 )
	 localSection.lumiSummary.InstantOccLumiErr[1] = 999;

      // Check lower bound
      if( localSection.lumiSummary.InstantOccLumiErr[0] < -999 )
	 localSection.lumiSummary.InstantOccLumiErr[0] = -999;

      if( localSection.lumiSummary.InstantOccLumiErr[1] < -999 )
	 localSection.lumiSummary.InstantOccLumiErr[1] = -999;

      // Quality etc ...
      localSection.lumiSummary.InstantOccLumiQlty[0] = 1;
      localSection.lumiSummary.InstantOccLumiQlty[1] = 1;
    
      localSection.lumiSummary.OccNormalization[0] = 1;
      localSection.lumiSummary.OccNormalization[1] = 1;

      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 localSection.lumiDetail.OccLumiQlty[0][iBX] = 1;
	 localSection.lumiDetail.OccLumiQlty[1][iBX] = 1;
	 localSection.lumiDetail.OccBXNormalization[0][iBX] = 1;
	 localSection.lumiDetail.OccBXNormalization[1][iBX] = 1;
      }

   }
  
   float LumiCalc::CalcOccErrorTotal( HCAL_HLX::LUMI_SECTION &localSection, unsigned int set ){
    
      // Check for the case where we have no data ...
      if( numBX_ ==0 || numHLX_ ==0 ) return -995;

      // Index
      int index = set1BelowIndex;
      if( set == 1 ) index = set2BelowIndex;
      
      // For occupancy we can just calculate the accumulated binomial error
      double nBelow = 0.0;
      double nTotal = 0.0;
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( !HLXMask_[iHLX] ) continue;
	 for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] != 1 ) continue;
	    nTotal += (double)(numActiveTowers_[iHLX][set]);
	    nBelow += localSection.occupancy[iHLX].data[index][iBX];
	 }
      }

      double sigma = 0.0;
      if( nTotal > 0 && nBelow > 0 && (nBelow<nTotal) ){
	 sigma += (1/nBelow)*(1-nBelow/nTotal);
      }
      std::cout << "Occ sigma total " << sigma << std::endl;

      // But we also need the noise error
      double nBelowN[NUM_CAP_BANKS] = {0.0,0.0,0.0,0.0};
      double nTotalN = 0.0;
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( !HLXMask_[iHLX] ) continue;
	 nTotalN += (double)(numActiveTowers_[iHLX][set]);
	 for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] != 2 ) continue;
	    nBelowN[iBX % NUM_CAP_BANKS] += localSection.occupancy[iHLX].data[index][iBX];
	 }
      }

      double avg = 0.0;
      if( nTotalN > 0 ){
	 for( int iCap=0; iCap < NUM_CAP_BANKS; ++iCap ){
	    if( sizeNoise_[iCap] == 0 ) continue;
	    if( nBelowN[iCap] <= 0 ) continue;
	    if( nBelowN[iCap] >= (nTotalN*(float)sizeNoise_[iCap]) ) continue;
	    avg += sqrt((1/nBelowN[iCap])*(1-nBelowN[iCap]/(nTotalN*(float)sizeNoise_[iCap])));
	 }
	 avg /= (double)NUM_CAP_BANKS;
      }
      std::cout << "Occ sigma total noise " << avg << std::endl;

      sigma += avg*avg; 

      return sqrt(sigma); // error
   }

   float LumiCalc::CalcOccErrorBX( HCAL_HLX::LUMI_SECTION &localSection, unsigned int set ){
    
      // Check for the case where we have no data ...
      if( numBX_ ==0 || numHLX_ ==0 ) return -995;

      // Index
      int index = set1BelowIndex;
      if( set == 1 ) index = set2BelowIndex;

      // For occupancy we can just calculate the accumulated binomial error
      float nBelow[HCAL_HLX_NUM_BUNCHES];
      float nTotal[HCAL_HLX_NUM_BUNCHES];
      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ) nBelow[iBX] = 0.0;
      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( BXMask_[iBX] != 1 ) continue;
	 nTotal[iBX] = (float)(numActiveTowersBX_[iBX][set]);
	 for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( !HLXMask_[iHLX] ) continue;
	    nBelow[iBX] += localSection.occupancy[iHLX].data[index][iBX];
	 }
      }

      float sigma = 0.0;
      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){    
	 if( BXMask_[iBX] != 1 ) continue;
	 if( nTotal[iBX] > 0 && nBelow[iBX] > 0 && (nBelow[iBX]<nTotal[iBX]) ){
	    sigma += sqrt((1/nBelow[iBX])*(1-nBelow[iBX]/nTotal[iBX]));
	 }
      }
      sigma /= (float)numBX_;

      // But we also need the noise error
      double nBelowN[NUM_CAP_BANKS] = {0.0,0.0,0.0,0.0};
      double nTotalN = 0.0;
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( !HLXMask_[iHLX] ) continue;
	 nTotalN += (double)(numActiveTowers_[iHLX][set]);
	 for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] != 2 ) continue;
	    nBelowN[iBX % NUM_CAP_BANKS] += localSection.occupancy[iHLX].data[index][iBX];
	 }
      }

      double avg = 0.0;
      if( nTotalN > 0 ){
	 for( int iCap=0; iCap < NUM_CAP_BANKS; ++iCap ){
	    if( sizeNoise_[iCap] == 0 ) continue;
	    if( nBelowN[iCap] <= 0 ) continue;
	    if( nBelowN[iCap] >= (nTotalN*(float)sizeNoise_[iCap]) ) continue;
	    avg += sqrt((1/nBelowN[iCap])*(1-nBelowN[iCap]/(nTotalN*(float)sizeNoise_[iCap])));
	 }
	 avg /= (double)NUM_CAP_BANKS;
      }

      sigma = sigma*sigma + avg*avg; 

      return sqrt(sigma); // error
   }


   void LumiCalc::CalcLumi( HCAL_HLX::LUMI_SECTION &localSection ){

      localSection.lumiSummary.InstantLumi = localSection.lumiSummary.InstantETLumi;

      localSection.lumiSummary.InstantLumiErr = localSection.lumiSummary.InstantETLumiErr;

      localSection.lumiSummary.InstantLumiQlty = 1;
   }

   float LumiCalc::CalcLumiError( unsigned int numNibbles, unsigned int numTowers, unsigned int numBunches, float intPerBX )
   {
    
      // Write method for total error
      return 1;
   }




   void LumiCalc::DoCalc(HCAL_HLX::LUMI_SECTION &localSection){

      // Should be called once per run
      CountActiveTowers( localSection );
      CountActiveTowersEt( localSection );

      localSection.lumiSummary.DeadtimeNormalization = 1.0;
      localSection.lumiSummary.LHCNormalization = 1.0;

      // once per lumi section
      CalcLHCLumi( localSection );
      CalcETSumLumi( localSection );
      CalcOccLumi( localSection );
      CalcLumi( localSection );
   }

// **************************** end Private member functions **********************************

  
   bool LumiCalc::Block(unsigned short int BX, unsigned short int numBlock){
      const u8 gap = 0;
      const u8 bunch = 1;
    
      if(BX > (u32)numBlock*80){
	 return gap;
      }else if(BX%80 > 72){
	 return gap;
      }else {
	 return bunch;
      }
   }
  
   bool LumiCalc::isBunch(unsigned short int BX, unsigned short int scheme ){
      // determine where you are in the bunch structure
      const u8 abortGap = 0;
      u16 BXrel; // relative bunch crossing
    
      if(scheme == 1){
	 BXrel = BX % HCAL_HLX_NUM_BUNCHES;
      
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
}
