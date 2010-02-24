//
// Original Author:  Konstantinos A. Petridis
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATElectronConversionFinder.cc,v 1.1 2009/03/26 10:20:52 kap01 Exp $
//
//

#include "TauAnalysis/RecoTools/plugins/PATElectronConversionFinder.h"
#include "DataFormats/Math/interface/Vector3D.h" 
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

PATElectronConversionFinderImp::PATElectronConversionFinderImp(const edm::ParameterSet& iConfig)
{
  tracks_ = iConfig.getParameter<edm::InputTag>("trackSource");
  conversions_ = iConfig.getParameter<edm::InputTag>("conversionSource");
  photons_ = iConfig.getParameter<edm::InputTag>("photonSource");
  cotTheta_ = iConfig.getParameter<double>("cotThetaCut");
  dca_ = iConfig.getParameter<double>("docaElecTrack");
  dRin_ = iConfig.getParameter<double>("dRElecTrack");
  doPXBcut_= iConfig.getParameter<bool>("doPixCut");
  nMax_ = iConfig.getParameter<double>("nTrkMax");
  doHists_ = iConfig.getParameter<bool>("doHists");
  useInnerParsForElec_ = iConfig.getParameter<bool>("useInnerParsForElec");
  useInnerParsForTrks_ = iConfig.getParameter<bool>("useInnerParsForTrks");
  useConversionColl_ = iConfig.getParameter<bool>("useConversionColl");
  
  
  if(doHists_){
    edm::Service<TFileService> fs;             
    
	hDeltaCotTheta=fs->make<TH1F>("DcotTheta","#Deltacot(#Theta)",500,-0.5,0.5);
    hDeltaCotThetaFilt=fs->make<TH1F>("DcotThetaFiltered","DcotTheta for events with 2 trks passing selection",200,-0.5,0.5);
    hDeltaCotThetaOS=fs->make<TH1F>("DcotThetaOS","DcotTheta for gsfElectron + OS trks",200,-0.5,0.5);
  	
	hDeltaR = fs->make<TH1F>("dR","#DeltaR",300,0,2);
    hDeltaRFilt=fs->make<TH1F>("dRinFiltered","#Delta R_{in} for events with 2 trks passing selection",100,0,0.4);
    hDeltaROS=fs->make<TH1F>("dRinOS","#Delta R_{in} for gsfElectron + OS tracks",100,0,0.4);
    
	hMassTrkAndElec=fs->make<TH1F>("mTrkAndElec","inv. mass of track and electron",300,0,30);
    hMassTrkAndElecFilt=fs->make<TH1F>("mTrkAndElecFiltered","inv. mass of track and electron for for evth 2 trks passing selection",100,0,1);
    hMassTrkAndElecOS=fs->make<TH1F>("mTrkAndElecOS","inv. mass of track and electron for gsfElectron + OS trks",100,0,1);
    
	hAbsPtDiff = fs->make<TH1F>("hAbsPtDiff","abs(Pt_{e}-Pt_{trk})",500,0,50);
    hAbsPtDiffFilt = fs->make<TH1F>("hAbsPtDiffFiltered","abs(Pt_{1}-Pt_{2}) for events with 2 tracks passing selection",100,0,50);
    hAbsPtDiffOS = fs->make<TH1F>("hAbsPtDiffOS","abs(Pt_{1}-Pt_{2}) for gsfElectron + OS trks",100,0,50);
    
	hPt = fs->make<TH1F>("hPt","Pt of all tracks",50,0,20);
	hPtOSFilt = fs->make<TH1F>("hPtFiltered","Pt of OS trk for events with 2 trks passing selection ",50,0,20);
	hPtOS = fs->make<TH1F>("hPtOS","Pt of OS tracks ",50,0,20);
	
	hOS = fs->make<TH1F>("hOS","Opposite sign tracks",2,0,2);
	hOSFilt = fs->make<TH1F>("hOSFiltered","Opposite sign for 2 tracks passing selection",2,0,2);
    
	hnTracksPass=fs->make<TH1F>("nTracksPass","num tracks passing #Delta cot(#Theta) and #DeltaR_{in} cuts",10,0,10);
    
	hDzin = fs->make<TH1F>("dZin","#Deltaz_{in}",500,-20,20);
	hDzinFilt = fs->make<TH1F>("dZinFiltered","#Deltaz_{in} for events with 2 trks passing selection",500,-20,20);
	hDzinOS = fs->make<TH1F>("dZinOS","#Deltaz_{in} for gElectron + OS trks",500,-20,20);
	
	hPXFDisk=fs->make<TH1F>("PXFDisk","FPix disk of innermost electron hit",5,0,5);
    hPXBLayer=fs->make<TH1F>("PXBLayer","BPix layer of innermost electron hit",5,0,5);
  
  	nElecs = fs->make<TH1F>("nElectrons","Number of Electrons",10,0,10);
  	nPhotons = fs->make<TH1F>("nPhotons","Number of Photons",10,0,10);
  	nConv = fs->make<TH1F>("nConv","Number of Conversions",10,0,10);
  	nConvTrks = fs->make<TH1F>("nConvTrks","Number of Conversion Tracks",20,0,20);
  }
}


