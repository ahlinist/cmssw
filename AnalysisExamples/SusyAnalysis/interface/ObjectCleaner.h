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


#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h" 
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h" 
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"


#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h" 
#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "AnalysisExamples/SusyAnalysis/interface/SusyRecoTools.h"


#include <vector>
#include <iostream>
#include <cmath>

class ObjectCleaner : public SusyRecoTools {

public:

// Constructor:
ObjectCleaner(MrEvent*);
ObjectCleaner(MrEvent*, edm::ParameterSet);

// Destructor:
virtual ~ObjectCleaner(){};


// Methods:
virtual bool CleanPrimaryVertex(void);
virtual bool CleanObject(int);
virtual bool NotDuplicateObject(int);
virtual bool CleanElectron(int);
virtual bool DuplicateElectron(int);
virtual bool CleanMuon(int);
virtual bool DuplicateMuon(int);
virtual bool CleanTau(int);
virtual bool CleanPhoton(int);
virtual bool DuplicatePhoton(int);
virtual bool CleanJet(int);
virtual bool ElectronJet(int);
virtual bool ConvertedPhoton(int, int);
virtual bool CleanEvent();
virtual bool CleanMET(float*);

void SetDebug(int debug)  { DEBUGLVL  = debug; } 

private:

 const BasicClusterShapeAssociationCollection * clusterShapeBarrelData; 
 const BasicClusterShapeAssociationCollection * clusterShapeEndcapData; 

//std::vector<MrParticle*>& RecoData;
//const TrackCollection * TrackData;
//const VertexCollection* VertexData;
//const CaloTowerCollection* CaloTowerData;

//const Vertex* primVx;

//int DEBUGLVL;

float clean_chisqVxmax;
float clean_dRVxmax;
float clean_dzVxmax;
float clean_etaTkfromVxmax;
float clean_sumPtTkfromVxmin;
float clean_distVxmax;
bool clean_UserDefinedElecID;
float clean_ElecHoverEBarmax; float clean_ElecHoverEEndmax;
float clean_ElecSigmaEtaEtaBarmax; float clean_ElecSigmaEtaEtaEndmax;
float clean_ElecSigmaPhiPhiBarmax; float clean_ElecSigmaPhiPhiEndmax;
float clean_ElecDeltaEtaInBarmax; float clean_ElecDeltaEtaInEndmax;
float clean_ElecDeltaPhiInBarmax; float clean_ElecDeltaPhiInEndmax;
float clean_ElecDeltaPhiOutBarmax; float clean_ElecDeltaPhiOutEndmax;
float clean_ElecEoverPInBarmin; float clean_ElecEoverPInEndmin;
float clean_ElecEoverPOutBarmin; float clean_ElecEoverPOutEndmin;
float clean_ElecInvEMinusInvPBarmax; float clean_ElecInvEMinusInvPEndmax;
float clean_ElecE9overE25Barmin; float clean_ElecE9overE25Endmin;
//  float clean_ElecBremFractionBarmax; float clean_ElecBremFractionEndmax;

std::string ecutquality;
edm::ParameterSet ecuts;

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
int clean_methodTksInJet;
int clean_nJetTkHitsmin;
float clean_JetTkPtmin;
float clean_jetCaloTowEFracmin;
float clean_dRTrkFromJet;
float clean_FracChminJet;
float clean_FracEmmaxJet;
float clean_dROSelecmax;
float clean_MOSelecmax;
float clean_FracChmin;
float clean_FracEmmin;
float clean_METmin;
float clean_dPhiJetMETmin;
float clean_dR12min;
float clean_dR21min;

  bool useEoverPIn;
  bool useDeltaEtaIn;
  bool useDeltaPhiIn;
  bool useHoverE;
  bool useE9overE25;
  bool useEoverPOut;
  bool useEseedOverPIn;
  bool useDeltaPhiOut;
  bool useInvEMinusInvP;
  bool useBremFraction;
  bool useSigmaEtaEta;
  bool useSigmaPhiPhi;
  bool useEseedOverPin;

 bool useFastSim;
  
};

#endif

