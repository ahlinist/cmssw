//
// Original Author:  Konstantinos A. Petridis
//         Created:  Wed Oct 15 21:33:34 CEST 2008
//		Major re-write: Jeff Kolb, March 2011
// $Id: PATElectronConversionFinder.cc,v 1.10 2011/04/07 09:44:50 jkolb Exp $
//
//
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"

#include "TauAnalysis/RecoTools/plugins/PATElectronConversionFinder.h"

PATElectronConversionFinderImp::PATElectronConversionFinderImp(const edm::ParameterSet& iConfig)
{
	//  get partner track source
	tracks_ = iConfig.getParameter<edm::InputTag>("trackSource");
	
	//  choose algorithm
	usePogMethod_ = iConfig.getParameter<bool>("usePogMethod");
	if( usePogMethod_ ) {
		dcsTag_ = iConfig.getParameter<edm::InputTag>("dcsTag");
		isData_ = iConfig.getParameter<bool>("isData");
		docaMax_ = iConfig.getParameter<double>("docaElecTrackMax");
	} else {
		conversions_ = iConfig.getParameter<edm::InputTag>("conversionSource");
		photons_ = iConfig.getParameter<edm::InputTag>("photonSource");
		useConversionColl_ = iConfig.getParameter<bool>("useConversionColl");
		useOSPairs_ = iConfig.getParameter<bool>("useOnlyOSPairs");
		dRinMax_ = iConfig.getParameter<double>("dRElecTrackMax");
	}

	//  choose pixel cut
	if( iConfig.exists("doPixCut") ) 
		doPXBcut_= iConfig.getParameter<bool>("doPixCut");
	if( iConfig.exists("doMissingHitsCut") ) 
		doMissingHitsCut_= iConfig.getParameter<bool>("doMissingHitsCut");
	
	if( doPXBcut_ && doMissingHitsCut_ ) {
		edm::LogWarning("PATElectronConversionFinder") << "Must not set "
			<< " both doPixCut and doMissingHitsCut to TRUE!" << std::endl
			<< " Setting doMissingHitsCut=False";
		doMissingHitsCut_ = 0;
	}
	
	//  choose deltaCot(theta) limits
	cotThetaMax_ = iConfig.getParameter<double>("cotThetaMax");
	
	//  choose limits on number of conversion track pairs
	nPairMax_ = iConfig.getParameter<double>("nConvPairMax");
	nPairMin_ = iConfig.getParameter<double>("nConvPairMin");
}


PATElectronConversionFinderImp::~PATElectronConversionFinderImp() {}

