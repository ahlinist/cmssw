/*Rct Validation from MC
Author : Michail Bachtis
University of Wisconsin-Madison
bachtis@hep.wisc.edu
*/

 
#include <memory>
#include <unistd.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"



//Include DQM core
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"


#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"


#include "CondFormats/L1TObjects/interface/L1CaloGeometry.h"
#include "CondFormats/DataRecord/interface/L1CaloGeometryRecord.h"
#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"
#include "CondFormats/DataRecord/interface/L1EmEtScaleRcd.h" 


#include "CondFormats/L1TObjects/interface/L1CaloEcalScale.h"
#include "CondFormats/DataRecord/interface/L1CaloEcalScaleRcd.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"


class RctValidation : public edm::EDAnalyzer {
public:
  RctValidation( const edm::ParameterSet& );
  ~RctValidation();

protected:
   

  /// BeginRun
  void beginRun(const edm::Run& r, const edm::EventSetup& c);

  /// Fake Analyze
  void analyze(const edm::Event& e, const edm::EventSetup& c) ;

  ///Luminosity Block 
  void beginLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
                            const edm::EventSetup& context) ;
  /// DQM Client Diagnostic
  void endLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
                          const edm::EventSetup& c);
  /// EndRun
  void endRun(const edm::Run& r, const edm::EventSetup& c);

  /// Endjob
  void endJob();



private:
  DQMStore* dbe_;  
  edm::InputTag ecalTPGs_;
  edm::InputTag rctEGammas_; 
  std::vector<edm::InputTag> gctEGammas_; 
  edm::InputTag gtEGammas_; 
  edm::InputTag refEGammas_;

  std::string directory_;

  double maxEt_;
  int    binsEt_; 
  int    binsEta_;
  int    binsPhi_;
  double matchDR_;
  double egammaThreshold_;
  double tpgSumWindow_;
  double thresholdForEtaPhi_;
 
  //MEs


  MonitorElement* TPG_Resolution;
  MonitorElement* RCT_Resolution;

  MonitorElement* refPt;
  MonitorElement* refEta;
  MonitorElement* refPhi;
  MonitorElement* refEtaPhi;

  MonitorElement* tpgEffPt;
  MonitorElement* tpgEffEta;
  MonitorElement* tpgEffPhi;
  MonitorElement* tpgEffEtaPhi;


  MonitorElement* rctEffPt;
  MonitorElement* rctEffEta;
  MonitorElement* rctEffPhi;
  MonitorElement* rctEffEtaPhi;


  MonitorElement* rctIsoEffPt;
  MonitorElement* rctIsoEffEta;
  MonitorElement* rctIsoEffPhi;
  MonitorElement* rctIsoEffEtaPhi;

  MonitorElement* gctEffPt;
  MonitorElement* gctEffEta;
  MonitorElement* gctEffPhi;
  MonitorElement* gctEffEtaPhi;


  MonitorElement* gctIsoEffPt;
  MonitorElement* gctIsoEffEta;
  MonitorElement* gctIsoEffPhi;
  MonitorElement* gctIsoEffEtaPhi;

  math::PtEtaPhiMLorentzVector rctLorentzVector(const L1GctEmCand&,const L1CaloGeometry* ,const  L1CaloEtScale *);

  double tpgSum(const math::XYZVector& direction,const EcalTrigPrimDigiCollection& ecalDigis,const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale,double deltaR);


  class RCTEmSorter {
  public:
    RCTEmSorter(){}
    ~RCTEmSorter(){}

    bool operator()(L1GctEmCand a1, L1GctEmCand a2)
    {
      return a1.rank() < a2.rank();
    }

  };



  class  TriggerTowerGeometry
  {
  public:
    TriggerTowerGeometry()
      {
	
	for(int i=1;i<=20;++i)
	  mappingEta_[i] =0.087;
	
	mappingEta_[21] = 0.09;
	mappingEta_[22] = 0.1;
	mappingEta_[23] = 0.113;
	mappingEta_[24] = 0.129;
	mappingEta_[25] = 0.15;
	mappingEta_[26] = 0.178;
	mappingEta_[27] = 0.15;
	mappingEta_[28] = 0.35;
      }
    
    double eta(int iEta)
    {
      double eta=0;
      for(int i=1;i<=abs(iEta);++i)
	{
	  eta+=mappingEta_[i];
	}
      eta-=mappingEta_[abs(iEta)]/2;
      
      if(iEta>0) return eta;
      else
	return -eta;
    }
    
    double phi(int iPhi)
    {
      return iPhi*0.087-0.087/2;
    }
    
    double towerEtaSize(int iEta)
    {
      return mappingEta_[abs(iEta)];
    }

  double towerPhiSize(int iPhi)
  {
    return 0.087;
  }
  
  private:
  std::map<int,double> mappingEta_;
  };

  TriggerTowerGeometry *geo;
};
