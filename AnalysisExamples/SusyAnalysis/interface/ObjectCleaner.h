#ifndef ObjectCleaner_h
#define ObjectCleaner_h


/*  \class ObjectCleaner
*
*  Class which checks the cleanness of MrParticle objects
*
*  Authors: Luc Pape & Filip Moortgat      Date: October 2006
*                                          Updated: 
*
*/


#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <iostream>
#include <cmath>

class ObjectCleaner {

public:

// Constructor:
ObjectCleaner(vector<MrParticle*>*, const TrackCollection *, 
    const VertexCollection*, const CaloTowerCollection*);
ObjectCleaner(vector<MrParticle*>*, const TrackCollection *, 
    const VertexCollection*, const CaloTowerCollection*, edm::ParameterSet);

// Destructor:
~ObjectCleaner(){};

// Methods:
bool CleanPrimaryVertex(void);
int GetPrimaryVertex(void);
bool IsFromPrimaryVx(int);
bool CleanObject(int);
bool NotDuplicateObject(int);
bool CleanElectron(int);
bool DuplicateElectron(int);
bool CleanMuon(int);
bool DuplicateMuon(int);
bool CleanTau(int);
bool CleanPhoton(int);
bool DuplicatePhoton(int);
bool CleanJet(int);
bool ElectronJet(int);
bool CleanEvent();
bool ConvertedPhoton(int, int);
bool CleanMET(float*);

void SetDebug(int debug)  {DEBUGLVL  = debug; } 

private:

std::vector<MrParticle*>& RecoData;
const TrackCollection * TrackData;
const VertexCollection* VertexData;
const CaloTowerCollection* CaloTowerData;

const Vertex* primVx;

int DEBUGLVL;

float clean_chisqVxmax;
float clean_dRVxmax;
float clean_dzVxmax;
float clean_etaTkfromVxmax;
float clean_sumPtTkfromVxmin;
float clean_distVxmax;
float clean_ElecEoPmin;
float clean_ElecEoPinvmax;
float clean_dRElecTowermax;
float clean_ElecHoEmax;
float clean_dRSSelecmax;
float clean_MuonDPbyPmax;
float clean_MuonChi2max;
float clean_MuonNHitsmin;
float clean_dRMuonTowermax;
float clean_dRSSmuonmax;
float clean_dRPhotTowermax;
float clean_PhotHoEmax;
float clean_dRPhotElemax;
float clean_dRPhotDupmax;
float clean_deltaRElecJetmax;
float clean_elecbyJetEratio;
float clean_dRTrkFromJet;
float clean_FracChmin;
float clean_FracEmmin;
float clean_dROSelecmax;
float clean_MOSelecmax;
float clean_METmin;
float clean_dPhiJetMETmin;
float clean_dR12min;
float clean_dR21min;

int FindNearestJet(int);
float GetPtwrtJet(int, int);
float GetJetTrkPtsum(float, float, float);
float DeltaPhi(float, float);
float GetDeltaR(float, float, float, float);

};

#endif

