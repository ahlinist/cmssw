/****************************************************************************

Author:  Adam Hunt
         ahunt@princeton.edu

Summary:  Library to calculate relative luminosity from HLX data

*****************************************************************************/

#include "RecoLuminosity/LumiCalc/interface/LumiCalc.hh"
#include <iostream>
#define NUM_CAP_BANKS 4

namespace HCAL_HLX {

  using std::cout;
  using std::endl;
  
  LumiCalc::LumiCalc(){
    
    std::vector< unsigned int > BXMask(HCAL_HLX_NUM_BUNCHES);
    std::vector< unsigned int > HLXMask(HCAL_HLX_NUM_HLXS);
    // Should these be combined to form a matrix so that different BXs are masked for different HLXs?

    // Masking convension: Masked 0, Data 1, Noise 2.  Anything marked as 2 will be used to calculate noise.

    for( unsigned int iBX = 0;  iBX < 120; ++iBX )                    BXMask[iBX] = 0; // Mask
    for( unsigned int iBX = 120;  iBX < 3400; ++iBX )                 BXMask[iBX] = 1; // UnMask
    for( unsigned int iBX = 3400; iBX < 3524; ++iBX )                 BXMask[iBX] = 2; // Noise
    for( unsigned int iBX = 3524; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ) BXMask[iBX] = 0; // Mask
    SetBXMask( BXMask );

    // Turn all HLXs on
    for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS;  ++iHLX ) HLXMask[iHLX] = 1;
    SetHLXMask( HLXMask );
  }
  
  //****************** Initialization and Configuration Functions ***************
      
  unsigned int LumiCalc::SetBXMask( const std::vector< unsigned int > &BXMask ){
    // Copy given mask into member variable and count number of active bunches

    const unsigned int maskSize = BXMask.size();
    BXMask_.resize( HCAL_HLX_NUM_BUNCHES );

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

  unsigned int LumiCalc::SetHLXMask( const std::vector<unsigned int> &HLXMask ){
    // Copy given mask into member variable and count number of active HLXs

    const unsigned int maskSize = HLXMask.size();
    HLXMask_.resize( HCAL_HLX_NUM_HLXS );
    numHLX_ = 0;

    for( unsigned int iHLX = 0; iHLX < maskSize ; ++iHLX ){
      HLXMask_[iHLX] = HLXMask[iHLX];
      if( HLXMask_[iHLX] == 1 ) 
	++numHLX_;
    }      

    for( unsigned int iHLX = maskSize; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX )
      HLXMask_[iHLX] = 0;

    return numHLX_;
  }

  //****************** end Initialization and Configuration Functions ***********

  void LumiCalc::CountActiveTowers( HCAL_HLX::LUMI_SECTION &localSection ){
    // Should only be calculated once per run.
    // Assumes all BXs have the same number of active towers

    TotalActiveTowers_ = 0;
    
    //find first unmasked BX
    unsigned int firstBX = 0;
    
    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
      if (BXMask_[iBX] != 0){
	firstBX = iBX;
	break;
      }
    }

    if( firstBX != HCAL_HLX_NUM_BUNCHES ){
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	if( HLXMask_[iHLX] == 1 ){
	  numActiveTowers_[iHLX][0] =  localSection.occupancy[iHLX].data[set1AboveIndex][firstBX] +
	    localSection.occupancy[iHLX].data[set1BetweenIndex][firstBX] +
	    localSection.occupancy[iHLX].data[set1BelowIndex][firstBX] ;
	  
	  numActiveTowers_[iHLX][1] = localSection.occupancy[iHLX].data[set2AboveIndex][firstBX] +
	    localSection.occupancy[iHLX].data[set2BetweenIndex][firstBX] +
	    localSection.occupancy[iHLX].data[set2BelowIndex][firstBX];

	} else {
	  numActiveTowers_[iHLX][0] = 0;
	  numActiveTowers_[iHLX][1] = 0;
	}
	TotalActiveTowers_ += (numActiveTowers_[iHLX][0] + numActiveTowers_[iHLX][1]);
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
    
    const float etSumToADC = 50221648; // 2**20 ( number of orbits ) * 24( number of towers ) * 2 (number of channels)
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
    float ETSumError = CalcETSumError( TotalNibbles, TotalActiveTowers_ ); 

    // Sum over HLXs
    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
      TotalEtSum[iBX] = 0.0;
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	TotalEtSum[iBX] += localSection.etSum[iHLX].data[iBX];
      }
    }
    
    // Calculate noise per cap bank    
    for( unsigned int iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ){
      TotalEtSumNoise[iCap] = 0;
    
      for( unsigned int iBX = iCap; iBX < HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
	if( BXMask_[iBX] == 2 )
	  TotalEtSumNoise[iCap] += TotalEtSum[iBX];
      }
      TotalEtSumNoise[iCap] /= (float)sizeNoise_[iCap];
    }

    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
      // Subtract noise
      localSection.lumiDetail.ETLumi[iBX]
	= (TotalEtSum[iBX] - TotalEtSumNoise[iBX % 4]) / etSumToADC;
      localSection.lumiDetail.ETLumiErr[iBX] = ETSumError;
    }

    if( TotalNibbles > 0 ){
      if( numBX_ > 0 ){ 
	localSection.lumiSummary.InstantETLumi = 0.0;
	for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	  if( BXMask_[iBX] == 1 ){
	    localSection.lumiSummary.InstantETLumi += localSection.lumiDetail.ETLumi[iBX];
	  }
	}
	localSection.lumiSummary.InstantETLumi /= (float)numBX_;
	localSection.lumiSummary.InstantETLumiErr = CalcETSumError( TotalNibbles, TotalActiveTowers_ , numBX_); 
      } else {
	localSection.lumiSummary.InstantETLumi = -994;  // error
	localSection.lumiSummary.InstantETLumiErr = -994; // error
      }
    } else {
      localSection.lumiSummary.InstantETLumi = -995; // error
      localSection.lumiSummary.InstantETLumiErr = -995; // error
    }

    if( isinf( localSection.lumiSummary.InstantETLumi ) )
      localSection.lumiSummary.InstantETLumi = -996;
    
    if( isnan( localSection.lumiSummary.InstantETLumi ) )
      localSection.lumiSummary.InstantETLumi = -997;

  }

  float LumiCalc::CalcETSumError( unsigned int numNibbles, unsigned int numTowers, unsigned int numBunches, float intPerBX ){

    float error = 0.0;
    
    if( (numNibbles > 0 ) && ( numTowers > 0 )&& ( intPerBX > 0)){
      error =  1.0/sqrt(numNibbles*numTowers)*(1 + 16/intPerBX);
    } else {
      error = -995;
    }

    if( isinf(error) ) error = -996;
    if( isnan(error) ) error = -997;
    
    return error;
  } 

  void LumiCalc::CalcOccLumi( HCAL_HLX::LUMI_SECTION &localSection ){

    float occNoise[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];

    // Calculate noise per cap bank
    for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
      for( unsigned int iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ){
	occNoise[iHLX][0][iCap] = 0.0;
	occNoise[iHLX][1][iCap] = 0.0;
	for(unsigned int iBX = iCap ; iBX < HCAL_HLX_NUM_BUNCHES; iBX += NUM_CAP_BANKS ){
	  occNoise[iHLX][0][iCap] += localSection.occupancy[iHLX].data[set1BelowIndex][iBX];
	  occNoise[iHLX][1][iCap] += localSection.occupancy[iHLX].data[set2BelowIndex][iBX];
	}
	occNoise[iHLX][0][iCap] /= (float)sizeNoise_[iCap];
	occNoise[iHLX][1][iCap] /= (float)sizeNoise_[iCap];
      }
      cout << "occNoise Set 1: " << occNoise[iHLX][0][0] << ":" << occNoise[iHLX][0][1] << ":" << occNoise[iHLX][0][2] << ":" << occNoise[iHLX][0][3] << endl;
      cout << "occNoise Set 2: " << occNoise[iHLX][1][0] << ":" << occNoise[iHLX][1][1] << ":" << occNoise[iHLX][1][2] << ":" << occNoise[iHLX][1][3] << endl;
    }
    
    localSection.lumiSummary.InstantOccLumi[0] = 0.0;
    localSection.lumiSummary.InstantOccLumi[1] = 0.0;

    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
      // Ring Set 1
      localSection.lumiDetail.OccLumi[0][iBX] = 0.0;
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
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
	  // Should we not use that HLX? No empty towers means infinite lumi?
	}
	
	// Ring Set 2
	localSection.lumiDetail.OccLumi[1][iBX] = 0.0;
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
	}
	// TO DO: Take into account different numbers of active towers in HLX.
	// Check for dropped nibbles.

      } // End Loop over HLXs

      // normailze to number of active HLXs
      localSection.lumiDetail.OccLumi[0][iBX] /= (float)numHLX_;
      localSection.lumiDetail.OccLumi[1][iBX] /= (float)numHLX_;

      // Not Finished yet
      localSection.lumiDetail.OccLumiErr[0][iBX] = sqrt( localSection.lumiDetail.OccLumiErr[0][iBX] );
      localSection.lumiDetail.OccLumiErr[1][iBX] = sqrt( localSection.lumiDetail.OccLumiErr[1][iBX] );

      // Sum over bunch crossings
      if( BXMask_[iBX] == 1){
	localSection.lumiSummary.InstantOccLumi[0] += localSection.lumiDetail.OccLumi[0][iBX];
	localSection.lumiSummary.InstantOccLumi[1] += localSection.lumiDetail.OccLumi[1][iBX];
      }
    }
    
    // normailze to the number of active bunches
    localSection.lumiSummary.InstantOccLumi[0] /= (float)numBX_;
    localSection.lumiSummary.InstantOccLumi[1] /= (float)numBX_;
    
    cout << "OccLumi: " << localSection.lumiSummary.InstantOccLumi[0] << ":" << localSection.lumiSummary.InstantOccLumi[1] << endl;

    if( isinf( localSection.lumiSummary.InstantOccLumi[0] ) ) localSection.lumiSummary.InstantOccLumi[0] = -996;
    if( isnan( localSection.lumiSummary.InstantOccLumi[0] ) ) localSection.lumiSummary.InstantOccLumi[0] = -997;

    if( isinf( localSection.lumiSummary.InstantOccLumi[1] ) ) localSection.lumiSummary.InstantOccLumi[1] = -996;
    if( isnan( localSection.lumiSummary.InstantOccLumi[1] ) ) localSection.lumiSummary.InstantOccLumi[1] = -997;

    // TODO: finish
    localSection.lumiSummary.InstantOccLumiErr[0] = 0.0;
    localSection.lumiSummary.InstantOccLumiErr[1] = 0.0;

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
  
  float LumiCalc::CalcOccError( unsigned int numNibbles, unsigned int numTowers, unsigned int numBunches, float intPerBX ){
    
    float error;

    if( numNibbles && numBunches && numTowers && intPerBX){
      error =  1.0/sqrt(numNibbles*numTowers)*20*sqrt(1/(exp(-0.05*intPerBX-0.66))-1)/intPerBX/sqrt(numBunches); 
    } else {
      error = -995;
    }

    if( isinf(error) ) error = -996;
    if( isnan(error) ) error = -997;

    return error; // error
  }


  void LumiCalc::CalcLumi( HCAL_HLX::LUMI_SECTION &localSection ){

    localSection.lumiSummary.InstantLumi = localSection.lumiSummary.InstantETLumi;

    localSection.lumiSummary.InstantLumiErr = localSection.lumiSummary.InstantETLumiErr;

    localSection.lumiSummary.InstantLumiQlty = 1;
  }

  float LumiCalc::CalcLumiError( unsigned int numNibbles, unsigned int numTowers, unsigned int numBunches, float intPerBX ){
    
    // Write method for total error
    return 1;
  }

  void LumiCalc::DoCalc(HCAL_HLX::LUMI_SECTION &localSection){

    // Should be called once per run
    CountActiveTowers( localSection );

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
