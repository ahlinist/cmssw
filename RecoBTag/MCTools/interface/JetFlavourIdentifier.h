#ifndef JetFlavourIdentifier_H
#define JetFlavourIdentifier_H

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoBTag/MCTools/interface/MCParton.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"
#include "DataFormats/Math/interface/LorentzVector.h"

class JetFlavourIdentifier {
public:

  JetFlavourIdentifier() {}
  JetFlavourIdentifier(const edm::ParameterSet& iConfig);
  void readEvent(const edm::Event& iEvent, std::string label_ = "source");

  JetFlavour identifyBasedOnPartons( const reco::Jet & jet );

  void getAssociatedLepton(const reco::Jet & theJet) const;

private:
  void fillInfo ( const HepMC::GenEvent * generated_event );

  JetFlavour  basicIdentityBasedOnPartons
	(const math::XYZTLorentzVector & jet4Vec, const double coneSize);

  void fillAlgorithmicDefinition(JetFlavour & jetFlavour);
  void fillPhysicsDefinition(JetFlavour & jetFlavour, const math::XYZTLorentzVector & jet4Vec);

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
