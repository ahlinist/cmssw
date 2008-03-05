#include "AnalysisExamples/SusyAnalysis/interface/EffLeptonToBeFake.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

EffLeptonToBeFake::EffLeptonToBeFake(Config_t * theConfig) : 
myConfig(theConfig), EventData(0)
{
   

}



//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------
// this method should contain fake rate tables
float EffLeptonToBeFake::GetLeptonFakes(int ind, MrEvent* theEventData)
{ 
  // returns the probability for a given lepton to be a fake
  //  this can be used to subtract fakes from e.g. mass distributions
  // ind = index of the lepton in RecoData
  // here, fake rates are dummy
  
  float fakeEl = 0.0;
  float fakeMu = 0.0;
  
  float ptcutEl = 2.;
  float etacutEl = 2.5;
  float ptcutMu = 3.;
  float etacutMu = 2.5;
  
  float eff = 0.;
  
  EventData = theEventData;
  std::vector<MrParticle*> & RecoData = *(EventData->recoData());
  
  // for electrons
  if (RecoData[ind]->particleType() == 10) { 
    if (RecoData[ind]->pt() < ptcutEl) {return 1.;}
    if (RecoData[ind]->eta() > etacutEl) {return 1.;}
    eff = fakeEl;
  }
  
  // for muons
  if (RecoData[ind]->particleType() == 20) { 
    if (RecoData[ind]->pt() < ptcutMu) {return 1.;}
    if (RecoData[ind]->eta() > etacutMu) {return 1.;}
    eff = fakeMu;
  }
  
  return eff;

}
  


  
