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
  egamma_(iConfig.getParameter<edm::InputTag>("egamma")),
  genEGamma_(iConfig.getParameter<edm::InputTag>("genEGamma")),
  directory_(iConfig.getParameter<std::string>("directory")),  
  maxEt_(iConfig.getUntrackedParameter<double>("maxEt",100)),
  binsEt_(iConfig.getUntrackedParameter<int>("binsEt",20)),
  binsEta_(iConfig.getUntrackedParameter<int>("binsEta",30)),
  binsPhi_(iConfig.getUntrackedParameter<int>("binsPhi",32)),
  matchDR_(iConfig.getUntrackedParameter<double>("matchDeltaR",0.3)),
  egammaThreshold_(iConfig.getUntrackedParameter<double>("gammaThreshold",5.))
{

  //Get General Monitoring Parameters
  DQMStore *store = &*edm::Service<DQMStore>();
  if(store)
    {
      store->setCurrentFolder(directory_);
      egamma_rank         = store->book1D("rctEgammaRank","Rct e/#gamma rank",64,0,128);
      egamma_et           = store->book1D("rctEgammaEt","Rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      egamma_eta          = store->book1D("rctEgammaEta","Rct e/#gamma #eta",binsEta_,-3.,3.);
      egamma_phi          = store->book1D("rctEgammaPhi","Rct e/#gamma #phi",binsPhi_,0.,3.2);
      egamma_deltaEt      = store->book1D("rctEgammaDeltaEt","Rct e/#gamma ET-MC e/#gamma E_[T}/MC e/#gamma E_{T} ",100,-1.,1.);
      egamma_et_eff_num   = store->book1D("rctEgammaEtEffNum","Rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      egamma_et_eff_num->getTH1F()->Sumw2();
      egamma_et_eff_denom = store->book1D("rctEgammaEtEffDenom","Rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      egamma_et_eff_denom->getTH1F()->Sumw2();

      egamma_et_eff_num2D = store->book2D("rctEgammaEtEffNum2D","Rct e/#gamma E_{T} 2D",binsEta_,-2.5,2.5,binsPhi_,0,3.2);

      isoegamma_et_eff_num2D = store->book2D("rctIsoEgammaEtEffNum2D","Rct e/#gamma E_{T} 2D",binsEta_,-2.5,2.5,binsPhi_,0,3.2);
      egamma_et_eff_num2D = store->book2D("rctEgammaEtEffNum2D","Rct e/#gamma E_{T} 2D",binsEta_,-2.5,2.5,binsPhi_,0,3.2);
      egamma_et_eff_denom2D = store->book2D("rctEgammaEtEffDenom2D","Rct e/#gamma E_{T} 2D",binsEta_,-2.5,2.5,binsPhi_,0,3.2);
      
      egamma_eta_eff_num = store->book1D("rctEgammaEtaEffNum","Rct e/#gamma #eta",binsEta_,-3.,3.);
      egamma_eta_eff_num->getTH1F()->Sumw2();

      egamma_eta_eff_denom = store->book1D("rctEgammaEtaEffDenom","Rct e/#gamma #eta",binsEta_,-3.,3.);
      egamma_eta_eff_denom->getTH1F()->Sumw2();

      egamma_phi_eff_num = store->book1D("rctEgammaPhiEffNum","Rct e/#gamma #phi",binsPhi_,0.,3.2);
      egamma_phi_eff_num->getTH1F()->Sumw2();

      egamma_phi_eff_denom = store->book1D("rctEgammaPhiEffDenom","Rct e/#gamma #phi",binsPhi_,0.,3.2);
      egamma_phi_eff_denom->getTH1F()->Sumw2();

      isoegamma_et_eff_num = store->book1D("rctIsoEgammaEtEffNum","Rct isolated e/#gamma E_{T}",binsEt_,0.,maxEt_);
      isoegamma_et_eff_num->getTH1F()->Sumw2();

      isoegamma_eta_eff_num = store->book1D("rctIsoEgammaEtaEffNum","Rct isolated e/#gamma #eta",binsEta_,-3.,3.);
      isoegamma_eta_eff_num->getTH1F()->Sumw2();

      isoegamma_phi_eff_num = store->book1D("rctIsoEgammaPhiEffNum","Rct isolated e/#gamma #phi",binsPhi_,0.,3.2);
      isoegamma_phi_eff_num->getTH1F()->Sumw2();
      
      region_rank=store->book1D("rctRegionRank","Rct region ET",64,0,128);
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
  const L1CaloGeometry* caloGeom = &( *caloGeomESH ) ;

  ESHandle< L1CaloEtScale > emScale ;
  iSetup.get< L1EmEtScaleRcd >().get( emScale ) ;
  
  //Get MC electrons
  edm::Handle<edm::View<reco::Candidate> > genEGamma;
  bool isMC = iEvent.getByLabel(genEGamma_,genEGamma) && !genEGamma.failedToGet();

  //Get EGammas
  L1GctEmCandCollection gctEGammas;

  edm::Handle<L1CaloEmCollection> egamma;
  if(iEvent.getByLabel(egamma_,egamma) && !egamma.failedToGet())
    for(L1CaloEmCollection::const_iterator i=egamma->begin();i!=egamma->end();++i)
      {
	L1CaloEmCand rctEGamma = *i;
	gctEGammas.push_back(L1GctEmCand(rctEGamma));
      }

 
  if(gctEGammas.size()>0)
    for(L1GctEmCandCollection::const_iterator i=gctEGammas.begin();i!=gctEGammas.end();++i)
      if(i->rank()>0)
      {
	//Convert Rct to GCT candidate to be able to access the geometry
	double eta = caloGeom->etaBinCenter( i->etaIndex(),true) ;
	double phi = caloGeom->emJetPhiBinCenter( i->phiIndex() ) ;
	double et = emScale->et( i->rank() ) ;
	et+=1e-6;//correction !

	egamma_rank->Fill(i->rank());
	egamma_et->Fill(et);
	egamma_eta->Fill(eta);
	egamma_phi->Fill(phi);
      }


  //OK Now Efficiency calculations
  if(isMC)
    for(unsigned int j=0;j<genEGamma->size();++j) 
      {
	egamma_et_eff_denom->Fill(genEGamma->at(j).pt());
	egamma_eta_eff_denom->Fill(genEGamma->at(j).eta());
	egamma_phi_eff_denom->Fill(genEGamma->at(j).phi());
	egamma_et_eff_denom2D->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());
	
	if(gctEGammas.size()>0)
	  for(L1GctEmCandCollection::const_iterator i=gctEGammas.begin();i!=gctEGammas.end();++i)
	  if(i->rank()>0)
	    {
	      //Convert Rct to GCT candidate to be able to access the geometry
	      double eta = caloGeom->etaBinCenter( i->etaIndex(),true) ;
	      double phi = caloGeom->emJetPhiBinCenter( i->phiIndex() ) ;
	      double et = emScale->et( i->rank() ) ;
	      et+=1e-6;//correction !
	      math::PtEtaPhiMLorentzVector v(et,eta,phi,0.0);
	      double deltaR = ROOT::Math::VectorUtil::DeltaR(v,genEGamma->at(j).p4());
	      //ok now match and do it over 10 GeV
	      if(deltaR<matchDR_&&et>egammaThreshold_)
		{
		  egamma_et_eff_num->Fill(genEGamma->at(j).pt());
		  egamma_eta_eff_num->Fill(genEGamma->at(j).eta());
		  egamma_phi_eff_num->Fill(genEGamma->at(j).phi());
		  egamma_et_eff_num2D->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());
		  
		  if(i->isolated())
		    {
		      isoegamma_et_eff_num->Fill(genEGamma->at(j).pt());
		      isoegamma_eta_eff_num->Fill(genEGamma->at(j).eta());
		      isoegamma_phi_eff_num->Fill(genEGamma->at(j).phi());
		      isoegamma_et_eff_num2D->Fill(genEGamma->at(j).eta(),genEGamma->at(j).phi());
		    }
      
		  //scale -Do it outside the saturation and for egammas over 10 GeV 
		  if(i->rank()<127)
		    {
		      egamma_deltaEt->Fill((v.pt()-genEGamma->at(j).pt())/genEGamma->at(j).pt());
		    }
		}
	    }
    }

  //Now regions
  edm::Handle<L1CaloRegionCollection> regions;
  if(iEvent.getByLabel(egamma_,regions))
    for(unsigned int i=0;i<regions->size();++i)
      if(regions->at(i).et()>0)
	region_rank->Fill(regions->at(i).et());
     
  
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


