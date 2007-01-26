#include "AnalysisExamples/SusyAnalysis/interface/ShapeAnalyzer.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

ShapeAnalyzer::ShapeAnalyzer(MrEvent* pEvtData):
myEventData(pEvtData), RecoData(*(myEventData->recoData())), MCData(*(myEventData->mcData())),
DEBUGLVL(0)
{};

/*
// Destructor:
 
ShapeAnalyzer::~ShapeAnalyzer(){

 if (myHemi != NULL){delete myHemi;}
 if (myHemiMatcher != NULL){delete myHemiMatcher;}

 if (!vPx.empty() ){
    vPx.clear();
    vPy.clear();
    vPz.clear();
    vE.clear();
 }
 if (!vA1.empty() ){
    vA1.clear();
    vA2.clear();
 }
 if (!vgroups.empty() ){
    vgroups.clear();
 }

}
*/

//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------

bool ShapeAnalyzer::AnalyzeHemi(void)
{
  // Reconstructs and matches the hemispheres


  // ******************************************************** 
  // Recontruct the hemispheres 
    
    if (DEBUGLVL >= 1){
      cout << "Hemisphere reconstruction " << endl;
    }
    
    for (int i = 0; i < (int) RecoData.size() ; i++){
     
      vPx.push_back(RecoData[i]->px());
      vPy.push_back(RecoData[i]->py());
      vPz.push_back(RecoData[i]->pz());
      vE.push_back(RecoData[i]->energy());
      
    }
    
    myHemi = new Hemisphere(vPx, vPy, vPz, vE, 2, 3);
    
    if (DEBUGLVL < 3){myHemi->SetDebug(0);}
    else {myHemi->SetDebug(1);}
    
    vA1 = myHemi->getAxis1();
    vA2 = myHemi->getAxis2();

    // store the hemi axes in the event data
    myEventData->setHemiAxis1(vA1);
    myEventData->setHemiAxis2(vA2);
    
    vgroups = myHemi->getGrouping();
    
    delete myHemi;
    vPx.clear();
    vPy.clear();
    vPz.clear();
    vE.clear();
    
    for (int i = 0; i < (int) RecoData.size() ; i++){
       RecoData[i]->setHemisphere(vgroups[i]);
    }
    vgroups.clear();
    

    if (DEBUGLVL >= 2){
     float Axis1_x = vA1[0];
     float Axis1_y = vA1[1];
     float Axis1_z = vA1[2];
     
     float Axis2_x = vA2[0];
     float Axis2_y = vA2[1];
     float Axis2_z = vA2[2];

     float Axis1_P = vA1[3];
     float Axis2_P = vA2[3];
     float Axis1_E = vA1[4];
     float Axis2_E = vA2[4];    
     cout << "  Hemisphere Axis 1: Nx = " << Axis1_x << ", Ny = " <<  Axis1_y
          << ", Nz = " << Axis1_z  << ", P = " << Axis1_P
          << ", E = " << Axis1_E<< endl;
     cout << "  Hemisphere Axis 2: Nx = " << Axis2_x << ", Ny = " <<  Axis2_y
          << ", Nz = " << Axis2_z  << ", P = " << Axis2_P
          << ", E = " << Axis2_E<< endl;
    }

  // end of hemisphere reconstruction
  

  // ******************************************************** 
  // Match the hemispheres to the MC truth
  // if hemispheres could not be matched, it is a crazy event and is rejected

    int firstsusymother[2];

    myHemiMatcher = new MatchHemis(RecoData, MCData, vA1, vA2);
    myHemiMatcher->SetDebug(DEBUGLVL);

    firstsusymother[0] = myHemiMatcher->GetMatchedSusyMother1();
    firstsusymother[1] = myHemiMatcher->GetMatchedSusyMother2();
    // store the hemi matched SUSY mothers in the event data
    myEventData->setIndexMatchedSusyMother1(firstsusymother[0]);
    myEventData->setIndexMatchedSusyMother2(firstsusymother[1]);
    if (firstsusymother[0] < 0) {
      if (DEBUGLVL >= 1){
        cout << " Hemispheres could not be matched " << endl;
      }
      return false;
    }
    else{
      if (DEBUGLVL >= 1){
       for (int i=0; i < 2; i++){
         cout << " Hemi " << i+1 
              << " matched to MC index = " << firstsusymother[i]
              << " ID = " << MCData[firstsusymother[i]]->pid() << endl;
         cout << " Indices of particles included: " << endl;
         for (int j = 0; j < (int) RecoData.size(); j++){
           if (RecoData[j]->hemisphere() == i+1){
             cout << "  " << j;
           }
         }
         cout << endl;
       }
      }
    }

    delete myHemiMatcher;
    vA1.clear();
    vA2.clear();

  // end of hemisphere matching
 
  return true;
}
//------------------------------------------------------------------------------

float ShapeAnalyzer::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float ShapeAnalyzer::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


