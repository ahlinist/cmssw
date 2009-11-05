#ifndef CalibrationIsolatedParticleseMatrizHCALDetIds_h
#define CalibrationIsolatedParticleseMatrizHCALDetIds_h

#include <vector>

#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"

namespace spr{

  std::vector<DetId> matrixHCALIds(std::vector<DetId>& dets, const HcalTopology* topology, int ieta, int iphi, bool debug=false);

  std::vector<DetId> matrixHCALIds(std::vector<DetId>& dets, const HcalTopology* topology, int ietaE, int ietaW, int iphiN, int iphiS, bool debug=false);

  std::vector<DetId> newHCALIdNS(std::vector<DetId>& dets, unsigned int last, const HcalTopology* topology, bool shiftNorth, int ieta, int iphi, bool debug=false);

  std::vector<DetId> newHCALIdNS(std::vector<DetId>& dets, unsigned int last, const HcalTopology* topology, bool shiftNorth, int ietaE, int ietaW, int iphiN, int iphiS, bool debug=false);

  std::vector<DetId> newHCALIdEW(std::vector<DetId>& dets, unsigned int last, const HcalTopology* topology, bool shiftEast, int ieta, bool debug=false);

  std::vector<DetId> newHCALIdEW(std::vector<DetId>& dets, unsigned int last, const HcalTopology* topology, bool shiftEast, int ietaE, int ietaW, bool debug=false);
}

#endif
