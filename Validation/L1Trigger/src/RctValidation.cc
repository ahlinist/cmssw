#include "Validation/L1Trigger/interface/RctValidation.h"
#include "Math/GenVector/VectorUtil.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <algorithm>

using namespace std;
using namespace reco;
using namespace edm;

//
// constructors and destructor
//
RctValidation::RctValidation( const edm::ParameterSet& iConfig ) :
  ecalTPGs_(iConfig.getParameter<edm::InputTag>("ecalTPGs")),
  hcalTPGs_(iConfig.getParameter<edm::InputTag>("hcalTPGs")),
  rctEGammas_(iConfig.getParameter<edm::InputTag >("rctEGamma")),
  gctEGammas_(iConfig.getParameter<std::vector<edm::InputTag> >("gctEGamma")),
  refEGammas_(iConfig.getParameter<edm::InputTag>("genEGamma")),
  //  refJetSums_(iConfig.getParameter<edm::InputTag>("genJetSums")),
  directory_(iConfig.getParameter<std::string>("directory")),  
  outfile_(iConfig.getUntrackedParameter<std::string>("outputFileName")),
  maxEt_(iConfig.getUntrackedParameter<double>("maxEt",100)),
  binsEt_(iConfig.getUntrackedParameter<int>("binsEt",20)),
  binsEta_(iConfig.getUntrackedParameter<int>("binsEta",32)),
  binsPhi_(iConfig.getUntrackedParameter<int>("binsPhi",36)),
  matchDR_(iConfig.getUntrackedParameter<double>("matchDeltaR",0.5)),
  HEcut_(iConfig.getUntrackedParameter<double>("heCut",0.1)),
  isolation_(iConfig.getUntrackedParameter<double>("iso",1.5)),
  


  egammaThreshold_(iConfig.getUntrackedParameter<double>("gammaThreshold",5.)),
  tpgSumWindow_(iConfig.getUntrackedParameter<double>("tpgSumWindow",0.4)),
  thresholdForEtaPhi_(iConfig.getUntrackedParameter<double>("thresholdForEtaPhi",4.)),
  barrelBoundry_(iConfig.getUntrackedParameter<double>("barrelBarrelBoundry",1.442)),
  endcapBoundry_(iConfig.getUntrackedParameter<double>("endcapBoundry",1.566)),
  matchL1Objects_(iConfig.getUntrackedParameter<bool>("matchL1Objects",true))


