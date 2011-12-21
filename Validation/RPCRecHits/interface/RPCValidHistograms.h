#ifndef Validation_RPCRecHits_RPCValidHistograms_H
#define Validation_RPCRecHits_RPCValidHistograms_H

#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include <string>

struct RPCValidHistograms
{
  typedef MonitorElement* MEP;

  RPCValidHistograms()
  {
    booked_ = false;
  };

  void bookHistograms(DQMStore* dbe, const std::string subDir);

  // Hit properties
  MEP clusterSize, clusterSizeBarrel, clusterSizeEndcap;
  MEP avgClusterSize, avgClusterSizeBarrel, avgClusterSizeEndcap;

  MEP nRefHitBarrel, nRefHitEndcap;
  MEP nRecHitBarrel, nRecHitEndcap;
  MEP nMatchHitBarrel, nMatchHitEndcap;

  // Occupancy 1D
  MEP refHitOccupancyBarrel_wheel, refHitOccupancyEndcap_disk, refHitOccupancyBarrel_station;
  MEP recHitOccupancyBarrel_wheel, recHitOccupancyEndcap_disk, recHitOccupancyBarrel_station;
  MEP matchOccupancyBarrel_wheel, matchOccupancyEndcap_disk, matchOccupancyBarrel_station;
  MEP umOccupancyBarrel_wheel, umOccupancyEndcap_disk, umOccupancyBarrel_station;

  // Occupancy 2D
  MEP refHitOccupancyBarrel_wheel_station, refHitOccupancyEndcap_disk_ring;
  MEP recHitOccupancyBarrel_wheel_station, recHitOccupancyEndcap_disk_ring;
  MEP matchOccupancyBarrel_wheel_station, matchOccupancyEndcap_disk_ring;
  MEP umOccupancyBarrel_wheel_station, umOccupancyEndcap_disk_ring;

  // Residuals
  MEP resBarrel, resEndcap;
  MEP res_wheel_res, res_disk_res, res_station_res, res_ring_res;
  
  // Pulls
  MEP pullBarrel, pullEndcap;
  MEP pull_wheel_pull, pull_disk_pull, pull_station_pull, pull_ring_pull;

private:
  bool booked_;
};

#endif

