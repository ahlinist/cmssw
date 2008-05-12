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

#include "ICTypeDefs.hh"
#include "LumiStructures.hh"
#include <cmath>

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

  private:

    unsigned int set1BelowIndex;
    unsigned int set1BetweenIndex;
    unsigned int set1AboveIndex;
    unsigned int set2BelowIndex;
    unsigned int set2BetweenIndex;
    unsigned int set2AboveIndex;
    
    // configuration
    void SetNoiseInterval(unsigned short int start,
			  unsigned short int end);

    unsigned short int startAG, endAG; // AG = Abort Gap.

    bool Block( unsigned short int iBX, unsigned short int numBlock);
    bool isBunch( unsigned short int iBX,  unsigned short int scheme = 1 );  

    float BXMask[HCAL_HLX_MAX_BUNCHES];
  };
}
  
