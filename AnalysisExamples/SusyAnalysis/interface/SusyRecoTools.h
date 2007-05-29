#ifndef SusyRecoTools_h
#define SusyRecoTools_h


/*  \class SusyRecoTools
*
*  Class which methods common to several classes of SusyAnalyzer
*
*  Authors: Luc Pape & Filip Moortgat      Date: March 2007
*                                          Updated: 
*
*/


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

#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h" 
#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 

#include <vector>
#include <iostream>
#include <cmath>

class SusyRecoTools {

public:

// Constructor:
SusyRecoTools(MrEvent*);
//SusyRecoTools(vector<MrParticle*>*, const TrackCollection *, 
//    const VertexCollection*, const CaloTowerCollection*);
//SusyRecoTools(vector<MrParticle*>*, const TrackCollection *, 
//    const VertexCollection*, const CaloTowerCollection*, edm::ParameterSet);

// Destructor:
virtual ~SusyRecoTools(){};

// Methods:
virtual void PrintRecoInfo(void);
virtual int GetPrimaryVertex(float);
virtual bool GetJetVx(int, int, int, float, float);
virtual void GetJetTrksFromCalo(int, int, float, float, vector<int> *);
virtual void GetJetTrksInCone(int, int, float, float, vector<int>*);
virtual float CaloTowerSizePhi(float);
virtual float CaloTowerSizeEta(float);
virtual bool IsFromPrimaryVx(int, float);
virtual int FindNearestJet(int);
virtual float GetPtwrtJet(int, int);
virtual float GetJetTrkPtsum(int, int, int, float, float);
virtual float IsoCalSum (int, float, float, float, float, float);
virtual float IsoTrkSum (int, float, float, float, float, float);
virtual float IsoCandSum (int, float, float, float, float, float);
virtual bool IsEMObjectInJet(int, int, math::XYZVector*);
virtual bool IsMuonInJet(int, int, math::XYZVector*);
virtual bool IsTauInJet(int, int, math::XYZVector*);
virtual void AddToJet(int);
virtual float GetPhiMin(float, float);
virtual float GetPhiMax(float, float);
virtual float DeltaPhi(float, float);
virtual float DeltaPhiSigned(float, float);
virtual bool IsInPhiWindow(float, float, float);
virtual float GetDeltaR(float, float, float, float);

void SetDebug(int debug)  {DEBUGLVL  = debug; } 

protected:

MrEvent * EventData;
std::vector<MrParticle*>& RecoData;
const TrackCollection * TrackData;
const VertexCollection* VertexData;
const CaloTowerCollection* CaloTowerData;

const Vertex* primVx;

int DEBUGLVL;


};

#endif

