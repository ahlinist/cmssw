#ifndef ElectroWeakAnalysis_MultiBosons_interface_VGammaMuonSelector_h
#define ElectroWeakAnalysis_MultiBosons_interface_VGammaMuonSelector_h

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>

class VGammaMuonSelector : public Selector<pat::Muon> {
 public:
  enum version { Jun252010 , Jun262010_jpsi  , N_VERSIONS};
  
  VGammaMuonSelector( const edm::ParameterSet& );

  bool operator()( const pat::Muon &, 
		   edm::EventBase const &, 
		   pat::strbitset & );

  using Selector<pat::Muon>::operator();

  bool operator()( const pat::Muon &, 
		   pat::strbitset & );

  bool Jun252010Cuts( const pat::Muon& mu,
		      pat::strbitset & ret );

  bool Jun262010_jpsiCuts( const pat::Muon& mu,
			   pat::strbitset & ret );


 private:
  version version_;
  std::vector<std::string> muonTypeRequirement_;
  std::string TMidType_;

  void init( const version& v,
	     const std::vector<std::string>& vmt, // valid muon types
	     const std::string & tmid,
	     const int & pixHits, // minimum number of pix hits
	     const int & tkHits, // min. tk hits
	     const double & d0, // d0 w.r.t. beam spot
	     const double & dz, // dz w.r.t. beam spot
	     const double & chi2, // global chi2/ndof
	     const int & chambers, // number of chambers in SA muon
	     const int & stations, // number of stations
	     const double & p, // min p
	     const double & pt, // min pt 
	     const double & eta, // max eta
	     const double & relIso // max rel comb iso
	     );
	     
};



#endif
