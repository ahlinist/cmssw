#ifndef MuAnalyzer_H
#define MuAnalyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "TProfile.h"

// forward declarations
class TFile;
class TH1D;
class TH2D;
class TTree;

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
     double etamu,phimu,ptmu,vxmu,vymu,vzmu,pxmu,pymu,pzmu,decayl,wtmu;
     int pid,isub,runnum,evtnum;
     double intlum,vxge,vyge,vzge,simpt;
     bool simmuon;
     int wtype,parton;
     //
     TFile*      fOutputFile ;
     TH1D* fHistNMu ;
     TH1D* fHistMuX ;
     TH1D* fHistMuY ;
     TH1D* fHistMuZ ;
     TH1D* fHistMuPhi ;
     TH2D* fHistMuRZ ;
     TH2D* fHistMuXY ;
     TH2D* fHistMuPvsPt ;
     TH2D* fHistMuPvsEta ;
     TH1D* fHistPtMu ;
     TH1D* fHistMuweight ;
     TH1D* fHistMuParent ;
     TH1D* fHistMuEta ;
     TH1D* fHistRate33 ;
     TH1D* fHistRate34 ;
     TH1D* fHistRate34Other ;
     TH1D* fHistRate34B;
     TH1D* fHistRate34C;
     TH1D* fHistRate34kaon ;
     TH1D* fHistRate34pion ;
     TH1D* fHistRate34tau ;
     TH1D* fHistRate34W;
     TH1D* fHistRate34Z;
     TH1D* fHistParentDecayLength ;
     TH1D* fHistMuParentStatus ;
     TH1D* fHistIsub ;
     TTree* tmuon;
};

#endif
