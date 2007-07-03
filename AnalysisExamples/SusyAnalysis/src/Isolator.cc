#include "AnalysisExamples/SusyAnalysis/interface/Isolator.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

Isolator::Isolator(MrEvent* pEvtData):
SusyRecoTools(pEvtData), 
//RecoData(*pData), TrackData(Tracks), CaloTowerData(CaloTowers),
//DEBUGLVL(0),
iso_MethodElec(1100), iso_jetbyElEmin(1.), iso_ptElwrtJetmin(7.),
iso_ElCalDRin(0.), iso_ElCalDRout(0.1), iso_ElCalSeed(0.1), 
iso_ElTkDRin(0.), iso_ElTkDRout(0.1), iso_ElTkSeed(0.1), 
iso_ElCalWeight(0.75), iso_ElIsoValue(0.5),
iso_MethodMuon(1100), iso_jetbyMuEmin(1.), iso_ptMuwrtJetmin(7.),
iso_MuCalDRin(0.), iso_MuCalDRout(0.1), iso_MuCalSeed(0.1),
iso_MuTkDRin(0.), iso_MuTkDRout(0.1), iso_MuTkSeed(0.1),
iso_MuCalWeight(0.75), iso_MuIsoValue(0.5),
iso_MethodTau(1100), iso_jetbyTauEmin(1.), iso_ptTauwrtJetmin(7.),
iso_MethodPhot(1100), iso_jetbyPhotEmin(1.), iso_ptPhotwrtJetmin(7.),
iso_PhCalDRin(0.), iso_PhCalDRout(0.1), iso_PhCalSeed(0.1), 
iso_PhTkDRin(0.), iso_PhTkDRout(0.1), iso_PhTkSeed(0.1), 
iso_PhCalWeight(0.75), iso_PhIsoValue(0.5),
iso_MethodUfo(1100), iso_jetbyUfoEmin(1.), iso_ptUfowrtJetmin(7.),
iso_UfoCalDRin(0.), iso_UfoCalDRout(0.1), iso_UfoCalSeed(0.1), 
iso_UfoTkDRin(0.), iso_UfoTkDRout(0.1), iso_UfoTkSeed(0.1), 
iso_UfoCalWeight(0.75), iso_UfoIsoValue(0.5)
{}

