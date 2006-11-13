#include "AnalysisExamples/SusyAnalysis/interface/Isolator.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

Isolator::Isolator(vector<MrParticle*>& Data, const TrackCollection * Tracks,
 const CaloTowerCollection* CaloTowers):
RecoData(Data), TrackData(Tracks), CaloTowerData(CaloTowers),
DEBUGLVL(0),
iso_MethodElec(1100), iso_jetbyElEmin(1.), iso_ptElwrtJetmin(7.),
iso_ElCalDRin(0.), iso_ElCalDRout(0.1), iso_ElCalSeed(0.1), iso_ElCalValue(0.5),
iso_ElTkDRin(0.), iso_ElTkDRout(0.1), iso_ElTkSeed(0.1), iso_ElTkValue(0.5),
iso_MethodMuon(1100), iso_jetbyMuEmin(1.), iso_ptMuwrtJetmin(7.),
iso_MuCalDRin(0.), iso_MuCalDRout(0.1), iso_MuCalSeed(0.1), iso_MuCalValue(0.5),
iso_MuTkDRin(0.), iso_MuTkDRout(0.1), iso_MuTkSeed(0.1), iso_MuTkValue(0.5),
iso_MethodTau(1100), iso_jetbyTauEmin(1.), iso_ptTauwrtJetmin(7.),
iso_MethodPhot(1100), iso_jetbyPhotEmin(1.), iso_ptPhotwrtJetmin(7.)
{};

Isolator::Isolator(vector<MrParticle*>& Data, const TrackCollection * Tracks,
 const CaloTowerCollection* CaloTowers, edm::ParameterSet param):
RecoData(Data), TrackData(Tracks), CaloTowerData(CaloTowers),
DEBUGLVL(0)
{
iso_MethodElec = param.getParameter<int>("iso_MethodElec") ;
iso_jetbyElEmin = param.getParameter<double>("iso_jetbyElEmin") ;
iso_ptElwrtJetmin = param.getParameter<double>("iso_ptElwrtJetmin") ;
iso_ElCalDRin = param.getParameter<double>("iso_ElCalDRin") ;
iso_ElCalDRout = param.getParameter<double>("iso_ElCalDRout") ;
iso_ElCalSeed = param.getParameter<double>("iso_ElCalSeed") ;
iso_ElCalValue = param.getParameter<double>("iso_ElCalValue") ;
iso_ElTkDRin = param.getParameter<double>("iso_ElTkDRin") ;
iso_ElTkDRout = param.getParameter<double>("iso_ElTkDRout") ;
iso_ElTkSeed = param.getParameter<double>("iso_ElTkSeed") ;
iso_ElTkValue = param.getParameter<double>("iso_ElTkValue") ;
iso_MethodMuon = param.getParameter<int>("iso_MethodMuon") ;
iso_jetbyMuEmin = param.getParameter<double>("iso_jetbyMuEmin") ;
iso_ptMuwrtJetmin = param.getParameter<double>("iso_ptMuwrtJetmin") ;
iso_MuCalDRin = param.getParameter<double>("iso_MuCalDRin") ;
iso_MuCalDRout = param.getParameter<double>("iso_MuCalDRout") ;
iso_MuCalSeed = param.getParameter<double>("iso_MuCalSeed") ;
iso_MuCalValue = param.getParameter<double>("iso_MuCalValue") ;
iso_MuTkDRin = param.getParameter<double>("iso_MuTkDRin") ;
iso_MuTkDRout = param.getParameter<double>("iso_MuTkDRout") ;
iso_MuTkSeed = param.getParameter<double>("iso_MuTkSeed") ;
iso_MuTkValue = param.getParameter<double>("iso_MuTkValue") ;
iso_MethodTau = param.getParameter<int>("iso_MethodTau") ;
iso_jetbyTauEmin = param.getParameter<double>("iso_jetbyTauEmin") ;
iso_ptTauwrtJetmin = param.getParameter<double>("iso_ptTauwrtJetmin") ;
iso_MethodPhot = param.getParameter<int>("iso_MethodPhot") ;
iso_jetbyPhotEmin = param.getParameter<double>("iso_jetbyPhotEmin") ;
iso_ptPhotwrtJetmin = param.getParameter<double>("iso_ptPhotwrtJetmin") ;
};

// Methods:

