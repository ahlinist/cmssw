#include "AnalysisExamples/SusyAnalysis/interface/MatchObjects.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

MatchObjects::MatchObjects(vector<MrParticle*>* pData, 
                           vector<MrParticle*>* pMC):
RecoData(*pData), MCData(*pMC),
DEBUGLVL(0),
mo_elecDRmax(0.15), mo_elecDPbyPmax(2.0),
mo_muonDRmax(0.2), mo_muonDPbyPmax(1.0),
mo_photonDRmax(0.2), mo_photonDPbyPmax(1.0),
mo_jetDRmax(0.15), mo_jetDPbyPmax(3.0),
mo_celecDRmax(0.15), mo_cmuonDRmax(0.2), mo_cphotonDRmax(0.15), mo_cjetDRmax(0.15)
{}

MatchObjects::MatchObjects(vector<MrParticle*>* pData, 
              vector<MrParticle*>* pMC, edm::ParameterSet param):
RecoData(*pData), MCData(*pMC),
DEBUGLVL(0)
{
mo_elecDRmax = param.getParameter<double>("mo_elecDRmax") ;
mo_elecDPbyPmax = param.getParameter<double>("mo_elecDPbyPmax") ;
mo_muonDRmax = param.getParameter<double>("mo_muonDRmax") ;
mo_muonDPbyPmax = param.getParameter<double>("mo_muonDPbyPmax") ;
mo_photonDRmax = param.getParameter<double>("mo_photonDRmax") ;
mo_photonDPbyPmax = param.getParameter<double>("mo_photonDPbyPmax") ;
mo_jetDRmax = param.getParameter<double>("mo_jetDRmax") ;
mo_jetDPbyPmax = param.getParameter<double>("mo_jetDPbyPmax") ;
mo_celecDRmax = param.getParameter<double>("mo_celecDRmax") ;
mo_cmuonDRmax = param.getParameter<double>("mo_cmuonDRmax") ;
mo_cphotonDRmax = param.getParameter<double>("mo_cphotonDRmax") ;
mo_cjetDRmax = param.getParameter<double>("mo_cjetDRmax") ;
mo_ufoDRmax = param.getParameter<double>("mo_ufoDRmax") ;
mo_ufoDPbyPmax = param.getParameter<double>("mo_ufoDPbyPmax") ;
mo_cufoDRmax = param.getParameter<double>("mo_cufoDRmax") ;
}

// Methods:

