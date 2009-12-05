/****************************************************************************

Author:  Adam Hunt, and Nadia Adam
         ahunt@princeton.edu, neadam@princeton.edu

Summary:  Library to calculate relative luminosity from HLX data

*****************************************************************************/

#ifdef STANDALONE
#include "LumiCalc.hh"
#include "LumiStructures.hh"
#else
#include "RecoLuminosity/LumiCalc/interface/LumiCalc.hh"
#include "RecoLuminosity/TCPReceiver/interface/LumiStructures.hh"
#endif

#ifdef __MACOSX__
#include <math.h> // isnan, isinf
#else
#include <cmath> // isnan, isinf
#endif
#include <cstring> // memset
#include <iostream>

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

// #ifndef isnan
// inline bool isnan(double x) {
//    volatile double d = x;
//    return d != d;
// }
// #endif

namespace HCAL_HLX {
  
   LumiCalc::LumiCalc(){
    
      // ******************* HLX to HF Wedge maps ********************** //
      // unsigned int HLXHFMap[] = { 31,32,33,34,35,18,  // s2f07 hf-    //
      // 			     13,14,15,16,17,0,   // s2f07 hf+    //
      // 			     25,26,27,28,29,30,  // s2f05 hf-    //
      // 			     7, 8, 9, 10,11,12,  // s2f05 hf+    //
      // 			     19,20,21,22,23,24,  // s2f02 hf-    //
      // 			     1, 2, 3, 4, 5, 6};  // s2f02 hf+    //
      // *************************************************************** //

      // All HLX's unmasked
      uint32_t HLXMask[HCAL_HLX_NUM_HLXS];
      SetHLXMask ( HLXMask, HCAL_HLX_NUM_HLXS );

      // Masking convention: kMasked, kActive, kInactive, kNoise, kOther
      // Self explanatory except for kOther, which should be used for ranges of BX's
      // where the user wants to look at data like splash or laser.
      uint32_t BXMask[HCAL_HLX_NUM_BUNCHES];
      for (uint32_t iBX = 0;    iBX < 3302; ++iBX )                 BXMask[iBX] = kInactive; // UnMask
      for (uint32_t iBX = 3302; iBX < 3450; ++iBX )                 BXMask[iBX] = kNoise; // Noise
      for (uint32_t iBX = 3450; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ) BXMask[iBX] = kMasked; // Mask
      SetBXMask ( BXMask, HCAL_HLX_NUM_BUNCHES );

      // Et sum error parameters
      for( int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 sigmaEtBX_[iBX] = 0;
	 sigmaEtWeightBX_[iBX] = 0.0;
      }
      
      for( int iCap=0; iCap<NUM_CAP_BANKS; ++iCap){ 
	 sigmaEtNoise_[iCap] = 0.0;   
	 for( int iType=0; iType<kNumTypes; ++iType ){
	    sigmaEt_[iType][iCap] = 0.0;  
	    sigmaEtWeight_[iType] = 0.0;
	 }
      }
      sigmaEtNoiseWeight_ = 0.0;
      etStoreSize_ = 0;
      etNoiseStoreSize_ = 0;

      //find first noise BX
      for( iBXNoise_ = 0; iBXNoise_ < HCAL_HLX_NUM_BUNCHES; ++iBXNoise_ ){
         if( BXMask_[iBXNoise_] == kNoise ) break;
      }  

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

      // Num cap banks squared (for error addition)
      nCap2_ = (float)(NUM_CAP_BANKS*NUM_CAP_BANKS);
   }
  
   //****************** Initialization and Configuration Functions ***************
    