bool Isolator::IsObjectIsolated(int ichk)
{
   
//   cout << " Isolation Particle = " << ichk 
//        << " type = " << RecoData[ichk]->particleType() << endl;
   bool isIsolated = true;
   if (ichk < 0){return isIsolated;}

   if (RecoData[ichk]->particleType() == 1){
     isIsolated = IsoElectron(ichk);
   }
   else if (RecoData[ichk]->particleType() == 2){
     isIsolated = IsoMuon(ichk);
   }
   else if (RecoData[ichk]->particleType() == 3){
     isIsolated = IsoTau(ichk);
   }
   else if (RecoData[ichk]->particleType() == 4){
     isIsolated = IsoPhoton(ichk);
   }
  
  return isIsolated;

}

//------------------------------------------------------------------------------

bool Isolator::IsoElectron(int ichk)
{
 // Check that electron with index ichk is isolated
 // Several isolation methods are available
 //  - isolation in a cone or in pT wrt nearest jet
 //  - calorimetric or tracker or weighted sum
 //  - using information in transverse plane or in 3D
 //  - cut on absolute value of Et or ratio or number of objects 
 
  bool isIsolated = true;
//  if (DEBUGLVL >= 2){
    cout << " Electron isolation method = " << iso_MethodElec << endl;
//  }

  if (RecoData[ichk]->particleType() != 1){
    cout << " *** Particle given " << ichk 
      << " of type " << RecoData[ichk]->particleType()
      << " does not correspond to the type for isolation method IsoElectron" << endl;
    return isIsolated;
  }
  if (iso_MethodElec == 0){return isIsolated;}

  // compute the isolation in pT wrt nearest jet
  if (iso_MethodElec / 1000 == 1){
    int iJet = FindNearestJet(ichk);
   
    if (iJet >= 0){
//      cout << " nearest jet index = " << iJet 
//         << " type = " << RecoData[iJet]->particleType() << endl;

      if (RecoData[iJet]->energy() < iso_jetbyElEmin * RecoData[ichk]->energy() )
        {isIsolated = true;}

      float ptwrtJet = GetPtwrtJet(ichk, iJet);
//      cout << " ptwrtJet = " << ptwrtJet << endl;
      if (ptwrtJet > iso_ptElwrtJetmin){
        isIsolated = true;
//        if (DEBUGLVL >= 2){
          cout << " electron is isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
//        }
      } else {
        isIsolated = false;
//        if (DEBUGLVL >= 2){
          cout << " electron is not isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
//        }
      }
    }
  }

  // compute the isolation by a cone algorithm
  else if (iso_MethodElec / 1000 == 2){
    int idet = (iso_MethodElec - 2000) / 100;
    int itra = (iso_MethodElec / 100);
    itra = (iso_MethodElec - 100*itra) / 10;
    int ival = (iso_MethodElec / 10);
    ival = iso_MethodElec - 10*ival;
    const Electron* elecand = RecoData[ichk]->electronCandidate();
//    cout << " pointer to elec cand " << elecand;
//    float elecmom = elecand->p();
//    cout << " momentum " << elecmom << endl;

    // using calorimeter information
    if (idet == 1 || idet == 3){
      const SuperCluster* supercluster = &(*(elecand->superCluster()));
//      cout << " pointer to supercluster " << supercluster;
//      float elenergy = supercluster->energy();
//      cout << " energy " << elenergy << endl;
      float etest = 0.;
      if (itra == 1) {
        float eta = supercluster->eta();
//        float theta = 2. * atan(exp(-eta)); // pseudorapidity or rapidity?
//        etest = supercluster->energy() * sin(theta);
        etest = supercluster->energy() / cosh(eta);
//        cout << " eta = " << eta << ", Et " << etest << endl;
      } else if (itra == 2) {
        etest = supercluster->energy();
//        cout << ", E " << etest << endl;
      }
      float elCalEta = supercluster->eta();
      float elCalPhi = supercluster->phi();
      float eSum = IsoCalSum (itra, elCalEta, elCalPhi, 
                              iso_ElCalDRin, iso_ElCalDRout, iso_ElCalSeed);
      if (itra < 3 && ival == 1) {
        eSum -= etest;
        if (eSum > iso_ElCalValue*etest){isIsolated = false;}
      } else if (itra < 3 && ival == 2) {
        eSum -= etest;
        if (eSum > iso_ElCalValue){isIsolated = false;}
      } else if (itra == 3) {
        eSum = (int) eSum - 1;
        if (eSum > iso_ElCalValue){isIsolated = false;}
      } else {
        cout << " *** Chosen isolation method is not implemented for electron ***" << endl;
      }
//      if (DEBUGLVL >= 2){
       if (itra < 3 && ival == 1) {
        if (isIsolated){
         cout << " electron is isolated in calorimeter, Esum = " << eSum << endl;
        } else {
         cout << " electron is not isolated in calorimeter, Esum = " << eSum << endl;
        }
       } else if (itra < 3 && ival == 2) {
        if (isIsolated){
         cout << " electron is isolated in calorimeter, Etsum = " << eSum << endl;
        } else {
         cout << " electron is not isolated in calorimeter, Etsum = " << eSum << endl;
        }
       } else if (itra == 3) {
        if (isIsolated){
         cout << " electron is isolated in calorimeter, #trks = " << eSum << endl;
        } else {
         cout << " electron is not isolated in calorimeter, #trks = " << eSum << endl;
        }
       }
//      }
    } 

    // using tracker information
    if (idet == 2 || idet == 3){
      const Track* eletrack = &(*(elecand->track()));
//      cout << " pointer to electron track " << eletrack;
      float ptest = 0.;
      if (itra == 1) {
        ptest = eletrack->pt();
//        cout << ", Pt " << ptest << endl;
      } else if (itra == 2) {
        ptest = eletrack->p();
//        cout << ", P " << ptest << endl;
      }
      float elTrkEta = eletrack->eta();
      float elTrkPhi = eletrack->phi();
      float pSum = IsoTrkSum (itra, elTrkEta, elTrkPhi, 
                              iso_ElTkDRin, iso_ElTkDRout, iso_ElTkSeed);
      if (itra < 3 && ival == 1) {
        pSum -= ptest;
        if (pSum > iso_ElTkValue*ptest){isIsolated = false;}
      } else if (itra < 3 && ival == 2) {
        pSum -= ptest;
        if (pSum > iso_ElTkValue){isIsolated = false;}
      } else if (itra == 3) {
        pSum = (int) pSum - 1;
        if (pSum > iso_ElTkValue){isIsolated = false;}
      } else {
        cout << " *** Chosen isolation method is not implemented for electron ***" << endl;
      }
//      if (DEBUGLVL >= 2){
       if (itra < 3 && ival == 1) {
        if (isIsolated){
         cout << " electron is isolated in tracker, Psum = " << pSum << endl;
        } else {
         cout << " electron is not isolated in tracker, Psum = " << pSum << endl;
        }
       } else if (itra < 3 && ival == 2) {
        if (isIsolated){
         cout << " electron is isolated in tracker, Ptsum = " << pSum << endl;
        } else {
         cout << " electron is not isolated in tracker, Ptsum = " << pSum << endl;
        }
       } else if (itra == 3) {
        if (isIsolated){
         cout << " electron is isolated in tracker, #trks = " << pSum << endl;
        } else {
         cout << " electron is not isolated in tracker, #trks = " << pSum << endl;
        }
       }
//      }
    }

    // using a weighted sum of calorimeter and tracker information
    if (idet == 3){
      cout << " *** Chosen isolation method is not implemented for electron ***" << endl;
    }

    // using candidate information
    // uses the cuts defined for tracks
    if (idet == 4){
      float ptest = 0.;
      if (itra == 1) {
        ptest = RecoData[ichk]->pt();
//        cout << ", Pt " << ptest << endl;
      } else if (itra == 2) {
        ptest = RecoData[ichk]->p();
//        cout << ", P " << ptest << endl;
      }
      float elCandEta = RecoData[ichk]->eta();
      float elCandPhi = RecoData[ichk]->phi();
      float pSum = IsoCandSum (itra, elCandEta, elCandPhi, 
                              iso_ElTkDRin, iso_ElTkDRout, iso_ElTkSeed);
      if (itra < 3 && ival == 1) {
        pSum -= ptest;
        if (pSum > iso_ElTkValue*ptest){isIsolated = false;}
      } else if (itra < 3 && ival == 2) {
        pSum -= ptest;
        if (pSum > iso_ElTkValue){isIsolated = false;}
      } else if (itra == 3) {
        pSum = (int) pSum - 1;
        if (pSum > iso_ElTkValue){isIsolated = false;}
      } else {
        cout << " *** Chosen isolation method is not implemented for electron ***" << endl;
      }
//      if (DEBUGLVL >= 2){
       if (itra < 3 && ival == 1) {
        if (isIsolated){
         cout << " electron is isolated from candidates, Psum = " << pSum << endl;
        } else {
         cout << " electron is not isolated from candidates, Psum = " << pSum << endl;
        }
       } else if (itra < 3 && ival == 2) {
        if (isIsolated){
         cout << " electron is isolated from candidates, Ptsum = " << pSum << endl;
        } else {
         cout << " electron is not isolated from candidates, Ptsum = " << pSum << endl;
        }
       } else if (itra == 3) {
        if (isIsolated){
         cout << " electron is isolated from candidates, #trks = " << pSum << endl;
        } else {
         cout << " electron is not isolated from candidates, #trks = " << pSum << endl;
        }
       }
//      }
    }
  }

  else{
    cout << " *** Chosen isolation method is not implemented for electron ***" << endl;
  }

  return isIsolated;

}

