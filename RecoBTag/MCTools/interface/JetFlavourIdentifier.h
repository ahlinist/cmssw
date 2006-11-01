#ifndef JetFlavourIdentifier_H
#define JetFlavourIdentifier_H

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoBTag/MCTools/interface/MCParton.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"

// #include "FWCore/Framework/interface/EventSetup.h"
// 
// #include "FWCore/ParameterSet/interface/ParameterSet.h"

class JetFlavourIdentifier {
public:

  JetFlavourIdentifier() {}
  JetFlavourIdentifier(const edm::ParameterSet& iConfig);
  void readEvent(const edm::Event& iEvent, std::string label_ = "source");

  template <class J>
  JetFlavour identifyBasedOnPartons       ( const edm::Ref<J> & jet );

  void getAssociatedLepton(const reco::Jet & theJet) const;

private:
  void fillInfo ( const HepMC::GenEvent * generated_event );

  JetFlavour  basicIdentityBasedOnPartons
	(const Hep3Vector & jet3Vec, const double coneSize);

  void fillAlgorithmicDefinition(JetFlavour & jetFlavour);
  void fillPhysicsDefinition(JetFlavour & jetFlavour, const Hep3Vector & jet3Vec);

  static double deltaR ( const HepLorentzVector & vec1 , const HepLorentzVector & vec2 ) { 
    HepLorentzVector v = vec1 - vec2;
    return hypot(v.eta(), v.phi());
  }

  // data members to be set in constructor
  double coneSizeToAssociate;

  bool fillPartons;
  bool fillHeavyHadrons;
  bool fillLeptons;
  bool physDefinition;
  bool rejectBCSplitting;
  std::vector<MCParton>      m_partons;
  bool vetoB, vetoC, vetoL, vetoG;


};

#endif
