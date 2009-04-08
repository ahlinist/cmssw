/****************************************************************************

Author:  Adam Hunt
         ahunt@princeton.edu

Summary:  Library to calculate relative luminosity from HLX data

*****************************************************************************/

#include "RecoLuminosity/TCPReceiver/interface/LumiStructures.hh"
#include "RecoLuminosity/LumiCalc/interface/LumiCalc.hh"

/********************************************************************
Quality Codes (Additive):

Infinite lumi = -500
Infinite lumi error = -200
NaN lumi = -510
NaN lumi error = -210
Out of bounds high lumi = -520
Out of bounds high lumi error = -220
Out of bounds low lumi = -530
Out of bounds low lumi error = -230
Zero lumi = -600;
Zero lumi error = -300;
No towers less than threshold = -701
No noise subtraction for occ = 99

Zero Nibbles = -50
Zero Active Towers = -200
Zero Good BX = -700

*******************************************************************/

namespace HCAL_HLX {
  
   LumiCalc::LumiCalc(){
    
      uint32_t BXMask[HCAL_HLX_NUM_BUNCHES];
      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ) BXMask[iBX] = 0;
      uint32_t HLXMask[HCAL_HLX_NUM_HLXS];
      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ) HLXMask[iHLX] = 1;
      // Should these be combined to form a matrix so that different BXs are masked for different HLXs?

      //std::cout << "In ctor set arrays" << std::endl;

      // Masking convension: Masked 0, Data 1, Noise 2.  Anything marked as 2 will be used to calculate noise.
      for( uint32_t iBX = 0;  iBX < 120; ++iBX )                    BXMask[iBX] = 0; // Mask
      for( uint32_t iBX = 120;  iBX < 3400; ++iBX )                 BXMask[iBX] = 1; // UnMask
      for( uint32_t iBX = 3400; iBX < 3524; ++iBX )                 BXMask[iBX] = 2; // Noise
      for( uint32_t iBX = 3524; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ) BXMask[iBX] = 0; // Mask

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
      for( uint32_t iLB = 0; iLB < ET_SUM_STORE_SIZE; ++iLB ){
	 for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
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
    
   uint32_t LumiCalc::SetBXMask( uint32_t BXMask[], uint32_t maskSize ){
      // Copy given mask into member variable and count number of active bunches

      numBX_ = 0;
      for( int iCap =0; iCap < NUM_CAP_BANKS; ++iCap ) sizeNoise_[iCap] = 0;
    
      for( uint32_t iBX = 0; iBX < maskSize ; ++iBX ){
	 if( BXMask[iBX] < 3 ) {
	    BXMask_[iBX] = BXMask[iBX];
	 } else {
	    BXMask_[iBX] = 1;
	 }
	 BXMaskBool_[iBX] = 0;
	 if( BXMask_[iBX] == 1 ){ 
	    ++numBX_;
	    BXMaskBool_[iBX] = 1;
	 }
	 if( BXMask_[iBX] == 2 ){ 
	    ++sizeNoise_[iBX % 4];
	    BXMaskBool_[iBX] = 1;
	 }

	 BXCapId_[iBX] = iBX % NUM_CAP_BANKS;
      }

      for( uint32_t iBX = maskSize; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 BXMask_[iBX] = 0;
	 BXMaskBool_[iBX] = 0;
	 BXCapId_[iBX] = iBX % NUM_CAP_BANKS;
      }
    
      for( int iCap =0; iCap < NUM_CAP_BANKS; ++iCap ) invSizeNoise_[iCap] = 1.0/(float)sizeNoise_[iCap];
      normNumBX_ = 1.0;
      if( numBX_ > 0 ) normNumBX_ = 1.0/(float)numBX_;

      return numBX_;
   }

   uint32_t LumiCalc::SetHLXMask( uint32_t HLXMask[], uint32_t maskSize ){
      // Copy given mask into member variable and count number of active HLXs

      numHLX_ = 0;

      for( uint32_t iHLX = 0; iHLX < maskSize ; ++iHLX ){
	 HLXMask_[iHLX] = ( HLXMask[iHLX] == 0 )? 0:1;
	 numHLX_ += HLXMask_[iHLX];
      }      

      for( uint32_t iHLX = maskSize; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX )
	 HLXMask_[iHLX] = 0;

      normNumHLX_ = 1.0;
      if( numHLX_ > 0 ) normNumHLX_ = 1.0/(float)numHLX_;
   
      return numHLX_;
   }

   //****************** end Initialization and Configuration Functions ***********

   void LumiCalc::CountActiveTowers( HCAL_HLX::LUMI_SECTION &localSection ){
      // Should only be calculated once per run.
    
      TotalActiveTowers_ = 0;

      //find first unmasked BX
      uint32_t iBX;
      for( iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( BXMask_[iBX] == 1 ) break;
      }

      if( iBX >= HCAL_HLX_NUM_BUNCHES ) return;
 
      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 numActiveTowers_[iHLX][0] =  ( localSection.occupancy[iHLX].data[set1AboveIndex][iBX] +
	 localSection.occupancy[iHLX].data[set1BetweenIndex][iBX] +
	 localSection.occupancy[iHLX].data[set1BelowIndex][iBX] )*HLXMask_[iHLX] ;

	 //std::cout << iHLX << " Num Total " << numActiveTowers_[iHLX][0] << std::endl;
	
	 numActiveTowers_[iHLX][1] = ( localSection.occupancy[iHLX].data[set2AboveIndex][iBX] +
	 localSection.occupancy[iHLX].data[set2BetweenIndex][iBX] +
	 localSection.occupancy[iHLX].data[set2BelowIndex][iBX] )*HLXMask_[iHLX];

	 TotalActiveTowers_ += (numActiveTowers_[iHLX][0] + numActiveTowers_[iHLX][1]);
      }
      //std::cout << "Total active towers " << TotalActiveTowers_ << std::endl;

      numActiveTowersBX_[0] = 0;
      numActiveTowersBX_[1] = 0;

      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( HLXMask_[iHLX] == 0 ) continue;

	 numActiveTowersBX_[0] += ( localSection.occupancy[iHLX].data[set1AboveIndex][iBX] +
	 localSection.occupancy[iHLX].data[set1BetweenIndex][iBX] +
	 localSection.occupancy[iHLX].data[set1BelowIndex][iBX] );
	
	 numActiveTowersBX_[1] += ( localSection.occupancy[iHLX].data[set2AboveIndex][iBX] +
	 localSection.occupancy[iHLX].data[set2BetweenIndex][iBX] +
	 localSection.occupancy[iHLX].data[set2BelowIndex][iBX] );
      } 
   }

   void LumiCalc::CountActiveTowersEt( HCAL_HLX::LUMI_SECTION &localSection ){
    
      // Renormalize to the number of nibbles in the Et sum hdr
      uint32_t toccNibbles = 0;
      uint32_t etNibbles = 0;
      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 toccNibbles += localSection.occupancy[iHLX].hdr.numNibbles;
	 etNibbles += localSection.etSum[iHLX].hdr.numNibbles;
      }

      TotalActiveTowersEt_ = 0;
      float reNorm = (float)etNibbles/(float)toccNibbles;
      float totalBX = float(toccNibbles*numOrbitsPerNB_*numBX_);
      float totalBXEt = float(etNibbles*numOrbitsPerNB_*numBX_);

      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( HLXMask_[iHLX] ){
	    float twrSum = float(numActiveTowers_[iHLX][0] + numActiveTowers_[iHLX][1]);
	    numActiveTowersEt_[iHLX] = (uint32_t)(twrSum*reNorm);
	    // If there are no masks ...
	    if( twrSum > 8*totalBX ){ numActiveTowersEt_[iHLX] = (uint32_t)(48*totalBXEt); }
	 } else {
	    numActiveTowersEt_[iHLX] = 0;
	 }
	 TotalActiveTowersEt_ += (numActiveTowersEt_[iHLX]);
      }
      //std::cout << "Total active towers " << TotalActiveTowers_ << std::endl;

      float totalHLX = float(toccNibbles*numOrbitsPerNB_*numHLX_);
      float totalHLXEt = float(etNibbles*numOrbitsPerNB_*numHLX_);
      float twrSum = float(numActiveTowersBX_[0] + numActiveTowersBX_[1]);
      numActiveTowersBXEt_ = (uint32_t)(twrSum*reNorm);
      if( twrSum > 8*totalHLX ){ numActiveTowersBXEt_ = (uint32_t)(48*totalHLXEt); }
   }
  
   void LumiCalc::CalcLHCLumi( HCAL_HLX::LUMI_SECTION &localSection ){
      // masking does not apply to LHC Lumi

      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 localSection.lumiDetail.LHCLumi[iBX] = 0;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    localSection.lumiDetail.LHCLumi[iBX] += localSection.lhc[iHLX].data[iBX];
	 }
      }
   }

   void LumiCalc::CalcETSumLumi(HCAL_HLX::LUMI_SECTION &localSection){

      localSection.lumiSummary.InstantETLumi = 0.0;
      localSection.lumiSummary.InstantETLumiQlty = 1;
      localSection.lumiSummary.ETNormalization = 1.0;

      // count nibbles.
      uint32_t TotalNibbles = 0;
      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 TotalNibbles += localSection.etSum[iHLX].hdr.numNibbles;
      }
      if( TotalNibbles == 0 ) localSection.lumiSummary.InstantETLumiQlty -= 51; 

      // Sum over HLXs && Fill Normalization and quality
      float TotalEtSum[HCAL_HLX_NUM_BUNCHES];
      float TotalEtSumNoise[NUM_CAP_BANKS]; 

      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 localSection.lumiDetail.ETLumi[iBX]                = 0.0;
	 localSection.lumiDetail.ETLumiQlty[iBX]            = 1;
	 localSection.lumiDetail.ETBXNormalization[iBX]     = 1.0;      
	 TotalEtSum[iBX] = 0.0;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    TotalEtSum[iBX] += localSection.etSum[iHLX].data[iBX];
	 }
      }

      float ETSumError = 0;
      if( TotalNibbles > 0 ) ETSumError = CalcETSumError( localSection ); 

      // Calculate noise per cap bank
      for( uint32_t iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ){
	 TotalEtSumNoise[iCap] = 0.0;
	 if( sizeNoise_[iCap] == 0 ) continue;
	 for( uint32_t iBX = iCap; iBX < HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
	    if( BXMask_[iBX] == 2 )
	       TotalEtSumNoise[iCap] += TotalEtSum[iBX];
	 }
	 TotalEtSumNoise[iCap] *= invSizeNoise_[iCap];
      }
      float ETSumNoiseError = 0;
      if( TotalNibbles > 0 ) ETSumNoiseError = CalcETSumNoiseError( localSection );

      uint32_t numGoodBX = numBX_;

      //std::cout << "Active towers " << TotalActiveTowers_ << std::endl;
      //std::cout << "Active towers Et " << TotalActiveTowersEt_ << std::endl;
      if( TotalActiveTowersEt_ > 0 ){
	 for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] != 1 ) continue;

	    localSection.lumiDetail.ETLumi[iBX]
	       = (TotalEtSum[iBX] - TotalEtSumNoise[BXCapId_[iBX]])/(float)TotalActiveTowersEt_;
	    localSection.lumiDetail.ETLumiErr[iBX] = sqrt(ETSumError + ETSumNoiseError);

	    // Check for zero
	    if( TotalEtSum[iBX] == 0 ){
	       localSection.lumiDetail.ETLumi[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 701;
	    }
	    // Check for infinity
	    if( isinf(localSection.lumiDetail.ETLumi[iBX]) ){
	       localSection.lumiDetail.ETLumi[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 501;
	    }
	    // Check for NaN
	    if( isnan(localSection.lumiDetail.ETLumi[iBX]) ){
	       localSection.lumiDetail.ETLumi[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 511;
	    }
	    // Check upper bound
	    if( localSection.lumiDetail.ETLumi[iBX] > 999 ){
	       localSection.lumiDetail.ETLumi[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 521;
	    }
	    // Check lower bound
	    if( localSection.lumiDetail.ETLumi[iBX] < -999 ){
	       localSection.lumiDetail.ETLumi[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 531;
	    }
	    // Errors
	    // Check for zero
	    if( localSection.lumiDetail.ETLumiErr[iBX] == 0 ){
	       localSection.lumiDetail.ETLumiErr[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 201;
	    }
	    // Check for infinity
	    if( isinf(localSection.lumiDetail.ETLumiErr[iBX]) ){
	       localSection.lumiDetail.ETLumiErr[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 201;
	    }
	    // Check for NaN
	    if( isnan(localSection.lumiDetail.ETLumiErr[iBX]) ){
	       localSection.lumiDetail.ETLumiErr[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 211;
	    }
	    // Check upper bound
	    if( localSection.lumiDetail.ETLumiErr[iBX] > 999 ){
	       localSection.lumiDetail.ETLumiErr[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 221;
	    }
	    // Check lower bound
	    if( localSection.lumiDetail.ETLumiErr[iBX] < -999 ){
	       localSection.lumiDetail.ETLumiErr[iBX] = 0;
	       localSection.lumiDetail.ETLumiQlty[iBX] -= 231;
	    }

	    if( localSection.lumiDetail.ETLumiQlty[iBX] > 0 ) 
	       localSection.lumiSummary.InstantETLumi += localSection.lumiDetail.ETLumi[iBX];
	    else
	       --numGoodBX;
	 }
      } else {
	 localSection.lumiSummary.InstantETLumiQlty -= 201;
      }
      
      if( TotalNibbles > 0 ){
	 if( numGoodBX > 0 ){ 
	    localSection.lumiSummary.InstantETLumi /= numGoodBX;
	    localSection.lumiSummary.InstantETLumiErr = sqrt( ETSumError*normNumBX_ + ETSumNoiseError );
	    localSection.lumiSummary.InstantETLumiQlty = static_cast<int16_t>(999*(float)numGoodBX*normNumBX_);
	 } else {
	    localSection.lumiSummary.InstantETLumi = 0;  // error
	    localSection.lumiSummary.InstantETLumiErr = 0; // error
	    localSection.lumiSummary.InstantETLumiQlty -= 701;
	 }
      } 
   }

   // Calculate the per BX Et sum error
   float LumiCalc::CalcETSumError( HCAL_HLX::LUMI_SECTION &localSection ){

      // Check for the case where we have no data ...
      if( numBX_ == 0 || numHLX_ == 0 || numActiveTowersBXEt_ == 0 ) return 0;
      //return 992016;

      // General case 
     
      // 1. Calculate the mean for each BX in this event (and the total mean ...)

      // If the store is maxed out, move everything back by 1 and zero the
      // final entry
      if( etStoreSize_ <= ET_SUM_STORE_SIZE ) ++etStoreSize_;
      if( etStoreSize_ > ET_SUM_STORE_SIZE ){
	 --etStoreSize_;
	 //std::cout << "Here in pop back errors! " << etStoreSize_ << std::endl;
	 for( uint32_t iLB = 0; iLB < ET_SUM_STORE_SIZE; ++iLB ){
	    for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( iLB < (ET_SUM_STORE_SIZE-1) ) etSumAvgPerLB_[iLB][iBX] = etSumAvgPerLB_[iLB+1][iBX];
	       else                              etSumAvgPerLB_[iLB][iBX] = 0.0;
	    }
	 }
      }
      
      float nTowerBX = (float)(numActiveTowersBXEt_);
      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( BXMask_[iBX] != 1 ) continue;
	 if( BXCapId_[iBX] != 0 ) continue; 
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( !HLXMask_[iHLX] ) continue;
	    etSumAvgPerLB_[etStoreSize_-1][iBX] += localSection.etSum[iHLX].data[iBX];
	 }
	 etSumAvgPerLB_[etStoreSize_-1][iBX] /= nTowerBX;
      }
      //return 992016;

      // 2. Calculate the final error ... for the special case of the first
      // LB take this from the HLX spread, if there are less BX's than HLX's.
      float deviation = 0.0;
      float sigma = 0.0;
      if( etStoreSize_ <= 1 && (numBX_ < numHLX_) ){
	 // Calculate total mean
	 for( uint32_t iCap = 0; iCap<NUM_CAP_BANKS; ++iCap )
	 {
	    int tmpTotal = 0;
	    float mean = 0;
	    for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       if( !HLXMask_[iHLX] ) continue;
	       float nTower = (float)(numActiveTowersEt_[iHLX]);
	       if( nTower == 0 ) continue;
	       float tmpMean = 0.0;
	       for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX] != 1 ) continue;
		  if( BXCapId_[iBX] != iCap ) continue; 
		  tmpMean += localSection.etSum[iHLX].data[iBX];
		  ++tmpTotal;
	       }
	       mean += tmpMean/nTower;
	    }
	    mean = mean*normNumHLX_;

	    // Calculate deviation ...
	    for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       if( !HLXMask_[iHLX] ) continue; 
	       float totalBX = 0.0;
	       float nTower = (float)(numActiveTowersEt_[iHLX]);
	       if( nTower == 0 ) continue;
	       for( int iBX=0; iBX<HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX] != 1 ) continue;
		  if( BXCapId_[iBX] != 0 ) continue; 
		  totalBX += localSection.etSum[iHLX].data[iBX]/nTower;
	       }
	       totalBX *= 4*normNumBX_;
	       float diff = totalBX - mean;
	       deviation += diff*diff;
	    }    
	    //sigma = sqrt(deviation)*normNumHLX_;
	    sigma += deviation*normNumHLX_*normNumHLX_;  // return the variance
	 }
	 sigma /= (float)NUM_CAP_BANKS;
      }
      // Otherwise use the stored values from previous LB's to get a spread.
      else{
	 for( uint32_t iCap = 0; iCap<NUM_CAP_BANKS; ++iCap )
	 {
	    float totalMean = 0.0;
	    float norm = 0;
	    for( uint32_t iLB = 0; iLB < etStoreSize_; ++iLB ){
	       ++norm;
	       for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX] != 1 ) continue;
		  if( BXCapId_[iBX] != 0 ) continue; 
		  totalMean += etSumAvgPerLB_[iLB][iBX];
		  //++norm; 
	       }
	    }
	    if( norm > 0 ){
	       norm = 4*normNumBX_/norm;
	       totalMean *= norm;
	    }
	    //std::cout << "Total mean " << totalMean << " norm " << norm << std::endl;

	    for( uint32_t iLB = 0; iLB < etStoreSize_; ++iLB ){
	       for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX] != 1 ) continue;
		  if( BXCapId_[iBX] != 0 ) continue; 
		  float diff = totalMean - etSumAvgPerLB_[iLB][iBX];
