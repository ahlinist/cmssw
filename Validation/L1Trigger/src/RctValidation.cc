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
  matchDR_(iConfig.getUntrackedParameter<double>("matchDeltaR",0.3))
{

  //Get General Monitoring Parameters
  DQMStore *store = &*edm::Service<DQMStore>();
  if(store)
    {
      store->setCurrentFolder(directory_);

      egamma_rank = store->book1D("rctEgammaRank","Rct e/#gamma rank",64,0,128);
      egamma_et = store->book1D("rctEgammaEt","Rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      egamma_eta = store->book1D("rctEgammaEta","Rct e/#gamma #eta",binsEta_,-3.,3.);
      egamma_phi = store->book1D("rctEgammaPhi","Rct e/#gamma #phi",binsPhi_,0.,3.2);
      egamma_deltaEt = store->book1D("rctEgammaDeltaEt","Rct e/#gamma ET-MC e/#gamma E_[T}/MC e/#gamma E_{T} ",100,-1.,1.);
      
      egamma_et_eff_num = store->book1D("rctEgammaEtEffNum","Rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      egamma_et_eff_num->getTH1F()->Sumw2();

      egamma_et_eff_denom = store->book1D("rctEgammaEtEffDenom","Rct e/#gamma E_{T}",binsEt_,0.,maxEt_);
      egamma_et_eff_denom->getTH1F()->Sumw2();
      
      egamma_eta_eff_num = store->book1D("rctEgammaEtaEffNum","Rct e/#gamma #eta",binsEta_,-3.,3.);
      egamma_eta_eff_num->getTH1F()->Sumw2();

      egamma_eta_eff_denom = store->book1D("rctEgammaEtaEffDenom","Rct e/#gamma #eta",binsEta_,-3.,3.);
      egamma_eta_eff_denom->getTH1F()->Sumw2();

      isoegamma_et_eff_num = store->book1D("rctIsoEgammaEtEffNum","Rct isolated e/#gamma E_{T}",binsEt_,0.,maxEt_);
      isoegamma_et_eff_num->getTH1F()->Sumw2();

      isoegamma_eta_eff_num = store->book1D("rctIsoEgammaEtaEffNum","Rct isolated e/#gamma #eta",binsEta_,-3.,3.);
      isoegamma_eta_eff_num->getTH1F()->Sumw2();
      
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

  //Fill Denominator Histograms
  if(isMC)
    for(unsigned int i=0;i<genEGamma->size();++i)
      {
	egamma_et_eff_denom->Fill(genEGamma->at(i).pt());
	egamma_eta_eff_denom->Fill(genEGamma->at(i).eta());
	egamma_eta_eff_denom->Fill(genEGamma->at(i).phi());
      }



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
	math::PtEtaPhiMLorentzVector v(et,eta,phi,0.0);
	
	egamma_rank->Fill(i->rank());
	egamma_et->Fill(et);
	egamma_eta->Fill(eta);
	egamma_phi->Fill(phi);

	//Now match to the nearest!For reducing double counting do it for only e/gammas>15 GeV
	double deltaR = 100.0;
	
	math::XYZTLorentzVector nearest;

	if(isMC&&et>15) {
	  for(unsigned int j=0;j<genEGamma->size();++j)
	    {
	      double dr =ROOT::Math::VectorUtil::DeltaR(v,genEGamma->at(j).p4());
	      if(dr<deltaR) {
		deltaR = dr;
		nearest = genEGamma->at(j).p4();
	      }
	    }
	  //ok now match and do it over 10 GeV
	  if(deltaR<matchDR_)
	    {
	      egamma_et_eff_num->Fill(nearest.pt());
	      egamma_eta_eff_num->Fill(nearest.eta());
	      egamma_eta_eff_num->Fill(nearest.phi());

	      if(i->isolated())
		{
		  isoegamma_et_eff_num->Fill(nearest.pt());
		  isoegamma_eta_eff_num->Fill(nearest.eta());
		  isoegamma_eta_eff_num->Fill(nearest.phi());
		}

	      //scale -Do it outside the saturation and for egammas over 10 GeV 
	      if(i->rank()<127)
		{
		  egamma_deltaEt->Fill((v.pt()-nearest.pt())/nearest.pt());
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


