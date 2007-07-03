#ifndef UserAnalysis_h
#define UserAnalysis_h


/*  \class UserAnalysis
*
*  Class where the user analysis code goes -- (now it contains just an example)
*
*  Authors: Luc Pape & Filip Moortgat      Date: December 2006
*                                          Updated: March 2007
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
virtual ~UserAnalysis(){
  endAnalysis();
};

// Methods:

virtual bool L1Driver(MrEvent*);
virtual bool HLTDriver(MrEvent*);
virtual void doAnalysis(MrEvent*);
void endAnalysis();
void setNtotEvtProc(int n){ntotEvtProc = n;};

private:
 
 
  // names of parameter sets  
  Config_t * myConfig;
  edm::ParameterSet useranalysis_params;
  
  // Define the parameters
  float user_metMin;
 
 
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
 

  MrEvent * EventData;
  //std::vector<MrParticle*> & RecoData;
  //std::vector<MrParticle*> & MCData;

// number of User selected events for final statistics
  int nTotEvtSelUser;

// Some (possibly) useful functions:
// Based on MCData
virtual int FindProducedSusyParticles(int *);
virtual int FindTopSusyMother(int);
virtual int FindLowSusyMother(int);
virtual int FirstSMParton(int);
virtual bool ComesFromSquark(int);
virtual bool ComesFromGluino(int);
// Based on RecoData
virtual int FindNearestJet(int);
virtual float GetPtwrtJet(int, int);
virtual void AddToJet(int);
// Just kinematics
virtual float DeltaPhi(float, float);
virtual float GetDeltaR(float, float, float, float);

// Define user counters
  int ntotEvtProc;

};

#endif

