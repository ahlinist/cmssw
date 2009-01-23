//
// $Id: PATObjectEventSelector.h,v 1.1 2009/01/22 16:30:03 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_PATObjectEventSelector_h
#define TauAnalysis_RecoTools_PATObjectEventSelector_h

#include "PhysicsTools/UtilAlgos/interface/AnySelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectCountEventSelector.h"
#include "PhysicsTools/UtilAlgos/interface/MinNumberSelector.h"
#include "PhysicsTools/PatUtils/interface/MaxNumberSelector.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Particle.h"

#include <vector>

namespace pat {

  typedef ObjectCountEventSelector<std::vector<Electron>, AnySelector, MinNumberSelector> PATElectronMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<Muon>,     AnySelector, MinNumberSelector> PATMuonMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<Tau>,      AnySelector, MinNumberSelector> PATTauMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<Photon>,   AnySelector, MinNumberSelector> PATPhotonMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<Jet>,      AnySelector, MinNumberSelector> PATJetMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<MET>,      AnySelector, MinNumberSelector> PATMETMinEventSelector;
  typedef ObjectCountEventSelector<std::vector<Particle>, AnySelector, MinNumberSelector> PATParticleMinEventSelector;

  typedef ObjectCountEventSelector<std::vector<Electron>, AnySelector, MaxNumberSelector> PATElectronMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<Muon>,     AnySelector, MaxNumberSelector> PATMuonMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<Tau>,      AnySelector, MaxNumberSelector> PATTauMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<Photon>,   AnySelector, MaxNumberSelector> PATPhotonMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<Jet>,      AnySelector, MaxNumberSelector> PATJetMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<MET>,      AnySelector, MaxNumberSelector> PATMETMaxEventSelector;
  typedef ObjectCountEventSelector<std::vector<Particle>, AnySelector, MaxNumberSelector> PATParticleMaxEventSelector;

}

#endif
