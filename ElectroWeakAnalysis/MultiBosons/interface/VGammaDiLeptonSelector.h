#ifndef ElectroWeakAnalysis_MultiBosons_interface_VGammaDiLeptonSelector_h
#define ElectroWeakAnalysis_MultiBosons_interface_VGammaDiLeptonSelector_h

#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

class VGammaDiLeptonSelector : public Selector<reco::CompositeCandidate> {
  public:
    enum version {Fsr2011Apr11, N_VERSIONS};

    VGammaDiLeptonSelector() {}
    VGammaDiLeptonSelector( const edm::ParameterSet& );

    bool operator()( const reco::CompositeCandidate & dilepton,
                     const edm::EventBase & event,
                     pat::strbitset & ret) {return operator()(dilepton, ret);}

    using Selector<reco::CompositeCandidate>::operator();

    bool operator()( const reco::CompositeCandidate&,
                     pat::strbitset & );

  private:
    version version_;

    bool passes_Fsr2011Apr11( const reco::CompositeCandidate& dilepton,
                              pat::strbitset& ret);
    void init_Fsr2011Apr11(
      // 1. total charge
      const int& charge,
      // 2. minimum invariant mass
      const double &minMass,
      // 3. maximum invariant mass
      const double &maxMass
    );

};

#endif
