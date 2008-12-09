/*****************************************************************************

Authors:  Adam Hunt 
          ahunt@princeton.edu


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

#include <cmath>

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

   class LumiCalc {
      public:

	 //LumiCalc();
	 LumiCalc( );
	 ~LumiCalc() {}

	 // Do the calculations
	 void DoCalc(HCAL_HLX::LUMI_SECTION & localSection);

	 // configuration
	 uint32_t SetBXMask( uint32_t BXMask[], uint32_t maskSize = HCAL_HLX_NUM_BUNCHES );
	 uint32_t SetHLXMask( uint32_t HLXMask[], uint32_t maskSize = HCAL_HLX_NUM_HLXS );
    
      private:

	 void CountActiveTowers(HCAL_HLX::LUMI_SECTION& localSection);
	 void CountActiveTowersEt(HCAL_HLX::LUMI_SECTION& localSection);
	 void CalcETSumLumi(HCAL_HLX::LUMI_SECTION& localSection);
	 float CalcETSumError( HCAL_HLX::LUMI_SECTION &localSection );
	 float CalcETSumNoiseError( HCAL_HLX::LUMI_SECTION &localSection );
	 void CalcLHCLumi(HCAL_HLX::LUMI_SECTION& localSection);
	 void CalcOccLumi(HCAL_HLX::LUMI_SECTION& localSection);
	 float CalcOccNoiseError( HCAL_HLX::LUMI_SECTION &localSection, uint32_t set ); 
	 float CalcOccErrorTotal( HCAL_HLX::LUMI_SECTION &localSection, uint32_t set ); 
	 float CalcOccErrorBX( HCAL_HLX::LUMI_SECTION &localSection, uint32_t set, uint32_t iBX ); 

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
	 uint32_t numBX_, numHLX_; 
	 float normNumBX_, normNumHLX_;

	 // Should only be called once per run
	 uint32_t numActiveTowers_[HCAL_HLX_NUM_HLXS][2];
	 uint32_t numActiveTowersBX_[2];
	 uint32_t TotalActiveTowers_;

	 bool         noMasks_;
	 uint32_t numOrbitsPerNB_;
	 uint32_t numActiveTowersEt_[HCAL_HLX_NUM_HLXS];
	 uint32_t numActiveTowersBXEt_;
	 uint32_t TotalActiveTowersEt_;


	 bool Block( unsigned short int iBX, unsigned short int numBlock);
	 bool isBunch( unsigned short int iBX, unsigned short int scheme = 1 );  

	 uint32_t BXCapId_[HCAL_HLX_NUM_BUNCHES];
	 uint32_t BXMask_[HCAL_HLX_NUM_BUNCHES];
	 uint32_t BXMaskBool_[HCAL_HLX_NUM_BUNCHES];
	 uint32_t HLXMask_[HCAL_HLX_NUM_HLXS];

	 // Make an array to store the previous averages, this way we can calculate 
	 // an estimate of the error as we go. We store an average for each four lumi
	 // nibbles (a lumi "bite")
	 float etSumAvgPerLB_[ET_SUM_STORE_SIZE][HCAL_HLX_NUM_BUNCHES];
	 float etSumNoiseAvgPerLB_[ET_SUM_STORE_SIZE][HCAL_HLX_NUM_BUNCHES];
	 uint32_t etStoreSize_;
	 uint32_t etNoiseStoreSize_;
	 float sigmaEt_;
	 float sigmaEtWeight_;
	 float sigmaEtNoise_;
	 float sigmaEtNoiseWeight_;
   };
}
  