   uint32_t LumiCalc::SetBXMask( uint32_t BXMask[], uint32_t maskSize ){
      // Copy given mask into member variable and count number of active bunches

      numBX_ = 0;
      numBXActive_=0;
      numBXInactive_=0;
      numBXOther_=0;
      for (int iCap =0; iCap < NUM_CAP_BANKS; ++iCap) sizeNoise_[iCap] = 0;
    
      for( uint32_t iBX = 0; iBX < maskSize ; ++iBX ){
	 if( BXMask[iBX] < kNumTypes ) BXMask_[iBX] = BXMask[iBX];
	 else                          BXMask_[iBX] = kMasked;

	 if( BXMask_[iBX] == kActive || BXMask_[iBX] == kInactive || 
	     BXMask_[iBX] == kNoise  || BXMask_[iBX] == kOther ){ 
	    ++numBX_;

	    if( BXMask_[iBX] == kActive )        ++numBXActive_;
	    else if( BXMask_[iBX] == kInactive ) ++numBXInactive_;
	    else if( BXMask_[iBX] == kOther )    ++numBXOther_;
	 }
	 if( BXMask_[iBX] == kNoise ) ++sizeNoise_[iBX % 4];

	 BXCapId_[iBX] = iBX % NUM_CAP_BANKS;
	 // If we are before the QIE reset roate the caps ...
	 // if( iBX < 335 ){
	 //    if( BXCapId_[iBX] > 0 ) BXCapId_[iBX] -= 1;
	 //    else                    BXCapId_[iBX] = 3;
	 // }
      }

      for( uint32_t iBX = maskSize; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 BXMask_[iBX] = kMasked;
	 BXCapId_[iBX] = iBX % NUM_CAP_BANKS;
      }
    
      for( int iCap =0; iCap < NUM_CAP_BANKS; ++iCap ) invSizeNoise_[iCap] = 1.0/(float)sizeNoise_[iCap];
      normNumBX_ = 1.0;
      normNumBXActive_ = 1.0;
      normNumBXInactive_ = 1.0;
      if( numBX_ > 0 )         normNumBX_         = 1.0/(float)numBX_;
      if( numBXActive_ > 0 )   normNumBXActive_   = 1.0/(float)numBXActive_;
      if( numBXInactive_ > 0 ) normNumBXInactive_ = 1.0/(float)numBXInactive_;

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
      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){

	 numActiveTowersBX_[iBX][0] = 0;
	 numActiveTowersBX_[iBX][1] = 0;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    numActiveTowers_[iHLX][iBX][0] = 0;
	    numActiveTowers_[iHLX][iBX][1] = 0;
	    if( HLXMask_[iHLX] == 0 ) continue;

	    numActiveTowers_[iHLX][iBX][0] =  ( localSection.occupancy[iHLX].data[set1AboveIndex][iBX] +
						localSection.occupancy[iHLX].data[set1BetweenIndex][iBX] +
						localSection.occupancy[iHLX].data[set1BelowIndex][iBX] );

	    numActiveTowers_[iHLX][iBX][1] = ( localSection.occupancy[iHLX].data[set2AboveIndex][iBX] +
					       localSection.occupancy[iHLX].data[set2BetweenIndex][iBX] +
					       localSection.occupancy[iHLX].data[set2BelowIndex][iBX] );

	    numActiveTowersBX_[iBX][0] += ( localSection.occupancy[iHLX].data[set1AboveIndex][iBX] +
					    localSection.occupancy[iHLX].data[set1BetweenIndex][iBX] +
					    localSection.occupancy[iHLX].data[set1BelowIndex][iBX] );
	    
	    numActiveTowersBX_[iBX][1] += ( localSection.occupancy[iHLX].data[set2AboveIndex][iBX] +
					    localSection.occupancy[iHLX].data[set2BetweenIndex][iBX] +
					    localSection.occupancy[iHLX].data[set2BelowIndex][iBX] );

	 }
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

