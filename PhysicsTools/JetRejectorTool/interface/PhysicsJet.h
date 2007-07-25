#ifndef PhysicsJet_h
#define PhysicsJet_h


#include "CLHEP/HepMC/GenEvent.h"
#include <CLHEP/HepMC/GenParticle.h>
#include <CLHEP/HepMC/GenVertex.h>
#include "FWCore/Framework/interface/Handle.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include <DataFormats/JetReco/interface/GenJet.h>
#include <DataFormats/JetReco/interface/CaloJet.h>
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include <vector>

using namespace reco;
using namespace std;
using namespace edm;
using namespace HepMC;

class PhysicsJet {
 public:
  
  PhysicsJet();
  //  PhysicsJet(edm::Handle<std::vector<reco::GenJet> >, vector<HepMC::GenParticle>);
  virtual ~PhysicsJet();
  vector<reco::JetTag> selection(edm::Handle<vector<reco::JetTag> >, double, double);
  vector<reco::CaloJet> selection(edm::Handle<vector<reco::CaloJet> >, double, double);
  vector<reco::GenJet> selection(edm::Handle<vector<reco::GenJet> >, double, double );
  bool isSelected(reco::Jet , double, double );


private:
  /*  HepMC::GenParticle parton;
  reco::GenJet genjet;
  reco::CaloJet calojet;*/
};

#endif
