#ifndef ShapeAnalyzer_h
#define ShapeAnalyzer_h


/*  \class ShapeAnalyzer
*
*  Class which analyzes the event shapes
*
*  Authors: Luc Pape & Filip Moortgat      Date: October 2006
*                                          Updated: 
*
*/


#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h" 
#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "AnalysisExamples/SusyAnalysis/interface/Hemisphere.h" 
#include "AnalysisExamples/SusyAnalysis/interface/MatchHemis.h" 

#include <vector>
#include <iostream>
#include <cmath>

class ShapeAnalyzer {

public:

// Constructor:
ShapeAnalyzer(MrEvent *);

// Destructor:
~ShapeAnalyzer(){};

// Methods:
void SetDebug(int debug)  {DEBUGLVL  = debug; } 

bool AnalyzeHemi(void);

vector<float> hemiAxis1(void) {return vA1;}
vector<float> hemiAxis2(void) {return vA2;}

int IndexMatchedSusyMother1(void) {return firstsusymother[0];}
int IndexMatchedSusyMother2(void) {return firstsusymother[1];}

private:
 
  MrEvent * myEventData;
  std::vector<MrParticle*> & RecoData;
  std::vector<MrParticle*> & MCData;
  // vectors used for hemisphere reconstruction
  vector<float> vPx, vPy, vPz, vE; 
  vector<float> vA1, vA2;
  vector<int> vgroups;
  
  int firstsusymother[2];
  
  Hemisphere * myHemi;
  MatchHemis * myHemiMatcher;

  int DEBUGLVL;


float DeltaPhi(float, float);
float GetDeltaR(float, float, float, float);

};

#endif

