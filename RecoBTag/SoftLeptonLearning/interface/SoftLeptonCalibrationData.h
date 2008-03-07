#ifndef RecoBTag_SoftLeptonLearning_SoftLeptonCalibrationData_h
#define RecoBTag_SoftLeptonLearning_SoftLeptonCalibrationData_h

#include <vector>
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

struct SoftLeptonCalibrationData {
    math::XYZTLorentzVector partonMomentum;         // parton momentum and energy
    int                     partonFlavour;          // parton flavour

    math::XYZTLorentzVector generatorMomentum;      // generator-level jet momentum and energy

    math::XYZTLorentzVector jetMomentum;            // calorimetric jet momentum and enrgy
    math::XYZVector         jetMomentumQuick;       // origin-corrected jet direction (original energy)
    math::XYZVector         jetMomentumDetailed;    // origin-corrected jet direction (original energy)
    math::XYZVector         jetMomentumTracks;      // charged tracks-based jet direction and energy
    double                  jetEnergyCorrected;     // corrected jet energy (MC Jet correction)
    double                  jetEnergyBJet;          // corrected jet energy (assuming b flavour)

    reco::Vertex                 vertex;            // event primary vertex (fallback to beamspot)
    std::vector<reco::TrackBase> jetTracks;         // tracks inside jet
    std::vector<reco::TrackBase> jetRecoLeptons;    // reconstructed leptons inside jet
    std::vector<reco::Particle>  jetSimLeptons;     // simulated leptons inside jet
    std::vector<int>             jetFakeLeptons;    // PDG id of non-lepton particles associated to leptons (0 for fakes)
    std::vector<bool>            jetGoodLeptons;    // reco lepton associated (by chi2) to sim leptons
    std::vector<bool>            jetLostLeptons;    // sim leptons NOT associated to reco leptons

    SoftLeptonCalibrationData() :
      partonMomentum(),
      partonFlavour(0),
      generatorMomentum(),
      jetMomentum(),
      jetMomentumQuick(),
      jetMomentumDetailed(),
      jetMomentumTracks(),
      jetEnergyCorrected(0.),
      jetEnergyBJet(0.),
      jetTracks(),
      jetRecoLeptons(),
      jetSimLeptons(),
      jetFakeLeptons(),
      jetGoodLeptons(),
      jetLostLeptons()
    { }
};

#endif // RecoBTag_SoftLeptonLearning_SoftLeptonCalibrationData_h
