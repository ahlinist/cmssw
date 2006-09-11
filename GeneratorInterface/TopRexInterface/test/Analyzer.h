#ifndef Analyzer_H
#define Analyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "TProfile.h"

// forward declarations
class TFile;
class TH1D;

class Analyzer : public edm::EDAnalyzer
{
  public:
     //
      explicit Analyzer( const edm::ParameterSet& ) ;
      virtual ~Analyzer() {} // no need to delete ROOT stuff
                                   // as it'll be deleted upon closing TFile
      
      virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
      virtual void beginJob( const edm::EventSetup& ) ;
      virtual void endJob() ;
      virtual void HistPart(int&, double&, double&, double&, double&);
 
      //     HepMC::GenEvent  *evt;
 
      void PythiaJet( const HepMC::GenEvent& evt ) ; 

   private:
   
     //
     std::string fOutputFileName ;
     TFile*      fOutputFile ;
     //     TH1D*       fHistZZMass ;
     TH1D* H_P_E[11] ;
     TH1D* H_P_PT[11] ;
     TH1D* H_P_ETA[11] ;
     TH1D* H_P_PHI[11] ;
     TH1D* H_P_NN[11] ;
     //
     TH1D* P_J_E ;
     TH1D* P_J_PT ;
     TH1D* P_J_ETA ;
     TH1D* P_J_M ;
     TH1D* P_J_NN ;
     //
     TH1D* C_J_E ;
     TH1D* C_J_PT ;
     TH1D* C_J_ETA ;
     TH1D* C_J_M ;
     TH1D* C_J_NN ;
};

#endif
