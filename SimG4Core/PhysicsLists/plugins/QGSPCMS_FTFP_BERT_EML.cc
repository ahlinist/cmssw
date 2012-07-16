#include "QGSPCMS_FTFP_BERT_EML.hh"
#include "SimG4Core/PhysicsLists/interface/CMSEmStandardPhysics95.h"
#include "SimG4Core/PhysicsLists/interface/CMSMonopolePhysics.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "G4DecayPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4DataQuestionaire.hh"
#include "HadronPhysicsQGSP_FTFP_BERT.hh"

#include <string>

QGSPCMS_FTFP_BERT_EML::QGSPCMS_FTFP_BERT_EML(G4LogicalVolumeToDDLogicalPartMap& map,
					     const HepPDT::ParticleDataTable * table_,
					     sim::FieldBuilder *fieldBuilder_, 
					     const edm::ParameterSet & p) : PhysicsList(map, table_, fieldBuilder_, p) {

  G4DataQuestionaire it(photon);
  
  int  ver     = p.getUntrackedParameter<int>("Verbosity",0);
  bool emPhys  = p.getUntrackedParameter<bool>("EMPhysics",true);
  bool hadPhys = p.getUntrackedParameter<bool>("HadPhysics",true);
  bool tracking= p.getParameter<bool>("TrackingCut");
  std::string region = p.getParameter<std::string>("Region");
  edm::LogInfo("PhysicsList") << "You are using the simulation engine: "
			      << "QGSP_FTFP_BERT_EML for Geant4 9.5 with Flags for EM Physics "
			      << emPhys << ", for Hadronic Physics "
			      << hadPhys << " and tracking cut " << tracking
                              << " with special region " << region;

  if (emPhys) {
    // EM Physics
    RegisterPhysics( new CMSEmStandardPhysics95("standard EM EML",ver,region));

    // Synchroton Radiation & GN Physics
    RegisterPhysics( new G4EmExtraPhysics(ver));
  }

  // Decays
  RegisterPhysics( new G4DecayPhysics(ver) );

  if (hadPhys) {
    // Hadron Elastic scattering
    RegisterPhysics( new G4HadronElasticPhysics(ver));

    // Hadron Physics
    RegisterPhysics( new HadronPhysicsQGSP_FTFP_BERT(ver)); 
  
    // Stopping Physics
    RegisterPhysics( new G4QStoppingPhysics(ver));

    // Ion Physics
    RegisterPhysics( new G4IonPhysics(ver));

    // Neutron tracking cut
    if (tracking) {
      RegisterPhysics( new G4NeutronTrackingCut(ver));
    }
  }

  // Monopoles
  RegisterPhysics( new CMSMonopolePhysics(table_,fieldBuilder_,p));
}
