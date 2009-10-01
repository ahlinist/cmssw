//
// Original Author:  Konstantinos A. Petridis
//         Created:  Wed Oct 15 21:33:34 CEST 2008
//
// Updated summer 09 by Jeff Kolb
//
// $Id: PATElectronConversionFinder.cc,v 1.1 2009/03/26 10:20:52 kap01 Exp $
//
//

#include "TauAnalysis/RecoTools/plugins/PATElectronConversionFinder.h"
#include "DataFormats/Math/interface/Vector3D.h" 
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/Math/interface/deltaR.h"

PATElectronConversionFinderImp::PATElectronConversionFinderImp(const edm::ParameterSet& iConfig)
{
  tracks_ = iConfig.getParameter<edm::InputTag>("trackSource");
  cotTheta_ = iConfig.getParameter<double>("cotThetaCut");
  dRin_ = iConfig.getParameter<double>("dRElecTrack");
  doPXBcut_= iConfig.getParameter<bool>("doPixCut");
  nMax_ = iConfig.getParameter<double>("nTrkMax");
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

	for( std::vector<pat::Electron>::const_iterator elec=elecs->begin();elec!=elecs->end(); ++elec)
	{
		int nTracksPass = 0;

		// loop over general tracks 
		for(size_t trk = 0; trk < trackCollection->size(); trk++)
		{
			// get dR between gsf electron and track
			double dR = ROOT::Math::VectorUtil::DeltaR(trackCollection->at(trk).innerPosition(),
						(*elec).gsfTrack()->innerPosition());
			// get z-component of dR between gsf electron and track
			// double dZ = trackCollection->at(trk).innerPosition().z()-(*elec).gsfTrack()->innerPosition().z();
			// get cot(theta) of opening angle between gsf electron and track
			double cotTheta = 1./tan((*elec).gsfTrack()->innerMomentum().Theta())-1./tan(trackCollection->at(trk).innerMomentum().Theta());
			// check if electron and track have opposite charge
			// bool os = ( (trackCollection->at(trk).charge() * ( (*elec).gsfTrack()->charge() )) == -1);

			// also calculate invariant mass of gsf electron and track
			TLorentzVector trackP,elecP;
			trackP.SetPtEtaPhiM(trackCollection->at(trk).pt(),
					trackCollection->at(trk).eta(),
					trackCollection->at(trk).phi(),
					0.);
			elecP.SetPtEtaPhiM((*elec).pt(),(*elec).eta(),
					(*elec).phi(),0.);
			double mTrackElec = (trackP + elecP).M();// Could also cut on Invariant Mass but its equivalent to DCotTheta
			          
			// check if track satisfies conversion criteria
			if( fabs(cotTheta) < cotTheta_ && dR < dRin_ ) nTracksPass++;
													  
		} // end tracks loop

		// add electrons to output collection if no conversion tracks are found
		if(nTracksPass <= nMax_) { //nMax_ should be 1 since gsf electron track might appear in general track list
			
			if(doPXBcut_) {  // Check for pixel inner layer hit
				int innerDetIdRaw = (*elec).gsfTrack()->innerDetId();
				DetId innerDetId(innerDetIdRaw);
				
				if(innerDetId.det() == DetId::Tracker) {
					if(innerDetId.subdetId()==1) {
						PXBDetId pxbDetId(innerDetId);
						if( pxbDetId.layer() == 1 ) selected_.push_back(&(*elec));
					}
					if(innerDetId.subdetId()==2) { // don't require FPix layer 1 hit
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

typedef ObjectSelector<PATElectronConversionFinderImp> PATElectronConversionFinder;
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_ANOTHER_FWK_MODULE(PATElectronConversionFinder);
