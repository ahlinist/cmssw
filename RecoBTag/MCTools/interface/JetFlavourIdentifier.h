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
  void readEvent(const edm::Event& iEvent);

  BTagMCTools::JetFlavour identifyBasedOnPartons( const reco::Jet & jet ) const;
  const std::vector<MCParton> & getListOfPartons() const { return m_partons; }

private:
  void fillInfo( const HepMC::GenEvent * generated_event );

  BTagMCTools::JetFlavour  basicIdentityBasedOnPartons(const math::XYZTLorentzVector & jet4Vec, const double coneSize) const;

  void fillAlgorithmicDefinition(BTagMCTools::JetFlavour & jetFlavour) const;
  void fillPhysicsDefinition(BTagMCTools::JetFlavour & jetFlavour, const math::XYZTLorentzVector & jet4Vec) const;

  double coneSizeToAssociate;

  bool fillPartons;
  bool fillHeavyHadrons;
  bool fillLeptons;
  bool physDefinition;
  bool rejectBCSplitting;
  std::vector<MCParton> m_partons;
  bool vetoB, vetoC, vetoL, vetoG;
  std::string moduleLabel;
  HepMC::GenEvent * generated_event;
};

#endif
