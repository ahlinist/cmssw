#include "Validation/L1Trigger/interface/RctValidation.h"
#include "Math/GenVector/VectorUtil.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using namespace std;
using namespace reco;
using namespace edm;

//
// constructors and destructor
//
RctValidation::RctValidation( const edm::ParameterSet& iConfig ) :
  ecalTPGs_(iConfig.getParameter<edm::InputTag>("ecalTPGs")),
  rctEGammas_(iConfig.getParameter<edm::InputTag >("rctEGamma")),
  gctEGammas_(iConfig.getParameter<std::vector<edm::InputTag> >("gctEGamma")),
  refEGammas_(iConfig.getParameter<edm::InputTag>("genEGamma")),
  directory_(iConfig.getParameter<std::string>("directory")),  
  maxEt_(iConfig.getUntrackedParameter<double>("maxEt",100)),
  binsEt_(iConfig.getUntrackedParameter<int>("binsEt",20)),
  binsEta_(iConfig.getUntrackedParameter<int>("binsEta",30)),
  binsPhi_(iConfig.getUntrackedParameter<int>("binsPhi",32)),
  matchDR_(iConfig.getUntrackedParameter<double>("matchDeltaR",0.38)),
  egammaThreshold_(iConfig.getUntrackedParameter<double>("gammaThreshold",5.)),
  tpgSumWindow_(iConfig.getUntrackedParameter<double>("tpgSumWindow",0.4)),
  thresholdForEtaPhi_(iConfig.getUntrackedParameter<double>("thresholdForEtaPhi",4.))
{
  geo = new TriggerTowerGeometry();

  //Get General Monitoring Parameters
  DQMStore *store = &*edm::Service<DQMStore>();
  if(store)
    {
      store->setCurrentFolder(directory_);
      
      TPG_Resolution    = store->book1D("TPG_Resolution"," e/#gamma ET-ref e/#gamma E_[T}/Ref e/#gamma E_{T} ",100,-1.,1.);
      RCT_Resolution    = store->book1D("RCT_Resolution"," e/#gamma ET-ref e/#gamma E_[T}/Ref e/#gamma E_{T} ",100,-1.,1.);

      refPt     = store->book1D("refPt" ,"ref e/#gamma E_{T}",binsEt_,0.,maxEt_);
      refEta    = store->book1D("refEta","ref e/#gamma E_{T}",binsEta_,-2.5,2.5);
      refPhi    = store->book1D("refPhi","ref e/#gamma E_{T}",binsPhi_,0,3.2);
      refEtaPhi = store->book2D("refEtaPhi","ref e/#gamma #eta #phi",binsEta_,-2.5,2.5,binsPhi_,0,3.2);

      refPt->getTH1F()->Sumw2();
      refEta->getTH1F()->Sumw2();
      refPhi->getTH1F()->Sumw2();
      //      refEtaPhi->getTH1F()->Sumw2();

      tpgEffPt     = store->book1D("tpgEffPt" ,"tpg e/#gamma E_{T}",binsEt_,0.,maxEt_);
      tpgEffEta    = store->book1D("tpgEffEta","tpg e/#gamma E_{T}",binsEta_,-2.5,2.5);
      tpgEffPhi    = store->book1D("tpgEffPhi","tpg e/#gamma E_{T}",binsPhi_,0.,3.2);
      tpgEffEtaPhi = store->book2D("tpgEffEtaPhi","tpg e/#gamma #eta #phi",binsEta_,-2.5,2.5,binsPhi_,0,3.2);

      tpgEffPt->getTH1F()->Sumw2();
      tpgEffEta->getTH1F()->Sumw2();
      tpgEffPhi->getTH1F()->Sumw2();
      //      tpgEffEtaPhi->getTH1F()->Sumw2();


      rctEffPt     = store->book1D("rctEffPt" ,"rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      rctEffEta    = store->book1D("rctEffEta","rct e/#gamma E_{T}",binsEta_,-2.5,2.5);
      rctEffPhi    = store->book1D("rctEffPhi","rct e/#gamma E_{T}",binsPhi_,0.,3.2);
      rctEffEtaPhi = store->book2D("rctEffEtaPhi","rct e/#gamma #eta #phi",binsEta_,-2.5,2.5,binsPhi_,0,3.2);

      rctEffPt->getTH1F()->Sumw2();
      rctEffEta->getTH1F()->Sumw2();
      rctEffPhi->getTH1F()->Sumw2();
      //      rctEffEtaPhi->getTH1F()->Sumw2();

      rctIsoEffPt     = store->book1D("rctIsoEffPt" ,"rctIso e/#gamma E_{T}",binsEt_,0.,maxEt_);
      rctIsoEffEta    = store->book1D("rctIsoEffEta","rctIso e/#gamma E_{T}",binsEta_,-2.5,2.5);
      rctIsoEffPhi    = store->book1D("rctIsoEffPhi","rctIso e/#gamma E_{T}",binsPhi_,0.,3.2);
      rctIsoEffEtaPhi = store->book2D("rctIsoEffEtaPhi","rctIso e/#gamma #eta #phi",binsEta_,-2.5,2.5,binsPhi_,0,3.2);

      rctIsoEffPt->getTH1F()->Sumw2();
      rctIsoEffEta->getTH1F()->Sumw2();
      rctIsoEffPhi->getTH1F()->Sumw2();
      //      rctIsoEffEtaPhi->getTH1F()->Sumw2();


      gctEffPt     = store->book1D("gctEffPt" ,"gct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      gctEffEta    = store->book1D("gctEffEta","gct e/#gamma E_{T}",binsEta_,-2.5,2.5);
      gctEffPhi    = store->book1D("gctEffPhi","gct e/#gamma E_{T}",binsPhi_,0.,3.2);
      gctEffEtaPhi = store->book2D("gctEffEtaPhi","gct e/#gamma #eta #phi",binsEta_,-2.5,2.5,binsPhi_,0,3.2);

      gctEffPt->getTH1F()->Sumw2();
      gctEffEta->getTH1F()->Sumw2();
      gctEffPhi->getTH1F()->Sumw2();
      //      gctEffEtaPhi->getTH1F()->Sumw2();

      gctIsoEffPt     = store->book1D("gctIsoEffPt" ,"gctIso e/#gamma E_{T}",binsEt_,0.,maxEt_);
      gctIsoEffEta    = store->book1D("gctIsoEffEta","gctIso e/#gamma E_{T}",binsEta_,-2.5,2.5);
      gctIsoEffPhi    = store->book1D("gctIsoEffPhi","gctIso e/#gamma E_{T}",binsPhi_,0.,3.2);
      gctIsoEffEtaPhi = store->book2D("gctIsoEffEtaPhi","gctIso e/#gamma #eta #phi",binsEta_,-2.5,2.5,binsPhi_,0,3.2);

      gctIsoEffPt->getTH1F()->Sumw2();
      gctIsoEffEta->getTH1F()->Sumw2();
      gctIsoEffPhi->getTH1F()->Sumw2();

      rctEtaCorr = store->bookProfile("rctEtaCorr", "RCT #eta corrections", binsEta_,-3,3, 50, 0, 2); 
      rctEtaCorrIEta = store->bookProfile("rctEtaCorrIEta", "RCT #eta corrections", 65, -32, 32, 50, 0, 2);
      rctEtaCorrAbsIEta = store->bookProfile("rctEtaCorrAbsIEta", "RCT |#eta| corrections", 33, 0, 32, 50, 0, 2);
    }
}

RctValidation::~RctValidation()
{
   
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//--------------------------------------------------------


//--------------------------------------------------------
void RctValidation::beginRun(const edm::Run& r, const EventSetup& context) {

}

//--------------------------------------------------------
void RctValidation::beginLuminosityBlock(const LuminosityBlock& lumiSeg, 
				      const EventSetup& context) {
  
}

// ----------------------------------------------------------
void 
RctValidation::analyze(const Event& iEvent, const EventSetup& iSetup )
{  
  //Get The Geometry and the scale
  ESHandle< L1CaloGeometry > caloGeomESH ;
  iSetup.get< L1CaloGeometryRecord >().get( caloGeomESH ) ;
  const L1CaloGeometry* caloGeom = caloGeomESH.product();


  // get energy scale to convert input from ECAL
  edm::ESHandle<L1CaloEcalScale> ecalScale;
  iSetup.get<L1CaloEcalScaleRcd>().get(ecalScale);
  const L1CaloEcalScale* eS = ecalScale.product();

  ESHandle< L1CaloEtScale > emScale ;
  iSetup.get< L1EmEtScaleRcd >().get( emScale ) ;
  const L1CaloEtScale* emS = emScale.product();
  
  //Get Reference Objects
  edm::Handle<edm::View<reco::Candidate> > genEGamma;
  bool gotRef = iEvent.getByLabel(refEGammas_,genEGamma) && !genEGamma.failedToGet();

  //get ECAL TPGs
  edm::Handle<EcalTrigPrimDigiCollection> ecalTPGs;
  bool gotECALTPs = iEvent.getByLabel(ecalTPGs_,ecalTPGs);
  if(!gotECALTPs)
    printf("NO ECAL TPs found\n");


  //Get Egammas from RCT
  L1GctEmCandCollection rctEGammas;
  edm::Handle<L1CaloEmCollection> egamma;
  if(iEvent.getByLabel(rctEGammas_,egamma) && !egamma.failedToGet())
    for(L1CaloEmCollection::const_iterator i=egamma->begin();i!=egamma->end();++i)
      {
	L1CaloEmCand rctEGamma = *i;
	rctEGammas.push_back(L1GctEmCand(rctEGamma));
      }

    

  //Get Egammas from GCT
  L1GctEmCandCollection gctEGammas;
  edm::Handle<L1GctEmCandCollection> gctEgamma;
  if(iEvent.getByLabel(gctEGammas_[0],gctEgamma))
    gctEGammas = *gctEgamma;


  L1GctEmCandCollection gctIsoEGammas;
  edm::Handle<L1GctEmCandCollection> gctIsoEgamma;
  if(iEvent.getByLabel(gctEGammas_[1],gctIsoEgamma))
    gctIsoEGammas = *gctIsoEgamma;


 
  //OK Now Efficiency calculations
  if(gotRef)
    for(unsigned int j=0;j<genEGamma->size();++j) 
      {

	refPt->Fill(genEGamma->at(j).pt());
	refEta->Fill(genEGamma->at(j).eta());
	refPhi->Fill(genEGamma->at(j).phi());

	if(genEGamma->at(j).pt()>thresholdForEtaPhi_)
	  refEtaPhi->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());

	//TPG Efficiency
	
	//get TPG Sum
	double tpgS = tpgSum(genEGamma->at(j).p4().Vect(),*ecalTPGs,caloGeom,eS,tpgSumWindow_);
	//If there is significant ecal energy over threshold passes:
	if(tpgS> egammaThreshold_)
	  {
	    tpgEffPt->Fill(genEGamma->at(j).pt());
	    tpgEffEta->Fill(genEGamma->at(j).eta());
	    tpgEffPhi->Fill(genEGamma->at(j).phi());
	    if(genEGamma->at(j).pt()>thresholdForEtaPhi_)
	      tpgEffEtaPhi->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());
	  }

	if(tpgS<32.)
	  TPG_Resolution->Fill((tpgS-genEGamma->at(j).pt())/genEGamma->at(j).pt());

	//get RCT Efficiency-Find the highest object near the ref
	L1GctEmCandCollection rctNearReference;

	if(rctEGammas.size()>0)
	  for(L1GctEmCandCollection::const_iterator i=rctEGammas.begin();i!=rctEGammas.end();++i)
	  if(i->rank()>0)
	    {
	      //create lorenzt vector
	      math::PtEtaPhiMLorentzVector rctVec = rctLorentzVector(*i,caloGeom,emS);
	      double deltaR = ROOT::Math::VectorUtil::DeltaR(rctVec,genEGamma->at(j).p4());
	      //ok now match and do it over 10 GeV
	      if(deltaR<matchDR_&&rctVec.pt()>=egammaThreshold_)
		{
		  rctNearReference.push_back(*i);
		}
	    }

	if(rctNearReference.size()>0) {
	  //Ok find the highest matched rct cand
	  RCTEmSorter sorter;
	  std::sort(rctNearReference.begin(),rctNearReference.end(),sorter);
	  L1GctEmCand highestRCT = rctNearReference.at(0);
	  //get its LV
	  math::PtEtaPhiMLorentzVector highestVec = rctLorentzVector(highestRCT,caloGeom,emS);
	  //ask for threshold
	  if(highestVec.pt()>egammaThreshold_) {
	    rctEffPt->Fill(genEGamma->at(j).pt());
	    rctEffEta->Fill(genEGamma->at(j).eta());
	    rctEffPhi->Fill(genEGamma->at(j).phi());
	    if(genEGamma->at(j).pt()>thresholdForEtaPhi_)
	      rctEffEtaPhi->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());

	    if(highestRCT.rank()<127)  // only for non-saturating
	      RCT_Resolution->Fill(( highestVec.pt()-genEGamma->at(j).pt())/genEGamma->at(j).pt() );

	    if(highestRCT.isolated())
	      {
		rctIsoEffPt->Fill(genEGamma->at(j).pt());
		rctIsoEffEta->Fill(genEGamma->at(j).eta());
		rctIsoEffPhi->Fill(genEGamma->at(j).phi());
		if(genEGamma->at(j).pt()>thresholdForEtaPhi_)
		  rctIsoEffEtaPhi->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());
                  
                  if( highestRCT.rank() < 127 )  // only for non-saturating
                  {
                    rctEtaCorr->Fill( highestVec.eta(), genEGamma->at(j).pt()/highestVec.pt() );
                    rctEtaCorrIEta->Fill( geo->iEta(highestVec.eta()), genEGamma->at(j).pt()/highestVec.pt() );
                    rctEtaCorrAbsIEta->Fill( fabs(geo->iEta(highestVec.eta())), genEGamma->at(j).pt()/highestVec.pt() );
                  }
	      }


	  }
	}




	//Now THE GCT Efficiency

	L1GctEmCandCollection gctNearReference;


	if(gctEGammas.size()>0)
	  for(L1GctEmCandCollection::const_iterator i=gctEGammas.begin();i!=gctEGammas.end();++i)
	  if(i->rank()>0)
	    {
	      //create lorenzt vector
	      math::PtEtaPhiMLorentzVector gctVec = rctLorentzVector(*i,caloGeom,emS);
	      double deltaR = ROOT::Math::VectorUtil::DeltaR(gctVec,genEGamma->at(j).p4());
	      //ok now match and do it over 10 GeV
	      if(deltaR<matchDR_&&gctVec.pt()>=egammaThreshold_)
		{
		  gctNearReference.push_back(*i);
		}
	    }

	if(gctNearReference.size()>0) {
	  //Ok find the highest matched gct cand
	  RCTEmSorter gsorter;
	  std::sort(gctNearReference.begin(),gctNearReference.end(),gsorter);
	  L1GctEmCand highestGCT = gctNearReference.at(0);
	  //get its LV
	  math::PtEtaPhiMLorentzVector highestVec = rctLorentzVector(highestGCT,caloGeom,emS);
	  //ask for threshold
	  if(highestVec.pt()>egammaThreshold_) {
	    gctEffPt->Fill(genEGamma->at(j).pt());
	    gctEffEta->Fill(genEGamma->at(j).eta());
	    gctEffPhi->Fill(genEGamma->at(j).phi());
	    if(genEGamma->at(j).pt()>thresholdForEtaPhi_)
	      gctEffEtaPhi->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());

	  }
	}


	L1GctEmCandCollection gctIsoNearReference;

	if(gctIsoEGammas.size()>0)
	  for(L1GctEmCandCollection::const_iterator i=gctIsoEGammas.begin();i!=gctIsoEGammas.end();++i)
	  if(i->rank()>0)
	    {
	      //create lorenzt vector
	      math::PtEtaPhiMLorentzVector gctIsoVec = rctLorentzVector(*i,caloGeom,emS);
	      double deltaR = ROOT::Math::VectorUtil::DeltaR(gctIsoVec,genEGamma->at(j).p4());
	      //ok now match and do it over 10 GeV
	      if(deltaR<matchDR_&&gctIsoVec.pt()>=egammaThreshold_)
		{
		  gctIsoNearReference.push_back(*i);
		}
	    }

	if(gctIsoNearReference.size()>0) {
	  //Ok find the highest matched gctIso cand
	  RCTEmSorter ggsorter;
	  std::sort(gctIsoNearReference.begin(),gctIsoNearReference.end(),ggsorter);
	  L1GctEmCand highestGCTISO = gctIsoNearReference.at(0);
	  //get its LV
	  math::PtEtaPhiMLorentzVector highestVec = rctLorentzVector(highestGCTISO,caloGeom,emS);
	  //ask for threshold
	  if(highestVec.pt()>egammaThreshold_) {
	    gctEffPt->Fill(genEGamma->at(j).pt());
	    gctEffEta->Fill(genEGamma->at(j).eta());
	    gctEffPhi->Fill(genEGamma->at(j).phi());
	    if(genEGamma->at(j).pt()>thresholdForEtaPhi_)
	      gctEffEtaPhi->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());

	    gctIsoEffPt->Fill(genEGamma->at(j).pt());
	    gctIsoEffEta->Fill(genEGamma->at(j).eta());
	    gctIsoEffPhi->Fill(genEGamma->at(j).phi());
	    if(genEGamma->at(j).pt()>thresholdForEtaPhi_)
	      gctIsoEffEtaPhi->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());
	  }

	}
      

      }

 

}