//------------------------------------------------------------------------------

bool Isolator::IsoMuon(int ichk)
{
 // Check that muon with index ichk is isolated
 // Several isolation methods should be available
 //  - isolation in a cone or in pT wrt nearest jet
 //  - calorimetric or tracker or weighted sum
 //  - using information in transverse plane or in 3D
 //  - cut on absolute value of Et or ratio or number of objects 
 
  bool isIsolated = true;
//  if (DEBUGLVL >= 2){
    cout << " Muon isolation method = " << iso_MethodMuon << endl;
//  }

  if (RecoData[ichk]->particleType() != 2){
    cout << " *** Particle given " << ichk 
      << " of type " << RecoData[ichk]->particleType()
      << " does not correspond to the type for isolation method IsoMuon" << endl;
    return isIsolated;
  }
  if (iso_MethodMuon == 0){return isIsolated;}

  // compute the isolation in pT wrt nearest jet
  if (iso_MethodMuon / 1000 == 1){
    int iJet = FindNearestJet(ichk);
 
    if (iJet > 0){
//    cout << " nearest jet index = " << iJet 
//         << " type = " << RecoData[iJet]->particleType() << endl;
      if (RecoData[iJet]->energy() < iso_jetbyMuEmin * RecoData[ichk]->energy() )
        {isIsolated = true;}

      float ptwrtJet = GetPtwrtJet(ichk, iJet);
//      cout << " ptwrtJet = " << ptwrtJet << endl;
      if (ptwrtJet > iso_ptMuwrtJetmin){
        isIsolated = true;
        cout << " muon is isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
      } else {
        isIsolated = false;
        cout << " muon is not isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
      }
    }
  }

  // compute the isolation by a cone algorithm
  else if (iso_MethodMuon / 1000 == 2){
    int idet = (iso_MethodMuon - 2000) / 100;
    int itra = (iso_MethodMuon / 100);
    itra = (iso_MethodMuon - 100*itra) / 10;
    int ival = (iso_MethodMuon / 10);
    ival = iso_MethodMuon - 10*ival;
    const Muon* mucand = RecoData[ichk]->muonCandidate();
//    cout << " pointer to muon cand " << mucand;
//    float mumom = mucand->p();
//    cout << " momentum " << mumom << endl;

    const Track* mutrack = &(*(mucand->track()));
//    cout << " pointer to muon track " << mutrack;
    float ptest = 0.;
    if (itra == 1) {
      ptest = mutrack->pt();
//      cout << ", Pt " << ptest << endl;
    } else if (itra == 2) {
      ptest = mutrack->p();
//      cout << ", P " << ptest << endl;
    }

    // using calorimeter information
    if (idet == 1 || idet == 3){
      float muCalEta = mutrack->eta();
      float muCalPhi = mutrack->phi();
      float eSum = IsoCalSum (itra, muCalEta, muCalPhi, 
                              iso_MuCalDRin, iso_MuCalDRout, iso_MuCalSeed);
      if (itra < 3 && ival == 1) {
        if (eSum > iso_MuCalValue*ptest){isIsolated = false;}
      } else if (itra < 3 && ival == 2) {
        if (eSum > iso_MuCalValue){isIsolated = false;}
      } else if (itra == 3) {
        if (eSum > iso_MuCalValue){isIsolated = false;}
      } else {
        cout << " *** Chosen isolation method is not implemented for muon ***" << endl;
      }
//      if (DEBUGLVL >= 2){
       if (itra < 3 && ival == 1) {
        if (isIsolated){
         cout << " muon is isolated in calorimeter, Esum = " << eSum << endl;
        } else {
         cout << " muon is not isolated in calorimeter, Esum = " << eSum << endl;
        }
       } else if (itra < 3 && ival == 2) {
        if (isIsolated){
         cout << " muon is isolated in calorimeter, Etsum = " << eSum << endl;
        } else {
         cout << " muon is not isolated in calorimeter, Etsum = " << eSum << endl;
        }
       } else if (itra == 3) {
        if (isIsolated){
         cout << " muon is isolated in calorimeter, #trks = " << eSum << endl;
        } else {
         cout << " muon is not isolated in calorimeter, #trks = " << eSum << endl;
        }
       }
//      }
    } 

    // using tracker information
    if (idet == 2 || idet == 3){
      float muTrkEta = mutrack->eta();
      float muTrkPhi = mutrack->phi();
      float pSum = IsoTrkSum (itra, muTrkEta, muTrkPhi, 
                              iso_MuTkDRin, iso_MuTkDRout, iso_MuTkSeed);
      if (itra < 3 && ival == 1) {
        pSum -= ptest;
        if (pSum > iso_MuTkValue*ptest){isIsolated = false;}
      } else if (itra < 3 && ival == 2) {
        pSum -= ptest;
        if (pSum > iso_MuTkValue){isIsolated = false;}
      } else if (itra == 3) {
        pSum = (int) pSum - 1;
        if (pSum > iso_MuTkValue){isIsolated = false;}
      } else {
        cout << " *** Chosen isolation method is not implemented for muon ***" << endl;
      }
//      if (DEBUGLVL >= 2){
       if (itra < 3 && ival == 1) {
        if (isIsolated){
         cout << " muon is isolated in tracker, Psum = " << pSum << endl;
        } else {
         cout << " muon is not isolated in tracker, Psum = " << pSum << endl;
        }
       } else if (itra < 3 && ival == 2) {
        if (isIsolated){
         cout << " muon is isolated in tracker, Ptsum = " << pSum << endl;
        } else {
         cout << " muon is not isolated in tracker, Ptsum = " << pSum << endl;
        }
       } else if (itra == 3) {
        if (isIsolated){
         cout << " muon is isolated in tracker, #trks = " << pSum << endl;
        } else {
         cout << " muon is not isolated in tracker, #trks = " << pSum << endl;
        }
       }
//      }
    }

    // using a weighted sum of calorimeter and tracker information
    if (idet == 3){
      cout << " *** Chosen isolation method is not implemented for muon ***" << endl;
    }

    // using candidate information
    // uses the cuts defined for tracks
    if (idet == 4){
      float ptest = 0.;
      if (itra == 1) {
        ptest = RecoData[ichk]->pt();
//        cout << ", Pt " << ptest << endl;
      } else if (itra == 2) {
        ptest = RecoData[ichk]->p();
//        cout << ", P " << ptest << endl;
      }
      float muCandEta = RecoData[ichk]->eta();
      float muCandPhi = RecoData[ichk]->phi();
      float pSum = IsoCandSum (itra, muCandEta, muCandPhi, 
                              iso_MuTkDRin, iso_MuTkDRout, iso_MuTkSeed);
      if (itra < 3 && ival == 1) {
        pSum -= ptest;
        if (pSum > iso_MuTkValue*ptest){isIsolated = false;}
      } else if (itra < 3 && ival == 2) {
        pSum -= ptest;
        if (pSum > iso_MuTkValue){isIsolated = false;}
      } else if (itra == 3) {
        pSum = (int) pSum - 1;
        if (pSum > iso_MuTkValue){isIsolated = false;}
      } else {
        cout << " *** Chosen isolation method is not implemented for muon ***" << endl;
      }
//      if (DEBUGLVL >= 2){
       if (itra < 3 && ival == 1) {
        if (isIsolated){
         cout << " muon is isolated from candidates, Psum = " << pSum << endl;
        } else {
         cout << " muon is not isolated from candidates, Psum = " << pSum << endl;
        }
       } else if (itra < 3 && ival == 2) {
        if (isIsolated){
         cout << " muon is isolated from candidates, Ptsum = " << pSum << endl;
        } else {
         cout << " muon is not isolated from candidates, Ptsum = " << pSum << endl;
        }
       } else if (itra == 3) {
        if (isIsolated){
         cout << " muon is isolated from candidates, #trks = " << pSum << endl;
        } else {
         cout << " muon is not isolated from candidates, #trks = " << pSum << endl;
        }
       }
//      }
    }
  }

  else{
    cout << " *** Chosen isolation method is not implemented for muon ***" << endl;
  }

  return isIsolated;

}

