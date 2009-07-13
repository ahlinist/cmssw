//Author Sudhir Malik 20 May 2009
#ifndef ValidationPhysicsToolsPATValidation_Electron_h
#define ValidationPhysicsToolsPATValidation_Electron_h

 
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"



class PATValidation_Electron : public edm::EDAnalyzer {

 public:
    
  PATValidation_Electron (const edm::ParameterSet&);
  ~PATValidation_Electron();

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void endJob() ;

  
 private:

  edm::InputTag recoElectron_;
  edm::InputTag patElectron_;
  std::string outputFile_;

  DQMStore* dbe;
  std::map<std::string, MonitorElement*> me;

  double maxPt_;
  double maxAbsEta_;
  double deltaR; 
  
  // histos limits and binning
  double etamin;
  double etamax;
  double phimin;
  double phimax;
  double ptmax;
  double pmax;
  double eopmax;
  double eopmaxsht;
  double detamin;
  double detamax;
  double dphimin;
  double dphimax;
  double detamatchmin;
  double detamatchmax;
  double dphimatchmin;
  double dphimatchmax;
  double fhitsmax;
  double lhitsmax;
  int nbineta;
  int nbinp;
  int nbinpt;
  int nbinpteff;
  int nbinphi;
  int nbinp2D;
  int nbinpt2D;
  int nbineta2D;
  int nbinphi2D;
  int nbineop;
  int nbineop2D;
  int nbinfhits;
  int nbinlhits;
  int nbinxyz;
  int nbindeta;
  int nbindphi;
  int nbindetamatch;
  int nbindphimatch;
  int nbindetamatch2D;
  int nbindphimatch2D;
  
};
  
#endif
 

