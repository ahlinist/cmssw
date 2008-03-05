#ifndef EffProcessor_h
#define EffProcessor_h


/*  \class EffProcessor
*
*  Class which processes the calculation of efficiencies 
*  of MrParticle objects
*
*  Authors: Luc Pape & Filip Moortgat      Date: December 2007
*                                          Updated: 
*
*/


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Ref.h"

#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h" 
#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h" 
#include "AnalysisExamples/SusyAnalysis/interface/EffTrigger.h"
#include "AnalysisExamples/SusyAnalysis/interface/EffLeptonID.h"
#include "AnalysisExamples/SusyAnalysis/interface/EffLeptonFakes.h"
#include "AnalysisExamples/SusyAnalysis/interface/EffLeptonToBeFake.h"

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

class EffProcessor {

public:

// Constructor:
EffProcessor(Config_t *);

// Destructor:
virtual ~EffProcessor(){};

// Methods:
virtual bool EffDriver(MrEvent *);
void SetDebug(int debug)  {DEBUGLVL  = debug; } 


private:
     
   // names of parameter sets  
  Config_t * myConfig;
  
  MrEvent * EventData;
//  std::vector<MrParticle*> & RecoData;
 
  
  // Define all pointers to objects
     
  EffTrigger * myEffTrigger;
  EffLeptonID * myEffLeptonID;
  EffLeptonFakes * myEffLeptonFakes; 
  EffLeptonToBeFake * myEffLeptonToBeFake;

  int DEBUGLVL;

  

};

#endif

