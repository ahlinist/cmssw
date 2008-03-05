#ifndef EffTrigger_h
#define EffTrigger_h


/*  \class EffTrigger
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

class EffTrigger {

public:

// Constructor:
EffTrigger(Config_t *);

// Destructor:
virtual ~EffTrigger(){
};

// Methods:

virtual float GetEffTrigger(MrEvent*);

private:
 
 
  // names of parameter sets  
  Config_t * myConfig;
  
  // Define the parameters


  MrEvent * EventData;
  
//  User functions

};

#endif