//------------------------------------------------------------------------------

bool Isolator::IsoTau(int ichk)
{
 // Check that electron with index ichk is isolated
 // Several isolation methods should be available
 //  - isolation in a cone or in pT wrt nearest jet
 //  - calorimetric or tracker or weighted sum
 //  - using information in transverse plane or in 3D
 //  - cut on absolute value of Et or ratio or number of objects 
 
  bool isIsolated = true;

  if (RecoData[ichk]->particleType() != 3){
    cout << " *** Particle given " << ichk 
      << " of type " << RecoData[ichk]->particleType()
      << " does not correspond to the type for isolation method IsoTau" << endl;
    return isIsolated;
  }
  if (iso_MethodElec == 0){return isIsolated;}

  // compute the isolation in pT wrt nearest jet
  if (iso_MethodTau / 1000 == 1){
    int iJet = FindNearestJet(ichk);

    if (iJet > 0){
//     cout << " nearest jet index = " << iJet 
//         << " type = " << RecoData[iJet]->particleType() << endl;

      if (RecoData[iJet]->energy() < iso_jetbyTauEmin * RecoData[ichk]->energy() )
        {isIsolated = true;}

      float ptwrtJet = GetPtwrtJet(ichk, iJet);
//      cout << " ptwrtJet = " << ptwrtJet << endl;
      if (ptwrtJet > iso_ptTauwrtJetmin){
        isIsolated = true;
        cout << " tau is isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
      } else {
        isIsolated = false;
        cout << " tau is not isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
      }
    }
  }
  else{
    cout << " *** Chosen isolation method is not implemented for tau ***" << endl;
  }

  return isIsolated;

}

