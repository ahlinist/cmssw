#ifndef MuonAnalysis_Examples_interface_muonStations_h
#define MuonAnalysis_Examples_interface_muonStations_h
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
namespace muon {
    /** Count the number of muon stations from a given track
        subdet = 0 for all subdetectors, or 1/2/3 for DT/CSC/RPC (as defined in MuonSubdetId 
        validOnly = true (default) to include only valid hits, = false to include also bad ones 
        note: requires the TrackExtra and the TrackingRecHits to be available.
      */
    int muonStations(const reco::TrackRef track, int subdet=0, bool validOnly=true) ;
}
#endif
