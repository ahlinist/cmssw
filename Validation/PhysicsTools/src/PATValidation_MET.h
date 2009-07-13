//Author Sudhir Malik 20 May 2009
#ifndef ValidationPhysicsToolsPATValidation_MET_h
#define ValidationPhysicsToolsPATValidation_MET_h


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//
// class declaration
//

class PATValidation_MET : public edm::EDAnalyzer {
   public:
      explicit PATValidation_MET(const edm::ParameterSet&); 
      ~PATValidation_MET();

  
   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;
      std::map<std::string, MonitorElement*> me;

      // ----------member data ---------------------------
    
edm::InputTag recoMET_;
edm::InputTag patMET_;

std::string outputFile_;
 std::string metname;
  std::string _source;

  // Et threshold for MET plots
  double _etThreshold;

  //histo binning parameters
  int    etaBin;
  double etaMin;
  double etaMax;

  int    phiBin;
  double phiMin;
  double phiMax;

  int    ptBin;
  double ptMin;
  double ptMax;

  //the histos
//RECO
  MonitorElement* recojetME;

  MonitorElement* recohNevents;
  MonitorElement* recohMEx;
  MonitorElement* recohMEy;
  MonitorElement* recohEz;
  MonitorElement* recohMETSig;
  MonitorElement* recohMET;
  MonitorElement* recohMETPhi;
  MonitorElement* recohSumET;
//PAT
 MonitorElement* patjetME;

  MonitorElement* pathNevents;
  MonitorElement* pathMEx;
  MonitorElement* pathMEy;
  MonitorElement* pathEz;
  MonitorElement* pathMETSig;
  MonitorElement* pathMET;
  MonitorElement* pathMETPhi;
  MonitorElement* pathSumET;
  


};

#endif