//------------------------------------------------------------------------------

bool Isolator::IsoPhoton(int ichk)
{
 // Check that photon with index ichk is isolated
 // Several isolation methods should be available
 //  - isolation in a cone or in pT wrt nearest jet
 //  - calorimetric or tracker or weighted sum
 //  - using information in transverse plane or in 3D
 //  - cut on absolute value of Et or ratio or number of objects 
 
  bool isIsolated = true;

  if (RecoData[ichk]->particleType() != 4){
    cout << " *** Particle given " << ichk 
      << " of type " << RecoData[ichk]->particleType()
      << " does not correspond to the type for isolation method IsoPhoton" << endl;
    return isIsolated;
  }
  if (iso_MethodPhot == 0){return isIsolated;}

  // compute the isolation in pT wrt nearest jet
  if (iso_MethodPhot / 1000 == 1){
    int iJet = FindNearestJet(ichk);
 
    if (iJet > 0){
//    cout << " nearest jet index = " << iJet 
//         << " type = " << RecoData[iJet]->particleType() << endl;
      if (RecoData[iJet]->energy() < iso_jetbyPhotEmin * RecoData[ichk]->energy() )
        {isIsolated = true;}

      float ptwrtJet = GetPtwrtJet(ichk, iJet);
//      cout << " ptwrtJet = " << ptwrtJet << endl;
      if (ptwrtJet > iso_ptPhotwrtJetmin){
        isIsolated = true;
        cout << " photon is isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
      } else {
        isIsolated = false;
        cout << " photon is not isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
      }
    }
  }
  else{
    cout << " *** Chosen isolation method is not implemented for photon ***" << endl;
  }

  return isIsolated;

}