void MatchObjects::DoMatch(void)
{
  // Matching of reconstructed objects to the MC truth
  // matching is based on DeltaR and relative difference in pt
  // the matched MCData index is stored in the RecoData
  
//  const float mojetDRmax = 0.9;  // used for ResolveMatchObjects test with event 9-11
//  const float mojetDPbyPmax = 5.0;
  const bool useResolveMatch = true;

    for(int i = 0; i < (int) RecoData.size() ;++i){
      int iMatch = -1;     

      for (int j = 0; j<(int)MCData.size(); ++j){

      // matching electrons with MC truth
        int ptype = RecoData[i]->particleType()/10;
        if (ptype == 1 && abs(MCData[j]->pid()) == 11
           &&  MCData[j]->status() == 1) {
          float deltaR = GetDeltaR(RecoData[i]->eta(), MCData[j]->eta(), 
                                   RecoData[i]->phi(), MCData[j]->phi());
          float dPtbyPt = 999.;
          if (MCData[j]->pt() > 0.){
            dPtbyPt = fabs(RecoData[i]->pt()-MCData[j]->pt())/MCData[j]->pt();
          }

          if (deltaR < mo_elecDRmax && dPtbyPt < mo_elecDPbyPmax) {
            if (iMatch < 0) {
	        iMatch = j;
            } else {
	        iMatch = BestMatch(iMatch, j, i);
            }	  
	  } 
        }   

      // matching muons with MC truth
        if (ptype == 2 && abs(MCData[j]->pid()) == 13
           &&  MCData[j]->status() == 1) {
          float deltaR = GetDeltaR(RecoData[i]->eta(), MCData[j]->eta(), 
                                   RecoData[i]->phi(), MCData[j]->phi());
          float dPtbyPt = 999.;
          if (MCData[j]->pt() > 0.){
            dPtbyPt = fabs(RecoData[i]->pt()-MCData[j]->pt())/MCData[j]->pt();
          }

          if (deltaR < mo_muonDRmax && dPtbyPt < mo_muonDPbyPmax) {	 
            if (iMatch < 0) {
	        iMatch = j;
            } else {
	        iMatch = BestMatch(iMatch, j, i);
            }	  
	  } 
        }   

      // matching photons with MC truth
        if (ptype == 4 && abs(MCData[j]->pid()) == 22
           &&  MCData[j]->status() == 1) {
          float deltaR = GetDeltaR(RecoData[i]->eta(), MCData[j]->eta(), 
                                   RecoData[i]->phi(), MCData[j]->phi());
          float dPtbyPt = 999.;
          if (MCData[j]->pt() > 0.){
            dPtbyPt = fabs(RecoData[i]->pt()-MCData[j]->pt())/MCData[j]->pt();
          }

          if (deltaR < mo_photonDRmax && dPtbyPt < mo_photonDPbyPmax) {	 
            if (iMatch < 0) {
	        iMatch = j;
            } else {
	        iMatch = BestMatch(iMatch, j, i);
            }	  
	  } 
        }   
    
      // matching jets with MC truth
        if ((ptype >= 5 && ptype <= 7) 
           && (abs(MCData[j]->pid()) == 21 || 
           (abs(MCData[j]->pid()) > 0 && abs(MCData[j]->pid()) < 6)) 
           &&  MCData[j]->status() == 2) {
          float deltaR = GetDeltaR(RecoData[i]->eta(), MCData[j]->eta(), 
                                   RecoData[i]->phi(), MCData[j]->phi());
          float dPtbyPt = 999.;
          if (MCData[j]->pt() > 0.){
            dPtbyPt = fabs(RecoData[i]->pt()-MCData[j]->pt())/MCData[j]->pt();
          }

         // or do we use BestMatch???
          if (deltaR < mo_jetDRmax && dPtbyPt < mo_jetDPbyPmax){
            if (iMatch < 0) {
	        iMatch = j;
            } else {
	        iMatch = BestMatch(iMatch, j, i);
            }	  
	  } 
        }

      // matching UFOs with MC truth
        if (ptype == 8 && 
          (abs(MCData[j]->pid()) == 211 || abs(MCData[j]->pid()) == 321)
           &&  MCData[j]->status() == 1) {
          float deltaR = GetDeltaR(RecoData[i]->eta(), MCData[j]->eta(), 
                                   RecoData[i]->phi(), MCData[j]->phi());
          float dPtbyPt = 999.;
          if (MCData[j]->pt() > 0.){
            dPtbyPt = fabs(RecoData[i]->pt()-MCData[j]->pt())/MCData[j]->pt();
          }

          if (deltaR < mo_ufoDRmax && dPtbyPt < mo_ufoDPbyPmax) {	 
            if (iMatch < 0) {
	        iMatch = j;
            } else {
	        iMatch = BestMatch(iMatch, j, i);
            }	  
	  } 
        }   
      }

      RecoData[i]->setPartonIndex(iMatch);
      if (DEBUGLVL >= 2){
       if (iMatch >= 0) {
         cout << "  object " << i 
              << ", type = " << RecoData[i]->particleType() 
              << " matched to MC " << iMatch
              << ", MC Pt = " << MCData[iMatch]->pt() << endl;
       }
       else{
        cout << "  object " << i 
             << ", type = " << RecoData[i]->particleType() 
             << " unmatched to MC " << endl;
       }
      }
    }

    // Check that several objects do not match with the same MC
    if (useResolveMatch){
      ResolveMatchObjects();
    }

    if (DEBUGLVL >= 1){
     int nMatched = 0;
     int nTried = 0;
     for(int i = 0; i < (int) RecoData.size() ;++i){
       nTried++;
       if (RecoData[i]->partonIndex() >= 0) {nMatched++;}
     }
     cout << "  Number of objects matched   = " << nMatched << endl;
     cout << "  Number of objects unmatched = " << nTried-nMatched << endl;
    }

 return;
}

//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------

int MatchObjects::BestMatch(int i, int j, int irecopart)
{
 // returns the index of the MCData object matching best a given recopart
 // i, j + indices in MCData of the 2 objects to be compared
 // comparison is made on the value of DeltaP/P

   MrParticle * recopart = RecoData[irecopart];
   
   float deltaRi = GetDeltaR(recopart->eta(), MCData[i]->eta(), 
                             recopart->phi(), MCData[i]->phi());
   float dPtbyPti = 999.;
   if (MCData[i]->pt() > 0.){
     dPtbyPti = fabs(recopart->pt()-MCData[i]->pt())/MCData[i]->pt();
   }
   float deltaRj = GetDeltaR(recopart->eta(), MCData[j]->eta(), 
                             recopart->phi(), MCData[j]->phi());
   float dPtbyPtj = 999.;
   if (MCData[j]->pt() > 0.){
     dPtbyPtj = fabs(recopart->pt()-MCData[j]->pt())/MCData[j]->pt();
   }

   if (dPtbyPti < dPtbyPtj) {return i;}
   else if (dPtbyPti > dPtbyPtj) {return j;}
   else{
     if (deltaRi <= deltaRj) {return i;}
     else {return j;}
   }

}

//------------------------------------------------------------------------------

