#include "AnalysisExamples/SusyAnalysis/interface/EffLeptonID.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

EffLeptonID::EffLeptonID(Config_t * theConfig) : 
myConfig(theConfig)
{
   

}



//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------
// this method should contain Reco and ID efficiency tables
  
float EffLeptonID::GetLeptonEfficiency(int idpart, float pt, float eta)
{ // returns the efficiencies for electrons (idpart=1) or muons (idpart=2)
  // here, efficiencies are dummy


  float effRecoElec = 1.;
  float effRecoMuon = 1.;
  
  float effElecID = 1.;
  float effMuonID = 1.;

  float eff = 1.;
  if (idpart == 1){
    eff = eff * effRecoElec * effElecID;
  }
  else if (idpart == 2){
    eff = eff * effRecoMuon * effMuonID;
  }

  
  return eff;    
}
  


  
