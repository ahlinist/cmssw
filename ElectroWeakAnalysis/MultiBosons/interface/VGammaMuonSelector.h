#ifndef ElectroWeakAnalysis_MultiBosons_interface_VGammaMuonSelector_h
#define ElectroWeakAnalysis_MultiBosons_interface_VGammaMuonSelector_h

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>

class VGammaMuonSelector : public Selector<pat::Muon> {
 public:
  enum version {Jun252010, Jun262010_jpsi, Sep062010, FsrApr082011,
                N_VERSIONS};

  VGammaMuonSelector() {}
  VGammaMuonSelector( const edm::ParameterSet& );

  bool operator()( const pat::Muon &,
                   edm::EventBase const &,
                   pat::strbitset & );

  using Selector<pat::Muon>::operator();

  bool operator()( const pat::Muon &,
                   pat::strbitset & );

  bool Jun252010Cuts( const pat::Muon& mu,
                      pat::strbitset & ret );

  bool Sep062010Cuts( const pat::Muon &mu,
                      pat::strbitset & ret );

  bool Jun262010_jpsiCuts( const pat::Muon& mu,
                           pat::strbitset & ret );

  bool FsrApr082011Cuts( const pat::Muon& mu,
                         pat::strbitset & ret );

 private:
  version version_;
  std::vector<std::string> muonTypeRequirement_;
  std::string TMidType_;

  void init( const std::vector<std::string>& vmt, // valid muon types
             const std::string & tmid,
             const int & pixHits, // minimum number of pix hits
             const int & tkHits, // min. tk hits
             const double & d0, // d0 w.r.t. beam spot
             const double & dz, // dz w.r.t. beam spot
             const double & chi2, // global chi2/ndof
             const int & chambers, // number of chambers in SA muon
             const int & stations, // number of stations with matched segments
             const double & p, // min p
             const double & pt, // min pt
             const double & eta, // max eta
             const double & relIso // max rel comb iso
             );

  void FsrApr082011Init(
    // 1. muon is reconstructed as a "global muon" (out-in fit)
    const int & isGlobalMuon,
    // 2. maximum global muon fit normalized chi2
    const double & maxNormChi2,
    // 3. minimum number of valid muon hits matched to the global fit
    const int & minMuonHits,
    // 4. muon is reconsturcted as a "tracker muon" (in-out fit)
    const bool & isTrackerMuon,
    // 5. minimum number of muon stations with matched segments
    //    (global track: out-in fit)
    const int & minMatches,
    // 6. minimum number of tracker (pixels + strips) hits
    const int & minTkHits,
    // 7. minimum number of pixel hits
    const int & minPixelHits,
    // 8. maximum inner track transverse impact parameter w.r.t the beam spot
    //    absolute value
    const double & maxAbsTrackDxyBS,
    // 9. sum of pT of tracks with pT >1.5 within a cone of DR < 0.3 around
    //    the muon direction, vetoing a cone of 0.015 around that direction
    const double & maxTrackIso,
    // 10. minimum transverse momentum
    const double & minPt,
    // 11. maximum pseudo-rapidity absolute value
    const double & maxAbsEta
  );

};



#endif