      for( int iCap=0; iCap<NUM_CAP_BANKS; ++iCap ) avNumActiveTowersEtNoise_[iCap] = 0;

      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( HLXMask_[iHLX] == 1 ){
	       float twrSum = float(numActiveTowers_[iHLX][iBX][0] + numActiveTowers_[iHLX][iBX][1]);
	       numActiveTowersEt_[iHLX][iBX] = (uint32_t)(twrSum*reNorm);
	       // If there are no masks ...
	       if( twrSum > 8*totalBX ){ numActiveTowersEt_[iHLX][iBX] = (uint32_t)(48*totalBXEt); }
	    } else {
	       numActiveTowersEt_[iHLX][iBX] = 0;
	    }
	    TotalActiveTowersEt_ += (numActiveTowersEt_[iHLX][iBX]);
	 }
	 //std::cout << "Total active towers " << TotalActiveTowers_ << std::endl;

	 float totalHLX = float(toccNibbles*numOrbitsPerNB_*numHLX_);
	 float totalHLXEt = float(etNibbles*numOrbitsPerNB_*numHLX_);
	 float twrSum = float(numActiveTowersBX_[iBX][0] + numActiveTowersBX_[iBX][1]);
	 numActiveTowersBXEt_[iBX] = (uint32_t)(twrSum*reNorm);
	 if( twrSum > 8*totalHLX ){ numActiveTowersBXEt_[iBX] = (uint32_t)(48*totalHLXEt); }
	 if( BXMask_[iBX] == kNoise ) avNumActiveTowersEtNoise_[BXCapId_[iBX]] += (twrSum*reNorm);
      }
      for( int iCap=0; iCap<NUM_CAP_BANKS; ++iCap ) avNumActiveTowersEtNoise_[iCap] *= invSizeNoise_[iCap];
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
      localSection.lumiSummary.InstantETLumiErr = 0.0;
      localSection.lumiSummary.InstantETLumiQlty = 1;
      localSection.lumiSummary.ETNormalization = 1.0;

      // count nibbles.
      uint32_t TotalNibbles = 0;
      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( HLXMask_[iHLX] == 1 ) TotalNibbles += localSection.etSum[iHLX].hdr.numNibbles;
      }
      if( TotalNibbles == 0 ) localSection.lumiSummary.InstantETLumiQlty -= 51; 

      // Sum over HLXs && Fill Normalization and quality
      // Also initialize ETLumi and Error to 0.0
      float TotalEtSum[HCAL_HLX_NUM_BUNCHES];
      float TotalEtSumNoise[NUM_CAP_BANKS]; 

      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 localSection.lumiDetail.ETLumi[iBX]                = 0.0;
	 localSection.lumiDetail.ETLumiErr[iBX]             = 0.0;
	 localSection.lumiDetail.ETLumiQlty[iBX]            = 1;
	 localSection.lumiDetail.ETBXNormalization[iBX]     = 1.0;      
	 TotalEtSum[iBX] = 0.0;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( HLXMask_[iHLX] == 1 ) TotalEtSum[iBX] += localSection.etSum[iHLX].data[iBX];
	 }
      }

      // Do the error calculations (per cap bank)
      float ETSumErrorActive[4] = {0,0,0,0};
      float ETSumErrorInactive[4] = {0,0,0,0};
      float ETSumFracErrorActive[4] = {0,0,0,0};
      float ETSumFracErrorInactive[4] = {0,0,0,0};
      if( TotalNibbles > 0 ){
	 PrepETSumError( localSection );
	 CalcETSumError( localSection, kActive, ETSumErrorActive, ETSumFracErrorActive );
	 CalcETSumError( localSection, kInactive, ETSumErrorInactive, ETSumFracErrorInactive );
      }  

      // Calculate noise per cap bank and the error on the noise
      for( uint32_t iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ) TotalEtSumNoise[iCap] = 0.0;
      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( BXMask_[iBX] == kNoise ) TotalEtSumNoise[BXCapId_[iBX]] += TotalEtSum[iBX];
      }
      for( uint32_t iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ){
	 if( sizeNoise_[iCap] == 0 ) continue;
	 TotalEtSumNoise[iCap] *= invSizeNoise_[iCap];
      }
      float ETSumNoiseError[4] = {0,0,0,0};
      if( TotalNibbles > 0 ) CalcETSumNoiseError( localSection, ETSumNoiseError );

      // Recall active = filled BX, inactive = empty BX
      uint32_t numGoodBX = numBXActive_+numBXInactive_+numBXOther_;

      // Calculate the Et sum per BX (if we have a non-zero number of towers)
      if( TotalActiveTowersEt_ > 0 ){
	 for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] == kMasked ) continue;

	    // ET lumi/BX = ( SumEt/NumTowers - SumNoise/NoiseTowers )
	    localSection.lumiDetail.ETLumi[iBX]
	       = (TotalEtSum[iBX]/(float)numActiveTowersBXEt_[iBX] - 
		  TotalEtSumNoise[BXCapId_[iBX]]/avNumActiveTowersEtNoise_[BXCapId_[iBX]]);

	    // Hack for bad BX's that seem to still have QIE reset mask in Et sum, 
	    // but not occupancy BX's 23 - 25.
	    if( iBX >= 23 && iBX <= 25 ){
	       localSection.lumiDetail.ETLumi[iBX]
		  = (TotalEtSum[iBX]/(float)numActiveTowersBXEt_[22] - 
		     TotalEtSumNoise[BXCapId_[iBX]]/avNumActiveTowersEtNoise_[BXCapId_[iBX]]);
	    }

	    // Set the ET lumi error according to the BX type
	    float error = 0.0;
	    if( BXMask_[iBX] == kActive )
	       error = ETSumErrorActive[BXCapId_[iBX]];
	    else if( BXMask_[iBX] == kInactive || BXMask_[iBX] == kNoise )
	       error = ETSumErrorInactive[BXCapId_[iBX]];
	    else{
	       // If BX type = kOther, use the fractional error from the inactive BX's
	       // Should use this type for e.g., Laser BX's or LED BX's or Splash BX's
	       error = ETSumFracErrorInactive[BXCapId_[iBX]]*TotalEtSum[iBX]/(float)numActiveTowersBXEt_[iBX];
	       error *= error;
	    }

	    // Final error = sqrt( DelEt^2 + DelNoise^2 )
	    localSection.lumiDetail.ETLumiErr[iBX] = sqrt(error + ETSumNoiseError[BXCapId_[iBX]]);

	    // *************************************************** //
	    //                   Now do error checks               //
	    // *************************************************** //

	    // Check for zero
	    if( !(TotalEtSum[iBX] > 0) ){
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

	    // Errors
	    // Check for zero
	    if( !(localSection.lumiDetail.ETLumiErr[iBX]> 0) ){
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

	    if( BXMask_[iBX] == kNoise ) continue;

	    // If this is not a noise BX, and the quality is ok,
	    // add it to the total Luminosity for this section.
	    // Note that for the error calculation only the SumEt error^2
	    // is accumulated, noise error has to be added at the end.
	    if( localSection.lumiDetail.ETLumiQlty[iBX] > 0 ){ 
		  localSection.lumiSummary.InstantETLumi += localSection.lumiDetail.ETLumi[iBX];
		  localSection.lumiSummary.InstantETLumiErr += error;
	    }else{
	       --numGoodBX;
	    }
	 }
      } else {
	 localSection.lumiSummary.InstantETLumiQlty -= 201;
      }
      
      if( TotalNibbles > 0 ){
	 if( numGoodBX > 0 ){ 
	    // Calculate the noise error contribution to the total luminosity
	    float nErr2 = 0;
	    for( int iCap=0; iCap<NUM_CAP_BANKS; ++iCap ) nErr2 += ETSumNoiseError[iCap];
	    float s2 = float(numGoodBX*numGoodBX)/nCap2_;
	    nErr2 *= s2;

	    localSection.lumiSummary.InstantETLumiErr = sqrt(localSection.lumiSummary.InstantETLumiErr + nErr2);
	    localSection.lumiSummary.InstantETLumiQlty = static_cast<int16_t>(999*(float)numGoodBX*normNumBX_);
	 } else {
	    localSection.lumiSummary.InstantETLumi = 0;  // error
	    localSection.lumiSummary.InstantETLumiErr = 0; // error
	    localSection.lumiSummary.InstantETLumiQlty -= 701;
	 }
      } 
   }

   void LumiCalc::PrepETSumError( HCAL_HLX::LUMI_SECTION &localSection ){

      if( numBX_ == 0 || numHLX_ == 0 ) return;

      // 1. Calculate the mean for each BX in this event (and the total mean ...)

      // If the store is maxed out, move everything back by 1 and zero the
      // final entry
      if( etStoreSize_ <= ET_SUM_STORE_SIZE ) ++etStoreSize_;
      if( etStoreSize_ > ET_SUM_STORE_SIZE ){
	 --etStoreSize_;
	 //std::cout << "Here in pop back errors! " << etStoreSize_ << std::endl;
	 for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    for( uint32_t iLB = 0; iLB < ET_SUM_STORE_SIZE; ++iLB ){
	       if( iLB < (ET_SUM_STORE_SIZE-1) ) etSumAvgPerLB_[iLB][iBX] = etSumAvgPerLB_[iLB+1][iBX];
	       else                              etSumAvgPerLB_[iLB][iBX] = 0.0;
	    }
	 }
      }
      
      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( BXMask_[iBX] == kMasked ) continue;
	 float nTowerBX = (float)(numActiveTowersBXEt_[iBX]);
	 if( !(nTowerBX > 0) ) continue;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( !HLXMask_[iHLX] ) continue;
	    etSumAvgPerLB_[etStoreSize_-1][iBX] += localSection.etSum[iHLX].data[iBX];
	 }
	 etSumAvgPerLB_[etStoreSize_-1][iBX] /= nTowerBX;
      }

   }


   // Calculate the per BX Et sum error
   void LumiCalc::CalcETSumError( HCAL_HLX::LUMI_SECTION &localSection, LumiCalc::BXType type,
				  float *sigma, float *frac ){

      // Check for the case where we have no data ...
      if( numBX_ == 0 || numHLX_ == 0 ) return;

      if( type == kActive && numBXActive_ == 0 ) return;
      if( type == kInactive && numBXInactive_ == 0 ) return;

      uint32_t localNumBX = numBXActive_;
      if( type == kInactive ) localNumBX = numBXInactive_;

      // 2. Calculate the final error ... for the special case of the first
      // LB take this from the HLX spread, if there are less BX's than HLX's.
      float deviation = 0.0;
      if( etStoreSize_ <= 1 && (localNumBX < numHLX_) ){
	 // Calculate total mean
	 for( uint32_t iCap = 0; iCap<NUM_CAP_BANKS; ++iCap ){
	    deviation = 0.0;
	    float mean = 0.0;
	    for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       if( !HLXMask_[iHLX] ) continue;
	       float tmpMean = 0.0;
	       for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX]  != static_cast<unsigned int>(type) ) continue;
		  if( BXCapId_[iBX] != iCap ) continue; 
		  float nTower = (float)(numActiveTowersEt_[iHLX][iBX]);
		  if( !(nTower > 0) ) continue;
		  tmpMean += localSection.etSum[iHLX].data[iBX]/nTower;
	       }
	       mean += tmpMean;
	    }
	    mean = mean*normNumHLX_;

	    // Calculate deviation ...
	    for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       if( !HLXMask_[iHLX] ) continue; 
	       float totalBX = 0.0;
	       for( int iBX=0; iBX<HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX]  != static_cast<unsigned int>(type) ) continue;
		  if( BXCapId_[iBX] != 0 ) continue; 
		  float nTower = (float)(numActiveTowersEt_[iHLX][iBX]);
		  // if( nTower == 0 ) continue;
		  if( !(nTower > 0) ) continue;
		  totalBX += localSection.etSum[iHLX].data[iBX]/nTower;
	       }
	       totalBX *= 4*normNumBX_;
	       float diff = totalBX - mean;
	       deviation += diff*diff;
	    }    
	    // Return the variance
	    sigma[iCap] = deviation*normNumHLX_*normNumHLX_;
	    frac[iCap] = sqrt(sigma[iCap])/mean;
	 }
      } else{
      // Otherwise use the stored values from previous LB's to get a spread.
	 for( uint32_t iCap = 0; iCap<NUM_CAP_BANKS; ++iCap ){
	    deviation = 0.0;
	    float totalMean = 0.0;
	    float norm = 0;
	    for( uint32_t iLB = 0; iLB < etStoreSize_; ++iLB ){
	       for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX]  != static_cast<unsigned int>(type) ) continue;
		  if( BXCapId_[iBX] != iCap ) continue; 
		  totalMean += etSumAvgPerLB_[iLB][iBX];
		  ++norm; 
	       }
	    }
	    if( norm > 0 ){
	       norm = 1.0/norm;
	       totalMean *= norm;
	    }

	    for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       for( uint32_t iLB = 0; iLB < etStoreSize_; ++iLB ){
		  if( BXMask_[iBX]  != static_cast<unsigned int>(type) ) continue;
		  if( BXCapId_[iBX] != iCap ) continue; 
		  float diff = totalMean - etSumAvgPerLB_[iLB][iBX];
		  deviation += diff*diff;
	       }
	    }

	    sigma[iCap] = deviation*norm;

	    float sumWeight = sigmaEtWeight_[type];
	    if( sumWeight > ET_SUM_STORE_SIZE ){
	       float weight = sumWeight/(sumWeight+(float)ET_SUM_STORE_SIZE);
	       sigma[iCap] = (weight*sigmaEt_[type][iCap] + (1-weight)*sigma[iCap]);
	    }
	    sigmaEt_[type][iCap] = sigma[iCap];
	    frac[iCap] = sqrt(sigma[iCap])/totalMean;
	 }
      }

      ++sigmaEtWeight_[type];
   } 


   // Calculate the total noise error (for all noise BX's in each cap).
   void LumiCalc::CalcETSumNoiseError( HCAL_HLX::LUMI_SECTION &localSection, float *sigma ){

      // Check for the case where we have no data ...
      if( numBX_ == 0 || numHLX_ == 0  ) return;

      // General case 
     
      // 1. Calculate the mean for each BX in this event (and the total mean ...)

      // If the noise store is maxed out, move everything back by 1 and zero the
      // final entry
      if( etNoiseStoreSize_ <= ET_SUM_STORE_SIZE ) ++etNoiseStoreSize_;
      if( etNoiseStoreSize_ > ET_SUM_STORE_SIZE ){
	 --etNoiseStoreSize_;
	 for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] != kNoise ) continue;
	    for( uint32_t iLB = 0; iLB < ET_SUM_STORE_SIZE-1; ++iLB ){
	       etSumNoiseAvgPerLB_[iLB][iBX] = etSumNoiseAvgPerLB_[iLB+1][iBX];
	    }
	    etSumNoiseAvgPerLB_[ET_SUM_STORE_SIZE-1][iBX] = 0.0;
	 }
      }

      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
 	 if( BXMask_[iBX] != kNoise ) continue;
	 if( sizeNoise_[BXCapId_[iBX]] == 0 ) continue;
	 float nTowerBX = (float)(numActiveTowersBXEt_[iBX]);
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( !HLXMask_[iHLX] ) continue;
	    etSumNoiseAvgPerLB_[etNoiseStoreSize_-1][iBX] += localSection.etSum[iHLX].data[iBX];
	 }
	 etSumNoiseAvgPerLB_[etNoiseStoreSize_-1][iBX] /= nTowerBX;
      }


      // 2. Calculate the final error ... for the special case of the first
      // LB take this from the HLX spread, if there are less BX's than HLX's.
      for( uint32_t iCap=0; iCap<NUM_CAP_BANKS; ++iCap ){
	 if( sizeNoise_[iCap] == 0 ) continue;
	 if( (sizeNoise_[iCap] < numHLX_) && etNoiseStoreSize_ <= 1 ){
	    // Get the overall mean ...
	    float mean[4] = {0,0,0,0};
	    float tmpNorm = 0;
	    for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       if( !HLXMask_[iHLX] ) continue;
	       float tmpMean = 0.0;
	       for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
		  if( BXMask_[iBX] != kNoise ) continue;
		  if( sizeNoise_[BXCapId_[iBX]] == 0 ) continue;
		  float nTower = (float)(numActiveTowersEt_[iHLX][BXCapId_[iBX]]);
		  // if( nTower == 0 ) continue;
		  if( !(nTower > 0) ) continue;
		  tmpMean += localSection.etSum[iHLX].data[iBX]/nTower;
		  ++tmpNorm;
	       }
	       mean[iCap] += tmpMean;
	    }
	    mean[iCap] = mean[iCap]/tmpNorm;

	    // Deviations from the mean
	    float deviation = 0.0;
	    for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	       if( !HLXMask_[iHLX] ) continue; 
	       float totalBX = 0.0;
	       for( uint32_t iBX=iCap; iBX<HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
		  if( BXMask_[iBX] != kNoise ) continue;
		  float nTower = (float)(numActiveTowersEt_[iHLX][iBX]);
		  if( !(nTower > 0) ) continue;
		  totalBX += localSection.etSum[iHLX].data[iBX]/nTower;
	       }
	       totalBX /= (float)sizeNoise_[iCap];
	       float diff = totalBX - mean[iCap];
	       deviation += diff*diff;
	    }    
	    sigma[iCap] = deviation*invSizeNoise_[iCap]*normNumHLX_;
	 } else{
	 // Otherwise use the stored values from previous LB's to get a spread.
	    float totalMean = 0.0;
	    float norm = 0;
	    for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( BXMask_[iBX] != kNoise ) continue;
	       if( (BXCapId_[iBX]) != iCap ) continue;
	       for( uint32_t iLB = 0; iLB < etNoiseStoreSize_; ++iLB ){
		  totalMean += etSumNoiseAvgPerLB_[iLB][iBX];
		  ++norm; 
	       }
	    }
	    if( norm > 0 ) totalMean /= norm;

	    float deviation = 0.0;
	    for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	       if( BXMask_[iBX] != kNoise ) continue;
	       if( (BXCapId_[iBX]) != iCap ) continue;
	       for( uint32_t iLB = 0; iLB < etNoiseStoreSize_; ++iLB ){
		  float diff = totalMean - etSumNoiseAvgPerLB_[iLB][iBX];
		  deviation += diff*diff;
	       }
	    }
	    if( norm > 0 ) sigma[iCap] = deviation*invSizeNoise_[iCap]/norm;  
	 }

	 if( sigmaEtNoiseWeight_ > ET_SUM_STORE_SIZE ){
	    float weight = sigmaEtNoiseWeight_/(sigmaEtNoiseWeight_+(float)ET_SUM_STORE_SIZE); 
	    sigma[iCap] = (weight*sigmaEtNoise_[iCap] + (1-weight)*sigma[iCap]);
	 }
	 sigmaEtNoise_[iCap] = sigma[iCap];

      }

      ++sigmaEtNoiseWeight_;
   } 


   // Calculate the occupancy
   void LumiCalc::CalcOccLumi( HCAL_HLX::LUMI_SECTION &localSection ){

      // Calculate the occupancy noise offset per cap id
      double occNoise[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
      double occNoiseTotal[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];

      for( uint32_t iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ){
	 if( sizeNoise_[iCap] == 0 ) continue;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    occNoise[iHLX][0][iCap] = 0;
	    occNoise[iHLX][1][iCap] = 0;
	    if( HLXMask_[iHLX] == 0 ) continue;
	    uint32_t tmpSum[2] = {0,0};
	    uint32_t tmpSumTotal[2] = {0,0};
	    uint32_t tmpNorm = 0;
	    for(uint32_t iBX = iCap ; iBX < HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
	       if( BXMask_[iBX] != kNoise ) continue;
	       tmpSum[0] += localSection.occupancy[iHLX].data[set1BelowIndex][iBX];
	       tmpSum[1] += localSection.occupancy[iHLX].data[set2BelowIndex][iBX];
	       tmpSumTotal[0] += localSection.occupancy[iHLX].data[set1BelowIndex][iBX];
	       tmpSumTotal[1] += localSection.occupancy[iHLX].data[set2BelowIndex][iBX];
	       tmpSumTotal[0] += localSection.occupancy[iHLX].data[set1BelowIndex+1][iBX];
	       tmpSumTotal[1] += localSection.occupancy[iHLX].data[set2BelowIndex+1][iBX];
	       tmpSumTotal[0] += localSection.occupancy[iHLX].data[set1BelowIndex+2][iBX];
	       tmpSumTotal[1] += localSection.occupancy[iHLX].data[set2BelowIndex+2][iBX];
	       ++tmpNorm;
	    }
	    // Average num towers below threshold per tower per BX
	    // occNoise[iHLX][0][iCap] = (double)tmpSum[0]*(double)invSizeNoise_[iCap];
	    // occNoise[iHLX][1][iCap] = (double)tmpSum[1]*(double)invSizeNoise_[iCap];
	    occNoise[iHLX][0][iCap] = (double)tmpSum[0]/(double)tmpNorm;
	    occNoise[iHLX][1][iCap] = (double)tmpSum[1]/(double)tmpNorm;
	    occNoiseTotal[iHLX][0][iCap] = (double)tmpSumTotal[0]/(double)tmpNorm;
	    occNoiseTotal[iHLX][1][iCap] = (double)tmpSumTotal[1]/(double)tmpNorm;
	 }
      }

      // Calculate the noise error ...
      float noiseError[2][NUM_CAP_BANKS];
      CalcOccNoiseError( localSection, 0, noiseError[0] );
      CalcOccNoiseError( localSection, 1, noiseError[1] );

      // Only use good BX's
      uint32_t numGoodBX[2] = {numBXActive_+numBXInactive_+numBXOther_,numBXActive_+numBXInactive_+numBXOther_};

      // Initialize luminosity and error to 0.0
      localSection.lumiSummary.InstantOccLumi[0] = 0.0;
      localSection.lumiSummary.InstantOccLumi[1] = 0.0;
      localSection.lumiSummary.InstantOccLumiErr[0] = 0.0;
      localSection.lumiSummary.InstantOccLumiErr[1] = 0.0;

      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 localSection.lumiDetail.OccLumi[0][iBX] = 0.0;
	 localSection.lumiDetail.OccLumi[1][iBX] = 0.0;
	 localSection.lumiDetail.OccLumiErr[0][iBX] = 0.0;
	 localSection.lumiDetail.OccLumiErr[1][iBX] = 0.0;
	 localSection.lumiDetail.OccLumiQlty[0][iBX] = 0;
	 localSection.lumiDetail.OccLumiQlty[1][iBX] = 0;
	 localSection.lumiDetail.OccBXNormalization[0][iBX] = 1;
	 localSection.lumiDetail.OccBXNormalization[1][iBX] = 1;

	 if( BXMask_[iBX] == kMasked ) continue;
	 localSection.lumiDetail.OccLumiQlty[0][iBX] = 1;
	 localSection.lumiDetail.OccLumiQlty[1][iBX] = 1;

	 uint32_t sumHLX[2] = {0,0};
	 double sumNoiseHLX[2] = {0,0};
	 double totalNoiseHLX[2] = {0,0};
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( !HLXMask_[iHLX] ) continue;
	    
	    // Ring Set 1 & 2
	    sumHLX[0] += localSection.occupancy[iHLX].data[set1BelowIndex][iBX];
	    sumHLX[1] += localSection.occupancy[iHLX].data[set2BelowIndex][iBX];
	
	    // Noise
	    if( sizeNoise_[BXCapId_[iBX]] == 0 ) continue;
	    sumNoiseHLX[0] += occNoise[iHLX][0][BXCapId_[iBX]];
	    sumNoiseHLX[1] += occNoise[iHLX][1][BXCapId_[iBX]];
	    totalNoiseHLX[0] += occNoiseTotal[iHLX][0][BXCapId_[iBX]];
	    totalNoiseHLX[1] += occNoiseTotal[iHLX][1][BXCapId_[iBX]];	
	 }

	 // Ring Set 1
	 if( sumHLX[0] > 0 ){
	    if( sumNoiseHLX[0] > 0 ){
	       if( numActiveTowersBX_[iBX][0] != (uint32_t)totalNoiseHLX[0] ){
	    	  double norm  = (double)numActiveTowersBX_[iBX][0]/totalNoiseHLX[0];
	    	  localSection.lumiDetail.OccLumi[0][iBX] = (float)log( norm * sumNoiseHLX[0] / (double)sumHLX[0] );
	       } else {
	    	  localSection.lumiDetail.OccLumi[0][iBX] = (float)log( sumNoiseHLX[0] / (double)sumHLX[0] );
	       }
	    }
	    else{
	       localSection.lumiDetail.OccLumi[0][iBX] = 
		  (float)log( (double)numActiveTowersBX_[iBX][0] / (double)sumHLX[0] );
	       localSection.lumiDetail.OccLumiQlty[0][iBX] += 99;
	    }
	 }
	 else{
	    localSection.lumiDetail.OccLumiQlty[0][iBX] -= 701;
	 }

	 // Ring Set 2
	 if( sumHLX[1] > 0 ){
	    if( sumNoiseHLX[1] > 0 ){
	       if( numActiveTowersBX_[iBX][1] != (uint32_t)totalNoiseHLX[1] ){
	    	  double norm  = (double)numActiveTowersBX_[iBX][1]/totalNoiseHLX[1];
		  localSection.lumiDetail.OccLumi[1][iBX] = (float)log( norm * sumNoiseHLX[1] / (double)sumHLX[1] );
	       } else {
		  localSection.lumiDetail.OccLumi[1][iBX] = (float)log( sumNoiseHLX[1] / (double)sumHLX[1] );
	       }
	    }
	    else{
	       localSection.lumiDetail.OccLumi[1][iBX] = 
		  (float)log( (double)numActiveTowersBX_[iBX][1] / (double)sumHLX[1] );
	       localSection.lumiDetail.OccLumiQlty[1][iBX] += 99;
	    }
	 }
	 else{
	    localSection.lumiDetail.OccLumiQlty[1][iBX] -= 701;
	 }

	 // Calculate errors
	 float occErrBX2[2];
	 occErrBX2[0] = CalcOccErrorBX( localSection, 0, iBX );
	 occErrBX2[1] = CalcOccErrorBX( localSection, 1, iBX );
	 localSection.lumiDetail.OccLumiErr[0][iBX] = sqrt(occErrBX2[0] + noiseError[0][BXCapId_[iBX]]);
	 localSection.lumiDetail.OccLumiErr[1][iBX] = sqrt(occErrBX2[1] + noiseError[1][BXCapId_[iBX]]);

	 // *************************************************** //
	 //                   Now do error checks               //
	 // *************************************************** //

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

	 if( BXMask_[iBX] == kNoise ) continue; 

	 // If we are the right type of BX, add to the total 
	 // luminosity for the section. Again only accumulate
	 // error from raw data (no noise).
	 if( localSection.lumiDetail.OccLumiQlty[0][iBX] > 0 ){
	    localSection.lumiSummary.InstantOccLumi[0] += localSection.lumiDetail.OccLumi[0][iBX];
	    localSection.lumiSummary.InstantOccLumiErr[0] += occErrBX2[0];
	 } else
	    --numGoodBX[0];
	 if( localSection.lumiDetail.OccLumiQlty[1][iBX] > 0 ){
	    localSection.lumiSummary.InstantOccLumi[1] += localSection.lumiDetail.OccLumi[1][iBX];
	    localSection.lumiSummary.InstantOccLumiErr[1] += occErrBX2[1];
	 } else
	    --numGoodBX[1];

      }

      if( numGoodBX[0] > 0 ){
	 localSection.lumiSummary.InstantOccLumiQlty[0] = static_cast<int16_t>(999*(float)numGoodBX[0]*normNumBX_);
      } else {
	 localSection.lumiSummary.InstantOccLumi[0] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[0] = -900;
      }
      if( numGoodBX[1] > 0 ){
	 localSection.lumiSummary.InstantOccLumiQlty[1] = static_cast<int16_t>(999*(float)numGoodBX[1]*normNumBX_);
      } else {
	 localSection.lumiSummary.InstantOccLumi[1] = 0;
	 localSection.lumiSummary.InstantOccLumiQlty[1] = -900;
      }

      // Sum error over bunch crossings
      float nErr2[2] = {0,0};
      for( int iCap=0; iCap<NUM_CAP_BANKS; ++iCap ){
	 nErr2[0] += noiseError[0][iCap];
	 nErr2[1] += noiseError[1][iCap];
      }
      float s2[2];
      s2[0] = float(numGoodBX[0]*numGoodBX[0])/nCap2_;
      s2[1] = float(numGoodBX[1]*numGoodBX[1])/nCap2_;
      nErr2[0] *= s2[0];
      nErr2[1] *= s2[1];

      // Calcualate total error
      localSection.lumiSummary.InstantOccLumiErr[0] = sqrt(localSection.lumiSummary.InstantOccLumiErr[0] + nErr2[0]);
      localSection.lumiSummary.InstantOccLumiErr[1] = sqrt(localSection.lumiSummary.InstantOccLumiErr[1] + nErr2[1]);

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

      // Normalization ... ???
      localSection.lumiSummary.OccNormalization[0] = 1;
      localSection.lumiSummary.OccNormalization[1] = 1;

   }

   void LumiCalc::CalcOccNoiseError( HCAL_HLX::LUMI_SECTION &localSection, uint32_t set, float *sigma ){

      uint32_t index = set1BelowIndex;
      if( set == 1 ) index = set2BelowIndex;

      // But we also need the noise error
      uint64_t nBelowN[NUM_CAP_BANKS] = {0,0,0,0};
      uint64_t nTotalN[NUM_CAP_BANKS] = {0,0,0,0};
      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( !HLXMask_[iHLX] ) continue;
	 for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	    if( BXMask_[iBX] != kNoise ) continue;
	    nBelowN[BXCapId_[iBX]] += localSection.occupancy[iHLX].data[index][iBX];
	    nTotalN[BXCapId_[iBX]] += localSection.occupancy[iHLX].data[index][iBX];
	    nTotalN[BXCapId_[iBX]] += localSection.occupancy[iHLX].data[index+1][iBX];
	    nTotalN[BXCapId_[iBX]] += localSection.occupancy[iHLX].data[index+2][iBX];
	 }
      }


      if( nTotalN > 0 ){
	 for( uint32_t iCap=0; iCap < NUM_CAP_BANKS; ++iCap ){
	    if( sizeNoise_[iCap] == 0 ) continue;
	    if( nBelowN[iCap] <= 0 ) continue;
	    if( nBelowN[iCap] > nTotalN[iCap] ) continue;
	    sigma[iCap] = (float)(1.0/(double)nBelowN[iCap] - 1.0/(double)nTotalN[iCap]);
	    //std::cout << "sigma[" << iCap << "] = " << sqrt(sigma[iCap]) << std::endl;
	 }
      }
   }
  
   float LumiCalc::CalcOccErrorTotal( HCAL_HLX::LUMI_SECTION &localSection, uint32_t set ){
    
      // Index
      uint32_t index = set1BelowIndex;
      if( set == 1 ) index = set2BelowIndex;
      
      // For occupancy we can just calculate the accumulated binomial error
      uint64_t nBelow = 0;
      uint64_t nTotal = 0;
      for( uint32_t iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	 if( !(BXMask_[iBX] == kActive || BXMask_[iBX] == kInactive) ) continue;
	 for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	    if( HLXMask_[iHLX] == 0 ) continue;
	    nBelow += localSection.occupancy[iHLX].data[index][iBX];
	    nTotal += localSection.occupancy[iHLX].data[index][iBX];
	    nTotal += localSection.occupancy[iHLX].data[index+1][iBX];
	    nTotal += localSection.occupancy[iHLX].data[index+2][iBX];
	 }
      }

      float sigma = 0.0;
      if( nTotal > 0 && nBelow > 0){
	 sigma = (float)(1.0/(double)nBelow - 1.0/(double)nTotal);
      }

      // Return variance (i.e., actually returning sigma^2 rather than sigma)
      return sigma;
   }

   float LumiCalc::CalcOccErrorBX( HCAL_HLX::LUMI_SECTION &localSection, uint32_t set, uint32_t iBX ){
    
      // Check for the case where we have no data ...
      if( numBX_ == 0 || numHLX_ == 0 ) return 0;
      if( BXMask_[iBX] == kMasked ) return 0;

      // Index
      int index = set1BelowIndex;
      if( set == 1 ) index = set2BelowIndex;

      // For occupancy we can just calculate the accumulated binomial error
      uint32_t nBelow = 0;
      //uint32_t nTotal = numActiveTowersBX_[set];
      uint32_t nTotal = 0;
      for( uint32_t iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	 if( !HLXMask_[iHLX] ) continue;
	 nBelow += localSection.occupancy[iHLX].data[index][iBX];
	 nTotal += localSection.occupancy[iHLX].data[index][iBX];
	 nTotal += localSection.occupancy[iHLX].data[index+1][iBX];
	 nTotal += localSection.occupancy[iHLX].data[index+2][iBX];
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

   float LumiCalc::CalcLumiError( uint32_t, uint32_t, uint32_t, float )
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
