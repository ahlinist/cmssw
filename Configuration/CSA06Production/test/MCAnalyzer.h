#ifndef MCAnalyzer_H
#define MCAnalyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include<vector>

// forward declarations
class TFile;
class TH1D;

class MCAnalyzer : public edm::EDAnalyzer
{

   public:
   
      //
      explicit MCAnalyzer( const edm::ParameterSet& ) ;
      virtual ~MCAnalyzer() {} // no need to delete ROOT stuff
                                   // as it'll be deleted upon closing TFile
      
      virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
      virtual void beginJob( const edm::EventSetup& ) ;
      virtual void endJob() ;
      int charge(const int&);
      
   private:
   
     //
     std::string fOutputFileName ;
     TFile*      fOutputFile ;
     TH1D*       fHist2muMass ;
     TH1D*       fHistmuPt ;
     TH1D*       fHistmuEta ;
     TH1D*       fHistmuPhi ;
     TH1D*       fPtHat ;
     TH1D*       fNPart ;
     TH1D*       fNChPart ;
     TH1D*       fProcessID ;
     TH1D*       fPartID ;
   
     unsigned int         Nevt, Nhlt;
     std::vector<unsigned int> Nall;
     
};

#endif
