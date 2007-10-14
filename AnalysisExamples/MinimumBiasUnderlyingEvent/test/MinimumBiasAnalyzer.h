#ifndef MinimumBiasAnalyzer_H
#define MinimumBiasAnalyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
//#include "TProfile.h"
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TProfile.h>
class TTree;
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
      int charge(const int&);

   private:
   
     //

     std::string objectAnalyzed;
     std::string fOutputFileName ;

TFile* hFile;
TTree * nt;

 int Ev_kind;


int Np;
float p_[1000];
float pt_[1000];
float eta_[1000];
float phi_[1000];
int charge_[1000];
int pid_[1000];

int Ntk;
float p_tk[1000];
float pt_tk[1000];
float eta_tk[1000];
float phi_tk[1000];
int nhits_tk[1000];

 int lead_tk;
 int lead_MC;
 float d_phi;

 TProfile*   dN_vs_eta;
 TProfile*   dN_vs_pt;

 TProfile*   dN_vs_etaMC;
 TProfile*   dN_vs_ptMC;

 TProfile* dN_vs_dphiMC;
 TProfile* dPT_vs_dphiMC;

 TProfile* dN_vs_dphi;
 TProfile* dPT_vs_dphi;


 TH1D*       temp1;
 TH1D*       temp2; 
 TH1D*       temp1MC;
 TH1D*       temp2MC;
 TH1D*       temp_dnMC;
 TH1D*       temp_dptMC;
 TH1D*       temp_dn;
 TH1D*       temp_dpt;

};

#endif
