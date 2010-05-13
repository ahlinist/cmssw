#include "MuonAnalysis/Examples/interface/muonStations.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/DTLayerId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

int muon::muonStations(const reco::TrackRef track, int subdet, bool validOnly) {
    int stations[4] = { 0,0,0,0 };
    if (track.isNull()) return 0;
    for (trackingRecHit_iterator it = track->recHitsBegin(), ed = track->recHitsEnd(); it != ed; ++it) {
        DetId id = (*it)->geographicalId();
        if (id.det() != DetId::Muon) continue;
        if (subdet != 0 && id.subdetId() != subdet) continue;
        if (validOnly && !(*it)->isValid()) continue;
        switch (id.subdetId()) {
            case MuonSubdetId::DT: 
                stations[DTLayerId(id).station()-1] = 1; 
                break;
            case MuonSubdetId::CSC:
                stations[CSCDetId(id).station()-1] = 1; 
                break;
            case MuonSubdetId::RPC: 
                stations[RPCDetId(id).station()-1] = 1; 
                break;
        }
    }
    return stations[0]+stations[1]+stations[2]+stations[3];
}