// 	       std::cout << iBX << " " << totalMean << " " << etSumAvgPerLB_[iLB][iBX] << " " 
// 			 << diff << std::endl;
		  deviation += diff*diff;
	       }
	    }
	    //sigma = sqrt(deviation/norm);
	    sigma += deviation*norm;
	 }
	 sigma /= (float)NUM_CAP_BANKS;

	 if( sigmaEtWeight_ > ET_SUM_STORE_SIZE ){
	    float weight = sigmaEtWeight_/(sigmaEtWeight_+(float)ET_SUM_STORE_SIZE); 
	    sigma = (weight*sigmaEt_ + (1-weight)*sigma);
	 }
      }
      //std::cout << "Et sum +/- " << sqrt(sigma) << std::endl;

      sigmaEt_ = sigma;
      ++sigmaEtWeight_;

      return sigma; // error
   } 

   // Calculate the total noise error (for all noise BX's in each cap).
   float LumiCalc::CalcETSumNoiseError( HCAL_HLX::LUMI_SECTION &localSection ){

      // Check for the case where we have no data ...
      if( numBX_ == 0 || numHLX_ == 0 || numActiveTowersBXEt_ == 0 ) return 0;
      //return 992016;

      // General case 
     
      // 1. Calculate the mean for each BX in this event (and the total mean ...)

      // If the noise store is maxed out, move everything back by 1 and zero the
      // final entry
      if( etNoiseStoreSize_ <= ET_SUM_STORE_SIZE ) ++etNoiseStoreSize_;
      if( etNoiseStoreSize_ > ET_SUM_STORE_SIZE ){
	 --etNoiseStoreSize_;
	 for( uint32_t iLB = 0; iLB < ET_SUM_STORE_SIZE; ++iLB ){
	    for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( iLB < (ET_SUM_STORE_SIZE-1) ) etSumNoiseAvgPerLB_[iLB][iBX] = etSumNoiseAvgPerLB_[iLB+1][iBX];
	       else                              etSumNoiseAvgPerLB_[iLB][iBX] = 0.0;
	    }
	 }
      }

      float nTowerBX = (float)(numActiveTowersBXEt_);
      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( BXMask_[iBX] != 2 ) continue;
	 if( sizeNoise_[BXCapId_[iBX]] == 0 ) continue;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( !HLXMask_[iHLX] ) continue;
	    etSumNoiseAvgPerLB_[etNoiseStoreSize_-1][iBX] += localSection.etSum[iHLX].data[iBX];
	 }
	 etSumNoiseAvgPerLB_[etNoiseStoreSize_-1][iBX] /= nTowerBX;
      }


      // 2. Calculate the final error ... for the special case of the first
      // LB take this from the HLX spread, if there are less BX's than HLX's.
      float sigma = 0.0;
      for( uint32_t iCap=0; iCap<NUM_CAP_BANKS; ++iCap ){
	 if( sizeNoise_[iCap] == 0 ) continue;
	 if( (sizeNoise_[iCap] < numHLX_) && etNoiseStoreSize_ <= 1 ){

	    // Get the overall mean ...
	    float mean[4] = {0,0,0,0};
	    for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       if( !HLXMask_[iHLX] ) continue;
	       float nTower = (float)(numActiveTowersEt_[iHLX]);
	       if( nTower == 0 ) continue;
	       float tmpMean = 0.0;
	       for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX] != 2 ) continue;
		  if( sizeNoise_[BXCapId_[iBX]] == 0 ) continue;
		  tmpMean += localSection.etSum[iHLX].data[iBX];
	       }
	       mean[iCap] += tmpMean/nTower;
	    }
	    mean[iCap] = mean[iCap]*invSizeNoise_[iCap]*normNumHLX_;

	    // Deviations from the mean
	    float deviation = 0.0;
	    for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       if( !HLXMask_[iHLX] ) continue; 
	       float totalBX = 0.0;
	       float nTower = (float)(numActiveTowersEt_[iHLX]);
	       if( nTower == 0 ) continue;
	       //for( uint32_t iBX=0; iBX<HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       for( uint32_t iBX=iCap; iBX<HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
		  if( BXMask_[iBX] != 2 ) continue;
		  //if( (BXCapId_[iBX]) != iCap ) continue;
		  totalBX += localSection.etSum[iHLX].data[iBX];
	       }
	       totalBX /= (nTower*(float)sizeNoise_[iCap]);
	       float diff = totalBX - mean[iCap];
	       deviation += diff*diff;
	    }    
	    //sigma += (sqrt(deviation)*normNumHLX_)/sqrt((float)sizeNoise_[iCap]);

	    //sigma += sqrt(deviation*invSizeNoise_[iCap])*normNumHLX_;
	    sigma += deviation*invSizeNoise_[iCap]*normNumHLX_*normNumHLX_;
	 }
	 // Otherwise use the stored values from previous LB's to get a spread.
	 else{
	    float totalMean = 0.0;
	    float norm = 0;
	    for( uint32_t iLB = 0; iLB < etNoiseStoreSize_; ++iLB ){
	       //for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       for( uint32_t iBX = iCap; iBX < HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
		  if( BXMask_[iBX] != 2 ) continue;
		  //if( (BXCapId_[iBX]) != iCap ) continue;
		  totalMean += etSumNoiseAvgPerLB_[iLB][iBX];
		  ++norm; 
	       }
	    }
	    if( norm > 0 ) totalMean /= norm;
	    //std::cout << "Total mean " << totalMean << " norm " << norm << std::endl;

	    float deviation = 0.0;
	    for( uint32_t iLB = 0; iLB < etNoiseStoreSize_; ++iLB ){
	       //for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       for( uint32_t iBX = iCap; iBX < HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
		  if( BXMask_[iBX] != 2 ) continue;
		  //if( (BXCapId_[iBX]) != iCap ) continue;
		  float diff = totalMean - etSumNoiseAvgPerLB_[iLB][iBX];
		  deviation += diff*diff;
	       }
	    }
	    //sigma += sqrt(deviation/norm)/sqrt((float)sizeNoise_[iCap]);  

	    //sigma += sqrt(deviation*invSizeNoise_[iCap]/norm);  
	    if( norm > 0 ) sigma += deviation*invSizeNoise_[iCap]/norm;  
	 }
      }
      //sigma /= (float)NUM_CAP_BANKS;
      sigma /= ((float)NUM_CAP_BANKS*(float)NUM_CAP_BANKS);

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

      //return;

      double occNoise[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
    
      // Calculate noise per cap bank
      for( uint32_t iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ){
	 if( sizeNoise_[iCap] == 0 ) continue;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    occNoise[iHLX][0][iCap] = 0;
	    occNoise[iHLX][1][iCap] = 0;
	    if( HLXMask_[iHLX] == 0 ) continue;
	    uint32_t tmpSum[2] = {0,0};
	    for(uint32_t iBX = iCap ; iBX < HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
	       if( BXMask_[iBX] != 2 ) continue;
	       tmpSum[0] += localSection.occupancy[iHLX].data[set1BelowIndex][iBX];
	       tmpSum[1] += localSection.occupancy[iHLX].data[set2BelowIndex][iBX];
	    }
	    // Average num towers below threshold per tower per BX
	    occNoise[iHLX][0][iCap] = (double)tmpSum[0]*(double)invSizeNoise_[iCap];
	    occNoise[iHLX][1][iCap] = (double)tmpSum[1]*(double)invSizeNoise_[iCap];
	 }
      }

      // Calculate the noise error ...
      float noiseError[2];
      noiseError[0] = CalcOccNoiseError( localSection, 0 );
      noiseError[1] = CalcOccNoiseError( localSection, 1 );

      // Only use good BX's
      uint32_t numGoodBX[2] = {numBX_,numBX_};

      localSection.lumiSummary.InstantOccLumi[0] = 0.0;
      localSection.lumiSummary.InstantOccLumi[1] = 0.0;

      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 localSection.lumiDetail.OccLumi[0][iBX] = 0.0;
	 localSection.lumiDetail.OccLumi[1][iBX] = 0.0;
	 localSection.lumiDetail.OccLumiQlty[0][iBX] = 0;
	 localSection.lumiDetail.OccLumiQlty[1][iBX] = 0;
	 localSection.lumiDetail.OccBXNormalization[0][iBX] = 1;
	 localSection.lumiDetail.OccBXNormalization[1][iBX] = 1;

	 if( BXMask_[iBX] != 1 ) continue;
	 localSection.lumiDetail.OccLumiQlty[0][iBX] = 1;
	 localSection.lumiDetail.OccLumiQlty[1][iBX] = 1;

	 uint32_t sumHLX[2] = {0,0};
	 double sumNoiseHLX[2] = {0,0};
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( !HLXMask_[iHLX] ) continue;
	    
	    // Ring Set 1
	    sumHLX[0] += localSection.occupancy[iHLX].data[set1BelowIndex][iBX];

	    // Ring Set 2
	    sumHLX[1] += localSection.occupancy[iHLX].data[set2BelowIndex][iBX];
	
	    if( sizeNoise_[BXCapId_[iBX]] == 0 ) continue;
	    sumNoiseHLX[0] += occNoise[iHLX][0][BXCapId_[iBX]];
	    sumNoiseHLX[1] += occNoise[iHLX][1][BXCapId_[iBX]];
	
	    // TO DO: Take into account different numbers of active towers in HLX.
	    // Check for dropped nibbles.
	 }

	 // Ring Set 1
	 if( sumHLX[0] > 0 ){
	    if( sumNoiseHLX[0] > 0 ){
	       localSection.lumiDetail.OccLumi[0][iBX] = (float)log( sumNoiseHLX[0] / (double)sumHLX[0] );
	    }
	    else{
	       localSection.lumiDetail.OccLumi[0][iBX] = (float)log( (double)numActiveTowersBX_[0] / (double)sumHLX[0] );
	       localSection.lumiDetail.OccLumiQlty[0][iBX] += 99;
	    }
	 }
	 else{
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 701;
	 }

	 // Ring Set 2
	 if( sumHLX[1] > 0 ){
	    if( sumNoiseHLX[1] > 0 ){
	       localSection.lumiDetail.OccLumi[1][iBX] = (float)log( sumNoiseHLX[1] / (double)sumHLX[1] );
	    }
	    else{
	       localSection.lumiDetail.OccLumi[1][iBX] = (float)log( (double)numActiveTowersBX_[1] / (double)sumHLX[1] );
	       localSection.lumiDetail.OccLumiQlty[1][iBX] += 99;
	    }
	 }
	 else{
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 701;
	 }

	 localSection.lumiDetail.OccLumiErr[0][iBX] = sqrt(CalcOccErrorBX( localSection, 0, iBX ) + noiseError[0]);
	 localSection.lumiDetail.OccLumiErr[1][iBX] = sqrt(CalcOccErrorBX( localSection, 1, iBX ) + noiseError[1]);


	 // Check for infinity
	 if( isinf(localSection.lumiDetail.OccLumi[0][iBX]) ){
	    localSection.lumiDetail.OccLumi[0][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 501;
	 }
	 if( isinf(localSection.lumiDetail.OccLumi[1][iBX]) ){
	    localSection.lumiDetail.OccLumi[1][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 501;
	 }
	 // Check for NaN
	 if( isnan(localSection.lumiDetail.OccLumi[0][iBX]) ){
	    localSection.lumiDetail.OccLumi[0][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 511;
	 }
	 if( isnan(localSection.lumiDetail.OccLumi[1][iBX]) ){
	    localSection.lumiDetail.OccLumi[1][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 511;
	 }
	 // Check upper bound
	 if( localSection.lumiDetail.OccLumi[0][iBX] > 999 ){
	    localSection.lumiDetail.OccLumi[0][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 521;
	 }
	 if( localSection.lumiDetail.OccLumi[1][iBX] > 999 ){
	    localSection.lumiDetail.OccLumi[1][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 521;
	 }
	 // Check lower bound
	 if( localSection.lumiDetail.OccLumi[0][iBX] < -999 ){
	    localSection.lumiDetail.OccLumi[0][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 531;
	 }
	 if( localSection.lumiDetail.OccLumi[1][iBX] < -999 ){
	    localSection.lumiDetail.OccLumi[1][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 531;
	 }
	 // Errors
	 // Check for infinity
	 if( isinf(localSection.lumiDetail.OccLumiErr[0][iBX]) ){
	    localSection.lumiDetail.OccLumiErr[0][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 201;
	 }
	 if( isinf(localSection.lumiDetail.OccLumiErr[1][iBX]) ){
	    localSection.lumiDetail.OccLumiErr[1][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 201;
	 }
	 // Check for NaN
	 if( isnan(localSection.lumiDetail.OccLumiErr[0][iBX]) ){
	    localSection.lumiDetail.OccLumiErr[0][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 211;
	 }
	 if( isnan(localSection.lumiDetail.OccLumiErr[1][iBX]) ){
	    localSection.lumiDetail.OccLumiErr[1][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 211;
	 }
	 // Check upper bound
	 if( localSection.lumiDetail.OccLumiErr[0][iBX] > 999 ){
	    localSection.lumiDetail.OccLumiErr[0][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 221;
	 }
	 if( localSection.lumiDetail.OccLumiErr[1][iBX] > 999 ){
	    localSection.lumiDetail.OccLumiErr[1][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 221;
	 }
	 // Check lower bound
	 if( localSection.lumiDetail.OccLumiErr[0][iBX] < -999 ){
	    localSection.lumiDetail.OccLumiErr[0][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 231;
	 }
	 if( localSection.lumiDetail.OccLumiErr[1][iBX] < -999 ){
	    localSection.lumiDetail.OccLumiErr[1][iBX] = 0;
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 231;
	 }

	 if( localSection.lumiDetail.OccLumiQlty[0][iBX] > 0 )
	    localSection.lumiSummary.InstantOccLumi[0] += localSection.lumiDetail.OccLumi[0][iBX];
	 else
	    --numGoodBX[0];
	 if( localSection.lumiDetail.OccLumiQlty[1][iBX] > 0 )
	    localSection.lumiSummary.InstantOccLumi[1] += localSection.lumiDetail.OccLumi[1][iBX];
	 else
	    --numGoodBX[1];

      }

      // normailze to the number of active, good bunches
      if( numGoodBX[0] > 0 ){
	 localSection.lumiSummary.InstantOccLumi[0] /= (float)numGoodBX[0];
	 localSection.lumiSummary.InstantOccLumiQlty[0] = static_cast<int16_t>(999*(float)numGoodBX[0]*normNumBX_);
      } else {
	 localSection.lumiSummary.InstantOccLumi[0] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[0] = -900;
      }
      if( numGoodBX[1] > 0 ){
	 localSection.lumiSummary.InstantOccLumi[1] /= (float)numGoodBX[1];
	 localSection.lumiSummary.InstantOccLumiQlty[1] = static_cast<int16_t>(999*(float)numGoodBX[1]*normNumBX_);
      } else {
	 localSection.lumiSummary.InstantOccLumi[1] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[1] = -900;
      }

      // Sum over bunch crossings
      localSection.lumiSummary.InstantOccLumiErr[0] = sqrt(CalcOccErrorTotal( localSection, 0 )+noiseError[0]);
      localSection.lumiSummary.InstantOccLumiErr[1] = sqrt(CalcOccErrorTotal( localSection, 1 )+noiseError[1]);

      // Check for infinity
      if( isinf( localSection.lumiSummary.InstantOccLumi[0] ) ){
	 localSection.lumiSummary.InstantOccLumi[0] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[0] = -500;
      }
      if( isinf( localSection.lumiSummary.InstantOccLumi[1] ) ){
	 localSection.lumiSummary.InstantOccLumi[1] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[1] = -500;
      }
      // Check for NaN
      if( isnan( localSection.lumiSummary.InstantOccLumi[0] ) ){
	 localSection.lumiSummary.InstantOccLumi[0] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[0] = -510;
      }
      if( isnan( localSection.lumiSummary.InstantOccLumi[1] ) ){
	 localSection.lumiSummary.InstantOccLumi[1] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[1] = -510;
      }
      // Check upper bound
      if( localSection.lumiSummary.InstantOccLumi[0] > 999 ){
	 localSection.lumiSummary.InstantOccLumi[0] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[0] = -520;
      }
      if( localSection.lumiSummary.InstantOccLumi[1] > 999 ){
	 localSection.lumiSummary.InstantOccLumi[1] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[1] = -520;
      }
      // Check lower bound
      if( localSection.lumiSummary.InstantOccLumi[0] < -999 ){
	 localSection.lumiSummary.InstantOccLumi[0] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[0] = -530;
      }
      if( localSection.lumiSummary.InstantOccLumi[1] < -999 ){
	 localSection.lumiSummary.InstantOccLumi[1] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[1] = -530;
      }
      // Errors ...
      // Check for infinity
      if( isinf( localSection.lumiSummary.InstantOccLumiErr[0] ) ){
	 localSection.lumiSummary.InstantOccLumiErr[0] = 0;
	 if( localSection.lumiSummary.InstantOccLumiQlty[0] < 0 ) 
	    localSection.lumiSummary.InstantOccLumiQlty[0] -= 201;
	 else 
	    localSection.lumiSummary.InstantOccLumiQlty[0] = -200;
      }
      if( isinf( localSection.lumiSummary.InstantOccLumiErr[1] ) ){
	 localSection.lumiSummary.InstantOccLumiErr[1] = 0;
	 if( localSection.lumiSummary.InstantOccLumiQlty[1] < 0 ) 
	    localSection.lumiSummary.InstantOccLumiQlty[1] -= 201;
	 else 
	    localSection.lumiSummary.InstantOccLumiQlty[1] = -200;
      }
      // Check for NaN
      if( isnan( localSection.lumiSummary.InstantOccLumiErr[0] ) ){
	 localSection.lumiSummary.InstantOccLumiErr[0] = 0;
	 if( localSection.lumiSummary.InstantOccLumiQlty[0] < 0 ) 
	    localSection.lumiSummary.InstantOccLumiQlty[0] -= 211;
	 else 
	    localSection.lumiSummary.InstantOccLumiQlty[0] = -210;
      }
      if( isnan( localSection.lumiSummary.InstantOccLumiErr[1] ) ){
	 localSection.lumiSummary.InstantOccLumiErr[1] = 0;
	 if( localSection.lumiSummary.InstantOccLumiQlty[1] < 0 ) 
	    localSection.lumiSummary.InstantOccLumiQlty[1] -= 211;
	 else 
	    localSection.lumiSummary.InstantOccLumiQlty[1] = -210;
      }    
      // Check upper bound
      if( localSection.lumiSummary.InstantOccLumiErr[0] > 999 ){
	 localSection.lumiSummary.InstantOccLumiErr[0] = 0;
	 if( localSection.lumiSummary.InstantOccLumiQlty[0] < 0 ) 
	    localSection.lumiSummary.InstantOccLumiQlty[0] -= 221;
	 else 
	    localSection.lumiSummary.InstantOccLumiQlty[0] = -220;
      }
      if( localSection.lumiSummary.InstantOccLumiErr[1] > 999 ){
	 localSection.lumiSummary.InstantOccLumiErr[1] = 0;
	 if( localSection.lumiSummary.InstantOccLumiQlty[1] < 0 ) 
	    localSection.lumiSummary.InstantOccLumiQlty[1] -= 221;
	 else 
	    localSection.lumiSummary.InstantOccLumiQlty[1] = -220;
      }
      // Check lower bound
      if( localSection.lumiSummary.InstantOccLumiErr[0] < -999 ){
	 localSection.lumiSummary.InstantOccLumiErr[0] = 0;
	 if( localSection.lumiSummary.InstantOccLumiQlty[0] < 0 ) 
	    localSection.lumiSummary.InstantOccLumiQlty[0] -= 231;
	 else 
	    localSection.lumiSummary.InstantOccLumiQlty[0] = -230;
      }
      if( localSection.lumiSummary.InstantOccLumiErr[1] < -999 ){
	 localSection.lumiSummary.InstantOccLumiErr[1] = 0;
	 if( localSection.lumiSummary.InstantOccLumiQlty[1] < 0 ) 
	    localSection.lumiSummary.InstantOccLumiQlty[1] -= 231;
	 else 
	    localSection.lumiSummary.InstantOccLumiQlty[1] = -230;
      }

      // Normalization ... ???
      localSection.lumiSummary.OccNormalization[0] = 1;
      localSection.lumiSummary.OccNormalization[1] = 1;

   }

   float LumiCalc::CalcOccNoiseError( HCAL_HLX::LUMI_SECTION &localSection, uint32_t set ){

      uint32_t index = set1BelowIndex;
      if( set == 1 ) index = set2BelowIndex;

      // But we also need the noise error
      uint64_t nBelowN[NUM_CAP_BANKS] = {0,0,0,0};
      uint64_t nTotalN = 0;
      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( !HLXMask_[iHLX] ) continue;
	 nTotalN += numActiveTowers_[iHLX][set];
	 for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] != 2 ) continue;
	    nBelowN[BXCapId_[iBX]] += localSection.occupancy[iHLX].data[index][iBX];
	 }
      }


      float avg = 0.0;
      if( nTotalN > 0 ){
	 for( uint32_t iCap=0; iCap < NUM_CAP_BANKS; ++iCap ){
	    //std::cout << "Size noise " << sizeNoise_[iCap] << std::endl;
	    //std::cout << "NBelow " << nBelowN[iCap] << " " << nTotalN*sizeNoise_[iCap] << std::endl;
	    if( sizeNoise_[iCap] == 0 ) continue;
	    if( nBelowN[iCap] <= 0 ) continue;
	    uint64_t total = nTotalN*sizeNoise_[iCap];
	    if( nBelowN[iCap] >= total ) continue;
	    //cout << iCap << " ** NBelow " << nBelowN[iCap] << " nTotalN " << total << endl; 
	    //avg += sqrt((1/nBelowN[iCap])*(1-nBelowN[iCap]/(nTotalN*(float)sizeNoise_[iCap])));
	    avg += (float)(1.0/(double)nBelowN[iCap] - 1.0/(double)total);
	 }
	 avg /= (float)NUM_CAP_BANKS;
      }

      return avg;
   }
  
   float LumiCalc::CalcOccErrorTotal( HCAL_HLX::LUMI_SECTION &localSection, uint32_t set ){
    
      // Check for the case where we have no data ...
      //if( numBX_ ==0 || numHLX_ ==0 ) return -995;
      //return -996;

      // Index
      uint32_t index = set1BelowIndex;
      if( set == 1 ) index = set2BelowIndex;
      
      // For occupancy we can just calculate the accumulated binomial error
      uint64_t nBelow = 0;
      uint64_t nTotal = (uint64_t)numActiveTowersBX_[set]*(uint64_t)numBX_;
      //std::cout << set << " NTower " << numActiveTowersBX_[set] << " " << numBX_ << std::endl;
      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( BXMask_[iBX] != 1 ) continue;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( HLXMask_[iHLX] == 0 ) continue;
	    nBelow += localSection.occupancy[iHLX].data[index][iBX];
	 }
      }
      //std::cout << set << " NTower " << nTotal << " " << nBelow << std::endl;

      float sigma = 0.0;
      if( nTotal > 0 && nBelow > 0){
	 sigma = (float)(1.0/(double)nBelow - 1.0/(double)nTotal);
      }
      //std::cout << index << " ** NBelow " << nBelow << " nTotal " << nTotal << std::endl; 
      //std::cout << "Occ sigma total " << sigma << std::endl;

      return sigma;
      //return sqrt(sigma); // error
   }

   float LumiCalc::CalcOccErrorBX( HCAL_HLX::LUMI_SECTION &localSection, uint32_t set, uint32_t iBX ){
    
      // Check for the case where we have no data ...
      //if( numBX_ ==0 || numHLX_ ==0 ) return -995;
      if( BXMask_[iBX] != 1 ) return 0;

      // Index
      int index = set1BelowIndex;
      if( set == 1 ) index = set2BelowIndex;

      // For occupancy we can just calculate the accumulated binomial error
      uint32_t nBelow = 0;
      uint32_t nTotal = numActiveTowersBX_[set];
      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( !HLXMask_[iHLX] ) continue;
	 nBelow += localSection.occupancy[iHLX].data[index][iBX];
      }

      float sigma = 0.0;
      if( nTotal > 0 && nBelow > 0 ){
	 sigma = (float)(1.0/(double)nBelow - 1.0/(double)nTotal);
      }

      return sigma; // error
   }


   void LumiCalc::CalcLumi( HCAL_HLX::LUMI_SECTION &localSection ){

      localSection.lumiSummary.InstantLumi = localSection.lumiSummary.InstantETLumi;

      localSection.lumiSummary.InstantLumiErr = localSection.lumiSummary.InstantETLumiErr;

      localSection.lumiSummary.InstantLumiQlty = 1;
   }

   float LumiCalc::CalcLumiError( uint32_t numNibbles, uint32_t numTowers, uint32_t numBunches, float intPerBX )
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
      const uint8_t gap = 0;
      const uint8_t bunch = 1;
    
      if(BX > (uint32_t)numBlock*80){
	 return gap;
      }else if(BX%80 > 72){
	 return gap;
      }else {
	 return bunch;
      }
   }
  
   bool LumiCalc::isBunch(unsigned short int BX, unsigned short int scheme ){
      // determine where you are in the bunch structure
      const uint8_t abortGap = 0;
      uint16_t BXrel; // relative bunch crossing
    
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
