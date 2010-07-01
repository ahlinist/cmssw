#ifndef ElectroWeakAnalysis_MultiBosons_interface_VGammaElectronSelector_h
#define ElectroWeakAnalysis_MultiBosons_interface_VGammaElectronSelector_h

#include "DataFormats/PatCandidates/interface/Electron.h"

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>

class VGammaElectronSelector : public Selector<pat::Electron> {
 public:
  enum version { Jul012010 , N_VERSIONS};
  
  VGammaElectronSelector( const edm::ParameterSet& );

  bool operator()( const pat::Electron &, 
		   edm::EventBase const &, 
		   pat::strbitset & );

  using Selector<pat::Electron>::operator();

  bool operator()( const pat::Electron &, 
		   pat::strbitset & );

  bool Jul012010Cuts( const pat::Electron& mu,
		      pat::strbitset & ret );

 private:
  version version_;
  std::string eid_;

  void init( const version& v,
	     std::string eid,
	     int eidlevel,
	     double minpt,
	     double maxeta
	     );
	     
};
#endif
