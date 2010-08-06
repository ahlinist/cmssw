#ifndef ElectroWeakAnalysis_MultiBosons_interface_VGammaPhotonSelector_h
#define ElectroWeakAnalysis_MultiBosons_interface_VGammaPhotonSelector_h

#include "DataFormats/PatCandidates/interface/Photon.h"

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>

class VGammaPhotonSelector : public Selector<pat::Photon> {
 public:
  enum version { Jul022010 , Jul022010_poter, Jul022010_poterrel , N_VERSIONS};
  
  VGammaPhotonSelector() {}
  VGammaPhotonSelector( const edm::ParameterSet& );

  bool operator()( const pat::Photon&, 
		   edm::EventBase const &, 
		   pat::strbitset & );

  using Selector<pat::Photon>::operator();

  bool operator()( const pat::Photon& pho, 
		   pat::strbitset & );

  bool Jul022010Cuts( const pat::Photon& pho,
		      pat::strbitset & ret );
  
  bool Jul022010_poterCuts( const pat::Photon& pho,
			    pat::strbitset & ret );
  
  bool Jul022010_poterrelCuts( const pat::Photon& pho,
			       pat::strbitset& ret);
  

 private:
  version version_;

  double barJurECALIsoConst_, barJurECALIsoSlope_;
  double barTowHCALIsoConst_, barTowHCALIsoSlope_;	
  double barMaxHOverE_;
  double barHConeTrkIsoConst_, barHConeTrkIsoSlope_;
  double barMaxSigIEtaIEta_;
  double endJurECALIsoConst_, endJurECALIsoSlope_;
  double endTowHCALIsoConst_, endTowHCALIsoSlope_;	
  double endMaxHOverE_;
  double endHConeTrkIsoConst_, endHConeTrkIsoSlope_;
  double endMaxSigIEtaIEta_;

  void init( const version& v,
	     const double& barJurECALIsoConst,
	     const double& barJurECALIsoSlope,
	     const double& barTowHCALIsoConst,
	     const double& barTowHCALIsoSlope,
	     const double& barMaxHOverE,
	     const double& barHConeTrkIsoConst,
	     const double& barHConeTrkIsoSlope,
	     const double& barMaxSigIEtaIEta,
	     const double& endJurECALIsoConst,
	     const double& endJurECALIsoSlope,
	     const double& endTowHCALIsoConst,
	     const double& endTowHCALIsoSlope,
	     const double& endMaxHOverE,
	     const double& endHConeTrkIsoConst,
	     const double& endHConeTrkIsoSlope,
	     const double& endMaxSigIEtaIEta,
	     const double& minPt,
	     const double& maxEta
	     );
	     
};



#endif
