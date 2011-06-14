

#ifndef TauAnalysis_RecoTools_NPATElectronConversionFinder_h
#define TauAnalysis_RecoTools_NPATElectronConversionFinder_h

// system include files
#include <memory>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/Vector3D.h" 

#include "Math/GenVector/VectorUtil.h"
#include "TLorentzVector.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"


class NPATElectronConversionFinderImp
{
    public:

        typedef std::vector<pat::Electron> PATElectronCollection;
        typedef std::vector<pat::Electron> collection;

        NPATElectronConversionFinderImp(const edm::ParameterSet& iConfig);
        ~NPATElectronConversionFinderImp();

        std::vector<const pat::Electron*>::const_iterator begin() const { return selected_.begin(); }
        std::vector<const pat::Electron*>::const_iterator end() const { return selected_.end();}

        void select(const edm::Handle<PATElectronCollection>&,edm::Event & iEvent, const edm::EventSetup & iSetup);

    private:

        std::vector<const pat::Electron*> selected_;

        int maxMissingInnerHits_;
        double minRxy_;
        double minFitProb_;
        int maxHitsBeforeVertex_;



};

#endif