void MatchObjects::ResolveMatchObjects(void) 
// ) 
{ 
  // Clean the matching of reconstructed objects to the MC truth
  // by resolving cases where different RecoData match to the same MCData
  // The updated matched MCData indices are stored in the RecoData

//  meaning of the vectors:
// indOrig = index in MCData of all particles from the RecoData
//           set to -1 if an ambiguous assignment is found
//           reset to best assignment in MCData
// indPart = index in RecoData of the particle being treated 
//           and all its ambiguous particles
// indNew  = initialized to -1
//           set to the best matched MCData not used by another ambiguous particle
//           after solving the ambiguities, this index is stored in RecoData

  vector<int> indOrig;
  vector<int> indPart;
  vector<int> indNew;
 
      // First collect all original MC indices in a vector
    for(int i = 0; i < (int) RecoData.size() ;++i){
      indOrig.push_back(RecoData[i]->partonIndex());
    }
      
      // loop over the RecoData and collect objects with same MC into vectors
    for(int i = 0; i < (int) RecoData.size() ;++i){
       int indcommon = -1;
       if (RecoData[i]->partonIndex() >= 0 ){
         bool newset = true;
         for(int j = i+1; j < (int) RecoData.size() ;++j){
           if (RecoData[i]->partonIndex() == RecoData[j]->partonIndex() ){
             if (newset){
               newset = false;
               indPart.push_back(i);
               indNew.push_back(-1);
               indcommon = RecoData[i]->partonIndex();
               indOrig[i] = -1;
             }
             indPart.push_back(j);
             indNew.push_back(-1);
             indOrig[j] = -1;
           }
         }
       }
       if (DEBUGLVL >= 2){
         if (indPart.size() > 0){
           cout << "  same MC for " << indPart.size() << " objects " << endl;
         }
       }
      // loop over the vectors to solve ambiguously matched MC
      bool found = true;
      while (found){
        found = false;
        // loop over all objects matched to the same MCData
        float dPtbyPtmin = 999.;
        int kbest = -1;
        int mcbest = -1;
        for (int k=0; k < (int) indPart.size(); ++k){
          if (indNew[k] < 0){
            // find the MCData with smallest difference in momentum
            // and which is not yet used by others
            for (int m=0; m<(int)MCData.size(); m++){
              // useind is a flag, true if this MCData can be used, false otherwise
              bool useind = true;
              for (int n=0; n<(int)indOrig.size(); ++n){
                if (m == indOrig[n]){
                  useind = false;
                }
              }
              float deltaR = 
                    GetDeltaR(RecoData[indPart[k]]->eta(), MCData[m]->eta(), 
                              RecoData[indPart[k]]->phi(), MCData[m]->phi());
              int ptype = RecoData[indPart[k]]->particleType()/10;
              if (ptype == 1) {
                if (abs(MCData[m]->pid()) != 11  
                || deltaR > mo_celecDRmax) {useind = false;}
              }
              else if (ptype == 2){
                if (abs(MCData[m]->pid()) != 13
                || deltaR > mo_cmuonDRmax) {useind = false;}
              }
              else if (ptype == 4){
                if (abs(MCData[m]->pid()) != 22
                || deltaR > mo_cphotonDRmax) {useind = false;}
              }
              else if (ptype >= 5 && ptype <= 7){
                if (!(abs(MCData[m]->pid()) == 21 || 
                 (abs(MCData[m]->pid()) > 0 && abs(MCData[m]->pid()) < 6)) 
                 ||  MCData[m]->status() != 2 
                 || deltaR > mo_cjetDRmax) {useind = false;}
              }
              else if (ptype == 8 ){
                 if( !(abs(MCData[m]->pid()) == 11 
                       || abs(MCData[m]->pid()) == 13
                       || abs(MCData[m]->pid()) == 211 
                       || abs(MCData[m]->pid()) == 321)
                  ||  MCData[m]->status() != 1
                  || deltaR > mo_cufoDRmax) {useind = false;}
              }
              
              if (useind){
               float dPtbyPt = 999.;
               if (MCData[m]->pt() > 0.){
                 dPtbyPt = fabs(RecoData[indPart[k]]->pt()-MCData[m]->pt())
                           / MCData[m]->pt();
               }
               if (dPtbyPt < dPtbyPtmin){
                 dPtbyPtmin = dPtbyPt;
                 kbest = k;
                 mcbest = m;
               }
              }
            }
          }
        }
        // keep the best of all solutions and try again to find the next one
        if (kbest >= 0){
          indNew[kbest] = mcbest;
          indOrig[indPart[kbest]] = mcbest; 
          found = true;
        }
      }

      for (int k=0; k<(int)indPart.size(); ++k){
        RecoData[indPart[k]]->setPartonIndex(indNew[k]);
        if (DEBUGLVL >= 2){
          int iMatch = RecoData[indPart[k]]->partonIndex();
          if (iMatch >= 0){
            cout << "  object " << indPart[k] 
              << ", type = " << RecoData[indPart[k]]->particleType() 
              << "  rematched to MC " << iMatch
              << ", MC Pt = " << MCData[iMatch]->pt() << endl;
          }
          else{
            cout << "  object " << indPart[k]
                 << ", type = " << RecoData[indPart[k]]->particleType() 
                 << " unmatched to MC " << endl;
          }
        }
      }

      if (!indPart.empty() ){
        indPart.clear();
        indNew.clear();
      }
    }  // end of loop over RecoData

 return;
}


//------------------------------------------------------------------------------

float MatchObjects::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float MatchObjects::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


