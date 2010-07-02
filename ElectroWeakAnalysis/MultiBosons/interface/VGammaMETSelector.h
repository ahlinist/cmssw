#ifndef ElectroWeakAnalysis_MultiBosons_interface_VGammaMETSelector_h
#define ElectroWeakAnalysis_MultiBosons_interface_VGammaMETSelector_h

#include "DataFormats/PatCandidates/interface/MET.h"

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>

class VGammaMETSelector : public Selector<pat::MET> {
 public:
  enum version { Jul022010 , N_VERSIONS};
  
  VGammaMETSelector( const edm::ParameterSet& );

  bool operator()( const pat::MET &, 
		   edm::EventBase const &, 
		   pat::strbitset & );

  using Selector<pat::MET>::operator();

  bool operator()( const pat::MET & , 
		   pat::strbitset & );

  bool Jul022010Cuts( const pat::MET& met,
		      pat::strbitset & ret );

 private:
  version version_;

  void init( const version& v,
	     double minmet
	     );
	     
};
#endif
