#ifndef RecoMuon_SeedGenerator_RPCSeedOverlapper_H
#define RecoMuon_SeedGenerator_RPCSeedOverlapper_H

/**  \class RPCSeedPattern
 *
 *  \author Haiyun.Teng - Peking University
 *
 *
 */


#include <DataFormats/TrajectorySeed/interface/TrajectorySeed.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <Geometry/RPCGeometry/interface/RPCGeometry.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <DataFormats/Common/interface/OwnVector.h>
#include <DataFormats/TrackingRecHit/interface/TrackingRecHit.h>


class RPCSeedOverlapper {

    public:
        RPCSeedOverlapper();
        ~RPCSeedOverlapper();
        void setIO(std::vector<TrajectorySeed> *goodRef, std::vector<TrajectorySeed> *candidateRef);
        void unsetIO();
        void run();    
        void configure(const edm::ParameterSet& iConfig);
        void setEventSetup(const edm::EventSetup& iSetup);
    private:
        void CheckOverlap(const edm::EventSetup& iSetup, std::vector<TrajectorySeed> *SeedsRef);
        bool isShareHit(const edm::OwnVector<TrackingRecHit> &RecHits, const TrackingRecHit& hit, edm::ESHandle<RPCGeometry> rpcGeometry);
        // Signal for call run()
        bool isConfigured;
        bool isIOset;
        bool isEventSetupset;
        // Parameters for configuration
        bool isCheckgoodOverlap;
        bool isCheckcandidateOverlap;
        unsigned int ShareRecHitsNumberThreshold;
        // IO ref
        std::vector<TrajectorySeed> *goodSeedsRef;
        std::vector<TrajectorySeed> *candidateSeedsRef;
        const edm::EventSetup *eSetup;
};

#endif
