#ifndef EffLeptonID_h
#define EffLeptonID_h


/*  \class EffLeptonID
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

class EffLeptonID {

public:

// Constructor:
EffLeptonID(Config_t *);

// Destructor:
virtual ~EffLeptonID(){
};

// Methods:

virtual float GetLeptonEfficiency(int, float, float);

private:
 
 
  // names of parameter sets  
  Config_t * myConfig;
  
  // Define the parameters

  
//  User functions

};

#endif
