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

#include "RecoLuminosity/HLXReadOut/CoreUtils/include/ICTypeDefs.hh"
#include "RecoLuminosity/HLXReadOut/HLXCoreLibs/include/LumiStructures.hh"
#include <cmath>
#include <vector>

#define set1BelowIndex   0
#define set1BetweenIndex 1
#define set1AboveIndex   2
#define set2BelowIndex   3
#define set2BetweenIndex 4
#define set2AboveIndex   5

namespace HCAL_HLX {

  class LumiCalc {
  public:
    //LumiCalc();
    LumiCalc();
    ~LumiCalc() {}

    // Do the calculations
    void DoCalc(HCAL_HLX::LUMI_SECTION & localSection);

    // configuration
    unsigned int SetBXMask( const std::vector< unsigned int > &BXMask );
    unsigned int SetHLXMask( const std::vector< unsigned int > &HLXMask );
    
  private:

    void CountActiveTowers(HCAL_HLX::LUMI_SECTION& localSection);
    void CalcETSumLumi(HCAL_HLX::LUMI_SECTION& localSection);
    float CalcETSumError( unsigned int numNibbles, 
			  unsigned int numTowers, 
			  unsigned int numBunches = 1, 
			  float intPerBX = 0.01 );
    void CalcLHCLumi(HCAL_HLX::LUMI_SECTION& localSection);
    void CalcOccLumi(HCAL_HLX::LUMI_SECTION& localSection);
    float CalcOccError( unsigned int numNibbles, 
			unsigned int numTowers, 
			unsigned int numBunches = 1, 
			float intPerBX = 0.01 );

    void CalcLumi( HCAL_HLX::LUMI_SECTION& localSection );
    float CalcLumiError( unsigned int numNibbles, 
			unsigned int numTowers, 
			unsigned int numBunches = 1, 
			float intPerBX = 0.01 );

    // Configuration
    unsigned int sizeNoise_[4];
    unsigned int numBX_, numHLX_; 

    unsigned int numActiveTowers_[HCAL_HLX_NUM_HLXS][2];
    unsigned int TotalActiveTowers_;

    bool Block( unsigned short int iBX, unsigned short int numBlock);
    bool isBunch( unsigned short int iBX, unsigned short int scheme = 1 );  

    std::vector< unsigned int > BXMask_;
    std::vector< unsigned int > HLXMask_;
  };
}
  
