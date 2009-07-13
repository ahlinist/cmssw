//Author Sudhir Malik 20 May 2009
#ifndef ValidationPhysicsToolsPATValidation_CaloMET_h
#define ValidationPhysicsToolsPATValidation_CaloMET_h


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//
// class declaration
//

class PATValidation_CaloMET : public edm::EDAnalyzer {
   public:
      explicit PATValidation_CaloMET(const edm::ParameterSet&); 
      ~PATValidation_CaloMET();

  
   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;



      std::map<std::string, MonitorElement*> me;

 void setSource(std::string source) {
    _source = source;
  }

  int evtCounter;


      // ----------member data ---------------------------
    
edm::InputTag recoCaloMET_;
edm::InputTag patCaloMET_;

std::string outputFile_;

 edm::ParameterSet parameters;
  // Switch for verbosity
  std::string metname;
  std::string _source;

  /// number of Jet or MB HLT trigger paths 
  unsigned int nHLTPathsJetMB_;
  // list of Jet or MB HLT triggers
  std::vector<std::string > HLTPathsJetMBByName_;
  // list of Jet or MB HLT trigger index
  std::vector<unsigned int> HLTPathsJetMBByIndex_;

  // Et threshold for MET plots
  double _etThreshold;
  //
  bool _allhist;

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
  MonitorElement* recohCaloMEx;
  MonitorElement* recohCaloMEy;
  MonitorElement* recohCaloEz;
  MonitorElement* recohCaloMETSig;
  MonitorElement* recohCaloMET;
  MonitorElement* recohCaloMETPhi;
  MonitorElement* recohCaloSumET;
  MonitorElement* recohCaloMExLS;
  MonitorElement* recohCaloMEyLS;

  MonitorElement* recoCaloMaxEtInEmTowers;
  MonitorElement* recohCaloMaxEtInHadTowers;
  MonitorElement* recohCaloEtFractionHadronic;
  MonitorElement* recohCaloEmEtFraction;

  MonitorElement* recohCaloHadEtInHB;
  MonitorElement* recohCaloHadEtInHO;
  MonitorElement* recohCaloHadEtInHE;
  MonitorElement* recohCaloHadEtInHF;
  MonitorElement* recohCaloHadEtInEB;
  MonitorElement* recohCaloHadEtInEE;
  MonitorElement* recohCaloEmEtInHF;
  MonitorElement* recohCaloEmEtInEE;
  MonitorElement* recohCaloEmEtInEB;
////PAT
  MonitorElement* patjetME;
  MonitorElement* pathNevents;
  MonitorElement* pathCaloMEx;
  MonitorElement* pathCaloMEy;
  MonitorElement* pathCaloEz;
  MonitorElement* pathCaloMETSig;
  MonitorElement* pathCaloMET;
  MonitorElement* pathCaloMETPhi;
  MonitorElement* pathCaloSumET;
  MonitorElement* pathCaloMExLS;
  MonitorElement* pathCaloMEyLS;

  MonitorElement* patCaloMaxEtInEmTowers;
  MonitorElement* pathCaloMaxEtInHadTowers;
  MonitorElement* pathCaloEtFractionHadronic;
  MonitorElement* pathCaloEmEtFraction;

  MonitorElement* pathCaloHadEtInHB;
  MonitorElement* pathCaloHadEtInHO;
  MonitorElement* pathCaloHadEtInHE;
  MonitorElement* pathCaloHadEtInHF;
  MonitorElement* pathCaloHadEtInEB;
  MonitorElement* pathCaloHadEtInEE;
  MonitorElement* pathCaloEmEtInHF;
  MonitorElement* pathCaloEmEtInEE;
  MonitorElement* pathCaloEmEtInEB;


  
};

#endif

