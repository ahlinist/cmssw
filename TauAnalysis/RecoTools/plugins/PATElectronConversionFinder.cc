//
// Original Author:  Konstantinos A. Petridis
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATLeptonPATTauOSSSChargeSelector.cc,v 1.1 2009/01/16 13:15:41 kap01 Exp $
//
//

#include "TauAnalysis/RecoTools/plugins/PATElectronConversionFinder.h"
          
PATElectronConversionFinderImp::PATElectronConversionFinderImp(const edm::ParameterSet& iConfig)
{
  elecs_ = iConfig.getParameter<edm::InputTag>("ElectronSource");
  tracks_= iConfig.getParameter<edm::InputTag>("TrackSource");
  cotTheta_ = iConfig.getParameter<double>("CotThetaCut");
  dca_ = iConfig.getParameter<double>("DcaElecTrack");
  dRin_ = iConfig.getParameter<double>("DRElecTrack");
  doPXBcut_= iConfig.getParameter<bool>("DoPXBcut");
  nMax_ = iConfig.getParameter<double>("Nmax");
  doHistos_ = iConfig.getParameter<bool>("DoHistos");
  if(doHistos_){
    edm::Service<TFileService> fs;             
    hDcotTheta=fs->make<TH2F>("DcotTheta","DcotTheta",40,-0.08,0.08,50,0,0.5);
    hDca=fs->make<TH2F>("Dca","Dca",50,-2.5,2.5,50,0,0.5);
    hnTracksPass=fs->make<TH1F>("nTracksPass","nTracksPass",10,0,10);
    hPXFDisk=fs->make<TH1F>("PXFDisk","PXFDisk",10,0,10);
    hPXBLayer=fs->make<TH1F>("PXBLayer","PXBLayer",10,0,10);
  }
}


PATElectronConversionFinderImp::~PATElectronConversionFinderImp() {}

void PATElectronConversionFinderImp::select(const edm::Handle<collection>& elecs,
					 edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  selected_.clear();
  
  using namespace edm;
  using namespace reco;

  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(tracks_,tracks);
  const reco::TrackCollection* trackCollection = tracks.product();
  
  edm::ESHandle<MagneticField> magFieldHandle;
  iSetup.get<IdealMagneticFieldRecord>().get( magFieldHandle );
  const MagneticField* magField = magFieldHandle.product();
  
  for( std::vector<pat::Electron>::const_iterator elec=elecs->begin();elec!=elecs->end(); ++elec)
    {
      int nTracksPass=0;
      
      for(size_t t=0;t<trackCollection->size();t++)
	{
	  double dRin=ROOT::Math::VectorUtil::DeltaR(trackCollection->at(t).innerPosition(),
						     (*elec).gsfTrack()->innerPosition());
	  //double dZin= trackCollection->at(t).innerPosition().z()-(*elec).gsfTrack()->innerPosition().z();
	  double dca=GetDcaBetweenTracks(iEvent,iSetup,magField,trackCollection->at(t),*((*elec).gsfTrack()));//NEED TO CHECK THIS
	  double cotTheta=1./tan((*elec).gsfTrack()->innerMomentum().Theta())-1./tan(trackCollection->at(t).innerMomentum().Theta());
	  
	  TLorentzVector trackP,elecP;
	  trackP.SetPtEtaPhiM(trackCollection->at(t).pt(),
			      trackCollection->at(t).eta(),
			      trackCollection->at(t).phi(),
			      0.);
	  elecP.SetPtEtaPhiM((*elec).pt(),(*elec).eta(),
			     (*elec).phi(),0.);
	  double mTracksElec = (trackP+elecP).M();// Could also cut on Invariant Mass but its equivalent to DCotTheta
	  if(doHistos_){
	    hDcotTheta->Fill(cotTheta,dRin,eventWeight_);
	    hDca->Fill(dca,dRin,eventWeight_);
	  }
	  
	  if(fabs(cotTheta)<cotTheta_&&dRin<dRin_)//Only cut on DCotTheta for now
	    nTracksPass++;
	}
      if(doHistos_)hnTracksPass->Fill(nTracksPass);
      // Check for pixel inner layer hit
      int innerDetIdRaw = (*elec).gsfTrack()->innerDetId();
      DetId innerDetId(innerDetIdRaw);
      if(innerDetId.det() == DetId::Tracker){
	if(innerDetId.subdetId()==1){PXBDetId pxbDetId(innerDetId); 
	  if(doHistos_)hPXBLayer->Fill(pxbDetId.layer(),eventWeight_);}
	if(innerDetId.subdetId()==2){PXFDetId pxfDetId(innerDetId); 
	  if(doHistos_)hPXFDisk->Fill(pxfDetId.disk(),eventWeight_);}
      }
      
      if(nTracksPass<nMax_){//nMax_ should be <2 (<=1) since gsf electron track might appear in general track list
	if(doPXBcut_){
	  if(innerDetId.det() == DetId::Tracker){
	    if(innerDetId.subdetId()==1){
	      PXBDetId pxbDetId(innerDetId);
	      if(pxbDetId.layer()==1)selected_.push_back(&(*elec));
	    }
	    if(innerDetId.subdetId()==2){
	      PXFDetId pxfDetId(innerDetId);
	      if(pxfDetId.disk()==1)selected_.push_back(&(*elec));
	    }
	  }
	}
	if(!doPXBcut_)
	  selected_.push_back(&(*elec));
      }
    }
}

double PATElectronConversionFinderImp::GetDcaBetweenTracks(const edm::Event& evt, 
							const edm::EventSetup& setup,
							const MagneticField* magField,
							const reco::Track& trk,
							const reco::GsfTrack& gsf)
{
 
  double gx=gsf.innerPosition().x(),gy=gsf.innerPosition().y(),gz=gsf.innerPosition().z();
  double rad_gsf = gsf.innerMomentum().Rho()/(0.3*magField->inTesla(GlobalPoint(gx,gy,gz)).z())*100.;
  
  double tx=gsf.innerPosition().x(),ty=trk.innerPosition().y(),tz=trk.innerPosition().z();
  double rad_trk = trk.innerMomentum().Rho()/(0.3*magField->inTesla(GlobalPoint(tx,ty,tz)).z())*100.;
  
  double x0_g = gx+rad_gsf*sin(gsf.innerMomentum().phi())*gsf.charge();
  double y0_g = gy-rad_gsf*cos(gsf.innerMomentum().phi())*gsf.charge();

  double x0_t = tx+rad_trk*sin(trk.innerMomentum().phi())*trk.charge();
  double y0_t = ty-rad_trk*cos(trk.innerMomentum().phi())*trk.charge();
  
  double dca = sqrt((x0_t-x0_g)*(x0_t-x0_g)+(y0_t-y0_g)*(y0_t-y0_g))-rad_gsf-rad_trk;

  return dca;
}


typedef ObjectSelector<PATElectronConversionFinderImp> PATElectronConversionFinder;
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_ANOTHER_FWK_MODULE(PATElectronConversionFinder);
