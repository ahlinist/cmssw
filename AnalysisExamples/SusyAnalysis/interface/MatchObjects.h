#ifndef MatchObjects_h
#define MatchObjects_h


/*  \class MatchObjects
*
*  Class which matches the reconstructed MrParticle objects to the MC truth
*
*  Authors: Luc Pape & Filip Moortgat      Date: October 2006
*                                          Updated: 
*
*/


#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <iostream>
#include <cmath>

class MatchObjects {

public:

// Constructor:
MatchObjects(vector<MrParticle*>*, vector<MrParticle*>*);
MatchObjects(vector<MrParticle*>*, vector<MrParticle*>*, edm::ParameterSet);

// Destructor:
~MatchObjects(){};

// Methods:

void DoMatch(void);
void SetDebug(int debug)  {DEBUGLVL  = debug; } 

private:

std::vector<MrParticle*>& RecoData;
std::vector<MrParticle*>& MCData;

int DEBUGLVL;

float mo_elecDRmax;
float mo_elecDPbyPmax;
float mo_muonDRmax;
float mo_muonDPbyPmax;
float mo_photonDRmax;
float mo_photonDPbyPmax;
float mo_jetDRmax;
float mo_jetDPbyPmax;
float mo_celecDRmax;
float mo_cmuonDRmax;
float mo_cphotonDRmax;
float mo_cjetDRmax;

int BestMatch(int, int, int);
void ResolveMatchObjects(void);
float DeltaPhi(float, float);
float GetDeltaR(float, float, float, float);


};

#endif

