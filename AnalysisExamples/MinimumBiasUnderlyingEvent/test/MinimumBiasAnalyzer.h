#ifndef MinimumBiasAnalyzer_H
#define MinimumBiasAnalyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "TProfile.h"

// forward declarations
class TFile;
class TH1D;

class MinimumBiasAnalyzer : public edm::EDAnalyzer
{

   public:
   
      //
      explicit MinimumBiasAnalyzer( const edm::ParameterSet& ) ;
      virtual ~MinimumBiasAnalyzer() {} // no need to delete ROOT stuff
                                   // as it'll be deleted upon closing TFile
      
      virtual void analyze( const edm::Event&, const edm::EventSetup& ) ;
      virtual void beginJob( const edm::EventSetup& ) ;
      virtual void endJob() ;
      float_t delR(const float_t&,const float_t&,const float_t&,const float_t&);


   private:
   
     //
     std::string fOutputFileName ;
     std::string objectAnalyzed;

     TFile*      fOutputFile ;
     TH1D*       fHistPtDist ;
     TH1D*       fHistEtaDist ;
     TH1D*       fHistPhiDist ;
     TH1D*       fHistChgDist ;
     TH1D*       fHistChgSimm ;
     TProfile*   pdN_vs_dphi;
     TProfile*   pdPt_vs_dphi;
     TProfile*   pdN_vs_eta;
     TProfile*   pdN_vs_pt;

     TH1D*       temp1;
     TH1D*       temp2;
     TH1D*       temp3;
     TH1D*       temp4;

     TH1D*       fHistPtDistJetChg;
     TH1D*       fHistEtaDistJetChg;
     TH1D*       fHistPhiDistJetChg;

};

#endif
