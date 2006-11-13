#ifndef Isolator_h
#define Isolator_h


/*  \class Isolator
*
*  Class which applies the isolation methods to MrParticle objects
*
*  Authors: Luc Pape & Filip Moortgat      Date: October 2006
*                                          Updated: 
*
*/


#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <iostream>
#include <cmath>

class Isolator {

public:

// Constructor:
Isolator(vector<MrParticle*>&, const TrackCollection *, 
    const CaloTowerCollection*);
Isolator(vector<MrParticle*>&, const TrackCollection *, 
    const CaloTowerCollection*, edm::ParameterSet);

// Destructor:
~Isolator(){};

// Methods:
bool IsObjectIsolated(int);
bool IsoElectron(int);
bool IsoMuon(int);
bool IsoTau(int);
bool IsoPhoton(int);
bool IsObjectMerged(int, bool);
bool IsElecInJet(int, int);
bool IsMuonInJet(int, int);
bool IsTauInJet(int, int);
bool IsPhotonInJet(int, int);

void SetDebug(int debug)  {DEBUGLVL  = debug; } 
 
private:

std::vector<MrParticle*>& RecoData;
const TrackCollection * TrackData;
const CaloTowerCollection* CaloTowerData;

int DEBUGLVL;

// iso_Methodxxx : identification of the isolation method
// is   1000*imeth + 100*idet + 10*itra + ival
// if iso_Methodxxx = 0 : object always considered as isolated, no test made
// imeth = 1 : isolation by Pt wrt nearest jet
//         2 : isolation in a cone
// idet  = 1 : using calorimeter information
//       = 2 : using tracker information
//       = 3 : using a weighted sum of the two
//       = 4 : using candidate information
// itra  = 0 : if isolation by Pt wrt nearest jet is chosen
//       = 1 : using p,E information in transverse plane
//       = 2 : using p,E information in 3D
//       = 3 : using the number of objects
// ival  = 0 : if isolation by Pt wrt nearest jet is chosen
//       = 1 : cut on relative value
//       = 2 : cut on absolute value
int   iso_MethodElec;
float iso_jetbyElEmin;
float iso_ptElwrtJetmin;
float iso_ElCalDRin;
float iso_ElCalDRout;
float iso_ElCalSeed;
float iso_ElCalValue;
float iso_ElTkDRin;
float iso_ElTkDRout;
float iso_ElTkSeed;
float iso_ElTkValue;
int   iso_MethodMuon;
float iso_jetbyMuEmin;
float iso_ptMuwrtJetmin;
float iso_MuCalDRin;
float iso_MuCalDRout;
float iso_MuCalSeed;
float iso_MuCalValue;
float iso_MuTkDRin;
float iso_MuTkDRout;
float iso_MuTkSeed;
float iso_MuTkValue;
int   iso_MethodTau;
float iso_jetbyTauEmin;
float iso_ptTauwrtJetmin;
int   iso_MethodPhot;
float iso_jetbyPhotEmin;
float iso_ptPhotwrtJetmin;

float IsoCalSum (int, float, float, float, float, float);
float IsoTrkSum (int, float, float, float, float, float);
float IsoCandSum (int, float, float, float, float, float);
int FindNearestJet(int);
float GetPtwrtJet(int, int);
float DeltaPhi(float, float);
float GetDeltaR(float, float, float, float);

};

#endif