math::PtEtaPhiMLorentzVector
RctValidation::rctLorentzVector(const L1GctEmCand& cand,const L1CaloGeometry*geom ,const  L1CaloEtScale* scale)
{
  double eta = geom->etaBinCenter( cand.etaIndex(),true) ;
  double phi = geom->emJetPhiBinCenter( cand.phiIndex() ) ;
  double et = scale->et( cand.rank() ) ;
  et+=1e-6;//correction !

  return   math::PtEtaPhiMLorentzVector(et,eta,phi,0); 
}



double 
RctValidation::tpgSum(const math::XYZVector& direction,const EcalTrigPrimDigiCollection& ecalDigis,const L1CaloGeometry* geom ,const  L1CaloEcalScale* scale,double deltaR)
{
  double tpgSum=0;
  for(EcalTrigPrimDigiCollection::const_iterator i=ecalDigis.begin();i!=ecalDigis.end();++i) {
    //calculate the TPG in eta phi space
    double eta = geo->eta(i->id().ieta());
    double phi = geo->phi(i->id().iphi());

    double et = scale->et(i->compressedEt(),abs(i->id().ieta()),i->id().ieta()/abs(i->id().ieta()));

    math::PtEtaPhiMLorentzVector v(et,eta,phi,0.0);

    if(ROOT::Math::VectorUtil::DeltaR(v.Vect(),direction)<deltaR) {
      tpgSum+=et;
    }
  }

  printf("TPG SUM =%f\n",tpgSum);
  return tpgSum;
}

 


//--------------------------------------------------------
void RctValidation::endLuminosityBlock(const LuminosityBlock& lumiSeg, 
				    const EventSetup& context) {
}
//--------------------------------------------------------
void RctValidation::endRun(const Run& r, const EventSetup& context){
}
//--------------------------------------------------------
void RctValidation::endJob(){
  return;
}



DEFINE_FWK_MODULE(RctValidation);