//------------------------------------------------------------------------------

float Isolator::IsoCalSum (int itra, float CalEta, float CalPhi, 
                 float iso_DRin, float iso_DRout, float iso_Seed){
// Computes the sum inside a cone from R=iso_ElDRin to R=iso_ElDRout
// using 4-vectors stored in CaloTower objects
// if itra = 1 : sum of Pt or Et
//         = 2 : sum of P or E
//         = 3 : number of calotowers accepted

  float sum = 0.;
  for (int i = 0; i < (int) CaloTowerData->size(); i++) {
    if ((*CaloTowerData)[i].energy() > iso_Seed){
      float eta = (*CaloTowerData)[i].eta();
      float phi = (*CaloTowerData)[i].phi();
      float DR = GetDeltaR(CalEta, eta, CalPhi, phi);
      if (DR <= 0.) {DR = 0.001;}
      if (DR > iso_DRin && DR < iso_DRout){
        if (itra == 1){
//        float theta = 2. * atan(exp(-eta)); // pseudorapidity or rapidity?
//        float pt = (*CaloTowerData)[i].energy() * sin(theta);
          float pt = (*CaloTowerData)[i].energy() / cosh(eta);
          sum += pt;
        } else if (itra == 2){
          sum += (*CaloTowerData)[i].energy();
        } else if (itra == 3){
          sum = (int) sum + 1;
        } else{
         cout << " *** method " << itra
          << " for IsoCalSum not implemented ***" << endl;
        }
      }
    }
  }
  
  return sum;

}