Isolator::Isolator(MrEvent* pEvtData, edm::ParameterSet param):
SusyRecoTools(pEvtData) 
//RecoData(*pData), TrackData(Tracks), CaloTowerData(CaloTowers),
//DEBUGLVL(0)
{
iso_MethodElec = param.getParameter<int>("iso_MethodElec") ;
iso_jetbyElEmin = param.getParameter<double>("iso_jetbyElEmin") ;
iso_ptElwrtJetmin = param.getParameter<double>("iso_ptElwrtJetmin") ;
iso_ElCalDRin = param.getParameter<double>("iso_ElCalDRin") ;
iso_ElCalDRout = param.getParameter<double>("iso_ElCalDRout") ;
iso_ElCalSeed = param.getParameter<double>("iso_ElCalSeed") ;
iso_ElTkDRin = param.getParameter<double>("iso_ElTkDRin") ;
iso_ElTkDRout = param.getParameter<double>("iso_ElTkDRout") ;
iso_ElTkSeed = param.getParameter<double>("iso_ElTkSeed") ;
iso_ElCalWeight = param.getParameter<double>("iso_ElCalWeight") ;
iso_ElIsoValue = param.getParameter<double>("iso_ElIsoValue") ;
iso_MethodMuon = param.getParameter<int>("iso_MethodMuon") ;
iso_jetbyMuEmin = param.getParameter<double>("iso_jetbyMuEmin") ;
iso_ptMuwrtJetmin = param.getParameter<double>("iso_ptMuwrtJetmin") ;
iso_MuCalDRin = param.getParameter<double>("iso_MuCalDRin") ;
iso_MuCalDRout = param.getParameter<double>("iso_MuCalDRout") ;
iso_MuCalSeed = param.getParameter<double>("iso_MuCalSeed") ;
iso_MuTkDRin = param.getParameter<double>("iso_MuTkDRin") ;
iso_MuTkDRout = param.getParameter<double>("iso_MuTkDRout") ;
iso_MuTkSeed = param.getParameter<double>("iso_MuTkSeed") ;
iso_MuCalWeight = param.getParameter<double>("iso_MuCalWeight") ;
iso_MuIsoValue = param.getParameter<double>("iso_MuIsoValue") ;
iso_MethodTau = param.getParameter<int>("iso_MethodTau") ;
iso_jetbyTauEmin = param.getParameter<double>("iso_jetbyTauEmin") ;
iso_ptTauwrtJetmin = param.getParameter<double>("iso_ptTauwrtJetmin") ;
iso_TauCalDRin = param.getParameter<double>("iso_TauCalDRin") ;
iso_TauCalDRout = param.getParameter<double>("iso_TauCalDRout") ;
iso_TauCalSeed = param.getParameter<double>("iso_TauCalSeed") ;
iso_TauTkDRin = param.getParameter<double>("iso_TauTkDRin") ;
iso_TauTkDRout = param.getParameter<double>("iso_TauTkDRout") ;
iso_TauTkSeed = param.getParameter<double>("iso_TauTkSeed") ;
iso_TauCalWeight = param.getParameter<double>("iso_TauCalWeight") ;
iso_TauIsoValue = param.getParameter<double>("iso_TauIsoValue") ;
iso_MethodPhot = param.getParameter<int>("iso_MethodPhot") ;
iso_jetbyPhotEmin = param.getParameter<double>("iso_jetbyPhotEmin") ;
iso_ptPhotwrtJetmin = param.getParameter<double>("iso_ptPhotwrtJetmin") ;
iso_PhCalDRin = param.getParameter<double>("iso_PhCalDRin") ;
iso_PhCalDRout = param.getParameter<double>("iso_PhCalDRout") ;
iso_PhCalSeed = param.getParameter<double>("iso_PhCalSeed") ;
iso_PhTkDRin = param.getParameter<double>("iso_PhTkDRin") ;
iso_PhTkDRout = param.getParameter<double>("iso_PhTkDRout") ;
iso_PhTkSeed = param.getParameter<double>("iso_PhTkSeed") ;
iso_PhCalWeight = param.getParameter<double>("iso_PhCalWeight") ;
iso_PhIsoValue = param.getParameter<double>("iso_PhIsoValue") ;
iso_MethodUfo = param.getParameter<int>("iso_MethodUfo") ;
iso_jetbyUfoEmin = param.getParameter<double>("iso_jetbyUfoEmin") ;
iso_ptUfowrtJetmin = param.getParameter<double>("iso_ptUfowrtJetmin") ;
iso_UfoCalDRin = param.getParameter<double>("iso_UfoCalDRin") ;
iso_UfoCalDRout = param.getParameter<double>("iso_UfoCalDRout") ;
iso_UfoCalSeed = param.getParameter<double>("iso_UfoCalSeed") ;
iso_UfoTkDRin = param.getParameter<double>("iso_UfoTkDRin") ;
iso_UfoTkDRout = param.getParameter<double>("iso_UfoTkDRout") ;
iso_UfoTkSeed = param.getParameter<double>("iso_UfoTkSeed") ;
iso_UfoCalWeight = param.getParameter<double>("iso_UfoCalWeight") ;
iso_UfoIsoValue = param.getParameter<double>("iso_UfoIsoValue") ;
}

// Methods:

bool Isolator::IsObjectIsolated(int ichk)
{
   
   bool isIsolated = true;
   if (ichk < 0){return isIsolated;}

   int ptype = RecoData[ichk]->particleType()/10;
   if (ptype == 1){
     isIsolated = IsoObject(ichk, 1);
   }
   else if (ptype == 2){
     isIsolated = IsoObject(ichk, 2);
   }
   else if (ptype == 3){
     isIsolated = IsoObject(ichk, 3);
   }
   else if (ptype == 4){
     isIsolated = IsoObject(ichk, 4);
   }
   else if (ptype == 8){
     isIsolated = IsoObject(ichk, 8);
   }
  
  return isIsolated;

}

//------------------------------------------------------------------------------

