#ifndef __LUMI_CALC_HH__
#define __LUMI_CALC_HH__

/*****************************************************************************

Authors:  Adam Hunt, Nadia Adam
          ahunt@princeton.edu, neadam@princeton.edu


Summary:  Library to calculate relative luminosity from HLX data


******************************************************************************
  Convension used to calculate Luminosity:
  
  occupancy[HLX].data[0][BX]  = occupancy above threshold set 1
  occupancy[HLX].data[1][BX]  = occupancy between threshold set 1
  occupancy[HLX].data[2][BX]  = occupancy below threshold set 1
  occupancy[HLX].data[3][BX]  = occupancy above threshold set 2
  occupancy[HLX].data[4][BX]  = occupancy between thresholds set 2
  occupancy[HLX].data[5][BX]  = occupancy below threshold set 2

******************************************************************************/

#include <stdint.h> // uint32_t, etc.

#define set1BelowIndex   0
#define set1BetweenIndex 1
#define set1AboveIndex   2
#define set2BelowIndex   3
#define set2BetweenIndex 4
#define set2AboveIndex   5

#define HCAL_HLX_NUM_BUNCHES 3564
#define HCAL_HLX_NUM_HLXS 36
#define NUM_CAP_BANKS 4
#define ET_SUM_STORE_SIZE 20

namespace HCAL_HLX {

   struct LUMI_SECTION;

   class LumiCalc {
   public:

      //LumiCalc();
      LumiCalc( );
      ~LumiCalc() {}

      // enum for BX types
      enum BXType {
	 kActive,
	 kInactive,
	 kNoise,
	 kMasked,
	 kOther,
	 kNumTypes
      };

      // Do the calculations
      void DoCalc(HCAL_HLX::LUMI_SECTION & localSection);

      // configuration
      uint32_t SetBXMask( uint32_t BXMask[], uint32_t maskSize = HCAL_HLX_NUM_BUNCHES );
      uint32_t SetHLXMask( uint32_t HLXMask[], uint32_t maskSize = HCAL_HLX_NUM_HLXS );
    
   private:

      void CountActiveTowers(HCAL_HLX::LUMI_SECTION& localSection);
      void CountActiveTowersEt(HCAL_HLX::LUMI_SECTION& localSection);
      void CalcETSumLumi(HCAL_HLX::LUMI_SECTION& localSection);
      void PrepETSumError( HCAL_HLX::LUMI_SECTION &localSection );
      void CalcETSumError( HCAL_HLX::LUMI_SECTION &localSection, BXType type, float *error, float *frac );
      void CalcETSumNoiseError( HCAL_HLX::LUMI_SECTION &localSection, float *error );
      void CalcLHCLumi(HCAL_HLX::LUMI_SECTION& localSection);
      void CalcOccLumi(HCAL_HLX::LUMI_SECTION& localSection);
      void CalcOccNoiseError (HCAL_HLX::LUMI_SECTION &localSection, uint32_t set, float *error ); 
      //float CalcOccNoiseError (HCAL_HLX::LUMI_SECTION &localSection, uint32_t set ); 
      float CalcOccErrorTotal (HCAL_HLX::LUMI_SECTION &localSection, uint32_t set ); 
      float CalcOccErrorBX (HCAL_HLX::LUMI_SECTION &localSection, uint32_t set, uint32_t iBX ); 

      void CalcLumi( HCAL_HLX::LUMI_SECTION& localSection );
      float CalcLumiError( uint32_t numNibbles, uint32_t numTowers, uint32_t numBunches = 1, float intPerBX = 0.01 );

      // Configuration
      //uint32_t startAG_, endAG_, sizeAG_; // AG = Abort Gap.
      uint32_t sizeNoise_[NUM_CAP_BANKS];
      float        invSizeNoise_[NUM_CAP_BANKS];
      float totalEtSumNoise_[NUM_CAP_BANKS];
      uint32_t etSumNoiseNorm_[NUM_CAP_BANKS];
      float totalOccNoise_[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
      uint32_t occNoiseNorm_[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
      uint32_t numBX_, numBXActive_, numBXInactive_, numBXOther_, numHLX_; 
      float normNumBX_, normNumBXActive_, normNumBXInactive_, normNumHLX_;

      // Should only be called once per run
      uint32_t numActiveTowers_[HCAL_HLX_NUM_HLXS][HCAL_HLX_NUM_BUNCHES][2];
      uint32_t numActiveTowersBX_[HCAL_HLX_NUM_BUNCHES][2];
      //uint32_t numActiveTowers_[HCAL_HLX_NUM_HLXS][2];
      //uint32_t numActiveTowersBX_[2];
      uint32_t TotalActiveTowers_;

      bool         noMasks_;
      uint32_t numOrbitsPerNB_;
      // uint32_t numActiveTowersEt_[HCAL_HLX_NUM_HLXS];
      // uint32_t numActiveTowersBXEt_;
      uint32_t numActiveTowersEt_[HCAL_HLX_NUM_HLXS][HCAL_HLX_NUM_BUNCHES];
      uint32_t numActiveTowersBXEt_[HCAL_HLX_NUM_BUNCHES];
      uint32_t TotalActiveTowersEt_;
      float    avNumActiveTowersEtNoise_[NUM_CAP_BANKS];
      float    nCap2_;

      bool Block( unsigned short int iBX, unsigned short int numBlock);
      bool isBunch( unsigned short int iBX, unsigned short int scheme = 1 );  

      uint32_t iBXNoise_;
      uint32_t BXCapId_[HCAL_HLX_NUM_BUNCHES];
      uint32_t BXMask_[HCAL_HLX_NUM_BUNCHES];
      uint32_t HLXMask_[HCAL_HLX_NUM_HLXS];

      // Make an array to store the previous averages, this way we can calculate 
      // an estimate of the error as we go. We store an average for each four lumi
      // nibbles (a lumi "bite")
      float etSumAvgPerLB_[ET_SUM_STORE_SIZE][HCAL_HLX_NUM_BUNCHES];
      float etSumNoiseAvgPerLB_[ET_SUM_STORE_SIZE][HCAL_HLX_NUM_BUNCHES];
      uint32_t etStoreSize_;
      uint32_t etNoiseStoreSize_;
      float sigmaEtBX_[HCAL_HLX_NUM_BUNCHES];
      float sigmaEtWeightBX_[HCAL_HLX_NUM_BUNCHES];
      float sigmaEt_[kNumTypes][NUM_CAP_BANKS];
      float sigmaEtWeight_[kNumTypes];
      float sigmaEtNoise_[NUM_CAP_BANKS];
      float sigmaEtNoiseWeight_;
   };
}
  
#endif
