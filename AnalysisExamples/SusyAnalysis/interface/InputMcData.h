#ifndef InputMcData_h
#define InputMcData_h


/*  \class InputMcData
*
*  Class which reads in the MC data and saves them as MrParticle objects
*
*  Authors: Luc Pape & Filip Moortgat      Date: September 2008
*                                          Updated: 
*
*/


// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h" 
#include "DataFormats/Candidate/interface/Candidate.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "AnalysisExamples/SusyAnalysis/interface/MrParticle.h"
#include "AnalysisExamples/SusyAnalysis/interface/MrEvent.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
  
//class InputMcData : public edm::EDAnalyzer {
class InputMcData {
   public:
// Constructor:
//      explicit InputMcData(Config_t *);
//   InputMcData(Config_t *);
   InputMcData(Config_t * , const edm::Event& );

// Destructor:
   virtual  ~InputMcData();

   virtual void InputMcDriver(MrEvent *);
   void SetDebug(int debug)  {DEBUGLVL  = debug; } 

   private:
  // ----------member data ---------------------------
  // names of modules, producing object collections
     string m_genParticles;
     string m_jetsgenSrc;

  // Define the vectors holding pointers to MC data

     vector<MrParticle*> MCData; 
     vector<MrParticle*> GenData; 
  
  // The pointer to the MrEvent data
     
     MrEvent * EventData;
  
  // names of parameter sets  

     Config_t * myConfig;
     edm::ParameterSet InputMC_params;
     
     const edm::Event& iEvent;
      

  // Define all pointers to objects


  
  int DEBUGLVL;
  

  // global counters

  // private methods

};

#endif