PATElectronConversionFinderImp::~PATElectronConversionFinderImp() {}

void PATElectronConversionFinderImp::select(const edm::Handle<collection>& elecs,
		edm::Event& iEvent, const edm::EventSetup& iSetup)
{

	selected_.clear();

	using namespace edm;
	using namespace reco;

	eventWeight_ = 1;

	edm::Handle<reco::TrackCollection> tracks;
	iEvent.getByLabel(tracks_,tracks);
	const reco::TrackCollection* trackCollection = tracks.product();

	edm::Handle<reco::ConversionCollection> conversions;
	iEvent.getByLabel(conversions_, conversions);

	edm::Handle<reco::PhotonCollection> photons;
	//edm::Handle<pat::PhotonCollection> photons;	
	iEvent.getByLabel(photons_, photons);

	/*	
		edm::ESHandle<MagneticField> magFieldHandle;
		iSetup.get<IdealMagneticFieldRecord>().get( magFieldHandle );
		const MagneticField* magField = magFieldHandle.product();
	 */
	
	
	if(doHists_) nElecs->Fill(elecs->size(), eventWeight_);
	
	for( std::vector<pat::Electron>::const_iterator elec=elecs->begin();elec!=elecs->end(); ++elec)
	{
		int nTracksPass = 0;

		// save electron and tracks for later comparison
		reco::TrackCollection trackRefVec;
		reco::GsfTrackRef elecRef = (*elec).gsfTrack();

		// use conversion tracks instead of general tracks
		if(useConversionColl_) {
			reco::TrackCollection *trackCollectionTmp = new reco::TrackCollection();
			
			if(doHists_) nPhotons->Fill(photons->size(),eventWeight_);
			
			for( unsigned int iPhoton = 0; iPhoton < photons->size(); iPhoton++) {
				reco::Photon photon = photons->at(iPhoton);
				//pat::Photon photon = photons->at(iPhoton);
				reco::ConversionRefVector conversions = photon.conversions();
				if(doHists_) nConv->Fill(conversions.size(),eventWeight_);
				for( unsigned int iConversion = 0; iConversion < conversions.size(); iConversion++) {
					std::vector<reco::TrackRef> tracks = conversions.at(iConversion)->tracks();
					if(doHists_) nConvTrks->Fill(tracks.size(),eventWeight_);
					for( unsigned int iTrk = 0; iTrk < tracks.size(); iTrk++) {
						trackCollectionTmp->push_back( *(tracks.at(iTrk)) );
					}
				}
			}
			trackCollection = trackCollectionTmp;
		
		}
		
		// loop over general tracks or conversion tracks 
		for(size_t trk = 0; trk < trackCollection->size(); trk++)
		{
			// get dR between gsf electron and track
			double dR = 1000;
			// get z-component of dR between gsf electron and track
			double dZ = 1000;
			// get cot(theta) of opening angle between gsf electron and track
			double cotTheta = 1000;
			if( useInnerParsForElec_ && useInnerParsForTrks_ ) { 
				dR = ROOT::Math::VectorUtil::DeltaR(trackCollection->at(trk).innerPosition(),
						(*elec).gsfTrack()->innerPosition());
				dZ = trackCollection->at(trk).innerPosition().z()-(*elec).gsfTrack()->innerPosition().z();
				cotTheta = 1./tan((*elec).gsfTrack()->innerMomentum().Theta())-1./tan(trackCollection->at(trk).innerMomentum().Theta());
			} else if( useInnerParsForElec_ && !useInnerParsForTrks_ ) { 
				dR = ROOT::Math::VectorUtil::DeltaR(trackCollection->at(trk).referencePoint(),
						(*elec).gsfTrack()->innerPosition());
				dZ = trackCollection->at(trk).referencePoint().z()-(*elec).gsfTrack()->innerPosition().z();
				cotTheta = 1./tan((*elec).gsfTrack()->innerMomentum().Theta())-1./tan(trackCollection->at(trk).momentum().Theta());
			} else if( !useInnerParsForElec_ && useInnerParsForTrks_ ) { 
				dR = ROOT::Math::VectorUtil::DeltaR(trackCollection->at(trk).innerPosition(),
						(*elec).gsfTrack()->referencePoint());
				dZ = trackCollection->at(trk).innerPosition().z()-(*elec).gsfTrack()->referencePoint().z();
				cotTheta = 1./tan((*elec).gsfTrack()->momentum().Theta())-1./tan(trackCollection->at(trk).innerMomentum().Theta());
			} else {
				dR = ROOT::Math::VectorUtil::DeltaR(trackCollection->at(trk).referencePoint(),
						(*elec).gsfTrack()->referencePoint());
				dZ = trackCollection->at(trk).referencePoint().z()-(*elec).gsfTrack()->referencePoint().z();
				cotTheta = 1./tan((*elec).gsfTrack()->momentum().Theta())-1./tan(trackCollection->at(trk).momentum().Theta());
			}
			// check if electron and track have opposite charge
			bool os = ( (trackCollection->at(trk).charge() * ( (*elec).gsfTrack()->charge() )) == -1);
			// get distance of closest approach (DOCA) for gsf electron and track 
			//double doca = GetDocaBetweenTracks(iEvent, iSetup,magField, trackCollection->at(trk), *((*elec).gsfTrack()));//NEED TO CHECK THIS

			// also calculate invariant mass of gsf electron and track
			TLorentzVector trackP,elecP;
			trackP.SetPtEtaPhiM(trackCollection->at(trk).pt(),
					trackCollection->at(trk).eta(),
					trackCollection->at(trk).phi(),
					0.);
			elecP.SetPtEtaPhiM((*elec).pt(),(*elec).eta(),
					(*elec).phi(),0.);
			double mTrackElec = (trackP + elecP).M();// Could also cut on Invariant Mass but its equivalent to DCotTheta

			if(doHists_) {
				hDzin->Fill(dZ,eventWeight_);
				hDeltaCotTheta->Fill(cotTheta,eventWeight_);
				hMassTrkAndElec->Fill(mTrackElec, eventWeight_);
				hDeltaR->Fill(dR, eventWeight_);
				hAbsPtDiff->Fill( fabs(trackCollection->at(trk).pt()-(*elec).pt()), eventWeight_);
				hOS->Fill( os, eventWeight_);
				hPt->Fill( trackCollection->at(trk).pt(), eventWeight_);
				if( os ) { 
					hDzinOS->Fill(dZ,eventWeight_);
					hDeltaCotThetaOS->Fill(cotTheta,eventWeight_);
					hMassTrkAndElecOS->Fill(mTrackElec, eventWeight_);
					hDeltaROS->Fill(dR, eventWeight_);
					hAbsPtDiffOS->Fill( fabs(trackCollection->at(trk).pt()-(*elec).pt()), eventWeight_);
					hPtOS->Fill( trackCollection->at(trk).pt(), eventWeight_);
				}
				//hDoca->Fill(doca,dRin,eventWeight_);
			}

			// check if track satisfies conversion criteria
			if( fabs(cotTheta) < cotTheta_ && dR < dRin_ ) {
				nTracksPass++;
				// save track for alter analysis
				trackRefVec.push_back( trackCollection->at(trk) );
			}
			
		} // end tracks loop
		if(doHists_) hnTracksPass->Fill(nTracksPass);

		if(nTracksPass == 2 && doHists_) {
			
			bool os = ( (trackRefVec.at(0).charge() * trackRefVec.at(1).charge() ) == -1);
			hOSFilt->Fill( os, eventWeight_);
			
			if( ( (*elec).gsfTrack()->charge() * trackRefVec.at(1).charge() ) == -1) {
				double cotTheta = 1./tan((*elec).gsfTrack()->innerMomentum().Theta()) - 1./tan( trackRefVec.at(1).innerMomentum().Theta() );
				hDeltaCotThetaFilt->Fill(cotTheta,eventWeight_);
			
				hPtOSFilt->Fill(trackRefVec.at(1).pt(), eventWeight_);
				
				double dR = ROOT::Math::VectorUtil::DeltaR((*elec).gsfTrack()->innerPosition(), trackRefVec.at(1).innerPosition());
				hDeltaRFilt->Fill(dR, eventWeight_);
			
				hAbsPtDiffFilt->Fill( fabs((*elec).gsfTrack()->pt() - trackRefVec.at(1).pt()), eventWeight_);
				//hMassTrkAndElecFilt->Fill(mTrackElec, eventWeight_);
			} else {
				double cotTheta = 1./tan((*elec).gsfTrack()->innerMomentum().Theta()) - 1./tan( trackRefVec.at(0).innerMomentum().Theta() );
				hDeltaCotThetaFilt->Fill(cotTheta,eventWeight_);
			
				hPtOSFilt->Fill(trackRefVec.at(0).pt(), eventWeight_);
				
				double dR = ROOT::Math::VectorUtil::DeltaR((*elec).gsfTrack()->innerPosition(), trackRefVec.at(0).innerPosition());
				hDeltaRFilt->Fill(dR, eventWeight_);
			
				hAbsPtDiffFilt->Fill( fabs((*elec).gsfTrack()->pt() - trackRefVec.at(0).pt()), eventWeight_);
				//hMassTrkAndElecFilt->Fill(mTrackElec, eventWeight_);
			}
		}

		// Check for pixel inner layer hit
		int innerDetIdRaw = (*elec).gsfTrack()->innerDetId();
		DetId innerDetId(innerDetIdRaw);
		if(innerDetId.det() == DetId::Tracker) {
			if(innerDetId.subdetId()==1) { 
				PXBDetId pxbDetId(innerDetId); 
				if(doHists_)
					hPXBLayer->Fill(pxbDetId.layer(),eventWeight_);
			}
			if(innerDetId.subdetId()==2) { 
				PXFDetId pxfDetId(innerDetId); 
				if(doHists_) 
					hPXFDisk->Fill(pxfDetId.disk(),eventWeight_);
			}
		}

		// add electrons to output collection if no conversion track pairs are found
		if(nTracksPass <= nMax_) { //nMax_ should be <=1 since gsf electron track might appear in the track list
			if(doPXBcut_) {
				if(innerDetId.det() == DetId::Tracker) {
					if(innerDetId.subdetId()==1) {
						PXBDetId pxbDetId(innerDetId);
						if( pxbDetId.layer() == 1 ) selected_.push_back(&(*elec));
					}
					if(innerDetId.subdetId()==2){
						//PXFDetId pxfDetId(innerDetId);
						//if( pxfDetId.disk()==1 ) 
						selected_.push_back(&(*elec));
					}
				}
			} else
				selected_.push_back(&(*elec));
		}	
	} // end PAT electron loop
}

