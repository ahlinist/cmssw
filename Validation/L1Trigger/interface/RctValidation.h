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
#include "CondFormats/L1TObjects/interface/L1RCTParameters.h"


#include "CondFormats/L1TObjects/interface/L1CaloGeometry.h"
#include "CondFormats/DataRecord/interface/L1CaloGeometryRecord.h"
#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"
#include "CondFormats/DataRecord/interface/L1EmEtScaleRcd.h" 


#include "CondFormats/L1TObjects/interface/L1CaloEcalScale.h"
#include "CondFormats/DataRecord/interface/L1CaloEcalScaleRcd.h"
#include "CondFormats/L1TObjects/interface/L1CaloHcalScale.h"
#include "CondFormats/DataRecord/interface/L1CaloHcalScaleRcd.h"


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
  DQMStore* store;
  edm::InputTag ecalTPGs_;
  edm::InputTag hcalTPGs_;
  edm::InputTag rctEGammas_; 
  std::vector<edm::InputTag> gctEGammas_; 
  edm::InputTag gtEGammas_; 
  edm::InputTag refEGammas_;
  edm::InputTag refJetSums_;



	const L1CaloGeometry* caloGeom;
	const L1CaloEtScale* emS ;
	const L1CaloEcalScale* eS ;
	const L1CaloHcalScale* hS ;
  std::string directory_;

  std::string outfile_;
  bool matchL1Objects_;
	
	L1RCTParameters*	rctGeo;


  double maxEt_;
  int    binsEt_; 
  int    binsEta_;
  int    binsPhi_;
  double matchDR_;
  double egammaThreshold_;

  double tpgSumWindow_;
  double thresholdForEtaPhi_;
	double barrelBoundry_;
	double endcapBoundry_;

	double isolation_;
	double HEcut_;
	

  //MEs


  MonitorElement* TPG_Resolution;
  MonitorElement* RCT_Resolution;
	MonitorElement* RCT_IetaResolution[28];
	MonitorElement* RCT_EtaResolution;
	MonitorElement* RCT_PhiResolution;
	
	
	MonitorElement* refPt;
	MonitorElement* refEt;
	MonitorElement* refPtBarrel;
	MonitorElement* refPtEndcap;
	
  MonitorElement* refEta;
  MonitorElement* refPhi;
  MonitorElement* refEtaPhi;

	MonitorElement* tpgEffEt;
	MonitorElement* tpgEffPt;
	MonitorElement* tpgEffPtBarrel;
	MonitorElement* tpgEffPtEndcap;
  MonitorElement* tpgEffEta;
  MonitorElement* tpgEffPhi;
  MonitorElement* tpgEffEtaPhi;

  MonitorElement* HCALtpgPt;

	MonitorElement* rctEffPt;
	MonitorElement* rctEffPtBarrel;
	MonitorElement* rctEffPtEndcap;
	MonitorElement* rctEffEt;
	MonitorElement* rctEffPtHighest;
	MonitorElement* rctEffEtHighest;
  MonitorElement* rctEffEta;
  MonitorElement* rctEffPhi;
  MonitorElement* rctEffEtaPhi;

	
	MonitorElement* regionEGtowers;
	MonitorElement* regionEGtowersRatio;
	MonitorElement* regionEG2towersRatio;
	MonitorElement* regionEGHighTowerEt;
	


	MonitorElement* rctIsoEffPt;
	MonitorElement* rctIsoEffPtBarrel;
	MonitorElement* rctIsoEffPtEndcap;
	MonitorElement* rctIsoEffEt;
	MonitorElement* rctIsoEffPtHighest;
	MonitorElement* rctIsoEffEtHighest;
  MonitorElement* rctIsoEffEta;
  MonitorElement* rctIsoEffPhi;
  MonitorElement* rctIsoEffEtaPhi;

	MonitorElement* gctEffPt;
	MonitorElement* gctEffEt;
  MonitorElement* gctEffEta;
  MonitorElement* gctEffPhi;
  MonitorElement* gctEffEtaPhi;


	MonitorElement* gctIsoEffPt;
	MonitorElement* gctIsoEffEt;
  MonitorElement* gctIsoEffEta;
  MonitorElement* gctIsoEffPhi;
  MonitorElement* gctIsoEffEtaPhi;


  MonitorElement* rctEtaCorr;
  MonitorElement* rctEtaCorrIEta;
  MonitorElement* rctEtaCorrAbsIEta;

  MonitorElement* rctFGneighborEt;
  MonitorElement* rctFGMainTowerEt;
  MonitorElement* rctNoFGMainTowerEt;
  MonitorElement* rctPtNoSpike;

  MonitorElement* rctPtEGHEcut;
  MonitorElement* rctPtEGFGcut;

  MonitorElement* rctPtIsoEGHEFGcut;
  MonitorElement* rctPtIsoEGHEFGIsocut;

	MonitorElement* rctEtaCorrEt;
	MonitorElement* rctEtaCorrIEtaEt;
	MonitorElement* rctEtaCorrAbsIEtaEt;
	
	MonitorElement* rctEtaCorr1Tower;
	MonitorElement* rctEtaCorrIEta1Tower;
	MonitorElement* rctEtaCorrAbsIEta1Tower;
	
	MonitorElement* rctEtaCorr1TowerEt;
	MonitorElement* rctEtaCorrIEta1TowerEt;
	MonitorElement* rctEtaCorrAbsIEta1TowerEt;
	
	MonitorElement* rctEtaCorr12Tower;
	MonitorElement* rctEtaCorrIEta12Tower;
	MonitorElement* rctEtaCorrAbsIEta12Tower;
	
	MonitorElement* rctHEvL1Et;
	MonitorElement* rctHEvEt;
	MonitorElement* rctHEvECALEt;
	MonitorElement* rctHEvHCALEt;
	MonitorElement* rctHE;
	MonitorElement* rctHEafterFG;

	MonitorElement* refRxtal;

	MonitorElement* rctEtaCorr12TowerEt;
	MonitorElement* rctEtaCorrIEta12TowerEt;
	MonitorElement* rctEtaCorrAbsIEta12TowerEt;

	MonitorElement* tpgECALsecondtower;
	MonitorElement* tpgHCALSurronding;
	MonitorElement* tpgECALSurronding;
	MonitorElement* dirHCALclosest;
	MonitorElement* dirHCALclosestWeighted;
	MonitorElement* sumECAL;
	MonitorElement* sumHCAL;
	MonitorElement* maxHCALneighbor;
	MonitorElement* maxHCALneighborNoEcal;
	MonitorElement* regionSum;
	MonitorElement* regionHE;
	MonitorElement* regionMaxHE;
	MonitorElement* regionMaxHEafterFG;
	MonitorElement* diffSumEgamma;
	MonitorElement* minLSum;
	MonitorElement* minLSumHE;

	math::PtEtaPhiMLorentzVector rctLorentzVector(const L1GctEmCand&);//,const L1CaloGeometry* ,const  L1CaloEtScale *);

	double tpgSum(const math::XYZVector& direction,const EcalTrigPrimDigiCollection& ecalDigis,double deltaR);//const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale,double deltaR);
	
	//	EcalTrigPrimDigiCollection
	  //	findRegionTowers(const L1GctEmCand& cand,const EcalTrigPrimDigiCollection& ecalDigis);//,const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale);
	void
	  findRegionTowers(const L1GctEmCand& cand,const EcalTrigPrimDigiCollection& ecalDigis, EcalTrigPrimDigiCollection* l1region);//,const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale);
	std::vector<double> find3x3Towers(int etaCentral, int phiCentral ,const EcalTrigPrimDigiCollection& ecalDigis,const HcalTrigPrimDigiCollection& hcalDigis);//, std::vector<double> tpgs3x3);

	

  class RCTEmSorter {
  public:
    RCTEmSorter(){}
    ~RCTEmSorter(){}

    bool operator()(L1GctEmCand a1, L1GctEmCand a2)
    {
      return a1.rank() < a2.rank();
    }

  };

	class RCTEmEnergySorter{
	public:
		RCTEmEnergySorter( const reco::Candidate& cand , const L1CaloEtScale* scale ) : b(cand), emS(scale){}
		~RCTEmEnergySorter(){}
		const	reco::Candidate& b;
		const	L1CaloEtScale* emS ;
		bool operator()(L1GctEmCand a1, L1GctEmCand a2)
		{
			double energyComp = b.pt();
			
			return abs(emS->et(a1.rank()) -energyComp) < abs(emS->et(a2.rank())-energyComp);
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

        mappingIEta_.push_back(0.087);
        for (int i=2;i<=28;++i)
            mappingIEta_.push_back(mappingIEta_.back() + mappingEta_[i]);
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

    int iEta(double eta)
    {
        int iEta = 1;

        std::vector<double>::iterator itr;
        for ( itr = mappingIEta_.begin(); itr < mappingIEta_.end(); ++itr )
        {
            if ( fabs(eta) < *itr )
                break;

            ++iEta;
        }

        return (eta>0)? iEta : -iEta ;
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
    std::vector<double> mappingIEta_;
  };

  TriggerTowerGeometry *geo;
};
