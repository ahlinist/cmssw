#ifndef EffLeptonFakes_h
#define EffLeptonFakes_h


/*  \class EffLeptonFakes
*
*  Class with example calculation of trigger efficiency
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

class EffLeptonFakes {

public:

// Constructor:
EffLeptonFakes(Config_t *);

// Destructor:
virtual ~EffLeptonFakes(){
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
