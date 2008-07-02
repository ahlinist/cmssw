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

using namespace ICCoreUtils;

namespace HCAL_HLX {

  class LumiCalc {
  public:
    //LumiCalc();
    LumiCalc(unsigned int  start = 3564 - 119,
	     unsigned int  end = 3564,
	     char scheme = 1 );
    ~LumiCalc() {}


    // Do the calculations
    void DoCalc(HCAL_HLX::LUMI_SECTION & localSection);

    // configuration
    unsigned int SetBXMask( const std::vector< unsigned int > &BXMask );
    unsigned int SetHLXMask( const std::vector< unsigned int > &HLXMask );
    void SetNoiseInterval(unsigned int start,
			  unsigned int end);
    
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

    unsigned int set1BelowIndex;
    unsigned int set1BetweenIndex;
    unsigned int set1AboveIndex;
    unsigned int set2BelowIndex;
    unsigned int set2BetweenIndex;
    unsigned int set2AboveIndex;
    
    // Configuration
    unsigned int startAG_, endAG_, sizeAG_; // AG = Abort Gap.
    unsigned int numBX_, numHLX_; 
    // Should only be called once per run
    unsigned int numActiveTowers_[36][2];
    unsigned int TotalActiveTowers_;

    bool Block( unsigned short int iBX, unsigned short int numBlock);
    bool isBunch( unsigned short int iBX, unsigned short int scheme = 1 );  

    std::vector< unsigned int > BXMask_;
    std::vector< unsigned int > HLXMask_;
  };
}
  