//------------------------------------------------------------------------------

float Isolator::IsoTrkSum (int itra, float TrkEta, float TrkPhi, 
                 float iso_DRin, float iso_DRout, float iso_Seed){
// Computes the sum inside a cone from R=iso_DRin to R=iso_DRout
// using 4-vectors stored in Track objects
// if itra = 1 : sum of Pt or Et
//         = 2 : sum of P or E
//         = 3 : number of objects accepted

  float sum = 0.;
  for (int i = 0; i < (int) TrackData->size(); i++) {
    if ((*TrackData)[i].p() > iso_Seed){
      float eta = (*TrackData)[i].eta();
      float phi = (*TrackData)[i].phi();
      float DR = GetDeltaR(TrkEta, eta, TrkPhi, phi);
      if (DR <= 0.) {DR = 0.001;}
      if (DR > iso_DRin && DR < iso_DRout){
        if (itra == 1){
          sum += (*TrackData)[i].pt();
        } else if (itra == 2){
          sum += (*TrackData)[i].p();
        } else if (itra == 3){
          sum = (int) sum + 1;
        } else{
         cout << " *** method " << itra
          << " for IsoTrkSum not implemented ***" << endl;
        }
      }
    }
  }
  
  return sum;

}

//------------------------------------------------------------------------------

float Isolator::IsoCandSum (int itra, float TrkEta, float TrkPhi, 
                 float iso_DRin, float iso_DRout, float iso_Seed){
// Computes the sum inside a cone from R=iso_DRin to R=iso_DRout
// using 4-vectors stored in MrParticle objects
// uses the same cuts as IsoTrkSum
// if itra = 1 : sum of Pt or Et
//         = 2 : sum of P or E
//         = 3 : number of objects accepted

// CAUTION: loops over the MrParticle objects with 1st acceptance cuts applied
//          -> energy is underestimated

  float sum = 0.;
  for (int i = 0; i < (int) RecoData.size(); i++) {
    if (RecoData[i]->p() > iso_Seed){
      float eta = RecoData[i]->eta();
      float phi = RecoData[i]->phi();
      float DR = GetDeltaR(TrkEta, eta, TrkPhi, phi);
      if (DR <= 0.) {DR = 0.001;}
      if (DR > iso_DRin && DR < iso_DRout){
        if (itra == 1){
          sum += RecoData[i]->pt();
        } else if (itra == 2){
          sum += RecoData[i]->p();
        } else if (itra == 3){
          sum = (int) sum + 1;
        } else{
         cout << " *** method " << itra
          << " for IsoCandSum not implemented ***" << endl;
        }
      }
    }
  }
  
  return sum;

}

//------------------------------------------------------------------------------

