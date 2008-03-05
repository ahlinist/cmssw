#ifndef EffLeptonToBeFake_h
#define EffLeptonToBeFake_h


/*  \class EffLeptonToBeFakes
*
*  Class with example calculation of probability for a lepton to be a fake
*
*  Authors: Luc Pape & Filip Moortgat      Date: December 2007
*                                          Updated: 
*
*/


#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h" 
#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include <vector>
#include <iostream>
#include <cmath>

class EffLeptonToBeFake {

public:

// Constructor:
EffLeptonToBeFake(Config_t *);

// Destructor:
virtual ~EffLeptonToBeFake(){
};

// Methods:

virtual float GetLeptonFakes(int, MrEvent*);

private:
 
 
  // names of parameter sets  
  Config_t * myConfig;
  
  // Define the parameters


  MrEvent * EventData;
  
//  User functions

};

#endif
