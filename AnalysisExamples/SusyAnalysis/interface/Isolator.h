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


#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h" 
#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "AnalysisExamples/SusyAnalysis/interface/SusyRecoTools.h"

#include <vector>
#include <iostream>
#include <cmath>

class Isolator : public SusyRecoTools {

public:

// Constructor:
Isolator(MrEvent*);
Isolator(MrEvent*, edm::ParameterSet);

// Destructor:
virtual ~Isolator(){};

// Methods:
virtual bool IsObjectIsolated(int);
virtual bool IsoObject(int, int);
virtual bool IsObjectMerged(int, bool);

void SetDebug(int debug)  {DEBUGLVL  = debug; } 
 
private:

//std::vector<MrParticle*>& RecoData;
//const TrackCollection * TrackData;
//const CaloTowerCollection* CaloTowerData;

//int DEBUGLVL;

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
float iso_ElTkDRin;
float iso_ElTkDRout;
float iso_ElTkSeed;
float iso_ElCalWeight;
float iso_ElIsoValue;
int   iso_MethodMuon;
float iso_jetbyMuEmin;
float iso_ptMuwrtJetmin;
float iso_MuCalDRin;
float iso_MuCalDRout;
float iso_MuCalSeed;
float iso_MuTkDRin;
float iso_MuTkDRout;
float iso_MuTkSeed;
float iso_MuCalWeight;
float iso_MuIsoValue;
int   iso_MethodTau;
float iso_jetbyTauEmin;
float iso_ptTauwrtJetmin;
float iso_TauCalDRin;
float iso_TauCalDRout;
float iso_TauCalSeed;
float iso_TauTkDRin;
float iso_TauTkDRout;
float iso_TauTkSeed;
float iso_TauCalWeight;
float iso_TauIsoValue;
int   iso_MethodPhot;
float iso_jetbyPhotEmin;
float iso_ptPhotwrtJetmin;
float iso_PhCalDRin;
float iso_PhCalDRout;
float iso_PhCalSeed;
float iso_PhTkDRin;
float iso_PhTkDRout;
float iso_PhTkSeed;
float iso_PhCalWeight;
float iso_PhIsoValue;
int   iso_MethodUfo;
float iso_jetbyUfoEmin;
float iso_ptUfowrtJetmin;
float iso_UfoCalDRin;
float iso_UfoCalDRout;
float iso_UfoCalSeed;
float iso_UfoTkDRin;
float iso_UfoTkDRout;
float iso_UfoTkSeed;
float iso_UfoCalWeight;
float iso_UfoIsoValue;

};

#endif

