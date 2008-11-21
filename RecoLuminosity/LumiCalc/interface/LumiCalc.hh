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

using namespace ICCoreUtils;

namespace HCAL_HLX {

   class LumiCalc {
      public:

	 typedef unsigned long  u32; // Unsigned 32-bit integer.

	 //LumiCalc();
	 LumiCalc( );
	 ~LumiCalc() {}

	 // Do the calculations
	 void DoCalc(HCAL_HLX::LUMI_SECTION & localSection);

	 // configuration
	 u32 SetBXMask( u32 BXMask[], u32 maskSize = HCAL_HLX_NUM_BUNCHES );
	 u32 SetHLXMask( u32 HLXMask[], u32 maskSize = HCAL_HLX_NUM_HLXS );
    
      private:

	 void CountActiveTowers(HCAL_HLX::LUMI_SECTION& localSection);
	 void CountActiveTowersEt(HCAL_HLX::LUMI_SECTION& localSection);
	 void CalcETSumLumi(HCAL_HLX::LUMI_SECTION& localSection);
	 float CalcETSumError( HCAL_HLX::LUMI_SECTION &localSection );
	 float CalcETSumNoiseError( HCAL_HLX::LUMI_SECTION &localSection );
	 void CalcLHCLumi(HCAL_HLX::LUMI_SECTION& localSection);
	 void CalcOccLumi(HCAL_HLX::LUMI_SECTION& localSection);
	 float CalcOccNoiseError( HCAL_HLX::LUMI_SECTION &localSection, u32 set ); 
	 float CalcOccErrorTotal( HCAL_HLX::LUMI_SECTION &localSection, u32 set ); 
	 float CalcOccErrorBX( HCAL_HLX::LUMI_SECTION &localSection, u32 set, u32 iBX ); 

	 void CalcLumi( HCAL_HLX::LUMI_SECTION& localSection );
	 float CalcLumiError( u32 numNibbles, u32 numTowers, u32 numBunches = 1, float intPerBX = 0.01 );

	 // Configuration
	 //u32 startAG_, endAG_, sizeAG_; // AG = Abort Gap.
	 u32 sizeNoise_[NUM_CAP_BANKS];
	 float        invSizeNoise_[NUM_CAP_BANKS];
	 float totalEtSumNoise_[NUM_CAP_BANKS];
	 u32 etSumNoiseNorm_[NUM_CAP_BANKS];
	 float totalOccNoise_[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
	 u32 occNoiseNorm_[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
	 u32 numBX_, numHLX_; 
	 float normNumBX_, normNumHLX_;

	 // Should only be called once per run
	 u32 numActiveTowers_[HCAL_HLX_NUM_HLXS][2];
	 u32 numActiveTowersBX_[2];
	 u32 TotalActiveTowers_;

	 bool         noMasks_;
	 u32 numOrbitsPerNB_;
	 u32 numActiveTowersEt_[HCAL_HLX_NUM_HLXS];
	 u32 numActiveTowersBXEt_;
	 u32 TotalActiveTowersEt_;


	 bool Block( unsigned short int iBX, unsigned short int numBlock);
	 bool isBunch( unsigned short int iBX, unsigned short int scheme = 1 );  

	 u32 BXCapId_[HCAL_HLX_NUM_BUNCHES];
	 u32 BXMask_[HCAL_HLX_NUM_BUNCHES];
	 u32 BXMaskBool_[HCAL_HLX_NUM_BUNCHES];
	 u32 HLXMask_[HCAL_HLX_NUM_HLXS];

	 // Make an array to store the previous averages, this way we can calculate 
	 // an estimate of the error as we go. We store an average for each four lumi
	 // nibbles (a lumi "bite")
	 float etSumAvgPerLB_[ET_SUM_STORE_SIZE][HCAL_HLX_NUM_BUNCHES];
	 float etSumNoiseAvgPerLB_[ET_SUM_STORE_SIZE][HCAL_HLX_NUM_BUNCHES];
	 u32 etStoreSize_;
	 u32 etNoiseStoreSize_;
	 float sigmaEt_;
	 float sigmaEtWeight_;
	 float sigmaEtNoise_;
	 float sigmaEtNoiseWeight_;
   };
}
  
