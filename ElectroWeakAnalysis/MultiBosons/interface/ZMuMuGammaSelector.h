#ifndef ElectroWeakAnalysis_MultiBosons_interface_ZMuMuGammaSelector_h
#define ElectroWeakAnalysis_MultiBosons_interface_ZMuMuGammaSelector_h

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

class ZMuMuGammaSelector : public Selector<reco::CompositeCandidate> {
  public:
    enum version {Fsr2011Apr11, N_VERSIONS};

    ZMuMuGammaSelector() {}
    ZMuMuGammaSelector( const edm::ParameterSet& );

    bool operator()( const reco::CompositeCandidate & mmgCand,
                     const edm::EventBase & event,
                     pat::strbitset & ret) {return operator()(mmgCand, ret);}

    using Selector<reco::CompositeCandidate>::operator();

    bool operator()( const reco::CompositeCandidate&,
                     pat::strbitset & );

  private:
    version version_;
    double photonTrackIsoConst_, photonTrackIsoSlope_;

    bool passes_Fsr2011Apr11( const reco::CompositeCandidate& mmgCand,
                              pat::strbitset& ret);
    void init_Fsr2011Apr11(
      // 1. maximum near muon HCAL isolation
      const double& maxNearMuonHcalIso,
      // 2. maximum far muon tracker isolation
      const double& maxFarMuonTrackIso,
      // 3. maximum far muon ECAL isolation
      const double& maxFarMuonEcalIso,
      // 4. maximum photon tracker isolation (near muon removed) = const + slope * pt
      const double& photonTrackIsoConst,
      const double& photonTrackIsoSlope,
      // 5. maximum Delta R distance between the photon and the near muon
      const double& maxDeltaRNear,
      // 6. minimum far muon transverse momentum
      const double& minFarMuonPt,
      // 7. minimum invariant mass
      const double& minMass,
      // 8. maximum invariant mass
      const double& maxMass
    );

};

#endif