bool Isolator::IsObjectMerged(int ichk, bool isIsolated)
{
// Checks whether an object is included in the nearest jet
// if yes and object is isolated, removes it from the jet 4-vector
// if no and object is not isolated, adds it to the jet 4-vector
// returns whether or not it merged the object with the jet
   
//  cout << " Merging Particle = " << ichk 
//       << " type = " << RecoData[ichk]->particleType() << endl;

  bool isMerged = false;
  if (ichk < 0) {return isMerged;}

  int iJet = FindNearestJet(ichk);
  if (iJet < 0) {return isMerged;}
  
  bool isInJet = false;
//  cout << " nearest jet index = " << iJet 
//       << " type = " << RecoData[iJet]->particleType() << endl;
 
   if (RecoData[ichk]->particleType() == 1){
     isInJet = IsElecInJet(ichk, iJet);
   }
   else if (RecoData[ichk]->particleType() == 2){
     isInJet = IsMuonInJet(ichk, iJet);
   }
   else if (RecoData[ichk]->particleType() == 3){
     isInJet = IsTauInJet(ichk, iJet);
   }
   else if (RecoData[ichk]->particleType() == 4){
     isInJet = IsPhotonInJet(ichk, iJet);
   }
   if (DEBUGLVL >= 2){
     if (isInJet){
       cout << " part " << ichk << " type = " << RecoData[ichk]->particleType()
            << "  is in jet " << iJet << endl;}
     else {
       cout << " part " << ichk << " type = " << RecoData[ichk]->particleType()
            << "  is not in jet " << iJet << endl;}
   }
  
  if (isIsolated && isInJet){
    RecoData[iJet]->setPx(RecoData[iJet]->px() - RecoData[ichk]->px());
    RecoData[iJet]->setPy(RecoData[iJet]->py() - RecoData[ichk]->py());
    RecoData[iJet]->setPz(RecoData[iJet]->pz() - RecoData[ichk]->pz());
    RecoData[iJet]->setEnergy(RecoData[iJet]->energy() - RecoData[ichk]->energy());
   // ??? or do we want to keep the jets massless?
   //RecoData[iJet]->setEnergy(RecoData[iJet]->p() );
    if (DEBUGLVL >= 2){
      cout << "  subtracted from jet " << endl;
    }
    if(RecoData[iJet]->energy() < RecoData[iJet]->p() ){
     float egy = RecoData[iJet]->energy();
     if (egy <= 0.){ egy = 0.001;}
     float scale = egy / RecoData[iJet]->p();
     RecoData[iJet]->setPx(RecoData[iJet]->px() * scale);
     RecoData[iJet]->setPy(RecoData[iJet]->py() * scale);
     RecoData[iJet]->setPz(RecoData[iJet]->pz() * scale);
    }
    isMerged = false;
  }
  else if (!isIsolated && !isInJet){
    RecoData[iJet]->setPx(RecoData[iJet]->px() + RecoData[ichk]->px());
    RecoData[iJet]->setPy(RecoData[iJet]->py() + RecoData[ichk]->py());
    RecoData[iJet]->setPz(RecoData[iJet]->pz() + RecoData[ichk]->pz());
    RecoData[iJet]->setEnergy(RecoData[iJet]->energy() + RecoData[ichk]->energy());
   // ??? or do we want to keep the jets massless?
   //RecoData[iJet]->setEnergy(RecoData[iJet]->p() );
    if (DEBUGLVL >= 2){
      cout << "  added to jet " << endl;
    }
    isMerged = true;
  }
  
  return isMerged;

}

//------------------------------------------------------------------------------

bool Isolator::IsElecInJet(int ichk, int iJet)
{
 // Check whether an electron is included in the jet energy

  return false;

}

//------------------------------------------------------------------------------

bool Isolator::IsMuonInJet(int ichk, int iJet)
{
 // Check whether a muon is included in the jet energy

  return false;

}

//------------------------------------------------------------------------------

bool Isolator::IsTauInJet(int ichk, int iJet)
{
 // Check whether a tau is included in the jet energy

  return false;

}

//------------------------------------------------------------------------------

bool Isolator::IsPhotonInJet(int ichk, int iJet)
{
 // Check whether a photon is included in the jet energy

  return false;

}

//------------------------------------------------------------------------------

int Isolator::FindNearestJet(int ichk)
{
// Looks for the nearest jet in deltaR to a given object
// and returns its RecoData index 
// returns -1 if no nearest jet

  int iJetMin = -1;
  if (ichk < 0){return iJetMin;}
  
  float deltaRmin = 999.;
  for(int i = 0; i < (int) RecoData.size(); i++){
   if(RecoData[i]->particleType() >= 5){
    float deltaR = GetDeltaR(RecoData[ichk]->eta(), RecoData[i]->eta(), 
                             RecoData[ichk]->phi(), RecoData[i]->phi());
    if (deltaR < deltaRmin && i != ichk){
      deltaRmin = deltaR;
      iJetMin = i;
    }
   }
  }
  
  return iJetMin;

}

//------------------------------------------------------------------------------

float Isolator::GetPtwrtJet(int ichk, int iJet)
{
// Computes the Pt of object ichk wrt the direction of object iJet
  
    if (ichk < 0 || iJet < 0){return -1.;}

    float plwrtJet = (RecoData[ichk]->px()*RecoData[iJet]->px() +
                      RecoData[ichk]->py()*RecoData[iJet]->py() +
                      RecoData[ichk]->pz()*RecoData[iJet]->pz() )
                    / RecoData[iJet]->p();
//    cout << " plwrtJet = " << plwrtJet
//         << " cosTheta = " << plwrtJet/RecoData[ichk]->p() << endl;
    float ptwrtJet = sqrt(RecoData[ichk]->p()*RecoData[ichk]->p() -
                          plwrtJet*plwrtJet);
//    cout << " ptwrtJet = " << ptwrtJet << endl;

 return ptwrtJet;

}

//------------------------------------------------------------------------------

float Isolator::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float Isolator::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