{
  geo = new TriggerTowerGeometry();
	rctGeo = new L1RCTParameters();
	
	if ( outfile_.size() != 0 ) 
	{
    	cout << "RCT Validation histograms will be saved to " << outfile_.c_str() << endl;
  	}

  
  //Get General Monitoring Parameters
  store = &*edm::Service<DQMStore>();
  if(store)
    {

      store->setCurrentFolder(directory_);

      TPG_Resolution    = store->book1D("TPG_Resolution"," e/#gamma ET-ref e/#gamma E_[T}/Ref e/#gamma E_{T} ",100,-1.,1.);
      RCT_Resolution    = store->book1D("RCT_Resolution"," e/#gamma ET-ref e/#gamma E_[T}/Ref e/#gamma E_{T} ",100,-1.,1.);
      char histo[200];	
      for(int i =0 ; i< 28 ; ++i){
	
	sprintf(histo, "Resolution for RCT electrons i#eta %i",i+1);
	RCT_IetaResolution[i] = 	 store->book1D(histo,histo,100,-1.,1.);
      }
      RCT_EtaResolution = store->book1D("RCT_EtaResolution"," e/#gamma #eta-ref e/#gamma #eta", 100, -2,2);
      RCT_PhiResolution = store->book1D("RCT_PhiResolution"," e/#gamma #phi-ref e/#gamma #phi", 100, -2,2);


      refPt     = store->book1D("refPt" ,"ref e/#gamma P_{T}",binsEt_,0.,maxEt_);
      //		refPt->setResetMe(false );
      refPtBarrel     = store->book1D("refPtBarrel" ,"ref e/#gamma P_{T}",binsEt_,0.,maxEt_);
      refPtEndcap     = store->book1D("refPtEndcap" ,"ref e/#gamma P_{T}",binsEt_,0.,maxEt_);
      refEt     = store->book1D("refEt" ,"ref e/#gamma E_{T}",binsEt_,0.,maxEt_);
      refEta    = store->book1D("refEta","ref e/#gamma E_{T}",binsEta_,-3,3);
      refPhi    = store->book1D("refPhi","ref e/#gamma E_{T}",binsPhi_,-3.145,3.145);
      refEtaPhi = store->book2D("refEtaPhi","ref e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.145,3.145);


      refPt->getTH1F()->Sumw2();
      refPtEndcap->getTH1F()->Sumw2();
      refPtBarrel->getTH1F()->Sumw2();
      refEt->getTH1F()->Sumw2();
      refEta->getTH1F()->Sumw2();
      refPhi->getTH1F()->Sumw2();
      //      refEtaPhi->getTH1F()->Sumw2();

      tpgEffPt     = store->book1D("tpgEffPt" ,"tpg e/#gamma P_{T}",binsEt_,0.,maxEt_);
      tpgEffPtBarrel     = store->book1D("tpgEffPtBarrel" ,"tpg e/#gamma P_{T}",binsEt_,0.,maxEt_);
      tpgEffPtEndcap     = store->book1D("tpgEffPtEndcap" ,"tpg e/#gamma P_{T}",binsEt_,0.,maxEt_);
      tpgEffEt     = store->book1D("tpgEffEt" ,"tpg e/#gamma E_{T}",binsEt_,0.,maxEt_);
      tpgEffEta    = store->book1D("tpgEffEta","tpg e/#gamma E_{T}",binsEta_,-3,3);
      tpgEffPhi    = store->book1D("tpgEffPhi","tpg e/#gamma E_{T}",binsPhi_,-3.145,3.145);
      tpgEffEtaPhi = store->book2D("tpgEffEtaPhi","tpg e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.145,3.145);


      tpgEffPt->getTH1F()->Sumw2();
      tpgEffPtBarrel->getTH1F()->Sumw2();		
      tpgEffPtEndcap->getTH1F()->Sumw2();
      
      tpgEffEt->getTH1F()->Sumw2();
      tpgEffEta->getTH1F()->Sumw2();
      tpgEffPhi->getTH1F()->Sumw2();

      HCALtpgPt = store->book1D("HCALtpgPt" ,"HCAL tpg e/#gamma P_{T}",80,0.,20);
      HCALtpgPt->getTH1F()->Sumw2();
      //      tpgEffEtaPhi->getTH1F()->Sumw2();



		rctEffPt     = store->book1D("rctEffPt" ,"rct e/#gamma P_{T}",binsEt_,0.,maxEt_);
		rctEffPtBarrel     = store->book1D("rctEffPtBarrel" ,"rct e/#gamma P_{T}",binsEt_,0.,maxEt_);
		rctEffPtEndcap     = store->book1D("rctEffPtEndcap" ,"rct e/#gamma P_{T}",binsEt_,0.,maxEt_);
		rctEffEt     = store->book1D("rctEffEt" ,"rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      rctEffEta    = store->book1D("rctEffEta","rct e/#gamma E_{T}",binsEta_,-3,3);
      rctEffPhi    = store->book1D("rctEffPhi","rct e/#gamma E_{T}",binsPhi_,-3.145,3.145);
      rctEffEtaPhi = store->book2D("rctEffEtaPhi","rct e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.145,3.145);

      
      rctEffPtHighest = store->book1D("rctEffPtHighest" ,"rct e/#gamma E_{T}",binsEt_,0.,maxEt_);

      rctPtNoSpike = store->book1D("rctPtNoSpike" ,"rct e/#gamma E_{T} wo spike",binsEt_,0.,maxEt_);
      rctPtEGHEcut = store->book1D("rctPtEGHEcut" ,"rct e/#gamma E_{T} objects with h/e and fg cut",binsEt_,0.,maxEt_);
      rctPtIsoEGHEFGcut = store->book1D("rctPtIsoEGHEFGcut" ,"rct isolated e/#gamma E_{T} h/e cut and fg cut ",binsEt_,0.,maxEt_);
      rctPtIsoEGHEFGIsocut = store->book1D("rctPtIsoEGHEFGIsocut" ,"rct isolated e/#gamma E_{T} h/e cut, fg cut, and iso ",binsEt_,0.,maxEt_);


      rctPtNoSpike->getTH1F()->Sumw2();
      rctPtEGHEcut->getTH1F()->Sumw2();
      rctPtIsoEGHEFGcut->getTH1F()->Sumw2();
      rctPtIsoEGHEFGIsocut->getTH1F()->Sumw2();


		rctEffPt->getTH1F()->Sumw2();
		rctEffPtBarrel->getTH1F()->Sumw2();
		rctEffPtEndcap->getTH1F()->Sumw2();
		rctEffEt->getTH1F()->Sumw2();
      rctEffEta->getTH1F()->Sumw2();
      rctEffPhi->getTH1F()->Sumw2();
      rctEffPtHighest->getTH1F()->Sumw2();
      //      rctEffEtaPhi->getTH1F()->Sumw2();


		rctIsoEffPt     = store->book1D("rctIsoEffPt" ,"rctIso e/#gamma P_{T}",binsEt_,0.,maxEt_);
		rctIsoEffPtBarrel     = store->book1D("rctIsoEffPtBarrel" ,"rctIso e/#gamma P_{T}",binsEt_,0.,maxEt_);
		rctIsoEffPtEndcap     = store->book1D("rctIsoEffPtEndcap" ,"rctIso e/#gamma P_{T}",binsEt_,0.,maxEt_);
		rctIsoEffEt     = store->book1D("rctIsoEffEt" ,"rctIso e/#gamma E_{T}",binsEt_,0.,maxEt_);
      rctIsoEffEta    = store->book1D("rctIsoEffEta","rctIso e/#gamma E_{T}",binsEta_,-3,3);
      rctIsoEffPhi    = store->book1D("rctIsoEffPhi","rctIso e/#gamma E_{T}",binsPhi_,-3.1458,3.1459);
      rctIsoEffEtaPhi = store->book2D("rctIsoEffEtaPhi","rctIso e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.1459,3.1459);


      rctIsoEffPtHighest = store->book1D("rctIsoEffPtHighest" ,"rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      
		rctIsoEffPt->getTH1F()->Sumw2();
		rctIsoEffPtBarrel->getTH1F()->Sumw2();
		rctIsoEffPtEndcap->getTH1F()->Sumw2();
		rctIsoEffEt->getTH1F()->Sumw2();
      rctIsoEffEta->getTH1F()->Sumw2();
      rctIsoEffPhi->getTH1F()->Sumw2();
      rctIsoEffPtHighest->getTH1F()->Sumw2();
      //      rctIsoEffEtaPhi->getTH1F()->Sumw2();


		gctEffPt     = store->book1D("gctEffPt" ,"gct e/#gamma P_{T}",binsEt_,0.,maxEt_);
		gctEffEt     = store->book1D("gctEffEt" ,"gct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      gctEffEta    = store->book1D("gctEffEta","gct e/#gamma E_{T}",binsEta_,-3,3);
      gctEffPhi    = store->book1D("gctEffPhi","gct e/#gamma E_{T}",binsPhi_,-3.1459,3.1459);
      gctEffEtaPhi = store->book2D("gctEffEtaPhi","gct e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.1459,3.1459);


		gctEffPt->getTH1F()->Sumw2();
		gctEffEt->getTH1F()->Sumw2();
      gctEffEta->getTH1F()->Sumw2();
      gctEffPhi->getTH1F()->Sumw2();
      //      gctEffEtaPhi->getTH1F()->Sumw2();

		gctIsoEffPt     = store->book1D("gctIsoEffPt" ,"gctIso e/#gamma P_{T}",binsEt_,0.,maxEt_);
		gctIsoEffEt     = store->book1D("gctIsoEffEt" ,"gctIso e/#gamma E_{T}",binsEt_,0.,maxEt_);
      gctIsoEffEta    = store->book1D("gctIsoEffEta","gctIso e/#gamma E_{T}",binsEta_,-3,3);
      gctIsoEffPhi    = store->book1D("gctIsoEffPhi","gctIso e/#gamma E_{T}",binsPhi_,-3.1459,3.1459);
      gctIsoEffEtaPhi = store->book2D("gctIsoEffEtaPhi","gctIso e/#gamma #eta #phi",binsEta_,-3,3,binsPhi_,-3.1459,3.1459);


		gctIsoEffPt->getTH1F()->Sumw2();

		gctIsoEffEt->getTH1F()->Sumw2();
      gctIsoEffEta->getTH1F()->Sumw2();
      gctIsoEffPhi->getTH1F()->Sumw2();

      rctEtaCorr = store->bookProfile("rctEtaCorr", "RCT #eta corrections", binsEta_,-3,3, 50, 0, 2); 

      rctEtaCorrIEta = store->bookProfile("rctEtaCorrIEta", "RCT #eta corrections", 65, -32.5, 32.5, 50, 0, 2);
      rctEtaCorrAbsIEta = store->bookProfile("rctEtaCorrAbsIEta", "RCT |#eta| corrections", 32, 0.5, 32.5, 50, 0, 2);

		rctEtaCorr1Tower = store->bookProfile("rctEtaCorr1Tower", "RCT #eta corrections (1 Tower restriction)", binsEta_,-3,3, 50, 0, 2); 
		rctEtaCorrIEta1Tower = store->bookProfile("rctEtaCorrIEta1Tower", "RCT #eta corrections (1 Tower restriction)", 65, -32.5, 32.5, 50, 0, 2);
		rctEtaCorrAbsIEta1Tower = store->bookProfile("rctEtaCorrAbsIEta1Tower", "RCT |#eta| corrections (1 Tower restriction)", 32, 0.5, 32.5, 50, 0, 2);

		rctEtaCorr12Tower = store->bookProfile("rctEtaCorr12Tower", "RCT #eta corrections (1 or 2 Tower restriction)", binsEta_,-3,3, 50, 0, 2); 
		rctEtaCorrIEta12Tower = store->bookProfile("rctEtaCorrIEta12Tower", "RCT #eta corrections (1 or 2 Tower restriction)", 65, -32.5, 32.5, 50, 0, 2);
		rctEtaCorrAbsIEta12Tower = store->bookProfile("rctEtaCorrAbsIEta12Tower", "RCT |#eta| corrections (1 or 2 Tower restriction)", 32, 0.5, 32.5, 50, 0, 2);
		
		
		rctEtaCorrEt = store->bookProfile("rctEtaCorrEt", "RCT #eta corrections", binsEta_,-3,3, 50, 0, 2); 
		rctEtaCorrIEtaEt = store->bookProfile("rctEtaCorrIEtaEt", "RCT #eta corrections", 65, -32.5, 32.5, 50, 0, 2);
		rctEtaCorrAbsIEtaEt = store->bookProfile("rctEtaCorrAbsIEtaEt", "RCT |#eta| corrections", 32, 0.5, 32.5, 50, 0, 2);
		
		rctEtaCorr1TowerEt = store->bookProfile("rctEtaCorr1TowerEt", "RCT #eta corrections (1 Tower restriction)", binsEta_,-3,3, 50, 0, 2); 
		rctEtaCorrIEta1TowerEt = store->bookProfile("rctEtaCorrIEta1TowerEt", "RCT #eta corrections (1 Tower restriction)", 65, -32.5, 32.5, 50, 0, 2);
		rctEtaCorrAbsIEta1TowerEt = store->bookProfile("rctEtaCorrAbsIEta1TowerEt", "RCT |#eta| corrections (1 Tower restriction)", 32, 0.5, 32.5, 50, 0, 2);
		
		rctEtaCorr12TowerEt = store->bookProfile("rctEtaCorr12TowerEt", "RCT #eta corrections (1 or 2 Tower restriction)", binsEta_,-3,3, 50, 0, 2); 
		rctEtaCorrIEta12TowerEt = store->bookProfile("rctEtaCorrIEta12TowerEt", "RCT #eta corrections (1 or 2 Tower restriction)", 65, -32.5, 32.5, 50, 0, 2);
		rctEtaCorrAbsIEta12TowerEt = store->bookProfile("rctEtaCorrAbsIEta12TowerEt", "RCT |#eta| corrections (1 or 2 Tower restriction)", 32, 0.5, 32.5, 50, 0, 2);
		
		
		
		regionEGtowers = store->book1D("regionEGtowers","nTowers in Region with L1 EG candidate",17,-0.5,16.5);
		regionEGtowersRatio = store->book1D("regionEGtowersRatio", "energy of peak tower to total of Region", 20, 0, 1);
		regionEG2towersRatio = store->book1D("regionEG2towersRatio", "energy of 2 peak tower to total of Region", 20, 0, 1);
		regionEGHighTowerEt = store->book1D("regionEGHighTowerEt" ,"tpg e/#gamma E_{T} of high tower in object",binsEt_,0.,maxEt_);


		regionEGtowersRatio->getTH1F()->Sumw2();
		regionEGHighTowerEt->getTH1F()->Sumw2();

		rctHEvL1Et = store->book2D("rctHEvL1Et","H/E values versus L1 e/#gamma E_{T}", 64, -0.5, 32.5, 40, 0, 1);
		rctHEvEt = store->book2D("rctHEvEt","H/E values versus e/#gamma E_{T}", 64, -0.5, 32.5, 40, 0, 1);
		rctHEvECALEt = store->book2D("rctHEvECALEt","H/E values versus ECAL e/#gamma E_{T}", 64, -0.5, 32.5, 40, 0, 1);
		rctHEvHCALEt = store->book2D("rctHEvHCALEt","H/E values versus HCAL e/#gamma E_{T}", 64, -0.5, 32.5, 40, 0, 1);
		rctHE = store->book1D("rctHE","H/E values of L1 e/#gamma objects", 40, 0, 1);
		rctHEafterFG = store->book1D("rctHEafterFG","H/E values of L1 e/#gamma objects with FG cut", 40, 0, 1);
		regionHE = store->book1D("regionHE","H/E values of L1 e/#gamma 3x3 objects", 40, 0, 1);
		regionMaxHE = store->book1D("regionMaxHE","H/E values of L1 e/#gamma 3x3 objects", 40, 0, 1);
		regionMaxHEafterFG = store->book1D("regionMaxHE","H/E values of L1 e/#gamma 3x3 objects", 40, 0, 1);

		rctHEvL1Et->getTH2F()->Sumw2();
		rctHEvEt->getTH2F()->Sumw2();
		rctHEvECALEt->getTH2F()->Sumw2();
		rctHEvHCALEt->getTH2F()->Sumw2();
		rctHE->getTH1F()->Sumw2();
		
		regionHE->getTH1F()->Sumw2();

		tpgECALsecondtower  = store->book1D("tpgECALsecondtower" ,"tpg e/#gamma E_{T} of neighboring tower in object",80,0.,20);

		tpgHCALSurronding  = store->book1D("tpgHCALSurronding" ,"tpg e/#gamma E_{T} sum of HCAL  of surronding towers )",80,0.,20);
		tpgECALSurronding  = store->book1D("tpgECALSurronding" ,"tpg e/#gamma E_{T} sum of HCAL region towers (minus neighbor)",80,0.,20);

		dirHCALclosest = store->book2D("dirHCALclosest","direction of a HCAL tower with energy in ieta/iphi space",3,-1.5,1.5,3,-1.5,1.5);
		dirHCALclosestWeighted = store->book2D("dirHCALclosestWeighted","direction of a HCAL tower with energy in ieta/iphi space*energy",3,-1.5,1.5,3,-1.5,1.5);
		sumECAL  = store->book1D("sumECAL" ,"tpg e/#gamma E_{T} sum of ECAL 3x3 region towers",80,0.,40);
		sumHCAL  = store->book1D("sumHCAL" ,"tpg e/#gamma E_{T} sum of HCAL 3x3 region towers",80,0.,40);
		regionSum =	    store->book1D("regionSum" ,"tpg e/#gamma E_{T} sum of 3x3 region towers",80,0.,40);
		diffSumEgamma = store->book1D("diffSumEgamma", "energy of other towers",40,0.,10);
		minLSum = store->book1D("minLSum", "Smallest neighbor energy of isolation comparison",40,0.,10);
		minLSumHE = store->book1D("minLSum", "Smallest neighbor energy of isolation comparison with HE 0.1 cut",40,0.,10);
	
		rctFGneighborEt = store->book1D("rctFGneighborEt","Max Et of neighbor with FG", 40, 0., 20);
		rctFGMainTowerEt = store->book1D("rctFGMainTowerEt"," Et of central with FG", 40, 0., 20);
		//		rctNoFGneighborEt = store->book1D("rctFGneighborEt","Max Et of neighbor without FG", 40, 0., 20);
		rctNoFGMainTowerEt = store->book1D("rctFGMainTowerEt"," Et of central without FG", 40, 0., 20);


		rctFGneighborEt->getTH1F()->Sumw2();
		rctFGMainTowerEt->getTH1F()->Sumw2();
		rctNoFGMainTowerEt->getTH1F()->Sumw2();
		tpgECALsecondtower->getTH1F()->Sumw2();
		tpgHCALSurronding->getTH1F()->Sumw2();
		tpgECALSurronding->getTH1F()->Sumw2();
		dirHCALclosest->getTH2F()->Sumw2();
		dirHCALclosestWeighted->getTH2F()->Sumw2();		
		sumECAL->getTH1F()->Sumw2();
		sumHCAL->getTH1F()->Sumw2();
		diffSumEgamma->getTH1F()->Sumw2();
		minLSum->getTH1F()->Sumw2();
		minLSumHE->getTH1F()->Sumw2();

		store->setVerbose(1);

    }
}

RctValidation::~RctValidation()
{
   if(geo != 0)
	   delete geo;
	
	if(rctGeo != 0)
		delete rctGeo;
	
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//--------------------------------------------------------


//--------------------------------------------------------
void RctValidation::beginRun(const edm::Run& r, const EventSetup& iSetup) {
	//Get The Geometry and the scale
	ESHandle< L1CaloGeometry > caloGeomESH ;
	iSetup.get< L1CaloGeometryRecord >().get( caloGeomESH ) ;
	// const L1CaloGeometry* caloGeom = caloGeomESH.product();
	caloGeom = caloGeomESH.product();
	
	// get energy scale to convert input from ECAL
	edm::ESHandle<L1CaloEcalScale> ecalScale;
	iSetup.get<L1CaloEcalScaleRcd>().get(ecalScale);
	//  const L1CaloEcalScale* eS = ecalScale.product();
	eS = ecalScale.product();

	// get energy scale to convert input from ECAL
	edm::ESHandle<L1CaloHcalScale> hcalScale;
	iSetup.get<L1CaloHcalScaleRcd>().get(hcalScale);
	//  const L1CaloEcalScale* eS = ecalScale.product();
	hS = hcalScale.product();
	
	ESHandle< L1CaloEtScale > emScale ;
	iSetup.get< L1EmEtScaleRcd >().get( emScale ) ;
	//  const L1CaloEtScale* emS = emScale.product();
	emS = emScale.product();
	
	//	std::cout << " referance pt nentries at begining of new run " << refPt->getEntries() << std::endl;
	
}

//--------------------------------------------------------
void RctValidation::beginLuminosityBlock(const LuminosityBlock& lumiSeg, 
				      const EventSetup& context) {
  
}

// ----------------------------------------------------------
void 
RctValidation::analyze(const Event& iEvent, const EventSetup& iSetup )
{
  //std::cout << " looking for referance " << std::endl;
  //     	std::cout << " referance pt nentries at begining of new event " << refPt->getEntries() << std::endl;
  //Get Reference Objects
  edm::Handle<edm::View<reco::Candidate> > genEGamma;
  bool gotRef = iEvent.getByLabel(refEGammas_,genEGamma) && !genEGamma.failedToGet();

	if(!gotRef)
		std::cout << " no referance found " << std::endl;
	else	
		std::cout << "  referance FOUND " << std::endl;
  //get ECAL TPGs
  edm::Handle<EcalTrigPrimDigiCollection> ecalTPGs;
  bool gotECALTPs = iEvent.getByLabel(ecalTPGs_,ecalTPGs);
  if(!gotECALTPs)
    printf("NO ECAL TPs found\n");

  //get ECAL TPGs
  edm::Handle<HcalTrigPrimDigiCollection> hcalTPGs;
  bool gotHCALTPs = iEvent.getByLabel(hcalTPGs_,hcalTPGs);
  if(!gotHCALTPs)
    printf("NO HCAL TPs found\n");

  //Get Egammas from RCT
  double highestEG = 0;
  double highestEGiso = 0;
  L1GctEmCandCollection rctEGammas;
  edm::Handle<L1CaloEmCollection> egamma;

  L1CaloEmCand rctEGamma;	
  if(iEvent.getByLabel(rctEGammas_,egamma) && !egamma.failedToGet())
    for(L1CaloEmCollection::const_iterator i=egamma->begin();i!=egamma->end();++i)
    {
      L1CaloEmCand rctEGammaTemp = *i;
      rctEGammas.push_back(L1GctEmCand(rctEGammaTemp));
      if(highestEG < emS->et(i->rank())) {
	highestEG = emS->et(i->rank());
	rctEGamma = *i;
      }
      if ( i->isolated() )
        highestEGiso = max(highestEGiso, emS->et(i->rank()));


    }


  if ( highestEG > 0 ) {
    rctEffPtHighest->Fill(highestEG);   
    if( highestEG>egammaThreshold_ && !matchL1Objects_){
      EcalTrigPrimDigiCollection* l1TrigTowers = new EcalTrigPrimDigiCollection();
	  findRegionTowers(rctEGamma,*ecalTPGs,l1TrigTowers);

	  EcalTriggerPrimitiveDigi highestEtTT;
	  int nTowers =0 ;
	  double totalEnergy = 0., highestEtTTet =-1.,highestEtTTHCAL= 0.;
	  bool centralFG = false, centralSpike = false;
	  for(EcalTrigPrimDigiCollection::const_iterator iTT = l1TrigTowers->begin() ; iTT != l1TrigTowers->end(); ++iTT){
	    //			  std::cout << "loop, high trig tower et " << highestEtTTet <<std::endl;
	    double et = eS->et(iTT->compressedEt(),abs(iTT->id().ieta()),iTT->id().ieta()/abs(iTT->id().ieta()));	
	    //			  std::cout << "loop, trig tower  compressed et " << iTT->compressedEt(); //<<std::endl;	
	    totalEnergy += et;
	    if(et> 0)
	      ++nTowers;
	    if(et > highestEtTTet ) {
	      highestEtTT = *iTT;
	      highestEtTTet =et;
	      centralFG = iTT->fineGrain();
	      centralSpike = !(iTT->l1aSpike());
	    }
	  }
	  if(!centralSpike) {// don't look at spike events or fineGrain events\	  
	  
	    rctPtNoSpike->Fill(emS->et(rctEGamma.rank()));
	  
	    int ieta =  highestEtTT.id().ieta();
	    int iphi = highestEtTT.id().iphi();
	    double energyMaxFGcorner = -1;
	    //    std::cout<< " new part begining";
	    std::vector<double> tpgs3x3 =    find3x3Towers(ieta, iphi, *ecalTPGs, *hcalTPGs);
	    //    std::cout << " after finding 3x3 towers size " << tpgs3x3.size() <<std::endl;
	    double ecal3x3Tot =0.;
	    double hcal3x3Tot =0.;
	    double highestNeighborEcal = -1;
	    double minELsum = 9999;
	    double highestHEneighbor = 0;
	    for(int i = 0; i< 9; ++i){

	      double ecalTTEt= tpgs3x3.at(i);
	      double hcalTTEt = tpgs3x3.at(i+9);
	    
	      if(hcalTTEt>0){
		dirHCALclosest->Fill((i%3)-1,(i/3)-1);
		dirHCALclosestWeighted->Fill((i%3)-1,(i/3)-1, hcalTTEt);
		if(highestHEneighbor < (hcalTTEt/ecalTTEt ))
		  highestHEneighbor = hcalTTEt/ecalTTEt ;
	      }
	      
	      ecal3x3Tot += ecalTTEt;
	      hcal3x3Tot += hcalTTEt;
	      if( i%2 ==1  )  { // find neighbors of ecal
		if(ecalTTEt > highestNeighborEcal)
		  highestNeighborEcal = ecalTTEt;
	      }

	      if(i%2 ==0 && i!=4){ 
		double tempELsum = 0;
		int cornerEta = i/3;
		int cornerPhi = i%3;
		for(int j=0; j<3; ++j){
		  if(tempELsum <  tpgs3x3.at(cornerEta*3 + j))
		    tempELsum =   tpgs3x3.at(cornerEta*3 + j);
		  if(tempELsum <  tpgs3x3.at(j*3 + cornerPhi))
		    tempELsum = tpgs3x3.at(j*3 + cornerPhi);
		}
		if(tempELsum < minELsum)
		  minELsum = tempELsum;
	      }
	      if(i==4)
		highestEtTTHCAL = hcalTTEt;
	      else
		if(tpgs3x3.at(i+18 ) == 1)
		  energyMaxFGcorner = ecalTTEt;	      
	    }
	    
	  
	    double rctHe =highestEtTTHCAL/highestEtTTet;
	    rctHE->Fill(rctHe);
	    rctHEvL1Et->Fill(emS->et(rctEGamma.rank()),rctHe);
	    
	    rctHEvECALEt->Fill(highestEtTTet,rctHe);
	    rctHEvHCALEt->Fill(highestEtTTHCAL,rctHe);
	    HCALtpgPt->Fill(highestEtTTHCAL);
	    tpgECALsecondtower->Fill(highestNeighborEcal);
	    tpgHCALSurronding->Fill(hcal3x3Tot);
	    tpgECALSurronding->Fill(ecal3x3Tot-highestEtTTet);
	    regionSum->Fill(hcal3x3Tot+ecal3x3Tot);
	    regionHE->Fill(hcal3x3Tot/ecal3x3Tot);
	    diffSumEgamma->Fill(ecal3x3Tot-highestEtTTet - highestNeighborEcal);
	    
	    regionMaxHE->Fill(highestHEneighbor);
	    if(centralFG) {
	      rctFGMainTowerEt->Fill(highestEtTTet);
	    } else {
	      rctHEafterFG->Fill(rctHe);
	      if(rctHe < HEcut_){
	      rctPtEGHEcut->Fill(emS->et(rctEGamma.rank()));
	      
	      if(energyMaxFGcorner > 0) 
		rctFGneighborEt->Fill(energyMaxFGcorner);
	      else {
		minLSum->Fill(minELsum);
		regionMaxHEafterFG->Fill(highestHEneighbor);
		if(highestHEneighbor < HEcut_) {
		  rctPtIsoEGHEFGcut->Fill(emS->et(rctEGamma.rank()));
		  minLSumHE->Fill(minELsum);
		  if(minELsum < isolation_)
		    rctPtIsoEGHEFGIsocut->Fill(emS->et(rctEGamma.rank()));
		}
	      }
	    }
	  }
	  }
    }
  }
  else	
    std::cout << " no RCT objects found" << std::endl;

  if ( highestEGiso > 0 )
    rctIsoEffPtHighest->Fill(highestEGiso);

  //Get Egammas from GCT
  L1GctEmCandCollection gctEGammas;
  edm::Handle<L1GctEmCandCollection> gctEgamma;
  if(iEvent.getByLabel(gctEGammas_[0],gctEgamma))
    gctEGammas = *gctEgamma;

  //Get Isolated Egammas from GCT
  L1GctEmCandCollection gctIsoEGammas;
  edm::Handle<L1GctEmCandCollection> gctIsoEgamma;
  if(iEvent.getByLabel(gctEGammas_[1],gctIsoEgamma))
    gctIsoEGammas = *gctIsoEgamma;

//  	std::cout << "testing" << std::endl;
  //OK Now Efficiency calculations
	
  //	std::cout << " number of objects " << genEGamma->size() <<std::endl;
	if(gotRef){
	  for(  edm::View<reco::Candidate> ::const_iterator j = genEGamma->begin() ;   j != genEGamma->end(); ++j )
 //   for(unsigned int j=0;j<genEGamma->size();++j) 
    {
      //      std::cout << " electron energy " << j->pt() <<std::endl;
		if(j->pt() < 1) 
			continue;
		
		
	std::vector<unsigned int>  refNearReference;	
		bool higherRefPresent = false;
//				for(  edm::View<reco::Candidate> ::const_iterator refCand = genEGamma->begin() ;   refCand != genEGamma->end(); ++refCand ){
		for(unsigned int refCand=0;refCand<genEGamma->size();++refCand) { 
			if(genEGamma->at(refCand).pt() == j->pt() && ROOT::Math::VectorUtil::DeltaR(genEGamma->at(refCand).p4(),j->p4()) == 0)  {

				continue;
			}
			//					if(refCand->pt() > j-pt()  && 
			if(genEGamma->at(refCand).pt() > 1){		
			  double deltaR = ROOT::Math::VectorUtil::DeltaR(genEGamma->at(refCand).p4(),j->p4());
			  if( deltaR < matchDR_) {
			    refNearReference.push_back(refCand);				
			    if(genEGamma->at(refCand).pt() > j->pt())
			      higherRefPresent = true;								
			  }
			}
		}
		if(higherRefPresent)
		  continue;
		
		
		refPt->Fill(j->pt());
		refEt->Fill(j->et());
		refEta->Fill(j->eta());
		refPhi->Fill(j->phi());
		
		if(fabs(j->phi()) < barrelBoundry_)
			refPtBarrel->Fill(j->pt());
		else if(fabs(j->phi()) > endcapBoundry_)
			refPtEndcap->Fill(j->pt());
	
      if(j->pt()>thresholdForEtaPhi_)
        refEtaPhi->Fill(j->eta(),j->phi());
//		else
//			continue;
		
      //TPG Efficiency
      //get TPG Sum
		double tpgS = tpgSum(j->p4().Vect(),*ecalTPGs,tpgSumWindow_);//,caloGeom,eS,tpgSumWindow_);
      //If there is cosignificant ecal energy over threshold passes:
      if(tpgS> egammaThreshold_)
      {
		  tpgEffPt->Fill(j->pt());
		  tpgEffEt->Fill(j->et());
        tpgEffEta->Fill(j->eta());
        tpgEffPhi->Fill(j->phi());
        if(j->pt()>thresholdForEtaPhi_)
          tpgEffEtaPhi->Fill(j->eta(),j->phi());
      }
      //		std::cout << "testing2 " << std::endl;
      if(tpgS<emS->et(63)) // max energy
        TPG_Resolution->Fill((tpgS-j->pt())/j->pt());

      //get RCT Efficiency-Find the highest object near the ref
      L1GctEmCandCollection rctNearReference;
	/*
		std::vector<unsigned int>  refNearReference;	
//		for(  edm::View<reco::Candidate> ::const_iterator refCand = genEGamma->begin() ;   refCand != genEGamma->end(); ++refCand ){
		for(unsigned int refCand=0;refCand<genEGamma->size();++refCand) { 
			if(genEGamma->at(refCand).pt()  == j->pt() && ROOT::Math::VectorUtil::DeltaR(genEGamma->at(refCand).p4(),j->p4()) == 0)  continue;
			if(genEGamma->at(refCand).pt() > 1){		
				double deltaR = ROOT::Math::VectorUtil::DeltaR(genEGamma->at(refCand).p4(),j->p4());
				 if( deltaR < matchDR_)
					 refNearReference.push_back(refCand);
			}
		}
	 */
      if(rctEGammas.size()>0)
        for(L1GctEmCandCollection::const_iterator i=rctEGammas.begin();i!=rctEGammas.end();++i)
          if(i->rank()>0)
          {
            //create lorenzt vector
			  math::PtEtaPhiMLorentzVector rctVec = rctLorentzVector(*i);//,caloGeom),emS);
			  double deltaR = ROOT::Math::VectorUtil::DeltaR(rctVec,j->p4());
            //ok now match and do it over threshold
            if( deltaR < matchDR_)// && rctVec.pt() >= egammaThreshold_ )
              rctNearReference.push_back(*i);
          }
      if(rctNearReference.size()==0)
		std::cout << "no L1 egamma found " << std::endl;
      if(rctNearReference.size()>0) 
      {
	//		  std::cout << "testing 3a" << std::endl;
        //Ok find the highest matched rct cand
        RCTEmSorter sorter;
		  RCTEmEnergySorter esorter(*j, emS);
		  std::sort(rctNearReference.begin(),rctNearReference.end(),esorter);
        L1GctEmCand highestRCT = rctNearReference.at(0);

	math::PtEtaPhiMLorentzVector highestVec = rctLorentzVector(highestRCT);//,caloGeom,emS);

	//		  		  std::cout << "highest RCT found rank" << highestRCT << std::endl;
    if(  matchL1Objects_){
      EcalTrigPrimDigiCollection* l1TrigTowers = new EcalTrigPrimDigiCollection();
	  findRegionTowers(highestRCT,*ecalTPGs,l1TrigTowers);

	  EcalTriggerPrimitiveDigi highestEtTT;
	  int nTowers =0 ;
	  double totalEnergy = 0., highestEtTTet =-1.,highestEtTTHCAL= 0.;
	  bool centralFG = false, centralSpike = false;
	  for(EcalTrigPrimDigiCollection::const_iterator iTT = l1TrigTowers->begin() ; iTT != l1TrigTowers->end(); ++iTT){
	    //			  std::cout << "loop, high trig tower et " << highestEtTTet <<std::endl;
	    double et = eS->et(iTT->compressedEt(),abs(iTT->id().ieta()),iTT->id().ieta()/abs(iTT->id().ieta()));	
	    //			  std::cout << "loop, trig tower  compressed et " << iTT->compressedEt(); //<<std::endl;	
	    totalEnergy += et;
	    if(et> 0)
	      ++nTowers;
	    if(et > highestEtTTet ) {
	      highestEtTT = *iTT;
	      highestEtTTet =et;
	      centralFG = iTT->fineGrain();
	      centralSpike = !(iTT->l1aSpike());
	    }
	  }
	  if(!centralSpike) {// don't look at spike events or fineGrain events\	  
	  
	    rctPtNoSpike->Fill(emS->et(highestRCT.rank()));
	  
	    int ieta =  highestEtTT.id().ieta();
	    int iphi = highestEtTT.id().iphi();
	    double energyMaxFGcorner = -1;
	    //    std::cout<< " new part begining";
	    std::vector<double> tpgs3x3 =    find3x3Towers(ieta, iphi, *ecalTPGs, *hcalTPGs);
	    //    std::cout << " after finding 3x3 towers size " << tpgs3x3.size() <<std::endl;
	    double ecal3x3Tot =0.;
	    double hcal3x3Tot =0.;
	    double highestNeighborEcal = -1;
	    double minELsum = 9999;
	    double highestHEneighbor = 0;
	    for(int i = 0; i< 9; ++i){

	      double ecalTTEt= tpgs3x3.at(i);
	      double hcalTTEt = tpgs3x3.at(i+9);
	    
	      if(hcalTTEt>0){
		dirHCALclosest->Fill((i%3)-1,(i/3)-1);
		dirHCALclosestWeighted->Fill((i%3)-1,(i/3)-1, hcalTTEt);
		if(highestHEneighbor < (hcalTTEt/ecalTTEt ))
		  highestHEneighbor = hcalTTEt/ecalTTEt ;
	      }
	      
	      ecal3x3Tot += ecalTTEt;
	      hcal3x3Tot += hcalTTEt;
	      if( i%2 ==1  )  { // find neighbors of ecal
		if(ecalTTEt > highestNeighborEcal)
		  highestNeighborEcal = ecalTTEt;
	      }

	      if(i%2 ==0 && i!=4){ 
		double tempELsum = 0;
		int cornerEta = i/3;
		int cornerPhi = i%3;
		for(int j=0; j<3; ++j){
		  if(tempELsum <  tpgs3x3.at(cornerEta*3 + j))
		    tempELsum =   tpgs3x3.at(cornerEta*3 + j);
		  if(tempELsum <  tpgs3x3.at(j*3 + cornerPhi))
		    tempELsum = tpgs3x3.at(j*3 + cornerPhi);
		}
		if(tempELsum < minELsum)
		  minELsum = tempELsum;
	      }
	      if(i==4)
		highestEtTTHCAL = hcalTTEt;
	      else
		if(tpgs3x3.at(i+18 ) == 1)
		  energyMaxFGcorner = ecalTTEt;	      
	    }
	    
	  
	    double rctHe =highestEtTTHCAL/highestEtTTet;
	    rctHE->Fill(rctHe);
	    rctHEvL1Et->Fill(emS->et(highestRCT.rank()),rctHe);
	    
	    rctHEvECALEt->Fill(highestEtTTet,rctHe);
	    rctHEvHCALEt->Fill(highestEtTTHCAL,rctHe);
	    HCALtpgPt->Fill(highestEtTTHCAL);
	    tpgECALsecondtower->Fill(highestNeighborEcal);
	    tpgHCALSurronding->Fill(hcal3x3Tot);
	    tpgECALSurronding->Fill(ecal3x3Tot-highestEtTTet);
	    regionSum->Fill(hcal3x3Tot+ecal3x3Tot);
	    regionHE->Fill(hcal3x3Tot/ecal3x3Tot);
	    diffSumEgamma->Fill(ecal3x3Tot-highestEtTTet - highestNeighborEcal);
	    
	    regionMaxHE->Fill(highestHEneighbor);
	    if(centralFG) {
	      rctFGMainTowerEt->Fill(highestEtTTet);
	    } else {
	      rctHEafterFG->Fill(rctHe);
	      if(rctHe < HEcut_){
	      rctPtEGHEcut->Fill(emS->et(highestRCT.rank()));
	      
	      if(energyMaxFGcorner > 0) 
		rctFGneighborEt->Fill(energyMaxFGcorner);
	      else {
		minLSum->Fill(minELsum);
		regionMaxHEafterFG->Fill(highestHEneighbor);
		if(highestHEneighbor < HEcut_) {
		  rctPtIsoEGHEFGcut->Fill(emS->et(highestRCT.rank()));
		  minLSumHE->Fill(minELsum);
		  if(minELsum < isolation_)
		    rctPtIsoEGHEFGIsocut->Fill(emS->et(highestRCT.rank()));
		}
	      }
	    }
	  }
	  }
    }	
      
        
	// find the towers of highest vec
	
	EcalTrigPrimDigiCollection* l1TrigTowers = new EcalTrigPrimDigiCollection();
	
	findRegionTowers(highestRCT,*ecalTPGs,l1TrigTowers);
	
	EcalTriggerPrimitiveDigi highestEtTT;
	int nTowers =0 ;
	double totalEnergy = 0., highestEtTTet =-1.,highestEtTTHCAL= 0.;
	for(EcalTrigPrimDigiCollection::const_iterator iTT = l1TrigTowers->begin() ; iTT != l1TrigTowers->end(); ++iTT){
	  //			  std::cout << "loop, high trig tower et " << highestEtTTet <<std::endl;
	  double et = eS->et(iTT->compressedEt(),abs(iTT->id().ieta()),iTT->id().ieta()/abs(iTT->id().ieta()));	
	  //			  std::cout << "loop, trig tower  compressed et " << iTT->compressedEt(); //<<std::endl;	
	  totalEnergy += et;
	  if(et> 0)
	    ++nTowers;
	  if(et > highestEtTTet ) {
	    highestEtTT = *iTT;
	    highestEtTTet =et;
	  }
	}
	int ieta =  highestEtTT.id().ieta();
	int iphi = highestEtTT.id().iphi();
	
	  HcalTrigTowerDetId ttDetId(ieta,iphi);
	
	  HcalTrigPrimDigiCollection::const_iterator hcalTT = hcalTPGs->find(ttDetId);
	  if(hcalTT->id() == ttDetId) // if TP is present in collection, hcalTT is the end piece
	    highestEtTTHCAL = hS->et(hcalTT->SOI_compressedEt(),ieta,ttDetId.zside());
	  else //
	    highestEtTTHCAL = 0;

	
	//			if(highestEtTTHCAL ==-1)  { //find out this sucker
	//			  std::cout << "- energy hcal found ! compressed et " <<hcalTT->SOI_compressedEt()  << " ieta " << HCALieta << " zside ? " << ttDetId.zside() <<std::endl;
	//			}
	//			  std::cout << " bad find: number of HcalTPGs is " << hcalTPGs->size() << std::endl;
	//			int HCALnewIeta = hcalTT->id().ieta();
	//			int HCALnewIphi = hcalTT->id().iphi();
	//			std::cout<< " trig tower ieta " << HCALieta << " trig tower iphi " <<iTT->id().iphi()<< "hcal tower id " << ttDetId.rawId() << std::endl << " translated hcal ieta " << HCALnewIeta << "translated hcal iphi " << HCALnewIphi << std::endl;


			if(emS->et(highestRCT.rank())>egammaThreshold_) {
		       
			  if(nTowers>0){
			    regionEGHighTowerEt->Fill(highestEtTTet);
				regionEGtowersRatio->Fill(highestEtTTet/totalEnergy);
			  }
			  regionEGtowers->Fill(nTowers);
			  if(j->pt() < egammaThreshold_){
				  
			    			    std::cout << "low object above thresh " << highestVec  << 	   " high resoulution RCT info"  << " elec  info ET" << j->pt() << " eta " << j->eta()<< " phi " << j->phi() <<std::endl;
			    //			    std::cout << " all rct objects near " << std::endl;
			    for(L1GctEmCandCollection::const_iterator ll = rctNearReference.begin(); ll != rctNearReference.end() ; ++ll){
					  std::cout << *ll  << std::endl;
			    }
			    //				  std::cout << " all ref objects near " << std::endl;
				  for(unsigned int lk = 0; lk < refNearReference.size() ; ++lk)
				    		  std::cout << " elec  info ET" << genEGamma->at(lk).pt() << " eta " << genEGamma->at(lk).eta()<< " phi " << genEGamma->at(lk).phi() << std::endl;
			  }

			  //			  std::cout << " h/e is " << rctHe << " ecal energy " << highestEtTTet << " hcal energy " << highestEtTTHCAL << std::endl ;

			  rctEffPt->Fill(j->pt());
			  rctEffEt->Fill(j->et());
          rctEffEta->Fill(j->eta());
          rctEffPhi->Fill(j->phi());
			  
			  if(fabs(j->phi()) < barrelBoundry_)
				  rctEffPtBarrel->Fill(j->pt());
			  else if(fabs(j->phi()) > endcapBoundry_)
				  rctEffPtEndcap->Fill(j->pt());
		
          if(j->pt()>thresholdForEtaPhi_)
            rctEffEtaPhi->Fill(j->eta(),j->phi());

			  if(j->pt()>egammaThreshold_ && highestRCT.rank()<63 && (j->pt() < (0.8 * emS->et(63)))){  // only for non-saturating
				  RCT_Resolution->Fill(( emS->et(highestRCT.rank())-j->pt())/j->pt() );
				  RCT_IetaResolution[abs(highestEtTT.id().ieta()) -1]->Fill(( emS->et(highestRCT.rank())-j->pt())/j->pt() );
			  if( (emS->et(highestRCT.rank())-j->pt())/j->pt() >1) {
				  continue;
//			  std:cout  << " high resoulution RCT info"  << highestRCT << " elec  info ET" << j->pt() << " eta " << j->eta()<< " phi " << j->phi() <<std::endl;
			  }
				  
			  }
	  //			std::cout << "testing 5" << std::endl;
		  if(highestRCT.isolated()) {
          
			  rctIsoEffPt->Fill(j->pt());
			  rctIsoEffEt->Fill(j->et());
            rctIsoEffEta->Fill(j->eta());
            rctIsoEffPhi->Fill(j->phi());
            if(j->pt()>thresholdForEtaPhi_)
              rctIsoEffEtaPhi->Fill(j->eta(),j->phi());
			  if(fabs(j->phi()) < barrelBoundry_)
				  rctIsoEffPtBarrel->Fill(j->pt());
			  else if(fabs(j->phi()) > endcapBoundry_)
				  rctIsoEffPtEndcap->Fill(j->pt());
			  
			  
                
            if( j->pt()>egammaThreshold_ && highestRCT.rank() <63 && (j->pt() < (1 * emS->et(63) )))  // only for non-saturating
            {
				
				double rctEt = emS->et(highestRCT.rank());
              rctEtaCorr->Fill( highestVec.eta(), j->pt()/highestVec.pt() );
              rctEtaCorrIEta->Fill( highestEtTT.id().ieta(), j->pt()/rctEt );
              rctEtaCorrAbsIEta->Fill( fabs(highestEtTT.id().ieta()), j->pt()/rctEt );
				if(nTowers ==1 || (nTowers==2 && highestEtTT.id().ieta() >=27)){
					rctEtaCorr1Tower->Fill( highestVec.eta(), j->pt()/rctEt );
					rctEtaCorrIEta1Tower->Fill( highestEtTT.id().ieta(), j->pt()/rctEt );
					rctEtaCorrAbsIEta1Tower->Fill( fabs(highestEtTT.id().ieta()), j->pt()/rctEt );
					rctEtaCorr1TowerEt->Fill( highestVec.eta(), j->et()/rctEt );
					rctEtaCorrIEta1TowerEt->Fill( highestEtTT.id().ieta(), j->et()/rctEt );
					rctEtaCorrAbsIEta1TowerEt->Fill( fabs(highestEtTT.id().ieta()), j->et()/rctEt );
					
				}
				if(nTowers ==2){
					rctEtaCorr12Tower->Fill( highestVec.eta(), j->pt()/rctEt );
					rctEtaCorrIEta12Tower->Fill( highestEtTT.id().ieta(), j->pt()/rctEt );
					rctEtaCorrAbsIEta12Tower->Fill( fabs(highestEtTT.id().ieta()), j->pt()/rctEt );
					rctEtaCorr12TowerEt->Fill( highestVec.eta(), j->et()/rctEt );
					rctEtaCorrIEta12TowerEt->Fill( highestEtTT.id().ieta(), j->et()/rctEt );
					rctEtaCorrAbsIEta12TowerEt->Fill( fabs(highestEtTT.id().ieta()), j->et()/rctEt );
					
				}
          }
          }
	  delete l1TrigTowers;
		  }
      }
      //		std::cout << "testing 6" << std::endl;

      //Now THE GCT Efficiency

      L1GctEmCandCollection gctNearReference;

      if(gctEGammas.size()>0)
        for(L1GctEmCandCollection::const_iterator i=gctEGammas.begin();i!=gctEGammas.end();++i)
          if(i->rank()>0)
          {
            //create lorenzt vector
			  math::PtEtaPhiMLorentzVector gctVec = rctLorentzVector(*i);//,caloGeom,emS);
            double deltaR = ROOT::Math::VectorUtil::DeltaR(gctVec,j->p4());
            //ok now match and do it over 10 GeV
            if(deltaR<matchDR_&&gctVec.pt()>=egammaThreshold_)
            {
              gctNearReference.push_back(*i);
            }
          }

      if(gctNearReference.size()>0) 
      {
        //Ok find the highest matched gct cand
        RCTEmSorter gsorter;

        std::sort(gctNearReference.begin(),gctNearReference.end(),gsorter);
        L1GctEmCand highestGCT = gctNearReference.at(0);
        //get its LV
		  math::PtEtaPhiMLorentzVector highestVec = rctLorentzVector(highestGCT);//,caloGeom,emS);
        //ask for threshold
        if(highestVec.pt()>egammaThreshold_) 
        {
			gctEffPt->Fill(j->pt());
			gctEffEt->Fill(j->et());
          gctEffEta->Fill(j->eta());
          gctEffPhi->Fill(j->phi());
          if(j->pt()>thresholdForEtaPhi_)
            gctEffEtaPhi->Fill(j->eta(),j->phi());
        }
      }


      L1GctEmCandCollection gctIsoNearReference;

      if(gctIsoEGammas.size()>0)
        for(L1GctEmCandCollection::const_iterator i=gctIsoEGammas.begin();i!=gctIsoEGammas.end();++i)
          if(i->rank()>0)
          {
              //create lorenzt vector
              math::PtEtaPhiMLorentzVector gctIsoVec = rctLorentzVector(*i);//,caloGeom,emS);
              double deltaR = ROOT::Math::VectorUtil::DeltaR(gctIsoVec,j->p4());
              //ok now match and do it over 10 GeV
              if(deltaR<matchDR_&&gctIsoVec.pt()>=egammaThreshold_)
              {
                gctIsoNearReference.push_back(*i);
              }
          }

      if(gctIsoNearReference.size()>0) 
      {
        //Ok find the highest matched gctIso cand
        RCTEmSorter ggsorter;
        std::sort(gctIsoNearReference.begin(),gctIsoNearReference.end(),ggsorter);
        L1GctEmCand highestGCTISO = gctIsoNearReference.at(0);
        //get its LV
		  math::PtEtaPhiMLorentzVector highestVec = rctLorentzVector(highestGCTISO);//,caloGeom,emS);
        //ask for threshold
        if(highestVec.pt()>egammaThreshold_) 
        {
			gctEffPt->Fill(j->pt());
			gctEffEt->Fill(j->et());
          gctEffEta->Fill(j->eta());
          gctEffPhi->Fill(j->phi());
          if(j->pt()>thresholdForEtaPhi_)
            gctEffEtaPhi->Fill(j->eta(),j->phi());

			gctIsoEffPt->Fill(j->pt());
			gctIsoEffEt->Fill(j->et());
          gctIsoEffEta->Fill(j->eta());
          gctIsoEffPhi->Fill(j->phi());
          if(j->pt()>thresholdForEtaPhi_)
            gctIsoEffEtaPhi->Fill(j->eta(),j->phi());
        }
      }
    }
	}
	//	std::cout << " referance pt nentries at end of event " << refPt->getEntries() << std::endl;
}




math::PtEtaPhiMLorentzVector
RctValidation::rctLorentzVector(const L1GctEmCand& cand)//,const L1CaloGeometry*geom ,const  L1CaloEtScale* scale)
{
  double eta = caloGeom->etaBinCenter( cand.etaIndex(),true) ;
  double phi = caloGeom->emJetPhiBinCenter( cand.phiIndex() ) ;
  double et = emS->et( cand.rank() ) ;
  et+=1e-6;//correction !

  return   math::PtEtaPhiMLorentzVector(et,eta,phi,0); 
}



double 
RctValidation::tpgSum(const math::XYZVector& direction,const EcalTrigPrimDigiCollection& ecalDigis,double deltaR)//const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale,double deltaR)
{
  double tpgSum=0;
  for(EcalTrigPrimDigiCollection::const_iterator i=ecalDigis.begin();i!=ecalDigis.end();++i) {
    //calculate the TPG in eta phi space
    double eta = geo->eta(i->id().ieta());
    double phi = geo->phi(i->id().iphi());

    double et = eS->et(i->compressedEt(),abs(i->id().ieta()),i->id().ieta()/abs(i->id().ieta()));

    math::PtEtaPhiMLorentzVector v(et,eta,phi,0.0);

    if(ROOT::Math::VectorUtil::DeltaR(v.Vect(),direction)<deltaR) {
      tpgSum+=et;
    }
  }

//  printf("TPG SUM =%f\n",tpgSum);
  return tpgSum;
}

void
RctValidation::findRegionTowers(const L1GctEmCand& cand,const EcalTrigPrimDigiCollection& ecalDigis, EcalTrigPrimDigiCollection* regionTPs)//,const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale)
{
  //	EcalTrigPrimDigiCollection regionTPs;
  //	std::cout << "finding regions initialization" <<std::endl;
	L1CaloRegionDetId caloDetId = cand.regionId();

	
	unsigned crate  = caloDetId.rctCrate();
	unsigned card	= caloDetId.rctCard();
	unsigned region = caloDetId.rctRegion();
//std::cout << "finding objects in highest RCT crate " << crate << " card " << card << " region " << region ;
	unsigned short iTow0 = 0;
	unsigned short iTow16 = 15;
	if(region == 1) {
		iTow0 = 16;
		iTow16 = 31;
	}
	short rctEtaMin = rctGeo->calcIEta(crate,card, iTow0);
	short rctEtaMax = rctGeo->calcIEta(crate,card, iTow16);
	
	
	unsigned short rctPhiMin = rctGeo->calcIPhi(crate,card, iTow0);
	unsigned short rctPhiMax = rctGeo->calcIPhi(crate,card, iTow16);
		
		if(rctPhiMax < rctPhiMin) {	// switch the two
			unsigned short tempPhi = rctPhiMax;
			rctPhiMax = rctPhiMin;
			rctPhiMin = tempPhi;
		}
		if(rctEtaMax < rctEtaMin) {	// switch the two
			short tempEta = rctEtaMax;
			rctEtaMax = rctEtaMin;
			rctEtaMin = tempEta;
		}

//	std::cout << " ieta range " << rctEtaMin  <<" - " << rctEtaMax << " iphi range " << rctPhiMin << " - " << rctPhiMax << std::endl;

		for(EcalTrigPrimDigiCollection::const_iterator i=ecalDigis.begin();i!=ecalDigis.end() ;++i) {	

			short ieta = (short) i->id().ieta(); 
			 // Note absIeta counts from 1-28 (not 0-27)

			if(ieta >= rctEtaMin && ieta <= rctEtaMax) {
				unsigned short cal_iphi = (unsigned short) i->id().iphi(); 
			     unsigned short iphi = (72 + 18 - cal_iphi) % 72; // transform TOWERS (not regions) into local rct (intuitive) phi bins
			     if(iphi >= rctPhiMin && iphi <= rctPhiMax){
			       //			       std::cout << "finding regions initialization loop" <<std::endl <<std::flush;
					regionTPs->push_back(*i);
//										std::cout << "finding regions pushed back loop region size " << regionTPs->size() <<std::endl <<std::flush;
//				 std::cout << "tt inside rct region " << *i << std::endl;
			     }
			}
			if( regionTPs->size() >= 16)
			  break;
		}
		//		std::cout << " at end of finding regions " << std::cout <<std::flush;
			
	
	return ;

}

std::vector<double>
RctValidation::find3x3Towers(int etaCentral, int phiCentral ,const EcalTrigPrimDigiCollection& ecalTPGs,const HcalTrigPrimDigiCollection& hcalTPGs)//, std::vector<double> tpgs3x3)//,const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale)
{
  std::vector<double> tpgs3x3(27);
  for(int k =0; k< 27; ++k)
    tpgs3x3[k] = 0;
  //  std::cout << " in 3x3 finding eta " << etaCentral << " phi " << phiCentral <<std::endl;
  for(int i = 0; i <= 2 ;++i){ //eta iterator
    int ieta = etaCentral + i -1;
    if(abs(ieta) > 28) // out of eta range
      continue;
    if(ieta == 0)
      ieta = -1*etaCentral;
    for(int j = 0; j<= 2; ++j){ //phi iterator


      int iphi = (phiCentral +j+72 -1) %72;
      if(iphi == 0) iphi = 72;
      //        std::cout << " in tower loop eta " << ieta << " phi " << iphi <<std::endl;
      HcalTrigTowerDetId ttDetId(ieta,iphi);
      HcalTrigPrimDigiCollection::const_iterator hcalTT = hcalTPGs.find(ttDetId);
      if(hcalTT->id() == ttDetId) // if TP is present in collection, hcalTT is the end piece
	tpgs3x3[i*3+j+9] = hS->et(hcalTT->SOI_compressedEt(),abs(ieta),ieta/abs(ieta));//hcalTT->SOI_compressedEt();
	//highestEtTTHCAL = 
      EcalSubdetector subdet = ( abs(ieta) <= 17  ) ? EcalBarrel : EcalEndcap ;
      EcalTrigTowerDetId ttEDetId(ieta/abs(ieta), subdet, abs(ieta), iphi);
      //      EcalTrigTowerDetId ttEDetId(ieta,iphi);
      EcalTrigPrimDigiCollection::const_iterator ecalTT = ecalTPGs.find(ttEDetId);

      if(ecalTT->id() == ttEDetId){ // if TP is present in collection, hcalTT is the end piece
	tpgs3x3[i*3+j] =eS->et(ecalTT->compressedEt(),abs(ieta),ieta/abs(ieta));// ecalTT->compressedEt();
	if(ecalTT->fineGrain())
	  tpgs3x3[i*3+j+18] = 1;
      }
	
	//      else //
	//	highestEtTTHCAL = 0;
    }
  }


  //    std::cout << " after finding 3x3 towers size " << tpgs3x3.size() <<std::endl;
  return tpgs3x3;
}


//--------------------------------------------------------
void RctValidation::endLuminosityBlock(const LuminosityBlock& lumiSeg, 
				    const EventSetup& context) {
}
//--------------------------------------------------------
void RctValidation::endRun(const Run& r, const EventSetup& context){
	
  //		std::cout << " referance pt nentries at end of  run " << refPt->getEntries() << std::endl;
}
//--------------------------------------------------------
void RctValidation::endJob(){
  if (  outfile_.size() != 0  &&store ) store->save(outfile_);
  return;
}



DEFINE_FWK_MODULE(RctValidation);





