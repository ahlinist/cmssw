#include "AnalysisExamples/SusyAnalysis/interface/MatchHemis.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

MatchHemis::MatchHemis(vector<MrParticle*>& DataReco, vector<MrParticle*>& DataMC,
          vector<float>& vAin1, vector<float>& vAin2):
          RecoData(DataReco), MCData(DataMC), vA1(vAin1), vA2(vAin2),
          DEBUGLVL(0),
          status(0), isMatchedOK(false)
{};


//------------------------------------------------------------------------------
// Methods:
//------------------------------------------------------------------------------

int MatchHemis::GetMatchedSusyMother1(void){
  if (status != 1){isMatchedOK = this->DoMatch();}
  if (isMatchedOK){
    return firstsusymother[0];
  } else {
    return -1;
  }
}

int MatchHemis::GetMatchedSusyMother2(void){
  if (status != 1){isMatchedOK = this->DoMatch();}
  if (isMatchedOK){
    return firstsusymother[1];
  } else {
    return -1;
  }
}

bool MatchHemis::DoMatch(void)
{ 
   // Match the hemispheres to the MC truth
   // is true (false) if matching succeeded (or not)
   // if matched, firstsusymother[2] returns the index of the matched susy particle
   // for each hemisphere
   // Note: matching is called ambiguous if both susymothers match best with the same
   // hemisphere axis. This is solved unless the flag includeambcases is set to false. 

    const bool trySMMOthers = true;
    const bool includeambcases = true;
   
    status = 1;
    
    if (vA1.size() < 4 || vA2.size() < 4){return false;}
    
    float axis1_x = vA1[0];
    float axis1_y = vA1[1];
    float axis1_z = vA1[2];
     
    float axis2_x = vA2[0];
    float axis2_y = vA2[1];
    float axis2_z = vA2[2];

    float axis1_P = vA1[3];
    float axis2_P = vA2[3];
    
    int topSusyMotherIndex[2] = {0, 0}; 
    int lspIndex[2] = {0, 0};
    int tsmctr = 0;
    int lspctr = 0;
    float topLSPdiff1_Px = 0.;
    float topLSPdiff1_Py = 0.;
    float topLSPdiff1_Pz = 0.;
    float topLSPdiff1_P =  0.;
    float topLSPdiff2_Px = 0.;
    float topLSPdiff2_Py = 0.;
    float topLSPdiff2_Pz = 0.;
    float topLSPdiff2_P =  0.;
    
    // SUSY events: Find the first pair produced SUSY particles and the LSPs
    for (int i = 0; i < (int) MCData.size(); i++){ 
      if ( (MCData[i]->pid() == 1000021 
       || (abs(MCData[i]->pid()) >= 1000001 && abs(MCData[i]->pid()) <= 1000006) 
       || (abs(MCData[i]->pid()) >= 2000001 && abs(MCData[i]->pid()) <= 2000006) ) 
       && abs(MCData[MCData[i]->motherIndex()]->pid()) < 22
       && tsmctr < 2) {
        topSusyMotherIndex[tsmctr] = i;
	tsmctr++;
      }

      if (MCData[i]->pid() == 1000022 && lspctr < 2){
        lspIndex[lspctr] = i;
	lspctr++;
      }

    }

    // and project the pair produced SUSY particles (minus their LSP) on the hemisphere axes
    if (tsmctr == 2 && lspctr == 2) {
     topLSPdiff1_Px = MCData[topSusyMotherIndex[0]]->px() - MCData[lspIndex[0]]->px();
     topLSPdiff1_Py = MCData[topSusyMotherIndex[0]]->py() - MCData[lspIndex[0]]->py();
     topLSPdiff1_Pz = MCData[topSusyMotherIndex[0]]->pz() - MCData[lspIndex[0]]->pz();
     topLSPdiff1_P = sqrt(topLSPdiff1_Px*topLSPdiff1_Px + topLSPdiff1_Py*topLSPdiff1_Py
                          + topLSPdiff1_Pz*topLSPdiff1_Pz);
     topLSPdiff2_Px = MCData[topSusyMotherIndex[1]]->px() - MCData[lspIndex[1]]->px();
     topLSPdiff2_Py = MCData[topSusyMotherIndex[1]]->py() - MCData[lspIndex[1]]->py();
     topLSPdiff2_Pz = MCData[topSusyMotherIndex[1]]->pz() - MCData[lspIndex[1]]->pz();
     topLSPdiff2_P = sqrt(topLSPdiff2_Px*topLSPdiff2_Px + topLSPdiff2_Py*topLSPdiff2_Py
                          + topLSPdiff2_Pz*topLSPdiff2_Pz);
    }
    
    // non-SUSY events: Find the first pair produced SM particles
    // seems tricky: here works only for t-tbar
    if (tsmctr == 0 && trySMMOthers) {
      for (int i = 0; i < (int) MCData.size(); i++){ 
        if ( abs(MCData[i]->pid()) == 6 && MCData[i]->status() == 3
         && tsmctr < 2) {
          topSusyMotherIndex[tsmctr] = i;
  	  tsmctr++;
        }
      }
      lspctr = 2;

      // and project them on the hemisphere axes
      if (tsmctr == 2) {
       topLSPdiff1_Px = MCData[topSusyMotherIndex[0]]->px();
       topLSPdiff1_Py = MCData[topSusyMotherIndex[0]]->py();
       topLSPdiff1_Pz = MCData[topSusyMotherIndex[0]]->pz();
       topLSPdiff1_P = sqrt(topLSPdiff1_Px*topLSPdiff1_Px + topLSPdiff1_Py*topLSPdiff1_Py
                          + topLSPdiff1_Pz*topLSPdiff1_Pz);
       topLSPdiff2_Px = MCData[topSusyMotherIndex[1]]->px();
       topLSPdiff2_Py = MCData[topSusyMotherIndex[1]]->py();
       topLSPdiff2_Pz = MCData[topSusyMotherIndex[1]]->pz();
       topLSPdiff2_P = sqrt(topLSPdiff2_Px*topLSPdiff2_Px + topLSPdiff2_Py*topLSPdiff2_Py
                          + topLSPdiff2_Pz*topLSPdiff2_Pz);
      }
    }

    if (tsmctr < 2) {return false;}
    if (lspctr < 2) {return false;}

    float projS1A1= -1.;
    float projS2A1= -1.;
    float projS1A2= -1.;
    float projS2A2= -1.;
    if (topLSPdiff1_P > 0. && topLSPdiff2_P > 0.){

     projS1A1 = (topLSPdiff1_Px * axis1_x +  topLSPdiff1_Py * axis1_y + topLSPdiff1_Pz  * axis1_z)
                      / topLSPdiff1_P;
     projS2A1 = (topLSPdiff2_Px * axis1_x +  topLSPdiff2_Py * axis1_y + topLSPdiff2_Pz  * axis1_z)
                      / topLSPdiff2_P;
     projS1A2 = (topLSPdiff1_Px * axis2_x +  topLSPdiff1_Py * axis2_y + topLSPdiff1_Pz  * axis2_z)
                      / topLSPdiff1_P;
     projS2A2 = (topLSPdiff2_Px * axis2_x +  topLSPdiff2_Py * axis2_y + topLSPdiff2_Pz  * axis2_z)
                      / topLSPdiff2_P;

     if (projS2A2<= -1.) { projS2A2 = -0.999999999;}
     if (projS2A2>= 1.) { projS2A2 = 0.999999999;}
     if (projS2A1<= -1.) { projS2A1 = -0.999999999;}
     if (projS2A1>= 1.) { projS2A1 = 0.999999999;}
     if (projS1A2<= -1.) { projS1A2 = -0.999999999;}
     if (projS1A2>= 1.) { projS1A2 = 0.999999999;}
     if (projS1A1<= -1.) { projS1A1 = -0.999999999;}
     if (projS1A1>= 1.) { projS1A1 = 0.999999999;}

    } else {return false;} 

    // Decide which pair produced SUSY particle matches each hemisphere
    
    if (projS1A1 >= projS1A2 && projS2A2 >= projS2A1) {
      firstsusymother[0] = topSusyMotherIndex[0];
      firstsusymother[1] = topSusyMotherIndex[1];
    } else if (projS1A2 >= projS1A1 && projS2A1 >= projS2A2) {
      firstsusymother[0] = topSusyMotherIndex[1];
      firstsusymother[1] = topSusyMotherIndex[0];
    } else { 
      if(!includeambcases){
    //     cout << "AMBIGUOUS PROJECTION!!!!" << endl;
         return false; 
      }
    }
     
    // handle the ambiguous cases
    if (includeambcases){
     if ((projS1A1 >= projS1A2 && projS2A1 >= projS2A2) || 
         (projS1A2 >= projS1A1 && projS2A2 >= projS2A1)) {
       
       float diff1122 = sqrt(       
       (topLSPdiff1_Px - axis1_x * axis1_P)*(topLSPdiff1_Px - axis1_x * axis1_P) 
      +(topLSPdiff1_Py - axis1_y * axis1_P)*(topLSPdiff1_Py - axis1_y * axis1_P) 
      +(topLSPdiff1_Pz - axis1_z * axis1_P)*(topLSPdiff1_Pz - axis1_z * axis1_P) 
       ) + sqrt(
       (topLSPdiff2_Px - axis2_x * axis2_P)*(topLSPdiff2_Px - axis2_x * axis2_P) 
      +(topLSPdiff2_Py - axis2_y * axis2_P)*(topLSPdiff2_Py - axis2_y * axis2_P) 
      +(topLSPdiff2_Pz - axis2_z * axis2_P)*(topLSPdiff2_Pz - axis2_z * axis2_P)        
       );
       float diff2112 = sqrt(       
       (topLSPdiff2_Px - axis1_x * axis1_P)*(topLSPdiff2_Px - axis1_x * axis1_P) 
      +(topLSPdiff2_Py - axis1_y * axis1_P)*(topLSPdiff2_Py - axis1_y * axis1_P) 
      +(topLSPdiff2_Pz - axis1_z * axis1_P)*(topLSPdiff2_Pz - axis1_z * axis1_P) 
       ) + sqrt(
       (topLSPdiff1_Px - axis2_x * axis2_P)*(topLSPdiff1_Px - axis2_x * axis2_P) 
      +(topLSPdiff1_Py - axis2_y * axis2_P)*(topLSPdiff1_Py - axis2_y * axis2_P) 
      +(topLSPdiff1_Pz - axis2_z * axis2_P)*(topLSPdiff1_Pz - axis2_z * axis2_P)        
       );
       
       if(diff1122 <= diff2112) {
         firstsusymother[0] = topSusyMotherIndex[0];
         firstsusymother[1] = topSusyMotherIndex[1];
       }else{
         firstsusymother[0] = topSusyMotherIndex[1];
         firstsusymother[1] = topSusyMotherIndex[0];
       }   
      }
     }

    return true;
}



