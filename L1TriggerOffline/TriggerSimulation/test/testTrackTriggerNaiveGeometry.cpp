

#include "SLHCUpgradeSimulations/L1Trigger/interface/TrackTriggerNaiveGeometry.h"

#include <TMath.h>

#include <vector>


TrackTriggerNaiveGeometry setupGeom() {

  // make vectors
  std::vector<double> radii;
  radii.push_back(100.);

  std::vector<double> lengths;
  lengths.push_back(120.);

  std::vector<double> barrelTowZSize;
  barrelTowZSize.push_back(8.);
  
  std::vector<double> barrelTowPhiSize;
  barrelTowPhiSize.push_back(8.268);

  return TrackTriggerNaiveGeometry(
					radii,
					lengths,
					barrelTowZSize,
					barrelTowPhiSize,
					1.0,
					0.015);

}

int test(TrackTriggerNaiveGeometry geom) {

	if (geom.nLayers() != 1) return 1;
	if (geom.barrelLayerRadius(0) != 100.) return 2;
	if (geom.barrelLayerLength(0) != 120.) return 3;	
	if (geom.nBarrelTowersZ(0) != 15.) return 4;
	if (geom.nBarrelTowersPhi(0) != 76.) return 5;	
	if (geom.towerSizeZ(0) != 8.) return 6;
	if (geom.towerSizePhi(0) != 8.268) return 7;
	
	if (geom.towerZSign(0x80000000)>0 || geom.towerZSign(0x0)<0) return 8;
	if (geom.towerIsBarrel(0x40000000) || !geom.towerIsBarrel(0x80188000)) return 9;

	double ez = 0.6;
	double ephi = TMath::Pi() / 99;

//	std::cout << "Testing : indices -> physical" << std::endl;
	for (unsigned iz=0; iz<geom.nBarrelTowersZ(0); ++iz) {
		for (unsigned iphi=0; iphi<geom.nBarrelTowersPhi(0); ++iphi) {
			double z = geom.towerSizeZ(0) * (iz+0.5);
			double phi = (-1*TMath::Pi()) + (geom.towerSizePhi(0) * (iphi + 0.5)/geom.barrelLayerRadius(0));
//			std::cout << "z=" << z << " phi=" << phi << " iz=" << iz << " iphi=" << iphi << std::endl;
//			std::cout << " nz=" << geom.barrelTowerCentreZ(0, iz) << " nphi=" << geom.barrelTowerCentrePhi(0, iphi) << std::endl;
			if ( fabs(geom.barrelTowerCentrePhi(0, iphi) - phi) > ephi ) return 11;
			if ( fabs(geom.barrelTowerCentreZ(0, iz) - z) > ez ) return 12;
		}
	}

//	std::cout << "Testing : indices -> physical -> id -> indices" << std::endl;
    for (unsigned iz=0; iz<geom.nBarrelTowersZ(0); ++iz) {
		for (unsigned iphi=0; iphi<geom.nBarrelTowersPhi(0); ++iphi) {
			double z = geom.towerSizeZ(0) * (iz+0.5);
			double phi = (-1*TMath::Pi()) + (geom.towerSizePhi(0) * (iphi + 0.5)/geom.barrelLayerRadius(0));
			unsigned id = geom.barrelTowerId(0, z, phi);
//			std::cout << "z=" << z << " phi=" << phi << " iz=" << iz << " iphi=" << iphi << std::endl;
			if (geom.barrelTowerIZ(id) != iz) return 13;
			if (geom.barrelTowerIPhi(id) != iphi) return 14;
//			std::cout << " id_iz=" << geom.barrelTowerIZ(id) << " id_iphi=" << geom.barrelTowerIPhi(id) << std::endl;
		}
	}

    //	std::cout << "Testing : physical -> pixel indices" << std::endl;    
	for (unsigned iz=0; iz<(geom.nBarrelTowersZ(0)*8); ++iz) {
		for (unsigned iphi=0; iphi<geom.nBarrelTowersPhi(0)*550; ++iphi) {
			double z = geom.barrelPixelSizeZ(0) * (iz+0.5);
			double phi = (-1*TMath::Pi()) + (geom.barrelPixelSizePhi(0) * (iphi + 0.5)/geom.barrelLayerRadius(0));
			unsigned id = geom.barrelTowerId(0, z, phi);
			std::cout << "z=" << z << " phi=" << phi << " iz=" << iz << " iphi=" << iphi << std::endl;
			std::cout << " row=" << geom.barrelPixelRow(id, z) << " col=" << geom.barrelPixelColumn(id, phi) << std::endl;
			if ( geom.barrelPixelColumn(id, phi) != iphi ) return 15;
			if ( geom.barrelPixelRow(id, z) != iz ) return 16;
		}
	}
    
    
	return 0;
	
}

int main() {

	TrackTriggerNaiveGeometry geom = setupGeom();

	std::cout << geom << std::endl;

	int result = test(geom);

	if (result != 0) std::cerr << "Test failed with error code " << result << std::endl; 
	return result;

}
