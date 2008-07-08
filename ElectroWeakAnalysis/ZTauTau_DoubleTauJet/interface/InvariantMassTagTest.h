#ifndef INVARIANT_MASSTAG_TEST_H
#define INVARIANT_MASSTAG_TEST_H

#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"


// Math
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"


class TH1F;
class TH2F;
class TProfile;

class InvariantMassTagTest : public edm::EDAnalyzer {
public:
  explicit InvariantMassTagTest(const edm::ParameterSet&);
  ~InvariantMassTagTest() {}

  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  virtual void beginJob(const edm::EventSetup& iSetup);
  virtual void endJob();
  void fillGenJetInvMass(const reco::PFTau& tau, const reco::GenJetCollection& genJets, 
			 const TrackingParticleCollection& trakingParticles);
  void findPFInvMass(const edm::Event& iEvent, const reco::PFTau& tau);
  void findTrueInvMass(const edm::Event& iEvent);


private:
  int nEvent;
  std::vector<float> nEventsUsed;
  std::string MassTagProducer_;
  std::string PFTauProducer_;
  std::string PFTauDiscByIsolationProducer_;
  edm::InputTag GenJetProducer_;
  std::string GenParticleProducer_;
  std::string PFSimParticleProducer_;
  edm::InputTag TrackingTruthProducer_;

  std::vector<float> nEventsRiso;

  // Parameters
  double min_tau_jet_pt;
  double matching_cone;
  double leading_trk_pt;
  double signal_cone;
  double cluster_jet_matching_cone;
  double cluster_trk_matching_cone;
  double inv_mass_cut;
  double isolation_cone;


  // book a few histograms    

  TH1F* discriminatorValue;

  TH1F* invMassFromPFJet;
  TH1F* invMass;
  TH1F* invMassSigComp;
  TH1F* invMass1;
  TH1F* invMass2;
  TH1F* invMass3;
  TH1F* invMassFromSigChHad1;

  TH1F* nGamPFJet;
  TH1F* nNeutalHadPFJet;
  TH1F* nChargedHadPFJet;
  TH1F* nGamSigCone;
  TH1F* nNeutalHadSigCone;
  TH1F* nChargedHadSigCone;
  TH1F* EtChargedHadSigCone;
  TH1F* EtNeutralHadSigCone;
  TH1F* EtGamSigCone;
  TH1F* nGamSelected;
  TH1F* nSigCandPFJet;

  TH1F* invMassHepMC;
  TH1F* invMassHepMCTrk1;

  TH1F* invMassTrueGam;
  TH1F* nGamTrue;

  TH1F* invMassGenJet;
  TH1F* invMassGenJetPt;
  TH1F* nGamGenJet;

  TH1F* invMassGenJetSigCone;
  TH1F* iDGenJetSigCone;
  TH1F* nGamGenJetSigCone;
  TH1F* nCandGenJetSigCone;

  TH1F* ptGamGenJetSigCone;
  TH1F* ptCandGenJetSigCone;
  TH1F* ptGamGenJetSigConeNotMatched;
  TH1F* ptCandGenJetSigConeNotMatched;
  TH1F* etaGamGenJetSigCone;
  TH1F* etaCandGenJetSigCone;
  TH1F* etaGamGenJetSigConeNotMatched;
  TH1F* etaCandGenJetSigConeNotMatched;
  TH1F* EresCandGenJetSigConeMatched;
  TH1F* EresGamGenJetSigConeMatched;
  TH1F* fracCandGenJetSigConeNotMatched;
  TH1F* fracGamGenJetSigConeNotMatched;

  TH1F* EresGamGenJetSigConeMatched1;
  TH1F* EresGamGenJetSigConeMatched2;
  TH1F* EresGamGenJetSigConeMatched3;
  TH1F* EresGamGenJetSigConeMatched4;

  TH1F* nGamGenJetSigConeNotMatched;
  TH1F* rVtxGamGenJetSigConeNotMatched;
  TH1F* rVtxGamGenJetSigConeMatched;
  TH1F* rVtxGamSimTrack;
  TH1F* nVtxGamGenJetSigConeNotMatched;
  TH1F* nVtxGamGenJetSigConeMatched;
  TH1F* nVtxGamSimTrack;

  TH1F* invMassRecGam1Prong;
  TH1F* invMassTrueGam1Prong;
  TH1F* invMassNoGam3Prong;
  TH1F* invMassRecGam3Prong;
  TH1F* invMassTrueGam3Prong;
  TH2F* MCTrkVsRecTrk;

  TH1F* EResTau;
  TH1F* EResTauSigCone;
  TH2F* EResTauVsDR;
  TH2F* EResTauSigConeVsDR;
  
};
#endif