double PATElectronConversionFinderImp::GetDocaBetweenTracks(const edm::Event& evt, 
							const edm::EventSetup& setup,
							const MagneticField* magField,
							const reco::Track& trk,
							const reco::GsfTrack& gsf)
{
 /*
  double gx=gsf.trackPosition().x(),gy=gsf.innerPosition().y(),gz=gsf.innerPosition().z();
  double rad_gsf = gsf.trackMomentumAtVtx().Rho()/(0.3*magField->inTesla(GlobalPoint(gx,gy,gz)).z())*100.;
  
  double tx=gsf.innerPosition().x(),ty=trk.innerPosition().y(),tz=trk.innerPosition().z();
  double rad_trk = trk.trackMomentumAtVtx().Rho()/(0.3*magField->inTesla(GlobalPoint(tx,ty,tz)).z())*100.;
  
  double x0_g = gx+rad_gsf*sin(gsf.trackMomentumAtVtx().phi())*gsf.charge();
  double y0_g = gy-rad_gsf*cos(gsf.trackMomentumAtVtx().phi())*gsf.charge();

  double x0_t = tx+rad_trk*sin(trk.trackMomentumAtVtx().phi())*trk.charge();
  double y0_t = ty-rad_trk*cos(trk.trackMomentumAtVtx().phi())*trk.charge();
  
  double doca = sqrt((x0_t-x0_g)*(x0_t-x0_g)+(y0_t-y0_g)*(y0_t-y0_g))-rad_gsf-rad_trk;
*/
  return 0;
}


typedef ObjectSelector<PATElectronConversionFinderImp> PATElectronConversionFinder;
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_ANOTHER_FWK_MODULE(PATElectronConversionFinder);
