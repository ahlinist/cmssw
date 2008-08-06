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
#include <vector>

#define set1BelowIndex   0
#define set1BetweenIndex 1
#define set1AboveIndex   2
#define set2BelowIndex   3
#define set2BetweenIndex 4
#define set2AboveIndex   5

#define HCAL_HLX_NUM_BUNCHES 3564
#define HCAL_HLX_NUM_HLXS 36
#define NUM_CAP_BANKS 4

using namespace ICCoreUtils;

namespace HCAL_HLX {

   class LumiCalc {
      public:

	 typedef std::vector< std::vector< float > > LumiVec;

	 //LumiCalc();
	 LumiCalc( );
	 ~LumiCalc(){
	    etSumAvgPerLB_.clear();
	    etSumNoiseAvgPerLB_.clear();
	 }


	 // Do the calculations
	 void DoCalc(HCAL_HLX::LUMI_SECTION & localSection);

	 // configuration
	 unsigned int SetBXMask( const std::vector< unsigned int > &BXMask );
	 unsigned int SetHLXMask( const std::vector< unsigned int > &HLXMask );
    
      private:

	 void CountActiveTowers(HCAL_HLX::LUMI_SECTION& localSection);
	 void CountActiveTowersEt(HCAL_HLX::LUMI_SECTION& localSection);
	 void CalcETSumLumi(HCAL_HLX::LUMI_SECTION& localSection);
	 float CalcETSumError( HCAL_HLX::LUMI_SECTION &localSection );
	 float CalcETSumNoiseError( HCAL_HLX::LUMI_SECTION &localSection );
	 void CalcLHCLumi(HCAL_HLX::LUMI_SECTION& localSection);
	 void CalcOccLumi(HCAL_HLX::LUMI_SECTION& localSection);
	 float CalcOccErrorTotal( HCAL_HLX::LUMI_SECTION &localSection, unsigned int set ); 
	 float CalcOccErrorBX( HCAL_HLX::LUMI_SECTION &localSection, unsigned int set ); 

	 void CalcLumi( HCAL_HLX::LUMI_SECTION& localSection );
	 float CalcLumiError( unsigned int numNibbles, 
	 unsigned int numTowers, 
	 unsigned int numBunches = 1, 
	 float intPerBX = 0.01 );

	 // Configuration
	 //unsigned int startAG_, endAG_, sizeAG_; // AG = Abort Gap.
	 unsigned int sizeNoise_[NUM_CAP_BANKS];
	 float totalEtSumNoise_[NUM_CAP_BANKS];
	 unsigned int etSumNoiseNorm_[NUM_CAP_BANKS];
	 float totalOccNoise_[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
	 unsigned int occNoiseNorm_[HCAL_HLX_NUM_HLXS][2][NUM_CAP_BANKS];
	 unsigned int numBX_, numHLX_; 

	 // Should only be called once per run
	 unsigned int numActiveTowers_[HCAL_HLX_NUM_HLXS][2];
	 unsigned int numActiveTowersBX_[HCAL_HLX_NUM_BUNCHES][2];
	 unsigned int TotalActiveTowers_;

	 bool         noMasks_;
	 unsigned int numOrbitsPerNB_;
	 unsigned int numActiveTowersEt_[HCAL_HLX_NUM_HLXS];
	 unsigned int numActiveTowersBXEt_[HCAL_HLX_NUM_BUNCHES];
	 unsigned int TotalActiveTowersEt_;


	 bool Block( unsigned short int iBX, unsigned short int numBlock);
	 bool isBunch( unsigned short int iBX, unsigned short int scheme = 1 );  

	 std::vector< unsigned int > BXMask_;
	 std::vector< unsigned int > HLXMask_;

	 // Make an array to store the previous averages, this way we can calculate 
	 // an estimate of the error as we go. We store an average for each four lumi
	 // nibbles (a lumi "bite")
	 LumiVec etSumAvgPerLB_;
	 LumiVec etSumNoiseAvgPerLB_;
	 unsigned int etStoreMaxSize_;
	 float sigmaEt_;
	 float sigmaEtWeight_;
	 float sigmaEtNoise_;
	 float sigmaEtNoiseWeight_;
   };
}
  
