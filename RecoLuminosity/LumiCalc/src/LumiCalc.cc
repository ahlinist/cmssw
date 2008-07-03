/****************************************************************************

Author:  Adam Hunt
         ahunt@princeton.edu

Summary:  Library to calculate relative luminosity from HLX data

*****************************************************************************/

#include "RecoLuminosity/LumiCalc/interface/LumiCalc.hh"
#define NUM_CAP_BANKS 4

namespace HCAL_HLX {
  
  LumiCalc::LumiCalc(unsigned int start,
		     unsigned int end, 
		     char scheme){
    
    set1BelowIndex   = 0;
    set1BetweenIndex = 1;
    set1AboveIndex   = 2;
    set2BelowIndex   = 3;
    set2BetweenIndex = 4;
    set2AboveIndex   = 5;
    
    SetNoiseInterval(start, end);
    
    std::vector<unsigned int> BXMask;
    std::vector<unsigned int> HLXMask;
    // Should these be combined to form a matrix so that different BXs are masked for different HLXs?

    for( unsigned int iBX = 0;  iBX < HCAL_HLX_NUM_BUNCHES; ++iBX )  BXMask.push_back(1);
    for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS;  ++iHLX ) HLXMask.push_back(1);
    
    SetBXMask( BXMask );
    SetHLXMask( HLXMask );

  }
  
  //****************** Initialization and Configuration Functions ***************
    
  void LumiCalc::SetNoiseInterval(unsigned int start, unsigned int end){
    // Should this be in the BXMask? The BXs used to determine noise could be part of the mask.

    if(end < start){  // start and end swapped
      unsigned int temp = end;
      end = start;
      start = temp;
    }

    //upper bound
    if( end > HCAL_HLX_NUM_BUNCHES - 1 ) 
      end = HCAL_HLX_NUM_BUNCHES - 1; 
    if( start > HCAL_HLX_NUM_BUNCHES - NUM_CAP_BANKS )
      start = HCAL_HLX_NUM_BUNCHES - NUM_CAP_BANKS; 
    
    // check for minimum interval
    // Should we allow for no noise interval? 
    if( (end - start ) < NUM_CAP_BANKS + 1 )
      end = start + NUM_CAP_BANKS - 1;
    
    // Reduce the interval until it is divisible by NUM_CAP_BANKS
    while( (end - start + 1) % NUM_CAP_BANKS > 0){ 
      end --;
      if( (end - start + 1 ) % NUM_CAP_BANKS > 0 ) 
	start ++; 
    }

    if( (end - start + 1) % NUM_CAP_BANKS == 0){ 
      endAG_ = end;
      startAG_ = start;
      
      sizeAG_ = endAG_ - startAG_ + 1; 
    } 

  }
  
  unsigned int LumiCalc::SetBXMask( const std::vector< unsigned int > &BXMask ){
    // Copy given mask into member variable and count number of active bunches

    const unsigned int maskSize = BXMask.size();
    BXMask_.resize(3564);

    numBX_ = 0;

    for( unsigned int iBX = 0; iBX < maskSize ; ++iBX ){
      BXMask_[iBX] = ( BXMask[iBX] == 0 )? 0:1;
      numBX_ += BXMask_[iBX];
    }

    for( unsigned int iBX = maskSize; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX )
      BXMask_[iBX] = 0;
    
    return numBX_;
  }

  unsigned int LumiCalc::SetHLXMask( const std::vector<unsigned int> &HLXMask ){
    // Copy given mask into member variable and count number of active HLXs

    const unsigned int maskSize = HLXMask.size();
    HLXMask_.resize(3564);
    numHLX_ = 0;

    for( unsigned int iHLX = 0; iHLX < maskSize ; ++iHLX ){
      HLXMask_[iHLX] = ( HLXMask[iHLX] == 0 )? 0:1;
      numHLX_ += HLXMask_[iHLX];
    }      

    for( unsigned int iHLX = maskSize; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX )
      HLXMask_[iHLX] = 0;
   
    return numHLX_ = 0;
  }

  //****************** end Initialization and Configuration Functions ***********

  void LumiCalc::CountActiveTowers( HCAL_HLX::LUMI_SECTION &localSection ){
    // Should only be calculated once per run.
    
    TotalActiveTowers_ = 0;

    //find first unmasked BX
    unsigned int iBX;
    for( iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
      if( BXMask_[iBX] != 0 )
	break;
    }
 
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
      TotalEtSumNoise[iCap] = 0.0;
      for(unsigned int iBX = startAG_ + ((iCap - startAG_ ) % NUM_CAP_BANKS) ; iBX < (endAG_ + 1); iBX += NUM_CAP_BANKS ){
	TotalEtSumNoise[iCap] += TotalEtSum[iBX];
      }
      TotalEtSumNoise[iCap] /= ((float)sizeAG_/4.0);
    }

    for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
      for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
	TotalEtSum[iBX] += localSection.etSum[iHLX].data[iBX];
      }
      localSection.lumiDetail.ETLumi[iBX]
	= TotalEtSum[iBX] - TotalEtSumNoise[iBX % 4];
      localSection.lumiDetail.ETLumiErr[iBX] = ETSumError;
    }

    if( numBX_ ){ // TODO: check for TotalNibbles = 0. This will be a big problem.
      localSection.lumiSummary.InstantETLumi = 0.0;
      for( unsigned int iBX = 0; iBX < HCAL_HLX_NUM_BUNCHES; ++iBX ){
	localSection.lumiSummary.InstantETLumi += localSection.lumiDetail.ETLumiErr[iBX]*BXMask_[iBX];
      }
      localSection.lumiSummary.InstantETLumi /= numBX_;
      localSection.lumiSummary.InstantETLumiErr = CalcETSumError( TotalNibbles, TotalActiveTowers_, numBX_ );
    } else {
      localSection.lumiSummary.InstantETLumi = 0.0;  // error
      localSection.lumiSummary.InstantETLumiErr = 0.0; // error
    }
  }

  float LumiCalc::CalcETSumError( unsigned int numNibbles, unsigned int numTowers, unsigned int numBunches, float intPerBX ){
    
    if( numNibbles && numTowers && intPerBX){
      return 1.0/sqrt(numNibbles*numTowers)*(1 + 16/intPerBX);
    } 
    return 0.0; // error
  } 

  void LumiCalc::CalcOccLumi( HCAL_HLX::LUMI_SECTION &localSection ){

    float occNoise[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
    // float occLumi[HCAL_HLX_NUM_HLXS][HCAL_HLX_NUM_BUNCHES];
    
    // Calculate noise per cap bank
    for( unsigned int iHLX = 0; iHLX < HCAL_HLX_NUM_HLXS; ++iHLX ){
      for( unsigned int iCap = 0; iCap < NUM_CAP_BANKS; ++iCap ){
	occNoise[iHLX][0][iCap] = 0.0;
	occNoise[iHLX][1][iCap] = 0.0;
	for(unsigned int iBX = startAG_ + ((iCap - startAG_ ) % NUM_CAP_BANKS) ; iBX < (endAG_ + 1); iBX += NUM_CAP_BANKS ){
	  occNoise[iHLX][0][iCap] += localSection.occupancy[iHLX].data[set1BelowIndex][iBX];
	  occNoise[iHLX][1][iCap] += localSection.occupancy[iHLX].data[set2BelowIndex][iBX];
	}
	occNoise[iHLX][0][iCap] /= ((float)sizeAG_/4.0);
	occNoise[iHLX][1][iCap] /= ((float)sizeAG_/4.0);
      }
    }
    
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
	  //Should we not use that HLX? No empty towers means infinite lumi?
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

      }
      // normailze to number of active HLXs
      localSection.lumiDetail.OccLumi[0][iBX] /= (float)numHLX_;
      localSection.lumiDetail.OccLumi[1][iBX] /= (float)numHLX_;
      
      localSection.lumiDetail.OccLumiErr[0][iBX] = sqrt( localSection.lumiDetail.OccLumiErr[0][iBX] );
      localSection.lumiDetail.OccLumiErr[1][iBX] = sqrt( localSection.lumiDetail.OccLumiErr[1][iBX] );
      // Sum over bunch crossings
      localSection.lumiSummary.InstantOccLumi[0] += localSection.lumiDetail.OccLumi[0][iBX]*BXMask_[iBX];
      localSection.lumiSummary.InstantOccLumi[1] += localSection.lumiDetail.OccLumi[1][iBX]*BXMask_[iBX];
    }
    // normailze to the number of active bunches
    localSection.lumiSummary.InstantOccLumi[0] /= (float)numBX_;
    localSection.lumiSummary.InstantOccLumi[1] /= (float)numBX_;
    
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
    
    if( numNibbles && numBunches && numTowers && intPerBX){
      return  1.0/sqrt(numNibbles*numTowers)*20*sqrt(1/(exp(-0.05*intPerBX-0.66))-1)/intPerBX/sqrt(numBunches); 
    }
    return 0.0; // error
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