void PATElectronConversionFinderImp::select(const edm::Handle<collection>& elecs,
		edm::Event& iEvent, const edm::EventSetup& iSetup)
{

    selected_.clear();

    edm::Handle<reco::TrackCollection> tracks;
    iEvent.getByLabel(tracks_,tracks);

    //using namespace edm;
    //using namespace reco;

    // iterate over GSF electrons in event
    for( std::vector<pat::Electron>::const_iterator elec = elecs->begin(); elec != elecs->end(); ++elec)
    {
        int nTracksPass = 0;

        if( !usePogMethod_) {  // use OLD method

            edm::Handle<reco::ConversionCollection> conversions;
            iEvent.getByLabel(conversions_, conversions);

            edm::Handle<reco::PhotonCollection> photons;
            iEvent.getByLabel(photons_, photons);

            const reco::TrackCollection* trackCollection = tracks.product();

            if(useConversionColl_) {
                // use conversion tracks instead of general tracks
                // get track from best conversion by taking conversion from photon
                reco::TrackCollection *trackCollectionTmp = new reco::TrackCollection();

                for( unsigned int iPhoton = 0; iPhoton < photons->size(); iPhoton++) {
                    reco::Photon photon = photons->at(iPhoton);
                    reco::ConversionRefVector conversions = photon.conversions();
                    for( unsigned int iConversion = 0; iConversion < conversions.size(); iConversion++) {
                        std::vector<edm::RefToBase<reco::Track> > tracks = conversions.at(iConversion)->tracks(); //compatible with CMSSW_3_9_X and higher
                        //std::vector<reco::TrackRef> tracks = conversions.at(iConversion)->tracks(); //compatible with CMSSW_3_8_X
                        for( unsigned int iTrk = 0; iTrk < tracks.size(); iTrk++) {
                            trackCollectionTmp->push_back( *(tracks.at(iTrk)) );
                        }
                    }
                }
                trackCollection = trackCollectionTmp;
            }

            // loop over partner tracks
            for(size_t trk = 0; trk < trackCollection->size(); trk++)
            {
                if(useOSPairs_ && ( trackCollection->at(trk).charge()*(*elec).gsfTrack()->charge() ) == 0) continue;
                // get dR between gsf electron and track
                double dR = 1000;
                // get z-component of dR between gsf electron and track
                double dZ = 1000;
                // get cot(theta) of opening angle between gsf electron and track
                double cotTheta = 1000;

                dR = ROOT::Math::VectorUtil::DeltaR(trackCollection->at(trk).innerPosition(),
                        (*elec).gsfTrack()->innerPosition());
                dZ = trackCollection->at(trk).innerPosition().z()-(*elec).gsfTrack()->innerPosition().z();
                cotTheta = 1./tan((*elec).gsfTrack()->innerMomentum().Theta())-1./tan(trackCollection->at(trk).innerMomentum().Theta());

                // check if track satisfies conversion criteria
                if( fabs(cotTheta) < cotThetaMax_ && dR < dRinMax_) {
                    nTracksPass++;
                }
            } // end tracks loop

        } else {
            //
            // NEW EGAMMA method
            //
            double evt_bField;
            // need the magnetic field
            //
            // if isData then derive bfield using the
            // magnet current from DcsStatus
            // otherwise take it from the IdealMagneticFieldRecord
            if (isData_) {
                edm::Handle<DcsStatusCollection> dcsHandle;
                iEvent.getByLabel(dcsTag_, dcsHandle);
                if( !dcsHandle.isValid() || dcsHandle->size() == 0) {
                    edm::LogError("PATElectronConversionFinderImp::select") 
                        << "Invalid DcsStatus handle OR DcsStatus collection size = 0!  Event forced to fail cut";
                    return;
                }
                // scale factor = 3.801/18166.0 which are
                // average values taken over a stable two
                // week period
                float currentToBFieldScaleFactor = 2.09237036221512717e-04;
                float current = (*dcsHandle)[0].magnetCurrent();
                evt_bField = current*currentToBFieldScaleFactor;
            } else {
                edm::ESHandle<MagneticField> magneticField;
                iSetup.get<IdealMagneticFieldRecord>().get(magneticField);

                evt_bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
            }

            ConversionFinder convFinder;
            ConversionInfo convInfo = convFinder.getConversionInfo(*elec, tracks, evt_bField);

            double dist = convInfo.dist();
            double dcot = convInfo.dcot();
            //double convradius = convInfo.radiusOfConversion();
            //math::XYZPoint convPoint = convInfo.pointOfConversion();

            // check if track satisfies conversion criteria
            if( fabs(dcot) < cotThetaMax_ && dist < docaMax_ ) {
                nTracksPass++;
            }
        }

        // add electrons to output collection if no conversion track pairs are found
        if(nTracksPass <= nPairMax_ && nTracksPass >= nPairMin_) { 
            if(doPXBcut_) {
                int innerDetIdRaw = (*elec).gsfTrack()->innerDetId();
                DetId innerDetId(innerDetIdRaw);
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
            } else if( doMissingHitsCut_) {
                const reco::Track *elec_track = (const reco::Track*)(elec->gsfTrack().get());  
                const reco::HitPattern& p_inner = elec_track->trackerExpectedHitsInner(); 
                if( p_inner.numberOfHits() < 1) 
                    selected_.push_back(&(*elec));
            } else
                selected_.push_back(&(*elec));
        }	
    } // end PAT electron loop
}

typedef ObjectSelector<PATElectronConversionFinderImp> PATElectronConversionFinder;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronConversionFinder);
