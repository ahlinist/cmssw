//Author Sudhir Malik 20 May 2009
#ifndef ValidationPhysicsToolsPATValidation_Muon_h
#define ValidationPhysicsToolsPATValidation_Muont_h

 
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"



class PATValidation_Muon : public edm::EDAnalyzer {

 public:
    
  PATValidation_Muon (const edm::ParameterSet&);
  ~PATValidation_Muon();

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void endJob() ;

  
 private:

  edm::InputTag recoMuon_;
  edm::InputTag patMuon_;
  std::string outputFile_;

  DQMStore* dbe;
  std::map<std::string, MonitorElement*> me;

  //histo binning parameters
  int etaBin;
  double etaMin;
  double etaMax;

  int thetaBin;
  double thetaMin;
  double thetaMax;

  int phiBin;
  double phiMin;
  double phiMax;

  int chi2Bin;
  double chi2Min;
  double chi2Max;

  int pBin;
  double pMin;
  double pMax;

  int ptBin;
  double ptMin;
  double ptMax;

  int pResBin;
  double pResMin;
  double pResMax;

  int rhBin;
  double rhMin;
  double rhMax;





//the RECOhistos
  MonitorElement* muReco;

  // global muon
  std::vector<MonitorElement*> etaGlbTrack;
  std::vector<MonitorElement*> etaResolution;
  std::vector<MonitorElement*> thetaGlbTrack;
  std::vector<MonitorElement*> thetaResolution;
  std::vector<MonitorElement*> phiGlbTrack;
  std::vector<MonitorElement*> phiResolution;
  std::vector<MonitorElement*> chi2OvDFGlbTrack;
  std::vector<MonitorElement*> pGlbTrack;
  std::vector<MonitorElement*> ptGlbTrack;
  std::vector<MonitorElement*> qGlbTrack;
  std::vector<MonitorElement*> qOverpResolution;
  std::vector<MonitorElement*> qOverptResolution;
  std::vector<MonitorElement*> oneOverpResolution;
  std::vector<MonitorElement*> oneOverptResolution;
  std::vector<MonitorElement*> rhAnalysis;
  // tracker muon
  MonitorElement* etaTrack;
  MonitorElement* thetaTrack;
  MonitorElement* phiTrack;
  MonitorElement* chi2OvDFTrack;
  MonitorElement* pTrack;
  MonitorElement* ptTrack;
  MonitorElement* qTrack;
  // sta muon
  MonitorElement* etaStaTrack;
  MonitorElement* thetaStaTrack;
  MonitorElement* phiStaTrack;
  MonitorElement* chi2OvDFStaTrack;
  MonitorElement* pStaTrack;
  MonitorElement* ptStaTrack;
  MonitorElement* qStaTrack;
  // efficiency
  std::vector<MonitorElement*> etaEfficiency;
  std::vector<MonitorElement*> phiEfficiency;
 
//the PAT histos
  MonitorElement* patmuReco;

// global muon
  std::vector<MonitorElement*> patetaGlbTrack;
  std::vector<MonitorElement*> patetaResolution;
  std::vector<MonitorElement*> patthetaGlbTrack;
  std::vector<MonitorElement*> patthetaResolution;
  std::vector<MonitorElement*> patphiGlbTrack;
  std::vector<MonitorElement*> patphiResolution;
  std::vector<MonitorElement*> patchi2OvDFGlbTrack;
  std::vector<MonitorElement*> patpGlbTrack;
  std::vector<MonitorElement*> patptGlbTrack;
  std::vector<MonitorElement*> patqGlbTrack;
  std::vector<MonitorElement*> patqOverpResolution;
  std::vector<MonitorElement*> patqOverptResolution;
  std::vector<MonitorElement*> patoneOverpResolution;
  std::vector<MonitorElement*> patoneOverptResolution;
  std::vector<MonitorElement*> patrhAnalysis;
  // tracker muon
  MonitorElement* patetaTrack;
  MonitorElement* patthetaTrack;
  MonitorElement* patphiTrack;
  MonitorElement* patchi2OvDFTrack;
  MonitorElement* patpTrack;
  MonitorElement* patptTrack;
  MonitorElement* patqTrack;
  // sta muon
  MonitorElement* patetaStaTrack;
  MonitorElement* patthetaStaTrack;
  MonitorElement* patphiStaTrack;
  MonitorElement* patchi2OvDFStaTrack;
  MonitorElement* patpStaTrack;
  MonitorElement* patptStaTrack;
  MonitorElement* patqStaTrack;
  // efficiency
  std::vector<MonitorElement*> patetaEfficiency;
  std::vector<MonitorElement*> patphiEfficiency;  

};
  
#endif
 

