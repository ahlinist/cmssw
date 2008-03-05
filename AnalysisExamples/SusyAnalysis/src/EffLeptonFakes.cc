#include "AnalysisExamples/SusyAnalysis/interface/EffLeptonFakes.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

EffLeptonFakes::EffLeptonFakes(Config_t * theConfig) : 
myConfig(theConfig), EventData(0)
{
   

}



//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------
// this method should contain fake rate tables
float EffLeptonFakes::GetLeptonFakes(int itype, MrEvent* theEventData)
{ 
  // returns the probability for an event of given topology to have a fake lepton
  // this can be used to subtract the number of fakes from a configuration
  // ind = 1 for electrons
  //     = 2 for muons
  // here, fake rates are dummy
  
  
  float fake = 0.;
  
  return fake;

}
  


  
