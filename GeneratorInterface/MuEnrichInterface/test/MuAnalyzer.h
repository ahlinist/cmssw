#ifndef MuAnalyzer_H
#define MuAnalyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "TProfile.h"

// forward declarations
class TFile;
class TH1D;

class MuAnalyzer : public edm::EDAnalyzer
{
  public:
     //
      explicit MuAnalyzer( const edm::ParameterSet& ) ;
      virtual ~MuAnalyzer() {} // no need to delete ROOT stuff
                                   // as it'll be deleted upon closing TFile
      
      virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
      virtual void beginJob( const edm::EventSetup& ) ;
      virtual void endJob() ;
 
      //     HepMC::GenEvent  *evt;
 

   private:
   
     std::string fOutputFileName ;
     double maxeta;
     double ptmin;
     double intlum;
     //
     TFile*      fOutputFile ;
     TH1D* fHistPtMu ;
     TH1D* fHistMuweight ;
     TH1D* fHistMuParent ;
     TH1D* fHistMuEta ;
     TH1D* fHistRate33 ;
     TH1D* fHistRate34 ;
     TH1D* fHistParentDecayLength ;
     TH1D* fHistOtherDecayLength ;
     TH1D* fHistMuParentStatus ;
     TH1D* fHistIsub ;

};

#endif