bool Isolator::IsoObject(int ichk, int itype)
{
 // Check that object with index ichk is isolated
 // Several isolation methods are available, defined by iso_Methodxxx
 // decoded in the following integers:
 //  - imeth = 1 if isolation in pT wrt nearest jet, = 2 if in a cone
 //  - idet = 1 if calorimetric, = 2 if tracker, = 3 if weighted sum
 //  - itra = 1 if using Et, = 2 if E, = 3 if number of objects
 //  - ival = 1 if cut on relative value of Et/E sum, = 2 if absolute value
 
 //parameters defined from the cfg file:
  int iso_MethodObj = 0;
  float iso_jetbyObjEmin = 0., iso_ptObjwrtJetmin = 0., 
        iso_ObjCalDRin = 0., iso_ObjCalDRout = 0., iso_ObjCalSeed = 0., 
        iso_ObjTkDRin = 0., iso_ObjTkDRout = 0., iso_ObjTkSeed = 0.,
        iso_ObjCalWeight = 0., iso_ObjIsoValue = 0.;
 
  int iElecFirst = 9999, iMuonFirst = 9999;
  int iTauFirst = 9999, iPhotFirst = 9999, iUfoFirst = 9999;
 
  bool isIsolated = true;
  iso_MethodObj = 0;

  int ptype = RecoData[ichk]->particleType()/10;
  if (ptype != itype){
    cout << " *** Particle given " << ichk 
      << " of type " << RecoData[ichk]->particleType()
      << " does not correspond to the type for isolation method " << itype << endl;
    return isIsolated;
  }
  
  if (ptype == 1){
    iso_MethodObj = iso_MethodElec;
    iso_jetbyObjEmin = iso_jetbyElEmin;
    iso_ptObjwrtJetmin = iso_ptElwrtJetmin;
    iso_ObjCalDRin = iso_ElCalDRin;
    iso_ObjCalDRout = iso_ElCalDRout;
    iso_ObjCalSeed = iso_ElCalSeed;
    iso_ObjTkDRin = iso_ElTkDRin;
    iso_ObjTkDRout = iso_ElTkDRout;
    iso_ObjTkSeed = iso_ElTkSeed;
    iso_ObjCalWeight = iso_ElCalWeight;
    iso_ObjIsoValue = iso_ElIsoValue;
    if (DEBUGLVL >= 2 && ichk < iElecFirst){
      iElecFirst = ichk;
      cout << " Electron isolation method = " << iso_MethodElec << endl;
    }
  }
  else if (ptype == 2){
    iso_MethodObj = iso_MethodMuon;
    iso_jetbyObjEmin = iso_jetbyMuEmin;
    iso_ptObjwrtJetmin = iso_ptMuwrtJetmin;
    iso_ObjCalDRin = iso_MuCalDRin;
    iso_ObjCalDRout = iso_MuCalDRout;
    iso_ObjCalSeed = iso_MuCalSeed;
    iso_ObjTkDRin = iso_MuTkDRin;
    iso_ObjTkDRout = iso_MuTkDRout;
    iso_ObjTkSeed = iso_MuTkSeed;
    iso_ObjCalWeight = iso_MuCalWeight;
    iso_ObjIsoValue = iso_MuIsoValue;
    if (DEBUGLVL >= 2 && ichk < iMuonFirst){
      iMuonFirst = ichk;
      cout << " Muon isolation method = " << iso_MethodMuon << endl;
    }
  }
  else if (ptype == 3){
    iso_MethodObj = iso_MethodTau;
    iso_jetbyObjEmin = iso_jetbyTauEmin;
    iso_ptObjwrtJetmin = iso_ptTauwrtJetmin;
    iso_ObjCalDRin = iso_TauCalDRin;
    iso_ObjCalDRout = iso_TauCalDRout;
    iso_ObjCalSeed = iso_TauCalSeed;
    iso_ObjTkDRin = iso_TauTkDRin;
    iso_ObjTkDRout = iso_TauTkDRout;
    iso_ObjTkSeed = iso_TauTkSeed;
    iso_ObjCalWeight = iso_TauCalWeight;
    iso_ObjIsoValue = iso_TauIsoValue;
    if (DEBUGLVL >= 2 && ichk < iTauFirst){
      iTauFirst = ichk;
      cout << " Tau isolation method = " << iso_MethodTau << endl;
    }
  }
  else if (ptype == 4){
    iso_MethodObj = iso_MethodPhot;
    iso_jetbyObjEmin = iso_jetbyPhotEmin;
    iso_ptObjwrtJetmin = iso_ptPhotwrtJetmin;
    iso_ObjCalDRin = iso_PhCalDRin;
    iso_ObjCalDRout = iso_PhCalDRout;
    iso_ObjCalSeed = iso_PhCalSeed;
    iso_ObjTkDRin = iso_PhTkDRin;
    iso_ObjTkDRout = iso_PhTkDRout;
    iso_ObjTkSeed = iso_PhTkSeed;
    iso_ObjCalWeight = iso_PhCalWeight;
    iso_ObjIsoValue = iso_PhIsoValue;
    if (DEBUGLVL >= 2 && ichk < iPhotFirst){
      iPhotFirst = ichk;
      cout << " Photon isolation method = " << iso_MethodPhot << endl;
    }
  }
  else if (ptype == 8){
    iso_MethodObj = iso_MethodUfo;
    iso_jetbyObjEmin = iso_jetbyUfoEmin;
    iso_ptObjwrtJetmin = iso_ptUfowrtJetmin;
    iso_ObjCalDRin = iso_UfoCalDRin;
    iso_ObjCalDRout = iso_UfoCalDRout;
    iso_ObjCalSeed = iso_UfoCalSeed;
    iso_ObjTkDRin = iso_UfoTkDRin;
    iso_ObjTkDRout = iso_UfoTkDRout;
    iso_ObjTkSeed = iso_UfoTkSeed;
    iso_ObjCalWeight = iso_UfoCalWeight;
    iso_ObjIsoValue = iso_UfoIsoValue;
    if (DEBUGLVL >= 2 && ichk < iUfoFirst){
      iUfoFirst = ichk;
      cout << " UFO isolation method = " << iso_MethodUfo << endl;
    }
  }
  
  if (iso_MethodObj == 0){return isIsolated;}
  
  // Decode the isolation method:
  // imeth = method: 1 for Pt iso, 2 for cone iso
  // idet = 1 for calo iso, = 2 for tracker iso, = 3 for weighted sum
  // itra = 1 for Et sum, = 2 for E sum, = 3 for #objects sum
  // ival = 1 if cut on relative value of Et/E sum, = 2 if absolute value
  int imeth = iso_MethodObj / 1000;
  int idet = (iso_MethodObj - 2000) / 100;
  int itra = (iso_MethodObj / 10) % 10;
  int ival = (iso_MethodObj % 10);
  if (imeth == 2) {
    if (idet < 1 || idet > 4 || itra < 1 || itra > 3 || ival < 1 || ival > 2
        || (itra == 3 && ival != 2) ) {
      cout << " *** Chosen isolation method " << iso_MethodObj
           << " is not implemented ***" << endl;
      return isIsolated;
    }
  }

  // compute the isolation in pT wrt nearest jet
  if (imeth == 1){
    int iJet = FindNearestJet(ichk);
   
    if (iJet >= 0){
//      cout << " nearest jet index = " << iJet 
//         << " type = " << RecoData[iJet]->particleType() << endl;

      if (RecoData[iJet]->energy() < iso_jetbyObjEmin * RecoData[ichk]->energy() )
        {isIsolated = true;}

      float ptwrtJet = GetPtwrtJet(ichk, iJet);
      RecoData[ichk]->setParticleIsoValue(ptwrtJet);
      if (ptwrtJet > iso_ptObjwrtJetmin){
        isIsolated = true;
        if (DEBUGLVL >= 2){
          cout << " object index = " << ichk
          << "type = " << RecoData[ichk]->particleType()
          << " is isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
        }
      } else {
        isIsolated = false;
        if (DEBUGLVL >= 2){
          cout << " object index = " << ichk
          << "type = " << RecoData[ichk]->particleType()
          << " is not isolated by Pt wrt jet, Pt = " << ptwrtJet << endl;
        }
      }
    }
  }

  // compute the isolation by a cone algorithm
  else if (imeth == 2){
  
    // Collect the information necessary for the calculation of isolation
    float eSum = 0., etest = 0., eSubtr = 0.;
    int ntkCalSubtr = 0;
    float calEta = 0., calPhi = 0.;
    float pSum = 0., ptest = 0., pSubtr = 0.;
    int ntkTrkSubtr = 0;
    float trkEta = 0., trkPhi = 0.;
    
    if (ptype == 1){
      // for Calo isolation
      const PixelMatchGsfElectron* elecand = RecoData[ichk]->electronCandidate();
      const SuperCluster* supercluster = &(*(elecand->superCluster()));
      if (itra == 1) {
        float eta = supercluster->eta();
        float theta = 2. * atan(exp(-eta)); // pseudorapidity or rapidity?
        etest = supercluster->energy() * fabs(sin(theta));
      } else if (itra == 2) {
        etest = supercluster->energy();
      } else if (itra == 3) {
        vector<CaloTowerDetId> eleDetId;
        GetEMCaloTowers(ichk, & eleDetId);
        etest = eleDetId.size();
        eleDetId.clear();
      }
      eSubtr = etest;
      ntkCalSubtr = (int)etest;
      calEta = supercluster->eta();
      calPhi = supercluster->phi();
      // for Tracker isolation
      const Track* eletrack = &(*(elecand->gsfTrack()));
      if (itra == 1) {
        ptest = eletrack->pt();
      } else if (itra == 2) {
        ptest = eletrack->p();
      } else if (itra == 3) {
        ptest = 1.;
      }
      pSubtr = ptest;
      ntkTrkSubtr = 1;
      trkEta = eletrack->eta();
      trkPhi = eletrack->phi();
    }
    else if (ptype == 2){
      // for Calo isolation
      const Muon* mucand = RecoData[ichk]->muonCandidate();
      const Track* mutrack = &(*(mucand->track()));
      if (itra == 1) {
        etest = mutrack->pt();
      } else if (itra == 2) {
        etest = mutrack->p();
      } else if (itra == 3) {
        etest = 0.;
      }
      eSubtr = 0.;
      ntkCalSubtr = 0;
//      calEta = mutrack->outereta();
//      calPhi = mutrack->outerphi();
      calEta = mutrack->eta();
      calPhi = mutrack->phi();
      // for Tracker isolation
      if (itra == 1) {
        ptest = mutrack->pt();
      } else if (itra == 2) {
        ptest = mutrack->p();
      } else if (itra == 3) {
        ptest = 1.;
      }
      pSubtr = ptest;
      ntkTrkSubtr = 1;
      trkEta = mutrack->eta();
      trkPhi = mutrack->phi();
    }
    else if (ptype == 3){
      // for Calo isolation
      if (itra == 1) {
        etest = RecoData[ichk]->pt();
      } else if (itra == 2) {
        etest = RecoData[ichk]->p();
      } else if (itra == 3) {
        etest = 0.;
      }
      eSubtr = 0.;
      ntkCalSubtr = 0;
      calEta = RecoData[ichk]->eta();
      calPhi = RecoData[ichk]->phi();
      // for Tracker isolation
      if (itra == 1) {
        ptest = RecoData[ichk]->pt();
      } else if (itra == 2) {
        ptest = RecoData[ichk]->p();
      } else if (itra == 3) {
        ptest = 0.;
      }
      pSubtr = 0.;
      ntkTrkSubtr = 0;
      trkEta = RecoData[ichk]->eta();
      trkPhi = RecoData[ichk]->phi();
    }
    else if (ptype == 4){
      const Photon* photcand = RecoData[ichk]->photonCandidate();
      if (photcand != NULL){
        const SuperCluster* supercluster = &(*(photcand->superCluster()));
        etest = supercluster->energy();
        calEta = supercluster->eta();
        calPhi = supercluster->phi();
      } else {
        etest = RecoData[ichk]->energy();
        calEta = RecoData[ichk]->eta();
        calPhi = RecoData[ichk]->phi();
      }
      if (itra == 1) {
        float theta = 2. * atan(exp(-calEta)); // pseudorapidity or rapidity?
        etest = etest * fabs(sin(theta));
      } else if (itra == 3) {
        vector<CaloTowerDetId> eleDetId;
        GetEMCaloTowers(ichk, & eleDetId);
        etest = eleDetId.size();
        eleDetId.clear();
      }
      eSubtr = etest;
      ntkCalSubtr = (int)etest;
      ptest = etest;
      pSubtr = 0.;
      ntkTrkSubtr = 0;
      trkEta = calEta;
      trkPhi = calPhi;
    }
    else if (ptype == 8){
      // for Calo isolation
      const Track* ufotrack = RecoData[ichk]->ufoTrack();
      if (itra == 1) {
        etest = ufotrack->pt();
      } else if (itra == 2) {
        etest = ufotrack->p();
      } else if (itra == 3) {
        etest = 0.;
      }
      eSubtr = 0.;
      ntkCalSubtr = 0;
//      calEta = ufotrack->outereta();
//      calPhi = ufotrack->outerphi();
      calEta = ufotrack->eta();
      calPhi = ufotrack->phi();
      // for Tracker isolation
      if (itra == 1) {
        ptest = ufotrack->pt();
      } else if (itra == 2) {
        ptest = ufotrack->p();
      } else if (itra == 3) {
        ptest = 1.;
      }
      pSubtr = ptest;
      ntkTrkSubtr = 1;
      trkEta = ufotrack->eta();
      trkPhi = ufotrack->phi();
    }
    
    // using calorimeter information
    if (idet == 1 || idet == 3){
      eSum = IsoCalSum (itra, calEta, calPhi, 
                              iso_ObjCalDRin, iso_ObjCalDRout, iso_ObjCalSeed);
//    cout << " Iso etest " << etest << " esum " << eSum << " eSubtr " << eSubtr << endl;
      if (itra < 3 && ival == 1) {
        if (iso_ObjCalDRin == 0.) {eSum = (eSum-eSubtr) / etest;}
        else{eSum = eSum / etest;}
      } else if (itra < 3 && ival == 2) {
        if (iso_ObjCalDRin == 0.) {eSum -= eSubtr;}
      } else if (itra == 3) {
        if (iso_ObjCalDRin == 0.) {eSum = (int) eSum - ntkCalSubtr;}
      }
      
      RecoData[ichk]->setParticleIsoValue(eSum);
      if (idet == 1 && eSum > iso_ObjIsoValue){isIsolated = false;}
      if (idet == 1 && DEBUGLVL >= 2){
        if (isIsolated){
         cout << " object index = " << ichk
         << " type = " << RecoData[ichk]->particleType()
         << " is isolated in calorimeter, Esum = " << eSum << endl;
        } else {
         cout << " object index = " << ichk
         << " type = " << RecoData[ichk]->particleType()
         << " is not isolated in calorimeter, Esum = " << eSum << endl;
        }
      }
    }

    // using tracker information
    if (idet == 2 || idet == 3){
      pSum = IsoTrkSum (itra, trkEta, trkPhi, 
                              iso_ObjTkDRin, iso_ObjTkDRout, iso_ObjTkSeed);
//    cout << " Iso ptest " << ptest << " psum " << pSum << " pSubtr " << pSubtr << endl;
      if (itra < 3 && ival == 1) {
        if (iso_ObjTkDRin == 0.) {pSum = (pSum-pSubtr) / ptest;}
        else {pSum = pSum / ptest;}
      } else if (itra < 3 && ival == 2) {
        if (iso_ObjTkDRin == 0.) {pSum -= pSubtr;}
      } else if (itra == 3) {
        if (iso_ObjTkDRin == 0.) {pSum = (int) pSum - ntkTrkSubtr;}
      }
      
      RecoData[ichk]->setParticleIsoValue(pSum);
      if (idet == 2 && pSum > iso_ObjIsoValue){isIsolated = false;}
      if (idet == 2 && DEBUGLVL >= 2){
        if (isIsolated){
         cout << " object index = " << ichk 
         << " type = " << RecoData[ichk]->particleType()
         << " is isolated in tracker, Psum = " << pSum << endl;
        } else {
         cout << " object index = " << ichk
         << " type = " << RecoData[ichk]->particleType()
         << " is not isolated in tracker, Psum = " << pSum << endl;
        }
      }
    }

    // using a weighted sum of calorimeter and tracker information
    if (idet == 3){
      pSum = pSum + iso_ObjCalWeight*eSum;
      RecoData[ichk]->setParticleIsoValue(pSum);
      if (pSum > iso_ObjIsoValue){isIsolated = false;}
      if (DEBUGLVL >= 2){
        if (isIsolated){
         cout << " object index = " << ichk
         << "type = " << RecoData[ichk]->particleType()
         << " is isolated in tracker+calo, Psum = " << pSum << endl;
        } else {
         cout << " object index = " << ichk
         << "type = " << RecoData[ichk]->particleType()
         << " is not isolated in tracker+calo, Psum = " << pSum << endl;
        }
      }
    }

    // using candidate information, not recommended, not advertized
    // uses the cuts defined for tracks
    if (idet == 4){
      if (itra == 1) {
        ptest = RecoData[ichk]->pt();
      } else if (itra == 2) {
        ptest = RecoData[ichk]->p();
      }
      pSubtr = 0.;
      if (RecoData[ichk]->particleType()/10 != 4){pSubtr = ptest;}
      trkEta = RecoData[ichk]->eta();
      trkPhi = RecoData[ichk]->phi();
      float pSum = IsoCandSum (itra,trkEta , trkPhi, 
                              iso_ObjTkDRin, iso_ObjTkDRout, iso_ObjTkSeed);
      if (itra < 3 && ival == 1) {
        if (iso_ObjTkDRin == 0.) {pSum = (pSum-pSubtr) / ptest;}
        else {pSum = pSum / ptest;}
      } else if (itra < 3 && ival == 2) {
        if (iso_ObjTkDRin == 0.) {pSum -= pSubtr;}
      } else if (itra == 3) {
        if (iso_ObjTkDRin == 0.) {pSum = (int) pSum - ntkTrkSubtr;}
      }
      RecoData[ichk]->setParticleIsoValue(pSum);
      if (pSum > iso_ObjIsoValue){isIsolated = false;}
    }
  }

  return isIsolated;

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
  
  int ptype = RecoData[ichk]->particleType()/10;
  if (ptype >= 5 && ptype <= 7) {return isMerged;}

  int iJet = FindNearestJet(ichk);
  if (iJet < 0) {return isMerged;}
  
  bool isInJet = false;
//  cout << " nearest jet index = " << iJet 
//       << " type = " << RecoData[iJet]->particleType() << endl;
 
   math::XYZVector sharedP(0., 0., 0.);
   float sharedE = 0.;
   float sharedEt_em = 0.;
   float sharedEt_had = 0.;
   float sharedPt_tracks = 0.;
   
   if (ptype == 1){
//     if (isIsolated){cout << endl; 
//       cout << "  Type = " << RecoData[ichk]->particleType() << "  Is isolated " << endl; }
//     else {cout << endl; 
//       cout << "  Type = " << RecoData[ichk]->particleType() << "  Is not isolated " << endl; }
     isInJet = IsEMObjectInJet(ichk, iJet, & sharedP);
     sharedE = sqrt(sharedP.X()*sharedP.X()+sharedP.Y()*sharedP.Y()
                   +sharedP.Z()*sharedP.Z());
     sharedEt_em = sqrt(sharedP.X()*sharedP.X()+sharedP.Y()*sharedP.Y());
     sharedEt_had = 0.;
     sharedPt_tracks = 0.;
     if (isInJet) {
       sharedPt_tracks = RecoData[ichk]->pt();
     }
   }
   else if (ptype == 2){
//     if (isIsolated){cout << endl; 
//     cout << "  Type = " << RecoData[ichk]->particleType() << " Is isolated " << endl; }
//     else {cout << endl; 
//       cout << "  Type = " << RecoData[ichk]->particleType() << "  Is not isolated " << endl; }
     isInJet = IsMuonInJet(ichk, iJet, & sharedP);
     sharedE = sqrt(sharedP.X()*sharedP.X()+sharedP.Y()*sharedP.Y()
                   +sharedP.Z()*sharedP.Z());
     sharedEt_em = sqrt(sharedP.X()*sharedP.X()+sharedP.Y()*sharedP.Y());
     sharedEt_had = 0.;
     sharedPt_tracks = 0.;
     if (isInJet) {
       sharedEt_had = RecoData[ichk]->et_had();
       sharedPt_tracks = RecoData[ichk]->pt_tracks();
     }
   }
   else if (ptype == 3){
//     if (isIsolated){cout << endl; 
//     cout << "  Type = " << RecoData[ichk]->particleType() << " Is isolated " << endl; }
//     else {cout << endl; 
//       cout << "  Type = " << RecoData[ichk]->particleType() << "  Is not isolated " << endl; }
     isInJet = IsTauInJet(ichk, iJet, & sharedP);
     sharedE = sqrt(sharedP.X()*sharedP.X()+sharedP.Y()*sharedP.Y()
                   +sharedP.Z()*sharedP.Z());
     sharedEt_em = sqrt(sharedP.X()*sharedP.X()+sharedP.Y()*sharedP.Y());
     sharedEt_had = 0.;
     sharedPt_tracks = 0.;
     if (isInJet) {
       sharedEt_had = RecoData[ichk]->et_had();
       sharedPt_tracks = RecoData[ichk]->pt_tracks();
     }
   }
   else if (ptype == 4){
//     if (isIsolated){cout << endl; 
//     cout << "  Type = " << RecoData[ichk]->particleType() << " Is isolated " << endl; }
//     else {cout << endl; 
//       cout << "  Type = " << RecoData[ichk]->particleType() << "  Is not isolated " << endl; }
     isInJet = IsEMObjectInJet(ichk, iJet, & sharedP);
     sharedE = sqrt(sharedP.X()*sharedP.X()+sharedP.Y()*sharedP.Y()
                   +sharedP.Z()*sharedP.Z());
     sharedEt_em = sqrt(sharedP.X()*sharedP.X()+sharedP.Y()*sharedP.Y());
     sharedEt_had = 0.;
     sharedPt_tracks = 0.;
   }
   else if (ptype == 8){
     isInJet = false;
   }
   if (DEBUGLVL >= 2){
     if (isInJet){
       cout << " object index = " << ichk << " type = " << RecoData[ichk]->particleType()
            << "  is in jet " << iJet << endl;}
     else {
       cout << " object index = " << ichk << " type = " << RecoData[ichk]->particleType()
            << "  is not in jet " << iJet << endl;}
   }
  
  if (isIsolated && isInJet){
    RecoData[iJet]->setPx(RecoData[iJet]->px() - sharedP.X());
    RecoData[iJet]->setPy(RecoData[iJet]->py() - sharedP.Y());
    RecoData[iJet]->setPz(RecoData[iJet]->pz() - sharedP.Z());
    RecoData[iJet]->setEnergy(RecoData[iJet]->energy() - sharedE);
   // ??? or do we want to keep the jets massless?
   //RecoData[iJet]->setEnergy(RecoData[iJet]->p() );
    RecoData[iJet]->setPt_tracks(RecoData[iJet]->pt_tracks() - sharedPt_tracks);
    RecoData[iJet]->setEt_em(RecoData[iJet]->et_em() - sharedEt_em);
    RecoData[iJet]->setEt_had(RecoData[iJet]->et_had() - sharedEt_had);
    if (DEBUGLVL >= 2){
      cout << "  Energy subtracted from jet = " << sharedE << endl;
    }
    if(RecoData[iJet]->energy() < RecoData[iJet]->p() ){
     float egy = RecoData[iJet]->energy();
     if (egy <= 0.){ egy = 0.001;}
     float scale = egy / RecoData[iJet]->p();
     RecoData[iJet]->setPx(RecoData[iJet]->px() * scale);
     RecoData[iJet]->setPy(RecoData[iJet]->py() * scale);
     RecoData[iJet]->setPz(RecoData[iJet]->pz() * scale);
     RecoData[iJet]->setEnergy(RecoData[iJet]->p());
    }
    if(RecoData[iJet]->pt_tracks() < 0.){RecoData[iJet]->setPt_tracks(0.);}
    if(RecoData[iJet]->et_em() < 0.){RecoData[iJet]->setEt_em(0.);}
    if(RecoData[iJet]->et_had() < 0.){RecoData[iJet]->setEt_had(0.);}
    isMerged = false;
  }
  else if (!isIsolated && isInJet){
    float diffPx = 0., diffPy = 0., diffPz = 0., diffE = 0.; 
    float diffEt_em = 0., diffEt_had = 0., diffPt_tracks = 0.;
    if (sharedE < RecoData[ichk]->energy() ){
      diffPx = RecoData[ichk]->px() - sharedP.X();
      diffPy = RecoData[ichk]->py() - sharedP.Y();
      diffPz = RecoData[ichk]->pz() - sharedP.Z();
      diffE = RecoData[ichk]->energy() - sharedE;
      diffEt_em = RecoData[ichk]->et_em() - sharedEt_em;
      diffEt_had = RecoData[ichk]->et_had() - sharedEt_had;
      diffPt_tracks = RecoData[ichk]->pt_tracks() - sharedPt_tracks;
      RecoData[iJet]->setPx(RecoData[iJet]->px() + diffPx);
      RecoData[iJet]->setPy(RecoData[iJet]->py() + diffPy);
      RecoData[iJet]->setPz(RecoData[iJet]->pz() + diffPz);
      RecoData[iJet]->setEnergy(RecoData[iJet]->energy() + diffE);
     // ??? or do we want to keep the jets massless?
     //RecoData[iJet]->setEnergy(RecoData[iJet]->p() );
      RecoData[iJet]->setPt_tracks(RecoData[iJet]->pt_tracks() + diffPt_tracks);
      RecoData[iJet]->setEt_em(RecoData[iJet]->et_em() + diffEt_em);
      RecoData[iJet]->setEt_had(RecoData[iJet]->et_had() + diffEt_had);
    }
    if (DEBUGLVL >= 2){
      cout << "  Energy partially added to jet = " << diffE << endl;
    }
    if(RecoData[iJet]->energy() < RecoData[iJet]->p() ){
     float egy = RecoData[iJet]->energy();
     if (egy <= 0.){ egy = 0.001;}
     float scale = egy / RecoData[iJet]->p();
     RecoData[iJet]->setPx(RecoData[iJet]->px() * scale);
     RecoData[iJet]->setPy(RecoData[iJet]->py() * scale);
     RecoData[iJet]->setPz(RecoData[iJet]->pz() * scale);
     RecoData[iJet]->setEnergy(RecoData[iJet]->p());
    }
    if(RecoData[iJet]->pt_tracks() < 0.){RecoData[iJet]->setPt_tracks(0.);}
    if(RecoData[iJet]->et_em() < 0.){RecoData[iJet]->setEt_em(0.);}
    if(RecoData[iJet]->et_had() < 0.){RecoData[iJet]->setEt_had(0.);}
    isMerged = true;
  }
  else if (!isIsolated && !isInJet){
    AddToJet(ichk, iJet);
    if (DEBUGLVL >= 2){
      cout << "  Energy added to jet = " << RecoData[ichk]->energy() << endl;
    }
    isMerged = true;
  }
  
  return isMerged;

}

  
  
