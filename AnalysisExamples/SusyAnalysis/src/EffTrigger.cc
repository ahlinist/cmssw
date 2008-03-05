#include "AnalysisExamples/SusyAnalysis/interface/EffTrigger.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

EffTrigger::EffTrigger(Config_t * theConfig) : 
myConfig(theConfig), EventData(0)
{
   

}



//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------
// this method should contain trigger efficiency tables
float EffTrigger::GetEffTrigger(MrEvent* EventData)
{ 
  // returns the trigger efficiency for an event
  // the efficiency should not include the acceptance cuts 
  // nor isolation efficiency
  // presently dummy
  
  float eff = 1.;
  
  return eff;

}
  


  
