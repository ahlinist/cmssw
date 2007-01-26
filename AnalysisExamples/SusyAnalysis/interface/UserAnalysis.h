#ifndef UserAnalysis_h
#define UserAnalysis_h


/*  \class UserAnalysis
*
*  Class where the user analysis code goes -- (now it contains just an example)
*
*  Authors: Luc Pape & Filip Moortgat      Date: December 2006
*                                          Updated: 
*
*/


#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h" 
#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TH1.h"
#include "TH2.h"
 
#include <vector>
#include <iostream>
#include <cmath>

class UserAnalysis {

public:

// Constructor:
UserAnalysis(Config_t *);

// Destructor:
~UserAnalysis(){};

// Methods:

void doAnalysis(MrEvent*);

private:
 
 
  // names of histograms
     TH1D*       hLeptonMult;
     TH1D*       hElectronMult;
     TH1D*       hMuonMult;
     TH1D*       hPhotonMult;
     TH1D*       hJetMult;
     TH1D*       hBJetMult;
     TH1D*       hPtElec;
     TH1D*       hPtMuon;
     TH1D*       hPtJet1;
     TH1D*       hPtJet2;
     TH1D*       hPtJet3;
     TH1D*       hMissingETmc;
     TH1D*       hMissingET;
     TH1D*       hEtSum;
     TH1D*       hHT;
     TH1D*       hHemiMass;
     TH1D*       hJetGoodEt;
     TH1D*       hJetWrongEt;
 

  MrEvent * myEventData;
  //std::vector<MrParticle*> & RecoData;
  //std::vector<MrParticle*> & MCData;

// Some (possibly) useful functions:
// Based on MCData
int FindProducedSusyParticles(int *);
int FindTopSusyMother(int);
int FindLowSusyMother(int);
int FirstSMParton(int);
bool ComesFromSquark(int);
bool ComesFromGluino(int);
// Based on RecoData
int FindNearestJet(int);
float GetPtwrtJet(int, int);
void AddToJet(int);
// Just kinematics
float DeltaPhi(float, float);
float GetDeltaR(float, float, float, float);

};

#endif

